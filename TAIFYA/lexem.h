#pragma once
#include <vector>

using std::vector;


struct Lexeme {
	int tableNumb=0;
	int valueNumb=0;
};

extern vector<Lexeme> lexemes;