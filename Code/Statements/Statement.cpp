#include "Statement.hpp"

std::string Statement::emit(int r) {
    std::string out = "";
    switch (stype){
        case StatementType::ASSIGN :
        break;
        case StatementType::IF :
        break;
        case StatementType::WHILE :
        break;
        case StatementType::REPEAT :
        break;
        case StatementType::FOR :
        break;
        case StatementType::STOP :
        break;
        case StatementType::RETURN :
        break;
        case StatementType::READ :
        break;
        case StatementType::WRITE :
        break;
    }

    return out;
}