#ifndef LVALUE_HPP
#define LVALUE_HPP

#include <string>
#include "Expressions/Expression.hpp"
#include "Type.hpp"

class LValue
{
public:
    std::string id;
    int base;
    int expr;
    std::shared_ptr<Type> type;
};

class IDLValue : public LValue
{
public:
    IDLValue(char* ident, std::shared_ptr<Type> t) {
        id = ident;
	type = t;
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
