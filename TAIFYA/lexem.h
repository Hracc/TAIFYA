#pragma once
#include <vector>

using std::vector;


struct Lexeme {
	int tableNumb;
	int valueNumb;
};

extern vector<Lexeme> lexemes;