#include <string>

#include "lexer.h"
#include "tables.h"

using std::string;

string S;
char CH;
std::ifstream fileInput("input.txt");

vector<Lexeme> lexemes;


bool gc() {
	if (fileInput.get(CH)) {
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

void out(int tableNumb, int valueNumb) {
	lexemes.push_back({ tableNumb, valueNumb });
}

void saveLexemesToFile(const string& filename) {
	std::ofstream fileLexems(filename, std::ios::trunc);
	if (fileLexems.is_open()) {
		for (const auto& lexeme : lexemes) {
			fileLexems << lexeme.tableNumber << ", " << lexeme.valueNumber << std::endl;
		}
		fileLexems.close();
	}
	else {
		std::cerr << "Err open file" << std::endl;
	}
}

int look(std::unordered_map<string, int>& table) {

	auto it = table.find(S);
	if (it != table.end()) {
		return it->second;
	}
	else {
		return 0;
	}
}