#include "Statement.hpp"

std::string Statement::emit(int r) {
    std::string out = "";
    switch (stype){
        case StatementType::ASSIGN :
            // out = "\taddi \t$t" + std::to_string(r) + ", $gp, " + std::to_string(sym->getMemLoc()) + "\n";
            // out += "\tsw \t$t" + std::to_string(e->r) + ", 0($t" + std::to_string(r) + ")\n";
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