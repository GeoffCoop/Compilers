#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "../Type.hpp"
#include <memory>
#include <iostream>
class Expression 
{
public:
    Expression(){}
    Expression(std::shared_ptr<Type> t, int reg): r(reg), type(t) {}
    std::shared_ptr<Type> type;
    int getValue() {return value;}
    int r;
    virtual bool isLiteral() { return false; }
private:
    int value = 0;
};

class Literal : public Expression {
public:
    Literal(char c, int r): Expression(BuiltInType::getChar(), r){ value = static_cast<int>(c); }
    Literal(int i, int r): Expression(BuiltInType::getInt(), r) { value = value; }
    Literal(bool b, int r): Expression(BuiltInType::getBoolean(), r) { value = b; }
    int getVal() { return value; }
    bool isLiteral() { return true; }
private:
    int value;
};

#endif
