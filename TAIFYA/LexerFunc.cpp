#include "lexer.h"
#include "tables.h"

using std::ofstream;

string S;
char CH;
ifstream fileInput("input.txt");

vector<Lexeme> lexemes;

int z = 0;


bool gc() {
	if (fileInput.get(CH)) {
		//std::cout << CH << std::endl;
		return true;
	}
	else {
		return false;
	}
}

bool let() {
	if (std::isalpha(CH)) {
		return true;
	}
	else {
		return false;
	}
}

bool digit() {
	if (std::isdigit(CH)) {
		return true;
	}
	else {
		return false;
	}
}

void add() {
	S.push_back(CH);
}

void nill() {
	S.clear();
}

void put(unordered_map<string, int>& table) {
	auto it = table.find(S);
	if (it == table.end()) {
		z = table.size() + 1;
		table[S] = z;
	}
	else {
		z = it->second;
	}
}

string findKeyByValue(const std::unordered_map<std::string, int>& table, int value) {
	for (const auto& pair : table) {
		//std::cout << pair.second << ", " << pair.first << std::endl;
		if (pair.second == value) {
			return pair.first;
		}
	}
	return ""; 
}

void printLex(int t, int v) {
	unordered_map<string, int> table;
	switch (t) {
	case 1:
		table = TW;
		break;
	case 2:
		table = TL;
		break;
	case 3:
		table = TN;
		break;
	case 4:
		table = TI;
		break;
	}
	string key = findKeyByValue(table, v);
	if (!key.empty()) {
		std::cout << "out: " << t << ", " << v << " : " << key << std::endl;
	}
	else {
		std::cout << "out: " << t << ", " << v << " : key not found" << std::endl;
	}
}
void out(int tableNumb, int valueNumb) {
	printLex(tableNumb, valueNumb);
	lexemes.push_back({ tableNumb, valueNumb });
}

void saveLexemesToFile(const string& filename) {
	ofstream fileLexems(filename, std::ios::trunc);
	if (fileLexems.is_open()) {
		for (const auto& lexeme : lexemes) {
			fileLexems << lexeme.tableNumb << ", " << lexeme.valueNumb << std::endl;
		}
		fileLexems.close();
	}
	else {
		std::cerr << "Err open file" << std::endl;
	}
}

void look(unordered_map<string, int>& table) {

	auto it = table.find(S);
	if (it != table.end()) {
		z = it->second;
	}
	else {
		z = 0;
	}
}

bool checkTL() {
	string STR;
	STR.push_back(CH);
	auto it = TL.find(STR);
	if (it != TL.end()) {
		z = it->second;
	}
	else {
		z = 0;
	}
	if (z != 0) {
		return true;
	}
	else {
		return false;
	}
}