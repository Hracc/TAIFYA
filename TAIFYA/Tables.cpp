#include "tables.h"

unordered_map<string, int> TW = { 
    {"{", 1},{"}", 2},{"%", 3},{"!", 4},{"$", 5},{"let", 6},
    {"if", 7},{"then", 8},{"else", 9},{"end_else", 10},{"for", 11},{"do", 12},
    {"while", 13},{ "loop", 14 },{"input", 15},{"output", 16},{"true", 17},{"false", 18},
};

unordered_map<string, int> TL = {
    {"<>", 1},{"<", 2},{">", 3},{"=", 4},{"<=", 5},{">=", 6},
    {"+", 7},{"-", 8},{"or", 9},{"*", 10},{"/", 11},{"and", 12},
    {"not", 13},{ ";", 14 },{":", 15},{",", 16},{"(", 17},{")", 18},
    {"[", 19},{"]", 20},{" ", 21},{"/*", 22},{"*/", 23},
};

unordered_map<string, int>  TN;
unordered_map<string, int>  TI;
