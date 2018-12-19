#ifndef LVALUE_HPP
#define LVALUE_HPP

#include <string>
#include "Expressions/Expression.hpp"

class LValue
{
public:
    std::string id;
    int base;
    int expr;
};

class IDLValue : public LValue
{
public:
    IDLValue(char* ident) {
        id = ident;
    }

private:
    
};

class MemberLValue : public LValue
{
    MemberLValue(LValue base, char* ident);
};

class ArrayAccesLValue : public LValue
{
    ArrayAccesLValue(LValue base, Expression expr);
};

#endif