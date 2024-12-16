#include <iostream>

#include "syntax.h"
#include "syntaxTree.h"

#include "semantic.h"




bool scanStatus = true;
string symbol;

// Прототипы функций:
//Программ
shared_ptr<Node> PROGRAMM();
shared_ptr<Node> BODY();
//Описание
shared_ptr<Node> DECLARATION(shared_ptr<Node> id);
// Идентификаторы
shared_ptr<Node> SYMBOL();
//Оператор
shared_ptr<Node> OPERATOR();
// Для Оператора
shared_ptr<Node> SOSTAVNOY();
shared_ptr<Node> ASSIGNMENT(shared_ptr<Node> id);
shared_ptr<Node> IF_ELSE();
shared_ptr<Node> FOR();
shared_ptr<Node> DO_WHILE();
shared_ptr<Node> INPUT();
shared_ptr<Node> OUTPUT();
// Пр. для Операторов
shared_ptr<Node> EXPRESSION();
shared_ptr<Node> OPERAND();
shared_ptr<Node> TERM();
shared_ptr<Node> FACTOR();
// Обработка описания и присваивания
shared_ptr<Node> DECL_OR_ASSIGM();




//Начало синтаксического анализа
bool syntaxScan() {
	std::cout << endl << "Syntax: Start ============================================" << endl << endl;

	shared_ptr<Node> RunProgramm = PROGRAMM();

	if (scanStatus) {
		Node::printRoot(RunProgramm);
	}

	return scanStatus;
}

//Начало и конец программы
shared_ptr<Node> PROGRAMM() {
	gl();

	shared_ptr<Node> programmNode = createNode(NodeType::PROGRAM,"Programm");

	if (EQ("{")) {
		gl();

		while (!EQ("}") && scanStatus) {
			shared_ptr<Node> child = BODY();
			if (child) {
				programmNode->addChild(child);
			}
		}
		if (!EQ("}")) {
			err_proc("}");
		}
	}
	else {
		err_proc("{");
	}

	return programmNode;
}

//Тело программы
shared_ptr<Node> BODY() {
	shared_ptr<Node> body;

	if (isID) {
		while (!EQ(";") && scanStatus) {
			body = DECL_OR_ASSIGM();
		}
		if (EQ(";") && scanStatus) {
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
		body = OPERATOR();
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
	return body;
}

// Функция для обработки одинакового начала у
// описания и присваивания.
shared_ptr<Node> DECL_OR_ASSIGM() {
	shared_ptr<Node> descrorprisv;
	shared_ptr<Node> id = SYMBOL();
	gl();

	if (EQ(",") || EQ(":")) {
		descrorprisv = DECLARATION(id);
		checkDeclaration(descrorprisv);
	}
	else if (EQ("=")) {
		gl();
		descrorprisv = ASSIGNMENT(id);
	}
	else {
		err_proc(SyntaxErr::UnexpectedLexem); // Временно такая обработка
	}
	return descrorprisv;
}

//Описание
shared_ptr<Node> DECLARATION(shared_ptr<Node> id) {
	shared_ptr<Node> declarationNode = createNode(NodeType::DECLARATION,"Declaration");
	declarationNode->addChild(id);
	shared_ptr<Node> typeNode;
	while (!EQ(":") && scanStatus) {
		if (EQ(",")) {
			gl();
			if (isID) {
				declarationNode->addChild(SYMBOL());
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
			typeNode = createNode(NodeType::INT);
			gl();
		}
		else if (EQ("!")) {
			typeNode = createNode(NodeType::DOUBLE);
			gl();
		}
		else if (EQ("$")) {
			typeNode = createNode(NodeType::BOOL);
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
shared_ptr<Node> OPERATOR() {
	shared_ptr<Node> oper;

	if (EQ("[")) {
		oper = SOSTAVNOY();
	}
	else if (EQ("let")) {
		oper = ASSIGNMENT(nullptr);
		checkAssigment(oper);
	}
	else if (isID) {
		shared_ptr<Node> id = SYMBOL();
		oper = ASSIGNMENT(id);
	}
	else if (EQ("if")) {
		oper = IF_ELSE();
	}
	else if (EQ("for")) {
		oper = FOR();
	}
	else if (EQ("do")) {
		oper = DO_WHILE();
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
shared_ptr<Node> SOSTAVNOY() {
	shared_ptr<Node> compoundNode = createNode(NodeType::COMPOUND,"Compound");
	gl();
	do
	{
		compoundNode->addChild(OPERATOR());
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
	return compoundNode;
}

// Присваивание
shared_ptr<Node> ASSIGNMENT(shared_ptr<Node> id) {
	shared_ptr<Node> assigmentNode = createNode(NodeType::ASSIGNMENT,"Assigment");
	if (id == nullptr) {
		gl();
		if (isID) {
			assigmentNode->addChild(SYMBOL());
			gl();
			if (EQ("=")) {
				gl();
				assigmentNode->addChild(EXPRESSION());
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
		assigmentNode->addChild(id);
		assigmentNode->addChild(EXPRESSION());
	}
	return assigmentNode;
}

// Условная операция
shared_ptr<Node> IF_ELSE() {
	shared_ptr<Node> if_elseNode = createNode(NodeType::IF,"If_Else");

	shared_ptr<Node> ifNode = createNode(NodeType::IF,"If");
	if_elseNode->addChild(ifNode);

	gl();
	ifNode->addChild(EXPRESSION());
	if (EQ("then")) {
		shared_ptr<Node> thenNode = createNode(NodeType::THEN,"Then");
		if_elseNode->addChild(thenNode);

		gl();
		thenNode->addChild(OPERATOR());
		if (EQ("else")) {
			shared_ptr<Node> elseNode = createNode(NodeType::ELSE,"Else");
			if_elseNode->addChild(elseNode);

			gl();
			elseNode->addChild(OPERATOR());
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
shared_ptr<Node> FOR() {
	shared_ptr<Node> loop_for = createNode(NodeType::LOOP_FOR,"For");

	gl();
	if (EQ("(")) {
		gl();
		if (EQ(";")) {
			gl();
			if (EQ(";")) {
				gl();
			}
			else {
				loop_for->addChild(EXPRESSION());
				if (EQ(";")) {
					gl();
				}
				else {
					err_proc(";");
				}
			}
		}
		else {
			loop_for->addChild(EXPRESSION());
			if (EQ(";")) {
				gl();
				if (EQ(";")) {
					gl();
				}
				else {
					loop_for->addChild(EXPRESSION());
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
			loop_for->addChild(OPERATOR());
		}
		else {
			loop_for->addChild(EXPRESSION());
			if (EQ(")")) {
				gl();
				OPERATOR();
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
shared_ptr<Node> DO_WHILE() {
	shared_ptr<Node> do_while = createNode(NodeType::DO_WHILE,"Do_While");

	gl();
	if (EQ("while")) {
		gl();
		do_while->addChild(EXPRESSION());
		do_while->addChild(OPERATOR());
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
shared_ptr<Node> INPUT() {
	shared_ptr<Node> input = createNode(NodeType::INPUT,"Input");

	gl();
	if (EQ("(")) {
		gl();
		do
		{
			if (isID) {
				input->addChild(SYMBOL());
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
shared_ptr<Node> OUTPUT() {
	shared_ptr<Node> output = createNode(NodeType::OUTPUT,"Output");

	gl();
	if (EQ("(")) {
		gl();
		do
		{
			output->addChild(EXPRESSION());
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

shared_ptr<Node> EXPRESSION() {
	shared_ptr<Node> expression = createNode(NodeType::EXPRESSION,"Expression");

	expression->addChild(OPERAND());
	while (EQ("=") || EQ("<") || EQ("<>") ||
			EQ("<=") || EQ(">") || EQ(">=")) {
		expression->addChild(SYMBOL());
		gl();
		expression->addChild(OPERAND());
	}
	return expression;
}

shared_ptr<Node> OPERAND() {
	shared_ptr<Node> operand = createNode(NodeType::OPERAND,"Operand");
	operand->addChild(TERM());
	while (EQ("+") || EQ("-") || EQ("or")) {
		operand->addChild(SYMBOL());
		gl();
		operand->addChild(TERM());
	}
	return operand;
}


shared_ptr<Node> TERM() {
	shared_ptr<Node> slag = createNode(NodeType::TERM,"Slagaemoe");

	slag->addChild(FACTOR());
	while (EQ("*") || EQ("/") || EQ("and")) {
		slag->addChild(SYMBOL());
		gl();
		slag->addChild(FACTOR());
	}
	return slag;
}


shared_ptr<Node> FACTOR() {
	shared_ptr<Node> mnog = createNode(NodeType::FACTOR,"Mnog");

	if (isID || isNumb || EQ("true") || EQ("false")) {
		mnog->addChild(SYMBOL());
		gl();
	}
	else if (EQ("not")) {
		mnog->addChild(SYMBOL());
		gl();
		mnog->addChild(FACTOR());
	}
	else if (EQ("(")) {
		mnog->addChild(SYMBOL());
		gl();
		mnog->addChild(EXPRESSION());

		if (EQ(")")) {
			mnog->addChild(SYMBOL());
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

//!!! ДАННАЯ ФУНКЦИЯ МОЖЕТ БЫТЬ СОВРЕМЕНЕМ ИЗМЕНЕНА!!!
shared_ptr<Node> SYMBOL() {
	shared_ptr<Node> operNode;
	switch (getCurrentLexem().tableNumb) {
	case 1:
		if (EQ("true")) {
			operNode = createNode(NodeType::TRUE);
		}
		else if (EQ("false")) {
			operNode = createNode(NodeType::FALSE);
		}
		else {
			operNode = createNode(NodeType::WORD);
		}
		break;
	case 2:
		if(EQ("not")) {
			operNode = createNode(NodeType::NOT);
		}
		else if (EQ("and") || EQ("or")) {
			operNode = createNode(NodeType::BOOL_OPERATION);
		}
		else if (EQ("+") || EQ("-") || EQ("*") || EQ("/")) {
			operNode = createNode(NodeType::NUMB_OPERATION);
		}
		else if (EQ("<") || EQ(">") || EQ("=") || EQ("<>") || EQ("<=") || EQ(">=")) {
			operNode = createNode(NodeType::EXPR_OPERATION);
		}
		else {
			operNode = createNode(NodeType::LIMITER);
		}
		break;
	case 3:
		operNode = createNode(NodeType::NUMBER);
		break;
	case 4:
		operNode = createNode(NodeType::IDENTIFIER);
		break;
	default:
		break;
	}
	return operNode;
}