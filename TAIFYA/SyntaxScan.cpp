#include <iostream>

#include "syntax.h"

bool scanStatus = true;
string lex;

// Прототипы функций:
//Программ
void PR();
void BODY();
//Описание
void DESCR();
//Оператор
void OPER();
// Для Оператора
void SOSTAVNOY();
void PRISV();
void IF();
void FOR();
void DOWHILE();
void INPUT();
void OUTPUT();
// Пр. для Операторов
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
			err_proc("}");
		}
	}
	else {
		err_proc("{");
	}
}

//Тело программы
void BODY() {
	std::cout << "BODY" << endl;
	do {
		if (isID) {
			gl();
			DESCR();
			if (EQ(";")) {
				gl();
			}
			else {
				err_proc(";");
				break;
			}
			continue;
		}
		else if (EQ("[") || 
			EQ("let") || EQ("if") ||
			EQ("for") || EQ("do") || 
			EQ("input") || EQ("output")) {
			OPER();
			if (EQ(";")) {
				gl();
			}
			else {
				err_proc(";");
				break;
			}
			continue;
		}
		else {
			err_proc(SyntaxErr::UnexpectedLexem);
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
			}
		}
		else {
			err_proc(":");
		}
	}
	if (scanStatus) {
		gl();
		if (EQ("%") || EQ("!") || EQ("$")) {
			gl();
		}
		else {
			err_proc(SyntaxErr::ExpectedType);
		}
		if (EQ(";")) {
			gl();
		}	
		else{
			err_proc(";");
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
	else if (EQ("if")) {
		IF();
	}
	else if (EQ("for")) {
		FOR();
	}
	else if (EQ("do")) {
		DOWHILE();
	}
	else if (EQ("input")) {
		INPUT();
	}
	else if (EQ("output")) {
		OUTPUT();
	}
	else {
		err_proc(SyntaxErr::UnexpectedLexem);
	}
}
void SOSTAVNOY() {
	gl();
	OPER();
	if (EQ("]")) {
		gl();
	}
	else {
		err_proc("]");
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
			err_proc("=");
		}
	}
	else {
		err_proc(SyntaxErr::ExpectedIdentifier);
	}
}

void IF() {
	gl();
	VIRAGENIYA();
	if (EQ("then")) {
		gl();
		OPER();
		if (EQ("else")) {
			gl();
			OPER();
		}
		if (EQ("end_else")) {
			gl();
		}
		else {
			err_proc("end_else");
		}
	}
	else {
		err_proc("then");
	}
}

void FOR() {
	gl();
	if (EQ("(")) {
		gl();
		if (EQ(";")) {
			gl();
			if (EQ(";")) {
				gl();
			}
			else {
				VIRAGENIYA();
				if (EQ(";")) {
					gl();
				}
				else {
					err_proc(";");
				}
			}
		}
		else {
			VIRAGENIYA();
			if (EQ(";")) {
				gl();
				if (EQ(";")) {
					gl();
				}
				else {
					VIRAGENIYA();
					if (EQ(";")) {
						gl();

					}
					else {
						err_proc(";");
					}
				}
			}
			else {
				err_proc(";");
			}
		}
		if (EQ(")")) {
			gl();
			OPER();
		}
		else {
			VIRAGENIYA();
			if (EQ(")")) {
				gl();
				OPER();
			}
			else {
				err_proc(")");
			}
		}
	}
	else {
		err_proc("(");
	}
}

void DOWHILE() {
	gl();
	if (EQ("while")) {
		gl();
		VIRAGENIYA();
		OPER();
		if (EQ("loop")) {
			gl();
		}
		else {
			err_proc("loop");
		}
	}
	else {
		err_proc("while");
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
			}
			else {
				err_proc(SyntaxErr::ExpectedIdentifier);
				break;
			}
		} while (!EQ(")") && scanStatus);
		
		if (EQ(")")) {
			gl();
		}
		else {
			err_proc(")");
		}
	}
	else {
		err_proc("(");
	}
}

void OUTPUT() {
	gl();
	if (EQ("(")) {
		gl();
		do
		{
			VIRAGENIYA();
		} while (!EQ(")") && scanStatus);

		if (EQ(")")) {
			gl();
		}
		else {
			err_proc(")");
		}
	}
	else {
		err_proc("(");
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
			err_proc(")");
		}
	}
	else {
		err_proc(SyntaxErr::InvalidExpression);
	}
}