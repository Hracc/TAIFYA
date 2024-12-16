#include <iostream>

#include "tables.h"
#include "semantic.h"
#include "syntaxTree.h"

unordered_map<int, IdLexeme> TI_semantic;

NodeType checkExpression(shared_ptr<Node> node);
NodeType checkOperand(shared_ptr<Node> node);
NodeType checkTerm(shared_ptr<Node> node);
NodeType checkFactor(shared_ptr<Node> node);

bool isTypeId(shared_ptr<Node> node) {
	NodeType typeNode = node->type;
	return (typeNode == NodeType::INT || typeNode == NodeType::DOUBLE || typeNode == NodeType::BOOL);
}

bool isDescrID(shared_ptr<Node> node, unordered_map<int, IdLexeme> table) {
	if (table.find(node->lexem.valueNumb) != table.end()) {
		return true;
	}
	return false;
}

bool isDescrID(shared_ptr<Node> node) {
	if (TI_semantic.find(node->lexem.valueNumb) != TI_semantic.end()) {
		return true;
	}
	return false;
}

string getNameId(int valueId) {
	for (auto pair : TI) {
		if (pair.second == valueId) {
			return pair.first;
		}
	}
	return "";
}

string printType(NodeType type) {
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

void printTable() {
	// Для проверки
	for (const auto& pair : TI_semantic) {
		std::cout << "Key: " << pair.first
			<< ", Name: " << pair.second.name
			<< ", Type: " << printType(pair.second.type) << endl;
	}
}

NodeType findNumbType(int value) {
	string number;

	for (auto pair : TN) {
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
			for (auto id : current_Id) {
				if (TI_semantic.find(id.first) != TI_semantic.end()) {
					throw std::runtime_error("Identifier is already declared");
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
			throw std::runtime_error("Identifier is already declared");
			continue;
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
				throw std::runtime_error("Invalid Condition Type");
			}
		}
	}
}

void checkAssigment(shared_ptr<Node> node) {
	NodeType idType;
	for (shared_ptr<Node> child : node->children) {
		if (child->type == NodeType::IDENTIFIER) {
			idType = TI_semantic[child->lexem.valueNumb].type;
		}
		else if (child->type == NodeType::EXPRESSION) {
			if (checkExpression(child) != idType) {
				throw std::runtime_error("Invalid Assigment Type");
			}
		}
	}
}

NodeType checkExpression(shared_ptr<Node> node) {
	bool haveOperation = false;
	NodeType leftExprType, rightExprType, resultExprType;
	for (shared_ptr<Node> child : node->children) {
		if (child->type == NodeType::OPERAND) {
			if (!haveOperation) {
				leftExprType = checkOperand(child);
			}
			else {
				rightExprType = checkOperand(child);
				if (leftExprType != rightExprType) {
					throw std::runtime_error("Different type");
				}
				leftExprType = rightExprType;
			}
		}
		else if (child->type == NodeType::EXPR_OPERATION) {
			haveOperation = true;
		}
	}
	if (haveOperation) {
		return NodeType::BOOL;
	}
	else {
		return leftExprType;
	}
}

NodeType checkOperand(shared_ptr<Node> node) {

	bool haveNumbOperation = false;
	bool haveBoolOperation = false;
	NodeType leftOperType, rightOperType;
	for (shared_ptr<Node> child : node->children) {
		if (child->type == NodeType::TERM) {
			if (!haveNumbOperation && !haveBoolOperation) {
				leftOperType = checkTerm(child);
			}
			else {
				rightOperType = checkTerm(child);
				if (haveBoolOperation &&
					(leftOperType != NodeType::BOOL ||
						rightOperType != NodeType::BOOL)) {
					throw std::runtime_error("Incorrect operation");
				}
				else if (leftOperType != rightOperType) {
					throw std::runtime_error("Different type");
				}
				leftOperType = rightOperType;
				haveNumbOperation = false;
				haveBoolOperation = false;
			}
		}
		else if (child->type == NodeType::NUMB_OPERATION) {
			haveNumbOperation = true;
		}
		else if (child->type == NodeType::BOOL_OPERATION) {
			haveBoolOperation = true;
		}
	}
	if (haveBoolOperation) {
		return NodeType::BOOL;
	}
	else {
		return leftOperType;
	}
}

NodeType checkTerm(shared_ptr<Node> node) {
	bool haveNumbOperation = false;
	bool haveBoolOperation = false;
	NodeType leftTermType, rightTermType;
	for (shared_ptr<Node> child : node->children) {
		if (child->type == NodeType::FACTOR) {
			if (!haveNumbOperation && !haveBoolOperation) {
				leftTermType = checkFactor(child);
			}
			else {
				rightTermType = checkFactor(child);
				if (haveBoolOperation && 
					(leftTermType != NodeType::BOOL ||
					rightTermType != NodeType::BOOL)) {
					throw std::runtime_error("Incorrect operation");
				}
				else if (leftTermType != rightTermType) {
					throw std::runtime_error("Different type");
				}
				leftTermType = rightTermType;
				haveNumbOperation = false;
				haveBoolOperation = false;
			}
		}
		else if (child->type == NodeType::NUMB_OPERATION) {
			haveNumbOperation = true;
		}
		else if (child->type == NodeType::BOOL_OPERATION) {
			haveBoolOperation = true;
		}
	}
	if (haveBoolOperation) {
		return NodeType::BOOL;
	}
	else {
		return leftTermType;
	}
}

NodeType checkFactor(shared_ptr<Node> node) {
	bool isBack = false;
	for (shared_ptr<Node> child : node->children) {
		if (isBack) {
			return checkExpression(child);
		}
		if (child->type == NodeType::IDENTIFIER) {
			if (isDescrID(child)) {
				return TI_semantic[child->lexem.valueNumb].type;
			}
			else {
				throw std::runtime_error("Identifier is not declared");
			}
		}
		else if (child->type == NodeType::NUMBER) {
			return findNumbType(child->lexem.valueNumb);
		}
		else if (child->type == NodeType::TRUE || child->type == NodeType::FALSE) {
			return NodeType::BOOL;
		}
		else {
			isBack = true;
		}
	}
}

void checkInput(shared_ptr<Node> node) {
	for (shared_ptr<Node> child : node->children) {
		if (child->type == NodeType::IDENTIFIER) {
			if (isDescrID(child)) {
				throw std::runtime_error("Identifier is not declared");
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
				throw std::runtime_error("Invalid Condition Type");
			}
		}
	}
}