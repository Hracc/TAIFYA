#include <fstream>
#include <iostream>

#include "lexer.h"
#include "lexem.h"
#include "tables.h"

bool lexScan() {
	char lowerCH;
	std::cout << endl << "Lexer: Start ============================================" << endl << endl;
	states CS;
	CS = H;
	int countBackup = 0;
	bool unskipSpace = false;
	bool isInput = false;
	bool isOutput = false;
	bool isBackup = false;
	gc();
	if (printLexStatus) {
		std::cout << "Line: 1" << endl;
	}
	while (CS != V) {
		switch (CS) {
// Стандартное состояниe
		case H:
			// Пропуск пустых символов и проверка на конец файла
			while (isspace(CH) && canRead) {
				if (CH == ' ') {
					if (unskipSpace) {
						out(2, 21);
					}
				}
				else if (CH == '\n') {
					line++;
					if (printLexStatus) {
						std::cout << "Line: " << line << endl;
					}
				}
				gc();
			}

			// Проверка на конец файла
			if (!canRead) {
				CS = ER;
				reportErr(LexerErr::MissingClosingBrace);
			}
			// Служебные слова и идентификаторы
			else if (let()) {
				nill();
				add();
				gc();
				CS = I; 
			}
			// Числа
			else if (CH == '0' || CH == '1') {
				nill();
				add();
				gc();
				CS = _2;
			}
			else if (CH >= '2' && CH <= '7') {
				nill();
				add();
				gc();
				CS = _8;
			}
			else if (CH >= '8' && CH <= '9') {
				nill();
				add();
				gc();
				CS = _10;
			}
			else if (CH == '.') {
				nill();
				add();
				gc();
				CS = RE;
			}
			else if (CH == '{') {
				nill();
				CS = BG;
			}
			// Меньше и похожие знаки длиною 2 символа
			else if (CH == '<') {
				CS = LS;
			}
			// Больше и похожие знаки длиною 2 символа
			else if (CH == '>') {
				CS = MR;
			}
			// Типы данных
			else if (CH == '%') {
				CS = INT;
			}
			else if (CH == '!') {
				CS = FLT;
			}
			else if (CH == '$') {
				CS = BL;
			}
			// Символ деления и начало комментария
			else if (CH == '/') {
				CS = DVD;
			}
			// Конечный символ программы
			else if (CH == '}') {
				out(1, 2);
				CS = V;
			}
			else if (CH == '(') {
				out(2, 17);
				if (isInput || isOutput) {
					isBackup = true;
					if (isOutput) {
						countBackup++;
					}
				}
				gc();
			}
			else if (CH == ')') {
				out(2, 18);
				if (isInput) {
					isInput = false;
					isBackup = false;
					unskipSpace = false;
				}
				if (isOutput) {
					countBackup--;
					if (countBackup == 0) {
						isOutput = false;
						isBackup = false;
						unskipSpace = false;
					}
				}
				gc();
			}
			// Ограничители из одного символа
			else {
				nill();
				add();
				look(TL);
				if (z == 0) {
					reportErr(LexerErr::UnknownSymbol, CH);
					CS = ER;
				}
				else
				{
					out(2, z);
					gc();
					CS = H;
				}
			}
			break;
// Начало обработки след символов
// Служебные слова и идентификаторы
		case I:
			while (let() || digit() || CH == '_') {
				add(); 
				gc(); 
			}
			look(TW);
			if (z != 0) { 
				if (z == 15) {
					isInput = true;
				}
				else if (z == 16) {
					isOutput = true;
				}
				out(1, z); 
			}
			else {
				look(TL);
				if (z != 0) {
					out(2, z);
				}
				else {
					put(TI);
					out(4, z);
					if (isInput || isOutput) {
						unskipSpace = true;
					}
				}
			}
			CS = H;
			break;
// Числа
		case _2:

			while (CH == '0' || CH == '1' && canRead) {
				add();
				gc();
			}

			lowerCH = tolower(CH);
			if (CH >= '2' && CH <= '7') {
				CS = _8;
			}
			else if (lowerCH == 'o') {
				add();
				gc();
				CS = _8E;
			}
			else if (CH >= '8' && CH <= '9') {
				CS = _10;
			}
			else if (lowerCH == 'd') {
				add();
				gc();
				CS = _10E;
			}
			else if (CH == '.') {
				add();
				gc();
				CS = RE;
			}
			else if (lowerCH == 'e') {
				add();
				gc();
				CS = EXP1;
			}
			else if (lowerCH == 'a' || lowerCH == 'c' || lowerCH == 'f') {
				CS = _16;
			}
			else if (lowerCH == 'h') {
				add();
				gc();
				CS = _16E;
			}
			else if (lowerCH == 'b') {
				add();
				gc();
				CS = _2E;
			}
			else if (checkTL()) {
				CS = _10E;
			}
			else {
				reportErr(LexerErr::InvalidNumberFormat);
				CS = ER;
			}
			break;
		case _8:
			while (CH >= '0' && CH <= '7' && canRead) {
				add();
				gc();
			}

			lowerCH = tolower(CH);


			if (CH >= '8' && CH <= '9') {
				CS = _10;
			}
			else if (lowerCH == 'd') {
				add();
				gc();
				CS = _10E;
			}
			else if (CH == '.') {
				add();
				gc();
				CS = RE;
			}
			else if (lowerCH == 'e') {
				add();
				gc();
				CS = EXP1;
			}
			else if (lowerCH == 'a' || lowerCH == 'b' || lowerCH == 'c' || lowerCH == 'f') {
				CS = _16;
			}
			else if (lowerCH == 'h') {
				add();
				gc();
				CS = _16E;
			}
			else if (lowerCH == 'o') {
				add();
				gc();
				CS = _8E;
			}
			else if (checkTL()) {
				CS = _10E;
			}
			else {
				reportErr(LexerErr::InvalidNumberFormat);
				CS = ER;
			}
			break;
		case _10:
			while (digit() && canRead) {
				add();
				gc();
			}

			lowerCH = tolower(CH);

			if (CH == '.') {
				add();
				gc();
				CS = RE;
			}
			else if (lowerCH == 'e') {
				add();
				gc();
				CS = EXP1;
			}
			else if (lowerCH == 'a' || lowerCH == 'b' || lowerCH == 'c' || lowerCH == 'f') {
				CS = _16;
			}
			else if (lowerCH == 'h') {
				add();
				gc();
				CS = _16E;
			}
			else if (lowerCH == 'd') {
				add();
				gc();
				CS = _10E;
			}
			else if (checkTL()) {
				CS = _10E;
			}
			else {
				reportErr(LexerErr::InvalidNumberFormat);
				CS = ER;
			}
			break;
		case _16:
			while ((digit() || CH >= 'a' && CH<='f' || CH >= 'A' && CH <= 'F') && canRead) {
				add();
				gc();
			}

			lowerCH = tolower(CH);

			if (lowerCH == 'h') {
				add();
				gc();
				CS = _16E;
			}
			else {
				reportErr(LexerErr::InvalidNumberFormat);
				CS = ER;
			}
			break;
		case _2E:
			lowerCH = tolower(CH);
			if (digit() || CH >= 'a' && CH <= 'f' || CH >= 'A' && CH <= 'F') {
				add();
				gc();
				CS = _16;
			}
			else if (lowerCH == 'h') {
				add();
				gc();
				CS = _16E;
			}
			else if (checkTL()) {
				put(TN);
				out(3, z);
				if (isOutput) { unskipSpace = true; }
				CS = H;
			}
			else {
				reportErr(LexerErr::InvalidNumberFormat);
				CS = ER;
			}
			break;
		case _8E:
			if (checkTL()) {
				put(TN);
				out(3, z);
				if (isOutput) { unskipSpace = true; }
				CS = H;
			}
			else {
				reportErr(LexerErr::InvalidNumberFormat);
				CS = ER;
			}
			break;
		case _10E:
			lowerCH = tolower(CH);
			if (digit() || CH >= 'a' && CH <= 'f' || CH >= 'A' && CH <= 'F') {
				add();
				gc();
				CS = _16;
			}
			else if (lowerCH == 'h') {
				add();
				gc();
				CS = _16E;
			}
			else if (checkTL()) {
				put(TN);
				out(3, z);
				if (isOutput) { unskipSpace = true; }
				CS = H;
			}
			else {
				reportErr(LexerErr::InvalidNumberFormat);
				CS = ER;
			}
			break;
		case _16E:
			if (checkTL()) {
				put(TN);
				out(3, z);
				if (isOutput) { unskipSpace = true; }
				CS = H;
			}
			else {
				reportErr(LexerErr::InvalidNumberFormat);
				CS = ER;
			}
			break;
// Действительные числа
		case RE:
			while (digit() && canRead) {
				add();
				gc();
			}
			lowerCH = tolower(CH);
			if (lowerCH == 'e') {
				add();
				gc();
				CS = EXP2;
			}
			else if (checkTL()) {
				put(TN);
				out(3, z);
				if (isOutput) { unskipSpace = true; }
				CS = H;
			}
			else {
				reportErr(LexerErr::InvalidNumberFormat);
				CS = ER;
			}
			break;

		case EXP1:
			lowerCH = CH;
			if (CH =='+' || CH == '-') {
				add();
				gc();
				while (digit() && canRead) {
					add();
					gc();
				}

				if (checkTL()) {
					put(TN);
					out(3, z);
					if (isOutput) { unskipSpace = true; }
					CS = H;
				}
				else {
					reportErr(LexerErr::InvalidNumberFormat);
					CS = ER;
				}
			}
			else if (digit()) {
				add();
				gc();
				while (digit() && canRead) {
					add();
					gc();
				}

				lowerCH = tolower(CH);

				if (digit() || CH >= 'a' && CH <= 'f' || CH >= 'A' && CH <= 'F') {
					add();
					gc();
					CS = _16;
				}
				else if (lowerCH == 'h') {
					add();
					gc();
					CS = _16E;
				}
				else if (checkTL()) {
					CS = H;
					put(TN);
					out(3, z);
					if (isOutput) { unskipSpace = true; }
				}
				else {
					reportErr(LexerErr::InvalidNumberFormat);
					CS = ER;
				}
				break;
			}
			else if (CH >= 'a' && CH <= 'f' || CH >= 'A' && CH <= 'F') {
				add();
				gc();
				CS = _16;
			}
			else if (CH == 'h' || CH == 'H') {
				add();
				gc();
				CS = _16E;
			}
			else if (checkTL() && digit()) {
				put(TN);
				out(3, z);
				if (isOutput) { unskipSpace = true; }
				CS = H;
			}
			else {
				reportErr(LexerErr::InvalidNumberFormat);
				CS = ER;
			}
			break;
		case EXP2:
			lowerCH = CH;
			if (CH == '+' || CH == '-' || digit()) {
				add();
				gc();
				while (digit() && canRead) {
					add();
					gc();
				}

				if (checkTL()) {
					put(TN);
					out(3, z);
					if (isOutput) { unskipSpace = true; }
					CS = H;
				}
				else {
					reportErr(LexerErr::InvalidNumberFormat);
					CS = ER;
				}
			}
			else {
				reportErr(LexerErr::InvalidNumberFormat);
				CS = ER;
			}
			break;
// Начальный символ программы
		case BG:
			out(1, 1);
			gc();
			CS = H;
			break;
// Типы данных
		case INT:
			out(1, 3);
			gc();
			CS = H;
			break;
		case FLT:
			out(1, 4);
			gc();
			CS = H;
			break;
		case BL:
			out(1, 5);
			gc();
			CS = H;
			break;
// Символ деления и  комментарии
		case DVD:
			gc();
			if (CH == '*') {
				CS = C1;
			}
			else {
				out(2, 11);
				CS = H;
			}
			break;
		case C1:
			gc();
			while (CH != '*' && canRead) {
				if (CH == '\n') {
					line++;
				}
				gc();
			}
			if (!canRead) {
				reportErr(LexerErr::MissingClosingComment);
				CS = ER;
			} 
			else {
				CS = C2;
			}
			break;
		case C2:
			gc();
			if (CH == '/') {
				CS = C3;
			}
			else {
				CS = C1;
			}
			break;
		case C3:
			gc();
			CS = H;
			break;
// Меньше и похожие знаки длиною 2 символа
		case LS:
			gc();
			if (CH == '>') {
				CS = LSN;
			}
			else if (CH == '=') {
				CS = LSE;
			}
			else {
				out(2, 2);
				CS = H;
			}
			break;
		case LSN:
			gc();
			out(2, 1);
			CS = H;
			break;
		case LSE:
			gc();
			CS = H;
			out(2, 5);
			break;
// Больше и похожие знаки длиною 2 символа
		case MR:
			gc();
			if (CH == '=') {
				CS = MRE;
			}
			else {
				out(2, 3);
				CS = H;
			}
			break;
		case MRE:
			gc();
			out(2, 6);
			CS = H;
			break;
// Завершение программы
		case ER:
			return false;
		case V:
			std::cout.flush();
			return true;
		}
	}
}