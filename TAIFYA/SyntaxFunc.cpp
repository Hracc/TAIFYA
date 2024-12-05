#include <iostream>
#include <sstream>

#include "syntax.h"
#include "tables.h"
#include "lexem.h"

#include "tree.h"

size_t currentLexemeIndex = 0;
bool isID, isNumb;
//Чтение лексем
void gl() {
    isID = false;
    isNumb = false;

    int tableNumb, value;
    char comma;

    Lexeme& currentLexeme = lexemes[currentLexemeIndex];
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
            std::cout << lex << endl;
            break;
        }
    }
    currentLexemeIndex++;
}

bool EQ(string S) {
    return lex==S;
}

void err_proc(SyntaxErr err) {
    switch (err) {
    case SyntaxErr::ExpectedType:
        std::cout << "[Err]: Expected type for Identifier"<< endl;
    case SyntaxErr::UnexpectedLexem:
        std::cout << "[Err]: Unexpected lexem in body of programm" << endl;
    case SyntaxErr::ExpectedIdentifier:
        std::cout << "[Err]: Expected identifier" << endl;
    case SyntaxErr::InvalidExpression:
        std::cout << "[Err]: Invalid Expression" << endl;
    }

    scanStatus = false;
}

void err_proc(string symbol) {
    std::cout << "[Err]:Missed lexem: '"<< symbol <<"'" << endl;
    scanStatus = false;
}