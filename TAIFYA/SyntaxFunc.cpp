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
        err_proc(SyntaxErr::OutOfBounds);
        return;
    }
    isID = false;
    isNumb = false;

    int tableNumb, value;
    char comma;

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
        err_proc(expectedLexem);
    }
}

Lexeme getCurrentLexem() {
    return currentLexeme;
}

string getPrintSymbol() {
    return "['" + symbol + "']";
}

// Обработка ошибок
void err_proc(SyntaxErr err) {
    unsigned int line = currentLexeme.linePos;
    std::cout << "[SyntaxError] Line "<< line <<": ";
    switch (err) {
    case SyntaxErr::ExpectedType:
        std::cout << "Expected type for Identifier"<< endl;
        break;
    case SyntaxErr::UnexpectedLexem:
        std::cout << "Unexpected lexem in body of programm" << endl;
        break;
    case SyntaxErr::ExpectedIdentifier:
        std::cout << "Expected identifier" << endl;
        break;
    case SyntaxErr::InvalidExpression:
        std::cout << "Invalid Expression" << endl;
        break;
    case SyntaxErr::OutOfBounds:
        std::cerr << "Out of bounds lexeme access" << std::endl;
        scanStatus = false;
        break;
    default:
        std::cerr << "Unknown Error" << std::endl;
    }
    scanStatus = false;
}

void err_proc(string symbol) {
    unsigned int line = currentLexeme.linePos;
    std::cout << "[SyntaxError] Line " << line << ": ";
    std::cout << "Missed lexem: '"<< symbol <<"'" << endl;
    scanStatus = false;
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