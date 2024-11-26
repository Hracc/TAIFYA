#include <iostream>

#include "lexer.h"

int main() {
	if (scan()) {
		std::cout<< "Lexer: Success"<< std::endl;
	}
	else {
		std::cout << "Lexer: Failed" << std::endl;
	}
	saveLexemesToFile("lexems.txt");


	return 0;
}