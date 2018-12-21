#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "../Type.hpp"
#include <memory>

class Expression 
{
public:
    Expression(std::shared_ptr<Type> t, int r): reg(r), type(t) {}
    std::shared_ptr<Type> type;
    int reg;
};

#endif