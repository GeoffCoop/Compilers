#ifndef STRINGEXPR_HPP
#define STRINGEXPR_HPP

#include "Expression.hpp"

class StringExpression : public Expression
{
    public:
    StringExpression( char* str) : m_value(str){}

    private:
    char* m_value;
    
};

#endif