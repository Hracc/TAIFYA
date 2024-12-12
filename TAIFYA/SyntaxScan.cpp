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
std::shared_ptr<Node> DESCR(std::shared_ptr<Node> id);
// Идентификаторы
std::shared_ptr<Node> OPERANDS();
//Оператор
std::shared_ptr<Node> OPER();
// Для Оператора
std::shared_ptr<Node> SOSTAVNOY();
//std::shared_ptr<Node> PRISV();
std::shared_ptr<Node> PRISV(std::shared_ptr<Node> id);
std::shared_ptr<Node> IF();
std::shared_ptr<Node> FOR();
std::shared_ptr<Node> DOWHILE();
std::shared_ptr<Node> INPUT();
std::shared_ptr<Node> OUTPUT();
// Пр. для Операторов
std::shared_ptr<Node> VIRAGENIYA();
std::shared_ptr<Node> OPERAND();
std::shared_ptr<Node> SLAG();
std::shared_ptr<Node> MNOG();

std::shared_ptr<Node> DESCRORPRISV();



std::shared_ptr<Node> root;

//Начало синтаксического анализа
bool syntaxScan() {
	std::cout << endl << "Syntax: Start ============================================" << endl << endl;

	std::shared_ptr<Node> RunProgramm = PR();

	if (scanStatus) {
		Node::printRoot(RunProgramm);
	}

	return scanStatus;
}

//Начало и конец программы
std::shared_ptr<Node> PR() {
	gl();

	Lexeme l;
	l.tableNumb = 0;
	l.valueNumb = 0;
	root = std::make_shared<Node>(
		NodeType::PROGRAM,
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
		body = DESCRORPRISV();
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

std::shared_ptr<Node> DESCRORPRISV() {
	std::shared_ptr<Node> descrorprisv;
	std::shared_ptr<Node> id = OPERANDS();
	gl();

	if (EQ(",") || EQ(":")) {
		gl();
		descrorprisv = DESCR(id);
	}
	else if (EQ("=")) {
		gl();
		descrorprisv = PRISV(id);
	}
	return descrorprisv;
}

//Описание
std::shared_ptr<Node> DESCR(std::shared_ptr<Node> id) {
	std::shared_ptr<Node> declarationNode = std::make_shared<Node>(
		NodeType::DECLARATION,
		"declaration");
	declarationNode->addChild(id);
	declarationNode->addChild(OPERANDS());

	std::shared_ptr<Node> typeNode;
	gl();

	while (!EQ(":") && scanStatus) {
		if (EQ(",")) {
			gl();
			if (isID) {
				//std::cout << lex << endl;
				declarationNode->addChild(OPERANDS());
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
				"['%'] (int)");
			gl();
		}
		else if (EQ("!")) {
			typeNode = std::make_shared<Node>(
				NodeType::DOUBLE,
				getCurrentLexem(),
				"['!'] (double)");
			gl();
		}
		else if (EQ("$")) {
			typeNode = std::make_shared<Node>(
				NodeType::BOOL,
				getCurrentLexem(),
				"['$'] (bool)");
			gl();
		}
		else {
			err_proc(SyntaxErr::ExpectedType);
		}
		if (EQ(";")) {
			gl();
		}
		else {
			err_proc(";");
		}
	}
	declarationNode->addChild(typeNode);
	return declarationNode;
}

// Операции
std::shared_ptr<Node> OPER() {
	std::shared_ptr<Node> oper;

	if (EQ("[")) {
		oper = SOSTAVNOY();
	}
	else if (EQ("let")) {
		oper = PRISV(nullptr);
	}
	else if (isID) {
		std::shared_ptr<Node> id = OPERANDS();
		oper = PRISV(id);
	}
	else if (EQ("if")) {
		oper = IF();
	}
	else if (EQ("for")) {
		oper = FOR();
	}
	else if (EQ("do")) {
		oper = DOWHILE();
	}
	else if (EQ("input")) {
		oper = INPUT();
	}
	else if (EQ("output")) {
		oper = OUTPUT();
	}
	else {
		err_proc(SyntaxErr::UnexpectedLexem);
	}
	return oper;
}
// Список операций:
// Составнной
std::shared_ptr<Node> SOSTAVNOY() {
	std::shared_ptr<Node> condition = std::make_shared<Node>(
		NodeType::CONDITION,
		"Condition"
	);

	gl();
	do
	{
		condition->addChild(OPER());
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
	return condition;
}

// Присваивание
std::shared_ptr<Node> PRISV(std::shared_ptr<Node> id) {
	std::shared_ptr<Node> prisv = std::make_shared<Node>(
		NodeType::ASSIGNMENT,
		"Assigment"
	);
	if (id == nullptr) {
		gl();
		if (isID) {
			prisv->addChild(OPERANDS());
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
	}
	else {
		prisv->addChild(id);
		//gl();
		prisv->addChild(VIRAGENIYA());
	}
	return prisv;
}

//std::shared_ptr<Node> PRISV(std::shared_ptr<Node> id) {
//	std::shared_ptr<Node> prisv = std::make_shared<Node>(
//		NodeType::ASSIGNMENT,
//		"Assigment"
//	);
//	std::cout << lex << endl;
//	prisv->addChild(id);
//	//gl();
//	prisv->addChild(VIRAGENIYA());
//
//	return prisv;
//}

// Условная операция
std::shared_ptr<Node> IF() {
	std::shared_ptr<Node> if_elseNode = std::make_shared<Node>(
		NodeType::IF,
		"If_Else"
	);

	std::shared_ptr<Node> ifNode = std::make_shared<Node>(
		NodeType::IF,
		"If"
	);
	if_elseNode->addChild(ifNode);


	gl();
	ifNode->addChild(VIRAGENIYA());
	if (EQ("then")) {
		std::shared_ptr<Node> thenNode = std::make_shared<Node>(
			NodeType::THEN,
			"Then"
		);
		if_elseNode->addChild(thenNode);

		gl();
		thenNode->addChild(OPER());
		if (EQ("else")) {
			std::shared_ptr<Node> elseNode = std::make_shared<Node>(
				NodeType::ELSE,
				"Else"
			);
			if_elseNode->addChild(elseNode);

			gl();
			elseNode->addChild(OPER());
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
	return if_elseNode;
}


// Фиксированный цикл
std::shared_ptr<Node> FOR() {
	std::shared_ptr<Node> loop_for = std::make_shared<Node>(
		NodeType::LOOP_FOR,
		"For"
	);

	gl();
	if (EQ("(")) {
		gl();
		if (EQ(";")) {
			gl();
			if (EQ(";")) {
				gl();
			}
			else {
				loop_for->addChild(VIRAGENIYA());
				if (EQ(";")) {
					gl();
				}
				else {
					err_proc(";");
				}
			}
		}
		else {
			loop_for->addChild(VIRAGENIYA());
			if (EQ(";")) {
				gl();
				if (EQ(";")) {
					gl();
				}
				else {
					loop_for->addChild(VIRAGENIYA());
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
			loop_for->addChild(OPER());
		}
		else {
			loop_for->addChild(VIRAGENIYA());
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
	return loop_for;
}


// Условный цикл
std::shared_ptr<Node> DOWHILE() {
	std::shared_ptr<Node> do_while = std::make_shared<Node>(
		NodeType::DO_WHILE,
		"Do_While"
	);

	gl();
	if (EQ("while")) {
		gl();
		do_while->addChild(VIRAGENIYA());
		do_while->addChild(OPER());
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
	return do_while;
}

// Операция ввода
std::shared_ptr<Node> INPUT() {
	std::shared_ptr<Node> input = std::make_shared<Node>(
		NodeType::INPUT,
		"Input"
	);

	gl();
	if (EQ("(")) {
		gl();
		do
		{
			if (isID) {
				input->addChild(OPERANDS());
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
	return input;
}

// Операция вывода
std::shared_ptr<Node> OUTPUT() {
	std::shared_ptr<Node> output = std::make_shared<Node>(
		NodeType::OUTPUT,
		"Output"
	);

	gl();
	if (EQ("(")) {
		gl();
		do
		{
			output->addChild(VIRAGENIYA());
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
	return output;
}

std::shared_ptr<Node> VIRAGENIYA() {
	std::shared_ptr<Node> expression = std::make_shared<Node>(
		NodeType::EXPRESSION,
		"Expression"
	);

	expression->addChild(OPERAND());
	while (EQ("=") || EQ("<") || EQ("<>") || EQ("<=") || EQ(">") || EQ(">=")) {
		expression->addChild(OPERANDS());
		gl();
		expression->addChild(OPERAND());
	}

	return expression;
}

std::shared_ptr<Node> OPERAND() {
	std::shared_ptr<Node> operand = std::make_shared<Node>(
		NodeType::OPERAND,
		"Operand"
	);

	operand->addChild(SLAG());
	while (EQ("+") || EQ("-") || EQ("or")) {
		operand->addChild(OPERANDS());
		gl();
		operand->addChild(SLAG());
	}
	return operand;
}


std::shared_ptr<Node> SLAG() {
	std::shared_ptr<Node> slag = std::make_shared<Node>(
		NodeType::SLAG,
		"Slagaemoe"
	);

	slag->addChild(MNOG());
	while (EQ("*") || EQ("/") || EQ("and")) {
		slag->addChild(OPERANDS());
		gl();
		slag->addChild(MNOG());
	}
	return slag;
}


std::shared_ptr<Node> MNOG() {
	std::shared_ptr<Node> mnog = std::make_shared<Node>(
		NodeType::MNOG,
		"Mnog"
	);

	if (isID || isNumb || EQ("true") || EQ("false")) {
		mnog->addChild(OPERANDS());

		gl();
	}
	else if (EQ("not")) {
		mnog->addChild(OPERANDS());

		gl();

		mnog->addChild(MNOG());
	}
	else if (EQ("(")) {
		mnog->addChild(OPERANDS());


		gl();

		mnog->addChild(VIRAGENIYA());

		if (EQ(")")) {
			mnog->addChild(OPERANDS());

			gl();
		}
		else {
			err_proc(")");
		}
	}
	else {
		err_proc(SyntaxErr::InvalidExpression);
	}
	return mnog;
}

// Доп. функции для синтаксического древа

std::shared_ptr<Node> OPERANDS() {
	std::shared_ptr<Node> operNode;

	switch (getCurrentLexem().tableNumb) {
	case 1:
		operNode = std::make_shared<Node>(
			NodeType::WORD,
			getCurrentLexem(),
			"['" + lex + "']");
		break;
	case 2:
		operNode = std::make_shared<Node>(
			NodeType::LIMITER,
			getCurrentLexem(),
			"['" + lex + "']");
		break;
	case 3:
		operNode = std::make_shared<Node>(
			NodeType::NUMBER,
			getCurrentLexem(),
			"['" + lex + "']");

			break;
	case 4:
		operNode = std::make_shared<Node>(
			NodeType::IDENTIFIER,
			getCurrentLexem(),
			"['" + lex + "']");
		break;
	default:
		break;
	}
	return operNode;
}
