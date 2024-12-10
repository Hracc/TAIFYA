#include <iostream>

#include "syntax.h"
#include "syntaxTree.h"


bool scanStatus = true;
string lex;

// Прототипы функций:
//Программ
std::shared_ptr<Node> PR();
std::shared_ptr<Node> BODY();
//Описание
std::shared_ptr<Node> DESCR();
// Идентификаторы
std::shared_ptr<Node> ID();
//Оператор
std::shared_ptr<Node> OPER();
// Для Оператора
void SOSTAVNOY();
std::shared_ptr<Node> PRISV();
void IF();
void FOR();
void DOWHILE();
void INPUT();
void OUTPUT();
// Пр. для Операторов
std::shared_ptr<Node> VIRAGENIYA();
std::shared_ptr<Node> OPERAND();
std::shared_ptr<Node> SLAG();
std::shared_ptr<Node> MNOG();



std::shared_ptr<Node> root;

//Начало синтаксического анализа
bool syntaxScan() {
	std::cout << "Syntax: Begin" << std::endl;

	std::shared_ptr<Node> RunProgramm = PR();

	if (scanStatus) {
		Node::printRoot(RunProgramm);
	}

	return scanStatus;
}

//Начало и конец программы
std::shared_ptr<Node> PR() {
	gl();

	root = std::make_shared<Node>(
		NodeType::PROGRAM, 
		getCurrentLexem(),
		"Programm"
	);

	if (EQ("{")) {
		gl();

		while (!EQ("}") && scanStatus) {
			std::shared_ptr<Node> child = BODY();
			if (child) {
				root->addChild(child);
			}
		}
		if (!EQ("}")) {
			err_proc("}");
		}
	}
	else {
		err_proc("{");
	}

	return root;
}

//Тело программы
std::shared_ptr<Node> BODY() {
	std::shared_ptr<Node> body;
	if (isID) {
		body = DESCR();
		if (EQ(";")) {
			gl();
		}
		else {
			err_proc(";");
		}
	}
	else if (EQ("[") || 
		EQ("let") || EQ("if") ||
		EQ("for") || EQ("do") || 
		EQ("input") || EQ("output")) {
		body = OPER();
		if (EQ(";")) {
			gl();
		}
		else {
			err_proc(";");
		}
	}
	else {
		err_proc(SyntaxErr::UnexpectedLexem);
	}
	//gl();
	return body;
}

//Описание
std::shared_ptr<Node> DESCR() {
	std::shared_ptr<Node> declarationNode = std::make_shared<Node>(
		NodeType::DECLARATION,
		getCurrentLexem(),
		"declaration");
	
	declarationNode->addChild(ID());
	std::shared_ptr<Node> typeNode;

	gl();

	while (!EQ(":") && scanStatus) {
		if (EQ(",")) {
			gl();
			if (isID) {
				//std::cout << lex << endl;
				declarationNode->addChild(ID());
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
		if (EQ("%")) {
			typeNode = std::make_shared<Node>(
				NodeType::INT,
				getCurrentLexem(),
				"% (int)");
			gl();
		}
		else if (EQ("!")) {
			typeNode = std::make_shared<Node>(
				NodeType::DOUBLE,
				getCurrentLexem(),
				"! (double)");
			gl();
		}
		else if (EQ("$")) {
			typeNode = std::make_shared<Node>(
				NodeType::BOOL,
				getCurrentLexem(),
				"$ (bool)");
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
	declarationNode->addChild(typeNode);
	return declarationNode;
}

std::shared_ptr<Node> ID() {
	std::shared_ptr<Node> operNode = std::make_shared<Node>(
		NodeType::IDENTIFIER,
		getCurrentLexem(),
		"Operand(" + lex + ")"
	);
	return operNode;
}

// Операции
std::shared_ptr<Node> OPER() {
	std::shared_ptr<Node> oper;

	if (EQ("[")) {
		SOSTAVNOY();
	}
	else if (EQ("let")) {
		oper = PRISV();
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
	return oper;
}
// Список операций:
// Составнной
void SOSTAVNOY() {
	gl();
	do
	{
		OPER();
		if (EQ(";")) {
			gl();
		}
		else {
			err_proc(";");
		}
	} while (!EQ("]") && scanStatus);
	if (EQ("]")) {
		gl();
	}
	else {
		err_proc("]");
	}
}

// Присваивание
std::shared_ptr<Node> PRISV() {
	std::shared_ptr<Node> prisv= std::make_shared<Node>(
		NodeType::ASSIGNMENT,
		getCurrentLexem(),
		"Assigment(" + lex + ")"
	);
	gl();
	if (isID) {
		gl();
		if (EQ("=")) {
			gl();
			prisv->addChild(VIRAGENIYA());
		}
		else {
			err_proc("=");
		}
	}
	else {
		err_proc(SyntaxErr::ExpectedIdentifier);
	}
	return prisv;
}

// Условная операция
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


// Фиксированный цикл
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


// Условный цикл
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

// Операция ввода
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

// Операция вывода
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

std::shared_ptr<Node> VIRAGENIYA() {
	std::shared_ptr<Node> expression = std::make_shared<Node>(
		NodeType::EXPRESSION,
		getCurrentLexem(),
		"Expression(" + lex + ")"
	);

	expression->addChild(OPERAND());
	while (EQ("=") || EQ("<") || EQ("<>") || EQ("<=") || EQ(">") || EQ(">=")) {
		gl();
		expression ->addChild(OPERAND());
	}

	return expression;
}

std::shared_ptr<Node> OPERAND() {
	std::shared_ptr<Node> operand = std::make_shared<Node>(
		NodeType::OPERAND,
		getCurrentLexem(),
		"Operand(" + lex + ")"
	);

	operand->addChild(SLAG());
	while (EQ("+") || EQ("-") || EQ("or")) {
		gl();
		operand->addChild(SLAG());
	}
	return operand;
}


std::shared_ptr<Node> SLAG() {
	std::shared_ptr<Node> slag = std::make_shared<Node>(
		NodeType::SLAG,
		getCurrentLexem(),
		"Slagaemoe(" + lex + ")"
	);

	slag->addChild(MNOG());
	while (EQ("*") || EQ("/") || EQ("and")) {
		gl();
		slag->addChild(MNOG());
	}
	return slag;
}


std::shared_ptr<Node> MNOG() {
	std::shared_ptr<Node> slag = std::make_shared<Node>(
		NodeType::MNOG,
		getCurrentLexem(),
		"Mnog(" + lex + ")"
	);

	if (isID || isNumb || EQ("true") || EQ("false")) {
		gl();
	}
	else if (EQ("not")) {
		gl();
		slag->addChild(MNOG());
	}
	else if (EQ("(")) {
		gl();
		slag->addChild(VIRAGENIYA());
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
	return slag;
}