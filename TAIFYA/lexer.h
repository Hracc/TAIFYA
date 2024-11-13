#ifndef LEXER_H
#define LEXER_H

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

using std::string;
using std::unordered_map;
using std::vector;

struct Lexeme {
	int tableNumber;
	int valueNumber;
};

extern vector<Lexeme> lexemes;

enum states {
	H,
	V
};

bool scan();

extern char CH;
extern string S;
bool gc();

void saveLexemesToFile(const string& filename);

void add();
void nill();

void out(int tableNumb, int valueNumb);
int look(unordered_map<string, int>& table);

#endif