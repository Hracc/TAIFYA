#pragma once
#include <vector>

using std::vector;


struct Lexeme {
	int tableNumb=0;
	int valueNumb=0;
	unsigned int linePos;	// ��� ������ ������ �� ����� ������
};

extern vector<Lexeme> lexemes;