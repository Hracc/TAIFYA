#include <iostream>
#include <sstream>

#include "syntax.h"
#include "tables.h"

#include "syntaxTree.h"

bool isID, isNumb;


size_t currentLexemeIndex = 0;

Lexeme currentLexeme;

//Чтение лексем
void gl() {
    if (currentLexemeIndex >= lexemes.size()) {
        syntax_err_proc(SyntaxErr::OutOfBounds);
        return;
    }
    isID = false;
    isNumb = false;

    currentLexeme = lexemes[currentLexemeIndex];

    unordered_map<string, int> table;

    switch (currentLexeme.tableNumb) {
    case 1:
        table = TW;
        break;
    case 2:
        table = TL;
        break;
    case 3:
        table = TN;
        isNumb = true;
        break;
    case 4:
        table = TI;
        isID = true;
        break;
    }
    for (const auto& pair : table) {
        if (pair.second == currentLexeme.valueNumb) {
            symbol = pair.first;
            //std::cout << lex << endl;
            break;
        }
    }
    currentLexemeIndex++;
}

// Функции для упрощения кода
//Проверка равенства S на нужную лексему
bool EQ(string S) {
    return symbol==S;
}

void checkAndAdvance(const std::string& expectedLexem) {
    if (EQ(expectedLexem)) {
        gl();
    }
    else {
        syntax_err_proc(expectedLexem);
    }
}

Lexeme getCurrentLexem() {
    return currentLexeme;
}

string getPrintSymbol() {
    return "['" + symbol + "']";
}

// Обработка ошибок
void syntax_err_proc(SyntaxErr err) {
    unsigned int line = currentLexeme.linePos;
    std::string error_message = "[SyntaxError] Line " + std::to_string(line) + ": ";

    switch (err) {
    case SyntaxErr::ExpectedType:
        error_message += "Expected type for Identifier";
        break;
    case SyntaxErr::UnexpectedLexem:
        error_message += "Unexpected lexem in body of program";
        break;
    case SyntaxErr::ExpectedIdentifier:
        error_message += "Expected identifier";
        break;
    case SyntaxErr::InvalidExpression:
        error_message += "Invalid Expression";
        break;
    case SyntaxErr::OutOfBounds:
        error_message += "Out of bounds lexeme access";
        break;
    default:
        error_message += "Unknown Error";
    }

    scanStatus = false;
    throw std::runtime_error(error_message);
}

void syntax_err_proc(std::string symbol) {
    unsigned int line = currentLexeme.linePos;
    std::string error_message = "[SyntaxError] Line " + std::to_string(line) +
        ": Missed lexem: '" + symbol + "'";
    scanStatus = false;
    throw std::runtime_error(error_message);
}

shared_ptr<Node> createNode(NodeType type, string description) {
    if (description == "") {
        return make_shared<Node>(
            type,
            getCurrentLexem(),
            getPrintSymbol());
    }
    else {
        return make_shared<Node>(
            type,
            description);
    }
}