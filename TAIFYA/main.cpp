#include <iostream>

#include "lexer.h"

int main() {
	if (scan()) {
		saveLexemesToFile("lexems.txt");
		std::cout<< "Lexer: Success"<< std::endl;
	}
	else {
		std::cout << "Lexer: Error" << std::endl;
	}

	return 0;
}