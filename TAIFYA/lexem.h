#pragma once
#include <vector>

using std::vector;


struct Lexeme {
	int tableNumb=0;
	int valueNumb=0;
	unsigned int linePos = 0;	// Для вывода ошибок на какой строке
};

extern vector<Lexeme> lexemes;