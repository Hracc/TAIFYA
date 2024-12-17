#pragma once

#include<string>
#include"syntaxTree.h"

using std::string;

enum class SemantErr {
	IdAlreadyDeclared,
	IdNotDeclared,
	InvalidCondition,
	InvalidAssigment,
	DifferentType,
	UnexpectedNode
};

void semant_err_proc(SemantErr err, shared_ptr<Node> node, string symbol="");