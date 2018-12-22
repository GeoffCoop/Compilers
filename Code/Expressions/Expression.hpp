#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "../Type.hpp"
#include <memory>

class Expression 
{
public:
    Expression(std::shared_ptr<Type> t, int reg): r(reg), type(t) {}
    std::shared_ptr<Type> type;
    int r;
};

#endif