#pragma once
#include <string>
#include <unordered_map>
#include "syntaxTree.h"

using std::string;
using std::unordered_map;


struct IdLexeme
{
	string name;
	bool descrt = false;
	NodeType type;
	int addr;
	
	//IdLexeme(string n, bool d)
	//	:name(n), descrt(d) {}
};

void checkDeclaration(shared_ptr<Node> node);

extern unordered_map<string,IdLexeme> TI_semantic;
