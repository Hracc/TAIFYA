#include <iostream>
#include <sstream>

#include "syntax.h"
#include "tables.h"

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
            lex = pair.first;
            //std::cout << lex << endl;
            break;
        }
    }
    currentLexemeIndex++;
}

// Функции для упрощения кода
//Проверка равенства S на нужную лексему
bool EQ(string S) {
    return lex==S;
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

// Обработка ошибок
void err_proc(SyntaxErr err) {
    switch (err) {
    case SyntaxErr::ExpectedType:
        std::cout << "[Err]: Expected type for Identifier"<< endl;
        break;
    case SyntaxErr::UnexpectedLexem:
        std::cout << "[Err]: Unexpected lexem in body of programm" << endl;
        break;
    case SyntaxErr::ExpectedIdentifier:
        std::cout << "[Err]: Expected identifier" << endl;
        break;
    case SyntaxErr::InvalidExpression:
        std::cout << "[Err]: Invalid Expression" << endl;
        break;
    case SyntaxErr::OutOfBounds:
        std::cerr << "[Err]: Out of bounds lexeme access" << std::endl;
        scanStatus = false;
        break;
    default:
        std::cerr << "[Err]: Unknown Error" << std::endl;
    }
    scanStatus = false;
}

void err_proc(string symbol) {
    std::cout << "[Err]:Missed lexem: '"<< symbol <<"'" << endl;
    scanStatus = false;
}