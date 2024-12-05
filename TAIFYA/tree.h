// В syntax.h
#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H

#include<iostream>

#include <string>
#include <vector>
#include <memory>

class Node {
public:
    std::string value;  // Лексема или тип узла
    std::vector<std::shared_ptr<Node>> children; // Дочерние узлы

    // Конструкторы
    explicit Node(const std::string& val) : value(val) {}

    // Добавление дочернего узла
    void addChild(const std::shared_ptr<Node>& child) {
        children.push_back(child);
    }
};

class SyntaxTree {
private:
    std::shared_ptr<Node> root; // Корень дерева

public:
    // Конструкторы
    SyntaxTree() : root(nullptr) {}
    explicit SyntaxTree(const std::string& rootValue) : root(std::make_shared<Node>(rootValue)) {}

    // Установить корень
    void setRoot(const std::shared_ptr<Node>& newRoot) {
        root = newRoot;
    }

    // Получить корень
    std::shared_ptr<Node> getRoot() const {
        return root;
    }

    // Печать дерева (рекурсивно)
    void printTree(const std::shared_ptr<Node>& node, int depth = 0) const {
        if (!node) return;
        std::cout << std::string(depth * 2, ' ') << node->value << std::endl;
        for (const auto& child : node->children) {
            printTree(child, depth + 1);
        }
    }
};

#endif // SYNTAX_TREE_H
