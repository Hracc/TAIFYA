#include <iostream>

#include "lexer.h"

int main() {
	if (scan()) {
		std::cout<< "Lexer: Success"<< std::endl;
	}
	else {
		std::cout << "Lexer: Error" << std::endl;
	}
	saveLexemesToFile("lexems.txt");


	return 0;
}