#include <iostream>

#include "tables.h"
#include "semantic.h"
#include "syntaxTree.h"

unordered_map<string, IdLexeme> TI_semantic;

bool isTypeId(shared_ptr<Node> node) {
	NodeType typeNode = node->type;
	return (typeNode == NodeType::INT || typeNode == NodeType::DOUBLE || typeNode == NodeType::BOOL);
}

void checkDeclaration(shared_ptr<Node> node) {
	unordered_map<string, IdLexeme> current_Id;

	for (shared_ptr<Node> child : node->children) {
		if (isTypeId(child)) {
			NodeType type = child->type;
			for (auto& id : current_Id) {
				id.second.type = type;
				if (TI_semantic.find(id.first) != TI_semantic.end()) {
					std::cerr << "Error: Identifier \"" << id.first << "\" is already declared!\n";
					continue;
				}

				TI_semantic[id.first] = id.second;
			}
			// Для проверки
			for (const auto& pair : TI_semantic) {
				std::cout << "Key: " << pair.first
					<< ", Name: " << pair.second.name
					<< ", Descrt: " << pair.second.descrt << endl;
			}

			std::cout << endl;
			break;
		}

		IdLexeme current_idLex;
		for (const auto& pair : TI) {
			if (pair.second == child->lexem.valueNumb) {
				current_idLex.name = pair.first;
				current_idLex.descrt = true;

				if (current_Id.find(current_idLex.name) != current_Id.end()) {
					//throw std::runtime_error("Error: Identifier \"" + current_idLex.name + "\" is already declared in this scope!");
				}

				current_Id[current_idLex.name] = current_idLex;
			}
		}
	}
}