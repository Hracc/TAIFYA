#include "lexer.h"
#include "lexem.h"
#include "tables.h"


using std::ofstream;

unsigned int line = 1;
int z = 0;
string S;
char CH;
bool canRead = true;
bool wasPut = false;

ifstream fileInput("input.txt");
vector<Lexeme> lexemes;


//Чтение символа
void gc() {
	if (fileInput.get(CH)) {
		canRead = true;
	}
	else {
		canRead = false;
	}
}

//Проверка символа на  букву
bool let() {
	if (std::isalpha(CH)) {
		return true;
	}
	else {
		return false;
	}
}

//Проверка символа на число
bool digit() {
	if (std::isdigit(CH)) {
		return true;
	}
	else {
		return false;
	}
}
// Добавление символа в S
void add() {
	S.push_back(CH);
}
// Очистка S
void nill() {
	S.clear();
}
// Создание значения в таблице
void put(unordered_map<string, int>& table) {
	auto it = table.find(S);
	if (it == table.end()) {
		z = static_cast<int>(table.size() + 1);
		table[S] = z;
		if (printStatus) {
			std::cout << "\t put & ";
		}
		wasPut = true;
	}
	else {
		z = it->second;
	}
}
// Добавление значения в таблицу (функция out)
static string findKeyByValue(const unordered_map<string, int>& table, int value) {
	for (const auto& pair : table) {
		if (pair.second == value) {
			return pair.first;
		}
	}
	return ""; 
}

static void printLex(int t, int v) {
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
		if (printStatus) {
			if (wasPut) {
				std::cout << "out: (" << t << ", " << v << ") : " << key << endl;
				wasPut = false;
			}
			else {
				std::cout << "\t out: (" << t << ", " << v << ") : " << key << endl;
			}
		}
	}
	else {
		std::cout << "\t out: " << t << ", " << v << " : key not found" << endl;
	}
}

void out(int tableNumb, int valueNumb) {
	printLex(tableNumb, valueNumb);
	lexemes.push_back({ tableNumb, valueNumb, line });
}

//Поиск значение в таблице
void look(unordered_map<string, int>& table) {

	auto it = table.find(S);
	if (it != table.end()) {
		z = it->second;
	}
	else {
		z = 0;
	}
}

// Прочее функции для удобства
bool checkTL() {
	string STR;
	STR.push_back(CH);
	auto it1 = TL.find(STR);
	//auto it2 = TW.find(STR);
	if (it1 != TL.end() ) {
		z = it1->second;
	}
	else {
		z = 0;
	}
	if (z != 0 || STR=="\n" || STR == "\t" || STR == " " || STR == "}" && canRead) {
		return true;
	}
	else {
		return false;
	}
}
void saveLexemesToFile(const string& filename) {
	ofstream fileLexems(filename, std::ios::trunc);
	if (fileLexems.is_open()) {
		for (const auto& lexeme : lexemes) {
			fileLexems << lexeme.tableNumb << ", " << lexeme.valueNumb << endl;
		}
		fileLexems.close();
	}
	else {
		std::cerr << "Err open file" << endl;
	}
}

void reportErr(LexerErr errorType) {
	std::cerr << "[LexError] Line " << line << ": ";
	switch (errorType) {
	case LexerErr::MissingClosingBrace:
		std::cerr << "Missing closing brace '}'." << std::endl;
		break;
	case LexerErr::MissingClosingComment:
		std::cerr << "Missing closing brace '*/'." << std::endl;
		break;
	case LexerErr::InvalidNumberFormat:
		std::cerr << "Invalid number format" << std::endl;
		break;
	default:
		std::cerr << "Unknown error." << std::endl;
		break;
	}
}

void reportErr(LexerErr errortype, char CH) {
	switch (errortype) {
	case LexerErr::UnknownSymbol:
		std::cerr << "Unknown symbol: '" << CH << "'" << std::endl;
		break;
	}
}
