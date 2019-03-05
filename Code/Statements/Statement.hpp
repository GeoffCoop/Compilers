#ifndef STATEMENT_HPP
#define STATEMENT_HPP

#include <string>
#include <memory>
#include "../Expressions/Expression.hpp"

enum StatementType {
    ASSIGN, // Lval = expr
    IF,     // expr, SS
    WHILE,  // expr, SS
    REPEAT, // SS, expr
    FOR,    // ident = expr, expr, SS
    STOP,   // 
    RETURN, // expr
    READ,   // lVals
    WRITE   // Exprs
};

class Statement 
{
    std::shared_ptr<Expression> e;

    std::string emit(int r);
    StatementType stype;
};

#endif