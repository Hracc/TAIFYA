#include <iostream>

#include "tables.h"
#include "semantic.h"
#include "syntaxTree.h"

unordered_map<int, IdLexeme> TI_semantic;


NodeType checkOperand(shared_ptr<Node> node);
NodeType checkTerm(shared_ptr<Node> node);
NodeType checkFactor(shared_ptr<Node> node);
void checkExpression(shared_ptr<Node> node);

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
					std::cout << "bruh" << endl;
					continue;
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
			std::cout << "bruh1" << endl;
			continue;
		}
		idLex.name = getNameId(valueNumb);
		idLex.descrt = true;
		idLex.type = child->type;
		current_Id[valueNumb] = idLex;
	}
}


void checkAssigment(shared_ptr<Node> node) {
	for (shared_ptr<Node> child : node->children) {
		if (child->type == NodeType::IDENTIFIER) {
		}
		else if (child->type == NodeType::EXPRESSION) {
			checkExpression(child);
		}
	}
}

void checkExpression(shared_ptr<Node> node) {
	bool haveOperation = false;
	NodeType leftExprType, rightExprType;
	for (shared_ptr<Node> child : node->children) {
		if (child->type == NodeType::OPERAND) {
			if (!haveOperation) {
				leftExprType = checkOperand(child);
			}
			else {
				rightExprType = checkOperand(child);
				if (leftExprType != rightExprType) {
					std::cout << "bruh2" << endl;
				}
				leftExprType = rightExprType;
				haveOperation = false;
			}
		}
		else if (child->type == NodeType::EXPR_OPERATION) {
			haveOperation = true;
		}
	}
}

NodeType checkOperand(shared_ptr<Node> node) {

	bool haveOperation = false;
	NodeType leftOperType, rightOperType;
	for (shared_ptr<Node> child : node->children) {
		if (child->type == NodeType::TERM) {
			if (!haveOperation) {
				leftOperType = checkTerm(child);
			}
			else {
				rightOperType = checkTerm(child);
				if (leftOperType != rightOperType) {
					std::cout << "bruh3" << endl;
				}
				leftOperType = rightOperType;
				haveOperation = false;
			}
		}
		else if (child->type == NodeType::NUMB_OPERATION) {
			haveOperation = true;
		}
	}
	return leftOperType;
}

NodeType checkTerm(shared_ptr<Node> node) {
	bool haveOperation = false;
	NodeType leftTermType, rightTermType;
	for (shared_ptr<Node> child : node->children) {
		if (child->type == NodeType::FACTOR) {
			if (!haveOperation) {
				leftTermType = checkFactor(child);
			}
			else {
				rightTermType = checkFactor(child);
				if (leftTermType != rightTermType) {
					std::cout << "bruh4" << endl;
				}
				leftTermType = rightTermType;
				haveOperation = false;
			}
		}
		else if (child->type == NodeType::NUMB_OPERATION) {
			haveOperation = true;
		}
	}
	return leftTermType;
}

NodeType checkFactor(shared_ptr<Node> node) {

	for (shared_ptr<Node> child : node->children) {
		if (child->type == NodeType::IDENTIFIER) {
			if (isDescrID(child)) {
				return NodeType::INT;
			}
		}
		else if (child->type == NodeType::NUMBER) {
			return findNumbType(child->lexem.valueNumb);
		}
	}
}
