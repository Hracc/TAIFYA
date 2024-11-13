#include "tables.h"

unordered_map<string, int> TW = { 
    {"{", 1},
    {"}", 2}
};

unordered_map<string, int> TL = {
    {"<>", 1},
    {"<", 2},
    {">", 3},
    {"=", 4},
    {"<=", 5},
    {">=", 6},

    {"+", 7},
    {"-", 8},
    {"or", 9},
    
    {"*", 10},
    {"/", 11},
    {"and", 12},
    
    {"not", 13}
};