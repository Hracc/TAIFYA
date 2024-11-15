#ifndef LEXER_H
#define LEXER_H

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

using std::string;
using std::unordered_map;
using std::vector;
using std::ifstream;

struct Lexeme {
	int tableNumb;
	int valueNumb;
};

extern vector<Lexeme> lexemes;

enum states {
	H,
	I,
	_2,
	_2E,
	_8,
	_8E,
	_10,
	_10E,
	_16,
	_16E,
	RE,
	EXP1,
	EXP2,
	BG,
	INT,
	FLT,
	BL,
	DVD,
	C1,
	C2,
	C3,
	LS,
	LSN,
	LSE,
	MR,
	MRE,
	V,
	ER,
	OG
};

extern int z;

bool scan();

extern char CH;
extern string S;

void saveLexemesToFile(const string& filename);



bool gc();

bool let();
bool digit();

void add();
void nill();

void put(unordered_map<string, int>& table);
void out(int tableNumb, int valueNumb);
void look(unordered_map<string, int>& table);

#endif