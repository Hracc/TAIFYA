#include <iostream>

#include "syntax.h"

bool scanStatus = true;
string lex;


// Прототипы фнукций
void PR();
void BODY();

void DESCR();

void OPER();
void SOSTAVNOY();
void PRISV();
void INPUT();
void OUTPUT();

void VIRAGENIYA();
void OPERAND();
void SLAG();
void MNOG();





//Начало синтаксического анализа
bool syntaxScan() {
	std::cout << "Syntax: Begin" << std::endl;
	PR();
	return scanStatus;
}

//Начало и конец программы
void PR() {
	gl();
	if (EQ("{")) {
		gl();
		BODY();
		if (!EQ("}")) {
			err_proc();
			std::cout << "err" << endl;
		}
	}
	else {
		std::cout << "err" << endl;

	}
}

//Тело программы
void BODY() {
	std::cout << "BODY" << endl;
	do {
		if (isID) {
			gl();
			DESCR();
			if (!EQ(";")) {
				err_proc();
				break;
			}
			continue;
		}
		else if (EQ("[") || EQ("let") || EQ("if") || EQ("for") || EQ("do") || EQ("input") || EQ("output")) {
			OPER();
			if (EQ(";")) {
				gl();
			}
			else {
				err_proc();
				break;
			}
			continue;
		}
		else if (EQ("{")) {
			err_proc();
			break;
		}
		else {
			std::cout << lex << endl;

			err_proc();
		}
		gl();
	} while (scanStatus && !EQ("}"));
}

//Описание
void DESCR() {
	std::cout << "DESCR" << endl;
	while (!EQ(":") && scanStatus) {
		if (EQ(",")) {
			gl();
			if (isID) {
				gl();
			} 
			else {
				err_proc();
			}
		}
		else {
			err_proc();
		}
	}
	if (scanStatus) {
		gl();
		if (EQ("%") || EQ("!") || EQ("$")) {
			gl();
		}
		else {
			err_proc();
		}
	}
}

// Операции
void OPER() {
	if (EQ("[")) {
		SOSTAVNOY();
	}
	else if (EQ("let")) {
		PRISV();
	}
	else if (EQ("input")) {
		INPUT();
	}
	else if (EQ("output")) {
		OUTPUT();
	}
	else {
		err_proc();
	}
}
void SOSTAVNOY() {
	gl();
	OPER();
	if (EQ("]")) {
		gl();
	}
	else {
		err_proc();
	}
}
void PRISV() {
	gl();
	if (isID) {
		gl();
		if (EQ("=")) {
			gl();
			VIRAGENIYA();
		}
		else {
			err_proc();
		}
	}
	else {
		err_proc();
	}
}

void INPUT() {
	gl();
	if (EQ("(")) {
		gl();
		do
		{
			if (isID) {
				gl();
			};
		} while (!EQ(")"));
		gl();
	}
	else {
		err_proc();
	}
}

void OUTPUT() {
	gl();
	if (EQ("(")) {
		gl();
		do
		{
			VIRAGENIYA();
		} while (!EQ(")"));
		gl();
	}
	else {
		err_proc();
	}
}

void VIRAGENIYA() {
	OPERAND();
	if (EQ("<") || EQ("<>") || EQ("<=") || EQ(">") || EQ(">=")) {
		gl();
		OPERAND();
	}
}

void OPERAND() {
	SLAG();
	if (EQ("+") || EQ("-") || EQ("or")) {
		gl();
		SLAG();
	}
}

void SLAG() {
	MNOG();
	if (EQ("*") || EQ("/") || EQ("and")) {
		gl();
		SLAG();
	}
}

void MNOG() {
	if (isID || isNumb || EQ("true") || EQ("false")) {
		gl();
	}
	else if (EQ("not")) {
		gl();
		MNOG();
	}
	else if (EQ("(")) {
		gl();
		VIRAGENIYA();
		if (EQ(")")) {
			gl();
		}
		else {
			err_proc();
		}
	}
	else {
		err_proc();
	}
}