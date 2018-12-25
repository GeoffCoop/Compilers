#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "../Type.hpp"
#include <memory>
#include <iostream>
class Expression 
{
public:
    Expression(std::shared_ptr<Type> t, int reg): r(reg), type(t) {}
    std::shared_ptr<Type> type;
    int r;
};

class Literal : public Expression {
public:
    Literal(char c){ value = static_cast<int>c; }
    Literal(int i) { value = value; }
    Literal(bool b) { value = b; }
    int getVal() { return value; }
private:
    int value;
}

#endif
