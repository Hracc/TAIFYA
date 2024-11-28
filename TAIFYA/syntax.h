#pragma once

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

using std::string;
using std::unordered_map;
using std::vector;
using std::ifstream;
using std::endl;

extern string lex;
bool syntaxScan();
void gl();
bool EQ(string S);
bool ID();
extern bool isID;