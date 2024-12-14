#pragma once
#include <vector>

using std::vector;


struct Lexeme {
	int tableNumb=0;
	int valueNumb=0;
	unsigned int linePos;	// Для вывода ошибок на какой строке
};

extern vector<Lexeme> lexemes;