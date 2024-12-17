#include <iostream>

#include "tables.h"
#include "semantic.h"
#include "semanticErr.h"

#include "syntaxTree.h"
unsigned int linePosition = 0;

unordered_map<int, IdLexeme> TI_semantic;

NodeType checkExpression(shared_ptr<Node> node);
NodeType checkOperand(shared_ptr<Node> node);
NodeType checkTerm(shared_ptr<Node> node);
NodeType checkFactor(shared_ptr<Node> node);

static bool isTypeId(shared_ptr<Node> node) {
	NodeType typeNode = node->type;
	return (typeNode == NodeType::INT || typeNode == NodeType::DOUBLE || typeNode == NodeType::BOOL);
}

static bool isDescrID(shared_ptr<Node> node, unordered_map<int, IdLexeme> table) {
	if (table.find(node->lexem.valueNumb) != table.end()) {
		return true;
	}
	return false;
}

static bool isDescrID(shared_ptr<Node> node) {
	if (TI_semantic.find(node->lexem.valueNumb) != TI_semantic.end()) {
		return true;
	}
	return false;
}

static string getNameId(int valueId) {
	for (auto& pair : TI) {
		if (pair.second == valueId) {
			return pair.first;
		}
	}
	return "";
}

static string printType(NodeType type) {
	switch (type)
	{
	case NodeType::INT:
		return "int";
	case NodeType::DOUBLE:
		return "double";
	case NodeType::BOOL:
		return "bool";
	default:
		break;
	}
}

static void printTable() {
	// Для проверки
	for (const auto& pair : TI_semantic) {
		std::cout << "Key: " << pair.first
			<< ", Name: " << pair.second.name
			<< ", Type: " << printType(pair.second.type) << endl;
	}
}

static NodeType findNumbType(int value) {
	string number;

	for (auto& pair : TN) {
		if (pair.second == value) {
			number = pair.first;
			break;
		}
	}

	if (
		(number.find('.') != std::string::npos ||
			number.find_first_of("Ee") != std::string::npos) &&
		number.back() != 'h') {
		return NodeType::DOUBLE;
	}
	else {
		return NodeType::INT;
	}
}

//Функции для семантики
void checkDeclaration(shared_ptr<Node> node) {
	unordered_map<int, IdLexeme> current_Id;

	for (shared_ptr<Node> child : node->children) {
		if (isTypeId(child)) {
			NodeType type = child->type;
			for (auto& id : current_Id) {
				if (TI_semantic.find(id.first) != TI_semantic.end()) {
					semant_err_proc(SemantErr::IdAlreadyDeclared, child, getNameId(id.first));
				}
				id.second.type = type;
				TI_semantic[id.first] = id.second;
			}
			std::cout << endl;
			break;
		}

		int valueNumb = child->lexem.valueNumb;
		IdLexeme idLex;

		if (isDescrID(child, current_Id)) {
			semant_err_proc(SemantErr::IdAlreadyDeclared, child, getNameId(valueNumb));
		}
		idLex.name = getNameId(valueNumb);
		idLex.type = child->type;
		current_Id[valueNumb] = idLex;
	}
}

void checkCondition(shared_ptr<Node> node) {
	for (shared_ptr<Node> child : node->children) {
		if (child->type == NodeType::EXPRESSION) {
			if (checkExpression(child) != NodeType::BOOL) {
				child->lexem.linePos = linePosition;
				semant_err_proc(SemantErr::InvalidCondition,child);
			}
		}
	}
}

void checkAssigment(shared_ptr<Node> node) {
	NodeType idType = NodeType::UNKNOWN;
	for (shared_ptr<Node> child : node->children) {
		if (child->type == NodeType::IDENTIFIER) {
			idType = TI_semantic[child->lexem.valueNumb].type;
		}
		else if (child->type == NodeType::EXPRESSION) {
			if (checkExpression(child) != idType) {
				child->lexem.linePos = linePosition;
				semant_err_proc(SemantErr::InvalidAssigment,child);
			}
		}
	}
}

void checkInput(shared_ptr<Node> node) {
	for (shared_ptr<Node> child : node->children) {
		if (child->type == NodeType::IDENTIFIER) {
			if (!isDescrID(child)) {
				semant_err_proc(SemantErr::IdNotDeclared,child, getNameId(child->lexem.valueNumb));
			}
		}
	}
}

void checkOutput(shared_ptr<Node> node) {
	for (shared_ptr<Node> child : node->children) {
		if (child->type == NodeType::EXPRESSION) {
			checkExpression(child);
		}
	}
}

void checkLoopFor(shared_ptr<Node> node) {
	int numbOfExpr = 0;
	for (shared_ptr<Node> child : node->children) {
		if (child->type == NodeType::EXPRESSION) {
			if (checkExpression(child) != NodeType::BOOL) {
				child->lexem.linePos = linePosition;
				semant_err_proc(SemantErr::InvalidCondition, child);
			}
		}
	}
}

// Функции для обработки выражений
NodeType checkExpression(shared_ptr<Node> node) {
	bool haveOperation = false;
	NodeType leftExprType=NodeType::UNKNOWN, rightExprType;
	for (shared_ptr<Node> child : node->children) {
		if (child->type == NodeType::OPERAND) {
			if (!haveOperation) {
				leftExprType = checkOperand(child);
			}
			else {
				rightExprType = checkOperand(child);
				if (leftExprType != rightExprType) {
					child->lexem.linePos = linePosition;
					semant_err_proc(SemantErr::DifferentType, child);
				}
				leftExprType = NodeType::BOOL;
			}
		}
		else if (child->type == NodeType::EXPR_OPERATION) {
			haveOperation = true;
		}
	}
	return leftExprType;
}

NodeType checkOperand(shared_ptr<Node> node) {

	bool haveOperation = false;
	NodeType leftOperType = NodeType::UNKNOWN, rightOperType;
	for (shared_ptr<Node> child : node->children) {
		if (child->type == NodeType::TERM) {
			if (!haveOperation) {
				leftOperType = checkTerm(child);
			}
			else {
				rightOperType = checkTerm(child);
				 if (leftOperType != rightOperType) {
					 child->lexem.linePos = linePosition;
					 semant_err_proc(SemantErr::DifferentType, child);
					 //throw std::runtime_error("Different type");
				}
				leftOperType = rightOperType;
				haveOperation = false;
			}
		}
		else if (child->type == NodeType::NUMB_OPERATION || 
			child->type == NodeType::BOOL_OPERATION) {
			haveOperation = true;
		}
	}
	return leftOperType;
}

NodeType checkTerm(shared_ptr<Node> node) {
	bool haveNumbOperation = false;
	NodeType leftTermType = NodeType::UNKNOWN, rightTermType;
	for (shared_ptr<Node> child : node->children) {
		if (child->type == NodeType::FACTOR) {
			if (!haveNumbOperation) {
				leftTermType = checkFactor(child);
			}
			else {
				rightTermType = checkFactor(child);
				if (leftTermType != rightTermType) {
					//throw std::runtime_error("Different type");
					child->lexem.linePos = linePosition;
					semant_err_proc(SemantErr::DifferentType, child);
				}
				leftTermType = rightTermType;
				haveNumbOperation = false;
			}
		}
		else if (child->type == NodeType::NUMB_OPERATION || 
			child->type == NodeType::BOOL_OPERATION) {
			haveNumbOperation = true;
		}
	}
	return leftTermType;
}


static void setLine(shared_ptr<Node> node) {
	linePosition = node->lexem.linePos;
}
NodeType checkFactor(shared_ptr<Node> node) {
	bool isBack = false;
	for (shared_ptr<Node> child : node->children) {
		if (isBack) {
			return checkExpression(child);
		}
		if (child->type == NodeType::IDENTIFIER) {
			if (isDescrID(child)) {
				setLine(child);
				return TI_semantic[child->lexem.valueNumb].type;
			}
			else {
				semant_err_proc(SemantErr::IdNotDeclared, child, getNameId(child->lexem.valueNumb));
			}
		}
		else if (child->type == NodeType::NUMBER) {
			setLine(child);
			return findNumbType(child->lexem.valueNumb);
		}
		else if (child->type == NodeType::TRUE || child->type == NodeType::FALSE) {
			setLine(child);
			return NodeType::BOOL;
		}
		else if (child->type == NodeType::BRACKET_OPEN) {
			isBack = true;
		}
		else if (child->type == NodeType::BRACKET_CLOSE ||
			child->type == NodeType::NOT) {

		}
		else if (child->type == NodeType::FACTOR) {
			return checkFactor(child);
		}
		else {
			semant_err_proc(SemantErr::UnexpectedNode,child);
		}
	}
}

