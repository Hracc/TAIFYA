#include <iostream>
#include "semanticErr.h"

void semant_err_proc(SemantErr err, shared_ptr<Node> node, string symbol) {
        std::string error_message = "[SemantError] Line " + std::to_string(node->lexem.linePos) + ": ";
        switch (err) {
        case SemantErr::IdAlreadyDeclared:
            error_message += "Identifier '"+ symbol +"' is already declared";
            break;
        case SemantErr::IdNotDeclared:
            error_message += "Identifier '" + symbol + "' is not declared";
            break;
        case SemantErr::InvalidCondition:
            error_message += "Invalid Condition Type";
            break;
        case SemantErr::InvalidAssigment:
            error_message += "Invalid Assigment Type";
            break;
        case SemantErr::UnexpectedNode:
            error_message += "Unexpected node structure";
            break;
        case SemantErr::DifferentType:
            error_message += "Different type";
            break;
        default:
            error_message += "Unknown Error";
        }
        throw std::runtime_error(error_message);
}