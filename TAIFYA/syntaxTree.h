#pragma once
#ifndef SYNTAX_ANALYZER_H
#define SYNTAX_ANALYZER_H

#include <iostream>
#include <memory>
#include <vector>

enum class NodeType {
	INT,
	DOUBLE,
	BOOL,
	IDENTIFIER,

	PROGRAM,		// ������ ��������
	DECLARATION,	// �������� / ���������� ����������
	OPERATOR,		// ��������
	ASSIGNMENT,		// ������������
	LOOP_FOR,		// ������������� ����
	LOOP_WHILE,		// �������� ����
	EXPRESSION,		// ���������
	END,			// ����� ��������
};

struct Node {
	NodeType type;
	std::string description;
	std::vector<std::shared_ptr<Node>> children;
	std::pair<int, int> token;

	Node(NodeType type, std::pair<int, int> token, std::string description = "") :
		type(type), token(token), description(description) {}

	void addChild(std::shared_ptr<Node> child) {
		children.push_back(child);
	}

	static void printTree(const std::shared_ptr<Node>& node, const std::string& prefix = "", bool isLast = true) {
		// �������� ������� ����
		std::cout << prefix;
		if (isLast) {
			std::cout << "--- ";
		}
		else {
			std::cout << "--- ";
		}
		std::cout << node->description << " (" << node->token.first << ", " << node->token.second << ") " << std::endl;

		// ����������� ����� ��� ���� �����
		for (size_t i = 0; i < node->children.size(); ++i) {
			printTree(node->children[i], prefix + (isLast ? "    " : "|-- "), i == node->children.size() - 1);
		}
	}
};

#endif