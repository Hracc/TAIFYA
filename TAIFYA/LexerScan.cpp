#include <fstream>
#include <iostream>

#include "lexer.h"
#include "tables.h"

bool scan() {
	char lowerCH = tolower(CH);
	std::cout << "Lexer: Start: " << std::endl;
	states CS;
	CS = H;
	gc();
	while (CS != V) {
		switch (CS) {
// Стандартное состояние
		case H:
			// Пропуск пустых символов и проверка на конец файла
			while (CH == ' ' || CH == '\n' || CH == '\t') {
				if (!gc()) {
					CS = ER;
				}
			}
			// Служебные слова и идентификаторы
			if (let()) {
				nill();
				add();
				gc();
				CS = I; 
			}
			// Числа
			else if (CH >= '0' && CH <= '1') {
				nill();
				add();
				gc();
				CS = _2;
			}
			else if (CH >= '2' && CH <= '7') {
				nill();
				add();
				gc();
				CS = _2;
			}
			else if (CH >= '8' && CH <= '9') {
				nill();
				add();
				gc();
				CS = _10;
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
			else if (CH == '$') {
				CS = FLT;
			}
			else if (CH == '!') {
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
			// Ограничители из одного символа
			else {
				CS = OG;
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
				out(1, z); 
				CS = H;
				break;
			}
			look(TL);
			if (z != 0) {
				out(2, z);
			}

			else {
				put(TI);
				out(4, z);
			}
			CS = H;
			break;
// Числа
		case _2:
			while (CH >= '2' && CH <= '7') {
				gc();
				add();
			}
			if (toupper(CH) == 'B') {
				add();
				CS = _2E;
			}
			else if (toupper(CH) == 'O') {
				add();
				CS = _8E;
			}
			else if (toupper(CH) == 'D') {
				add();
				CS = _10E;
			}
			else if (toupper(CH) == 'H') {
				add();
				CS = _16E;
			}
			else if (CH >= '2' && CH <= '7') {
				CS = _8;
			}
			else if (CH >= '8' && CH <= '9') {
				CS = _10;
			}
			else if (CH >= '8' && CH <= '9') {
				CS = _10;
			}
			else if (lowerCH == 'a' || lowerCH == 'c' || lowerCH == 'f') {
				CS = _16;
			}
			else {
				CS = _10E;
			}
			break;
		case _8:
			while (CH >= '0' && CH <= '7') {
				gc();
				add();
			}
			if (toupper(CH) == 'O') {
				add();
				CS = _8E;
			}
			else if (toupper(CH) == 'D') {
				add();
				CS = _10E;
			}
			else if (toupper(CH) == 'H') {
				add();
				CS = _16E;
			}
			else if (CH >= '8' && CH <= '9') {
				CS = _10;
			}
			else {
				add();
				CS = _10E;
			}
		case _10:
			while (digit()) {
				gc();
				add();
			}
			if (toupper(CH) == 'D') {
				add();
				CS = _10E;
			}
			else if (toupper(CH) == 'H') {
				add();
				CS = _16E;
			}
			else {
				add();
				CS = _10E;
			}
		case _16:
			while (digit() || lowerCH >= 'a' && lowerCH <= 'f') {
				gc();
				add();
			}
			if (toupper(CH) == 'H') {
				add();
				CS = _16E;
			}
			else {
				add();
				CS = _10E;
			}
		case _2E:
			gc();
			CS = H;
			put(TN);
			out(3, z);
			break;
		case _8E:
			gc();
			CS = H;
			put(TN);
			out(3, z);
			break;
		case _10E:
			CS = H;
			put(TN);
			out(3, z);
			break;
		case _16E:
			gc();
			CS = H;
			put(TN);
			out(3, z);
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
				CS = H;
				out(2, 11);
			}
			break;
		case C1:
			gc();
			while (CH != '*') {
				gc();
			}
			CS = C2;
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
				CS = H;
				out(2, 2);
			}
			break;
			case LSN:
				gc();
				CS = H;
				out(2, 1);
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
					CS = H;
					out(2, 3);
				}
				break;
			case MRE:
				gc();
				CS = H;
				out(2, 6);
				break;
// Завершение программы
		case ER:
			return false;
		case V:
			return true;
// Ограничители из одного символа
		case OG:
			int a = 0;
			nill(); add();
			look(TL);
			if (z==0) {
				CS = ER;
			} else
			{
				CS = H;
				gc();
				out(2, z);
			}
			break;
		}
	}
}