#include <iostream>

#include "syntax.h"

bool scanStatus = true;


string lex;
//���� ���������
void DESCR() {
	std::cout << "DESCR" << endl;
	while (!EQ(":") && scanStatus) {
		if (EQ(",")) {
			gl();
			if (isID) {
				gl();
			} else {
				scanStatus = false;
			}
		}
		else {
			scanStatus = false;
		}
	}
	if (scanStatus) {
		gl();
		if (EQ("%") || EQ("!") || EQ("$")) {
			gl();
		}
	}
}
void BODY() {
	std::cout << "BODY" << endl;
	do {
		if (isID) {
			gl();
			DESCR();
			if (!EQ(";")) {
				scanStatus = false;
				break;
			}
		}
		gl();
	} while (scanStatus && !EQ("}"));
}
//������ � ����� ���������
void PR() {
	gl();
	if (EQ("{")) {
		gl();
		BODY();
		if (!EQ("}")) {
			scanStatus = false;
			std::cout << "err" << endl;
		}
	}
	else {
		std::cout << "err" << endl;
		
	}
}
//������ ��������������� �������
bool syntaxScan() {
	std::cout << "Syntax: Begin" << std::endl;
	PR();
	return scanStatus;
}
