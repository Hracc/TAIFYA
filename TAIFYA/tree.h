// � syntax.h
#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H

#include<iostream>

#include <string>
#include <vector>
#include <memory>

class Node {
public:
    std::string value;  // ������� ��� ��� ����
    std::vector<std::shared_ptr<Node>> children; // �������� ����

    // ������������
    explicit Node(const std::string& val) : value(val) {}

    // ���������� ��������� ����
    void addChild(const std::shared_ptr<Node>& child) {
        children.push_back(child);
    }
};

class SyntaxTree {
private:
    std::shared_ptr<Node> root; // ������ ������

public:
    // ������������
    SyntaxTree() : root(nullptr) {}
    explicit SyntaxTree(const std::string& rootValue) : root(std::make_shared<Node>(rootValue)) {}

    // ���������� ������
    void setRoot(const std::shared_ptr<Node>& newRoot) {
        root = newRoot;
    }

    // �������� ������
    std::shared_ptr<Node> getRoot() const {
        return root;
    }

    // ������ ������ (����������)
    void printTree(const std::shared_ptr<Node>& node, int depth = 0) const {
        if (!node) return;
        std::cout << std::string(depth * 2, ' ') << node->value << std::endl;
        for (const auto& child : node->children) {
            printTree(child, depth + 1);
        }
    }
};

#endif // SYNTAX_TREE_H
