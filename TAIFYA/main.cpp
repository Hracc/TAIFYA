#include <iostream>

#include "lexer.h"
#include "syntax.h"
#include "tables.h"

bool printStatus = false;


int main() {
	if (lexScan()) {
		std::cout<< "Lexer: Success"<< std::endl;
		if (syntaxScan()) {
			std::cout << "Syntax: Success" << std::endl;
		}
		else {
			std::cout << "Syntax: Failed" << std::endl;
		}
	}
	else {
		std::cout << "Lexer: Failed" << std::endl;
	}
	saveLexemesToFile("lexems.txt");


	return 0;
}