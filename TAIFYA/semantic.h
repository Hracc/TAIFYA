#pragma once
#include <string>
#include <unordered_map>
#include "syntaxTree.h"

using std::string;
using std::unordered_map;


struct IdLexeme
{
	string name;
	NodeType type= NodeType::UNKNOWN;
	int addr = 0;
};

void checkDeclaration(shared_ptr<Node> node);
void checkAssigment(shared_ptr<Node> node);
void checkCondition(shared_ptr<Node> node);
void checkLoopFor(shared_ptr<Node> node);
void checkInput(shared_ptr<Node> node);
void checkOutput(shared_ptr<Node> node);

extern unordered_map<int,IdLexeme> TI_semantic;
