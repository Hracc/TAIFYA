#include <iostream>
#include <sstream>

#include "syntax.h"
#include "tables.h"

vector<std::pair<int, int>> lexems;
ifstream lexFile("lexems.txt");
bool isID, isNumb;
//Чтение лексем
void gl() {
    unordered_map<string, int> table;
    isID = false;
    isNumb = false;

    if (!lexFile.is_open()) {
        std::cerr << "Err1:" << std::endl;
    }
    int tableNumb, value;
    char comma;
    if (lexFile >> tableNumb >> comma >> value) {
        switch (tableNumb) {
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
            if (pair.second == value) {
                lex = pair.first;
                std::cout << lex << endl;
            }
        }
    }
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