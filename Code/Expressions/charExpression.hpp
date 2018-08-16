#ifndef CHAREXPR_HPP
#define CHAREXPR_HPP

#include "Expression.hpp"

class CharExpression : public Expression
{
    public:
    CharExpression(char a): m_value(a){}

    private:
    char m_value;
};

#endif