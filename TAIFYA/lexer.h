#pragma once

#include <string> 
#include <fstream> 

using namespace std;


enum sost {
	ER, // ������ 
	V,  // ����� 
	H,  // ������ 
	I,  // ������������� 
	C1, C2, C3, // ����������� 
	M1, // < <= <> 
	M2, // > >= 
	N2, // �������� ����� 
	N8, // ������������ ����� 
	N10,// ���������� ����� 
	N16,// ����������������� ����� 
	B,  // 'B' ��� 'b' 
	O,  // 'O' ��� 'o' 
	D,  // 'D' ��� 'd' 
	HX, // 'H' ��� 'h' 
	E11,// 'E' ��� 'e' 
	E12, E13, E21, E22, // ������� ����� 
	ZN, // ���� ������� 
	P1, // ����� 
	P2, // ������� ����� 
	OG
};// ������������ 

enum e_tables {
	TW = 1, // ������� ��������� ���� 
	TL = 2, // ������� ������������� 
	TN = 3, // ������� ����� 
	TI = 4
};// ������� ��������������� 
const int s_stack = 1000, n_tables = 6;
char CH, stack[s_stack];
string tbl[6][s_stack];
// ������� �����: ���� ����, 
// ���� �������������, �������������� ���� 
// ������������ ������� 
fstream fcin("input.txt"), f_slova("tbl_slova.txt"),
f_ogran("tbl_ogranichitel.txt");
ofstream  f_out("result_tbl.txt");
int u_stack, z,
s_tbl0, s_tbl1, s_tbl2, s_tbl3, s_tbl4, s_tbl5,
s_res_tbl,
// ������ ������� �� 
cur_res_tbl, // ������� ������� ������� �� 
n_m_err;// ���������� ������ 

void gc(char currentChar) {
	if (fcin.get(currentChar) && currentChar == '\n') currentLine++;
}