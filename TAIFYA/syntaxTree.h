#pragma once
#ifndef SYNTAX_ANALYZER_H
#define SYNTAX_ANALYZER_H

#include <iostream>
#include <memory>
#include <vector>

#include <optional>

#include "lexem.h"

//std::optional<Lexeme> lexem;

enum class NodeType {
	INT,
	DOUBLE,
	BOOL,

	WORD,
	LIMITER,

	IDENTIFIER,
	NUMBER,

	PROGRAM,		// Начало программ
	DECLARATION,	// Описание / Объявление переменных
	OPERATOR,		// Оператор
	ASSIGNMENT,		// Присваивание
	LOOP_FOR,		// Фиксированный цикл
	LOOP_WHILE,		// Условный цикл

	EXPRESSION,		// Выражение
	OPERAND,		// Операнд
	SLAG,			// Слагаемое
	MNOG,

	END,			// Конец программ (?)
};

struct Node {
	NodeType type;
	std::string description;
	std::vector<std::shared_ptr<Node>> children;
	Lexeme lexem;

	Node(NodeType type, Lexeme token, std::string description = "") :
		type(type), lexem(token), description(description) {}

	Node(NodeType type, std::string description = "") :
		type(type), description(description) {}


	void addChild(std::shared_ptr<Node> child) {
		children.push_back(child);
	}

	static void printRoot(const std::shared_ptr<Node>& root) {
		std::cout << root->description;
		if (root->lexem.tableNumb == 0) {
			std::cout << endl;
		}
		else {
			std::cout << " (" << root->lexem.tableNumb << ", " << root->lexem.valueNumb << ") " << std::endl;
		}

		for (size_t i = 0; i < root->children.size(); ++i) {
			printTree(root->children[i], "", i == root->children.size() - 1);
		}
	}

	static void printTree(const std::shared_ptr<Node>& node, const std::string& prefix = "", bool isLast = true) {
		std::cout << prefix;
		if (isLast) {
			std::cout << "`- ";
		}
		else {
			std::cout << "|- ";
		}

		std::cout << node->description;
		if (node->lexem.tableNumb == 0) {
			std::cout << endl;
		}
		else {
			std::cout << " (" << node->lexem.tableNumb << ", " << node->lexem.valueNumb << ") " << std::endl;
		}

		for (size_t i = 0; i < node->children.size(); ++i) {
			//printTree(node->children[i], prefix + (isLast ? "\t" : "|\t"), i == node->children.size() - 1);
			printTree(node->children[i], prefix + (isLast ? "   " : "|   "), i == node->children.size() - 1);

		}
	}
};

extern std::shared_ptr<Node> root;

#endif