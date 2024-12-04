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
using std::endl;

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
extern char CH;
extern string S;

// Прочее переменные
extern bool canRead;
extern bool printStatus;

//Лексический анализатор
bool lexScan();

//Чтение символа
void gc();

//Проверка символа на  букву или число
bool let();
bool digit();

// Управление S
void add();
void nill();

// Создание, добавление и поиск значения в таблицах
void put(unordered_map<string, int>& table);
void out(int tableNumb, int valueNumb);
void look(unordered_map<string, int>& table);

//Обработка ошибок
using std::string;
enum class LexerErr {
	MissingClosingBrace,
	InvalidNumberFormat,
	UnknownSymbol,
	MissingClosingComment
};
extern unsigned int line;
void reportErr(LexerErr errorType);
void reportErr(LexerErr errorType, char CH);

// Прочее функции для удобства
void saveLexemesToFile(const string& filename);
bool checkTL();
#endif