#include <iostream>

#include "lexer.h"
#include "syntax.h"
#include "tables.h"

#include <chrono>

bool printLexStatus = true;
bool printSyntaxStatus = true;

int main() {
	auto start = std::chrono::high_resolution_clock::now();
	if (lexScan()) {
		std::cout<< endl << "Lexer: Success ============================================"<< endl << endl;
		//saveLexemesToFile("lexems.txt");
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
	auto end = std::chrono::high_resolution_clock::now();
	//std::cout << "Execution time: "
	//	<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
	//	<< " ms\n";

	return 0;
}