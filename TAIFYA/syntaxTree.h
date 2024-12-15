#pragma once
#ifndef SYNTAX_ANALYZER_H
#define SYNTAX_ANALYZER_H

#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include "lexem.h"

using std::shared_ptr;
using std::string;
using std::make_shared;
using std::endl;

// Тип для синтаксического древа
// !!! ТИПЫ БУДУТ ПОПОЛНЯТЬСЯ И МЕНЯТЬСЯ НА ЭТАПЕ СЕМАНТИЧЕСКОГО АНАЛИЗАТОРА !!!
enum class NodeType {

// Тип чисел 
	INT,
	DOUBLE,
	BOOL,

// Тип Лексем
	WORD,
	LIMITER,
	NUMBER,
	IDENTIFIER,

// Основа программы
	PROGRAM,		// Начало программ
	DECLARATION,	// Описание / Объявление переменных
	OPERATOR,		// Оператор

// Операторы
	COMPOUND,		// Составной 
	IF,				// Для if-else
	THEN, 
	ELSE,
	ASSIGNMENT,		// Присваивание
	INPUT,			// Ввод
	OUTPUT,			// Вывод
	LOOP_FOR,		// Фиксированный цикл
	DO_WHILE,		// Условный цикл
	DO,				// (Пока что не используются)
	WHILE,

	EXPRESSION,		// Выражение
	OPERAND,		// Операнд
	TERM,			// Слагаемое
	FACTOR,			// Множитель

	END,			// (Пока что не используются)
};

// Узел
struct Node {
	NodeType type;
	std::string description;
	std::vector<shared_ptr<Node>> children;
	Lexeme lexem;

	Node(NodeType type, Lexeme token, std::string description = "") :
		type(type), lexem(token), description(description) {}

	Node(NodeType type, std::string description = "") :
		type(type), description(description) {}


	void addChild(shared_ptr<Node> child) {
		children.push_back(child);
	}

	static void printRoot(const shared_ptr<Node>& root) {
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

	static void printTree(const shared_ptr<Node>& node, const std::string& prefix = "", bool isLast = true) {
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
			printTree(node->children[i], prefix + (isLast ? "   " : "|   "), i == node->children.size() - 1);

		}
	}
};

shared_ptr<Node> createNode(NodeType type, string description = "");

#endif