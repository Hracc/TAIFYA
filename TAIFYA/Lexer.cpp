#include <iostream>
#include <fstream> 
#include <cctype> 
#include <string>

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

bool gc() {
    if (!fcin.eof()) {
        fcin.get(CH);
        return false; // Условие для продолжения
    }
    else {
        CH = '\0'; // Условие для конца файла
        return true; // Возвращаем true, если достигнут конец файла
    }
}


bool let() {
    return std::isalpha(CH);
}

// Очищает стек (начинает новую лексему)
void nill() {
    u_stack = 0;
}

// Добавляет текущий символ в стек для текущей лексемы
void add() {
    if (u_stack < s_stack - 1) {
        stack[u_stack++] = CH;
    }
    else {
        std::cerr << "Ошибка: Переполнение стека!" << std::endl;
    }
}

void out(int table, int index) {
    f_out << "Таблица: " << table << ", Индекс: " << index << std::endl;
}

bool digit() {
    return std::isdigit(CH);
}

void look(int table) {
    z = 0; // Инициализируем как "не найдено"

    std::string lexeme(stack, u_stack); // Создаем строку из символов в стеке

    for (int i = 0; i < s_stack && !tbl[table][i].empty(); ++i) {
        if (tbl[table][i] == lexeme) {
            z = i + 1; // Индексация начинается с 1
            break;
        }
    }
}

void put(int table) {
    std::string lexeme(stack, u_stack);  // Создаем строку из символов в стеке

    for (int i = 0; i < s_stack && !tbl[table][i].empty(); ++i) {
        if (tbl[table][i] == lexeme) {
            z = i + 1; // Если лексема найдена, возвращаем её индекс
            return;
        }
    }

    // Если не найдена, добавляем в таблицу
    tbl[table][s_tbl0++] = lexeme; // s_tbl0 - индекс следующего пустого места
    z = s_tbl0; // Возвращаем индекс новой лексемы
}

bool check_hex() {
    return (CH >= '0' && CH <= '9') ||
        (CH >= 'A' && CH <= 'F') ||
        (CH >= 'a' && CH <= 'f');
}

void translate(int base) {
    std::string lexeme(stack, u_stack);  // Преобразуем текущий стек в строку
    int value = 0;

    for (char ch : lexeme) {
        int digit = (std::isdigit(ch) ? ch - '0' : std::toupper(ch) - 'A' + 10);
        value = value * base + digit;
    }

    // Очистим стек и добавим преобразованное значение обратно
    u_stack = 0;
    while (value > 0) {
        stack[u_stack++] = (value % 10) + '0'; // Преобразуем обратно в строку
        value /= 10;
    }
    std::reverse(stack, stack + u_stack); // Инвертируем строку для правильного порядка
}

void convert() {
    std::string lexeme(stack, u_stack);  // Преобразуем текущий стек в строку
    double value = std::stod(lexeme);  // Используем `std::stod` для преобразования строки в число с плавающей точкой

    // Очистим стек и добавим преобразованное значение обратно
    u_stack = 0;
    std::string result = std::to_string(value);

    for (char ch : result) {
        if (ch == '.') break; // Удаляем незначащие нули после запятой
        stack[u_stack++] = ch;
    }
}

bool scanner() {
    sost CS;
    gc();
    CS = H;

    do {
        switch (CS) {
        case H: {
            while ((CH == ' ' || CH == '\n') && !fcin.eof())
                gc();
            if (fcin.eof())
                CS = ER;

            if (let()) {
                nill();
                add();
                gc();
                CS = I;
            }
            else if (CH == '0' || CH == '1') {
                nill();
                CS = N2;
                add();
                gc();
            }
            else if (CH >= '2' && CH <= '7') {
                nill();
                CS = N8;
                add();
                gc();
            }
            else if (CH >= '8' && CH <= '9') {
                nill();
                CS = N10;
                add();
                gc();
            }
            else if (CH == '.') {
                nill();
                add();
                gc();
                CS = P1;
            }
            else if (CH == '/') {
                gc();
                CS = C1;
            }
            else if (CH == '<') {
                gc();
                CS = M1;
            }
            else if (CH == '>') {
                gc();
                CS = M2;
            }
            else if (CH == '}') {
                out(2, 2);
                CS = V;
            }
            else {
                CS = OG;
            }
            break;
        }

        case I: {
            while (let() || digit()) {
                add();
                gc();
            }
            look(TW);
            if (z != 0) {
                out(1, z);
                CS = H;
            }
            else {
                put(TI);
                out(4, z);
                CS = H;
            }
            break;
        }

        case N2: {
            while (CH == '0' || CH == '1') {
                add();
                gc();
            }
            if (CH >= '2' && CH <= '7')
                CS = N8;
            else if (CH == '8' || CH == '9')
                CS = N10;
            else if (CH == 'A' || CH == 'a' || CH == 'C' || CH == 'c' || CH == 'F' || CH == 'f')
                CS = N16;
            else if (CH == 'E' || CH == 'e') {
                add();
                gc();
                CS = E11;
            }
            else if (CH == 'D' || CH == 'd') {
                add();
                gc();
                CS = D;
            }
            else if (CH == 'O' || CH == 'o')
                CS = O;
            else if (CH == 'H' || CH == 'h') {
                gc();
                CS = HX;
            }
            else if (CH == '.') {
                add();
                gc();
                CS = P1;
            }
            else if (CH == 'B' || CH == 'b') {
                add();
                gc();
                CS = B;
            }
            else if (let())
                CS = ER;
            else
                CS = N10;
            break;
        }

        case N8: {
            while (CH >= '2' && CH <= '7') {
                add();
                gc();
            }
            if (CH == '8' || CH == '9')
                CS = N10;
            else if (CH == 'A' || CH == 'a' || CH == 'B' || CH == 'b' || CH == 'C' || CH == 'c' || CH == 'F' || CH == 'f')
                CS = N16;
            else if (CH == 'E' || CH == 'e') {
                add();
                gc();
                CS = E11;
            }
            else if (CH == 'D' || CH == 'd') {
                add();
                gc();
                CS = D;
            }
            else if (CH == 'H' || CH == 'h') {
                gc();
                CS = HX;
            }
            else if (CH == '.') {
                add();
                gc();
                CS = P1;
            }
            else if (CH == 'O' || CH == 'o') {
                gc();
                CS = O;
            }
            else if (let())
                CS = ER;
            else
                CS = N10;
            break;
        }

        case N10: {
            while (CH == '8' || CH == '9') {
                add();
                gc();
            }
            if (CH == 'A' || CH == 'a' || CH == 'B' || CH == 'b' || CH == 'C' || CH == 'c' || CH == 'F' || CH == 'f')
                CS = N16;
            else if (CH == 'E' || CH == 'e') {
                add();
                gc();
                CS = E11;
            }
            else if (CH == 'H' || CH == 'h') {
                gc();
                CS = HX;
            }
            else if (CH == '.') {
                add();
                gc();
                CS = P1;
            }
            else if (CH == 'D' || CH == 'd') {
                add();
                gc();
                CS = D;
            }
            else if (let())
                CS = ER;
            else {
                put(TN);
                out(3, z);
                CS = H;
            }
            break;
        }

        case N16: {
            while (check_hex()) {
                add();
                gc();
            }
            if (CH == 'H' || CH == 'h') {
                gc();
                CS = HX;
            }
            else {
                CS = ER;
            }
            break;
        }

        case B: {
            if (check_hex())
                CS = N16;
            else if (CH == 'H' || CH == 'h') {
                gc();
                CS = HX;
            }
            else if (let())
                CS = ER;
            else {
                translate(2);
                put(TN);
                out(3, z);
                CS = H;
            }
            break;
        }

        case O: {
            if (let() || digit())
                CS = ER;
            else {
                translate(8);
                put(TN);
                out(3, z);
                CS = H;
            }
            break;
        }

        case D: {
            if (CH == 'H' || CH == 'h') {
                gc();
                CS = HX;
            }
            else if (check_hex())
                CS = N16;
            else if (let())
                CS = ER;
            else {
                put(TN);
                out(3, z);
                CS = H;
            }
            break;
        }

        case HX: {
            if (let() || digit())
                CS = ER;
            else {
                translate(16);
                put(TN);
                out(3, z);
                CS = H;
            }
            break;
        }

        case E11: {
            if (digit()) {
                add();
                gc();
                CS = E12;
            }
            else if (CH == '+' || CH == '-') {
                add();
                gc();
                CS = ZN;
            }
            else if (CH == 'H' || CH == 'h') {
                gc();
                CS = HX;
            }
            else if (check_hex()) {
                add();
                gc();
                CS = N16;
            }
            else {
                CS = ER;
            }
            break;
        }

        case ZN: {
            if (digit()) {
                add();
                gc();
                CS = E13;
            }
            else {
                CS = ER;
            }
            break;
        }

        case E12: {
            while (digit()) {
                add();
                gc();
            }
            if (check_hex())
                CS = N16;
            else if (CH == 'H' || CH == 'h') {
                gc();
                CS = HX;
            }
            else if (let())
                CS = ER;
            else {
                convert();
                put(TN);
                out(3, z);
                CS = H;
            }
            break;
        }

        case E13: {
            while (digit()) {
                add();
                gc();
            }
            if (let() || CH == '.')
                CS = ER;
            else {
                convert();
                put(TN);
                out(3, z);
                CS = H;
            }
            break;
        }

        case P1: {
            if (digit())
                CS = P2;
            else
                CS = ER;
            break;
        }

        case P2: {
            while (digit()) {
                add();
                gc();
            }
            if (CH == 'E' || CH == 'e') {
                add();
                gc();
                CS = E21;
            }
            else if (let() || CH == '.')
                CS = ER;
            else {
                convert();
                put(TN);
                out(3, z);
                CS = H;
            }
            break;
        }

        case E21: {
            if (CH == '+' || CH == '-') {
                add();
                gc();
                CS = ZN;
            }
            else if (digit())
                CS = E22;
            else
                CS = ER;
            break;
        }

        case E22: {
            while (digit()) {
                add();
                gc();
            }
            if (let() || CH == '.')
                CS = ER;
            else {
                convert();
                put(TN);
                out(3, z);
                CS = H;
            }
            break;
        }

        case C1: {
            if (CH == '*') {
                gc();
                CS = C2;
            }
            else {
                out(2, 16);
                CS = H;
            }
            break;
        }

        case C2: {
            int flag = 0;
            while (CH != '*' && !flag && CH != '}') {
                flag = gc();
            }
            if (CH == '}' || flag)
                CS = ER;
            else {
                gc();
                CS = C3;
            }
            break;
        }

        case C3: {
            if (CH == '/') {
                gc();
                CS = H;
            }
            else {
                CS = C2;
            }
            break;
        }

        case M1: {
            if (CH == '>') {
                gc();
                out(2, 18);
                CS = H;
            }
            else if (CH == '=') {
                gc();
                out(2, 21);
                CS = H;
            }
            else {
                out(2, 20);
                CS = H;
            }
            break;
        }

        case M2: {
            if (CH == '=') {
                gc();
                out(2, 22);
                CS = H;
            }
            else {
                out(2, 19);
                CS = H;
            }
            break;
        }

        case OG: {
            nill();
            add();
            look(TL);
            if (z != 0) {
                gc();
                out(2, z);
                CS = H;
            }
            else {
                CS = ER;
            }
            break;
        }
        } // end switch
    } while (CS != V && CS != ER);

    return CS;
}

int main() {
    scanner();
}