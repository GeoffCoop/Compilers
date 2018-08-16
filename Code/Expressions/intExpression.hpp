#ifndef INTEXPR_HPP
#define INTEXPR_HPP

#include "Expression.hpp"

class IntExpression : public Expression 
{
    public:
    IntExpression(int a): m_value(a){}

    private:
    int m_value;
};

#endif