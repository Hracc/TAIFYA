#pragma once
using std::string;
enum class ErrorType {
    MissingClosingBrace,
    InvalidNumberFormat,
    UnknownSymbol,
    MissingClosingComment
};
extern unsigned int line;
void reportErr(ErrorType errorType);
void reportErr(ErrorType errorType, char CH);
