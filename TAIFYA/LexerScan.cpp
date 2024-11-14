#include <fstream>
#include <iostream>

#include "lexer.h"
#include "tables.h"

bool scan() {
	std::cout << "Lexer: Start: " << std::endl;
	states CS;
	CS = H;
	gc();
	while (CS != V) {
		switch (CS) {
// ����������� ���������
		case H:
			// ������� ������ �������� � �������� �� ����� �����
			while (CH == ' ' || CH == '\n' || CH == '\t') {
				if (!gc()) {
					CS = ER;
				}
			}
			// ��������� ����� � ��������������
			if (let()) {
				nill();
				add();
				gc();
				CS = I; 
			}
			// �����
			else if (CH >= '0' && CH <= '1') {
				CS = _2;
			}
			else if (CH >= '1' && CH <= '7') {
				CS = _2;
			}
			else if (CH >= '8' && CH <= '9') {
				CS = _10;
			}
			else if (CH == '{') {
				CS = BG;
			}
			// ������ � ������� ����� ������ 2 �������
			else if (CH == '<') {
				CS = LS;
			}
			// ������ � ������� ����� ������ 2 �������
			else if (CH == '>') {
				CS = MR;
			}
			// ���� ������
			else if (CH == '%') {
				CS = INT;
			}
			else if (CH == '$') {
				CS = FLT;
			}
			else if (CH == '!') {
				CS = BL;
			}
			// ������ ������� � ������ �����������
			else if (CH == '/') {
				CS = DVD;
			}
			// �������� ������ ���������
			else if (CH == '}') {
				out(1, 2);
				CS = V;
			}
			// ������������ �� ������ �������
			else {
				CS = OG;
			}
			break;
// ������ ��������� ���� ��������
// ��������� ����� � ��������������
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
// �����
		case _2:
			gc();
			while (CH >= '0' && CH <= '1' || toupper(CH) != 'B') {
				add();
				gc();
			}
			break;

// ��������� ������ ���������
		case BG:
			out(1, 1);
			gc();
			CS = H;
			break;
// ���� ������
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
// ������ ������� �  �����������
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
// ������ � ������� ����� ������ 2 �������
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
// ������ � ������� ����� ������ 2 �������
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
// ���������� ���������
		case ER:
			return false;
		case V:
			return true;
// ������������ �� ������ �������
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