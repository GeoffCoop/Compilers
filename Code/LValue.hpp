#ifndef LVALUE_HPP
#define LVALUE_HPP

#include <string>
#include "Expressions/Expression.hpp"

class LValue
{
    
};

class IDLValue : public LValue
{
    IDLValue(char* ident);
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