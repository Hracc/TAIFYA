#include <iostream>
#include <sstream>

#include "syntax.h"
#include "tables.h"


vector<std::pair<int, int>> lexems;
ifstream lexFile("lexems.txt");
bool isID;

void gl() {
    unordered_map<string, int> table;
    isID = false;

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
