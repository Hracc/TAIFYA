#pragma once

#include <string> 
#include <fstream> 

using namespace std;


enum sost {
	ER, // ошибка 
	V,  // выход 
	H,  // начало 
	I,  // идентификатор 
	C1, C2, C3, // комментарий 
	M1, // < <= <> 
	M2, // > >= 
	N2, // двоичное число 
	N8, // восьмеричное число 
	N10,// десятичное число 
	N16,// шестнадцатеричное число 
	B,  // 'B' или 'b' 
	O,  // 'O' или 'o' 
	D,  // 'D' или 'd' 
	HX, // 'H' или 'h' 
	E11,// 'E' или 'e' 
	E12, E13, E21, E22, // порядок числа 
	ZN, // знак порядка 
	P1, // точка 
	P2, // дробная часть 
	OG
};// ограничитель 

enum e_tables {
	TW = 1, // таблица служебных слов 
	TL = 2, // таблица ограничителей 
	TN = 3, // таблица чисел 
	TI = 4
};// таблица идентификаторов 
const int s_stack = 1000, n_tables = 6;
char CH, stack[s_stack];
string tbl[6][s_stack];
// внешние файлы: файл слов, 
// файл ограничителей, результирующий файл 
// лексического анализа 
fstream fcin("input.txt"), f_slova("tbl_slova.txt"),
f_ogran("tbl_ogranichitel.txt");
ofstream  f_out("result_tbl.txt");
int u_stack, z,
s_tbl0, s_tbl1, s_tbl2, s_tbl3, s_tbl4, s_tbl5,
s_res_tbl,
// размер таблицы ЛА 
cur_res_tbl, // текущий элемент таблицы ЛА 
n_m_err;// количество ошибок 

void gc(char currentChar) {
	if (fcin.get(currentChar) && currentChar == '\n') currentLine++;
}