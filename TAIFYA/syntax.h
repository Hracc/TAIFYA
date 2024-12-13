#pragma once

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "lexem.h"


using std::string;
using std::unordered_map;
using std::vector;
using std::ifstream;
using std::endl;

extern string symbol;
extern bool scanStatus;
extern bool isID;
extern bool isNumb;

bool syntaxScan();
void gl();
bool EQ(string S);

Lexeme getCurrentLexem();
string getPrintSymbol();

void checkAndAdvance(const string& expectedLexem);

enum class SyntaxErr {
    ExpectedType,
    UnexpectedLexem,
    ExpectedIdentifier,
    InvalidExpression,
    OutOfBounds
};

void err_proc(string symbol);
void err_proc(SyntaxErr err);