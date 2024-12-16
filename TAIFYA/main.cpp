#include <iostream>

#include "lexer.h"
#include "syntax.h"
#include "tables.h"

bool printStatus = true;


int main() {
	if (lexScan()) {
		std::cout<< endl << "Lexer: Success ============================================"<< endl << endl;
		saveLexemesToFile("lexems.txt");
		if (syntaxScan()) {
			std::cout << endl << "Syntax & Semantic: Success ============================================" << endl << endl;
		}
		else {
			std::cout << endl << "Syntax & Semantic: Failed ============================================" << endl << endl;
		}
	}
	else {
		std::cout << endl << "Lexer: Failed ============================================" << endl << endl;
	}


	return 0;
}