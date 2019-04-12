#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "../Type.hpp"
#include "../StringTable.hpp"

#include <memory>
#include <iostream>
#include <string>

enum ExpressionType {
    OR,
    AND,
    EQUAL,
    NOTEQUAL,
    LTE,
    GTE,
    GT,
    LT,
    MINUS,
    PLUS,
    MULT,
    DIV,
    MODULO,
    NOT,
    UMIN,
    FUNCCALL,
    CHR,
    ORD,
    PRED,
    SUCC,
    LVAL,
    CHRCONST,
    INTCONST,
    STRINGCONST,
    BOOLEANCONST,
    NONE
};

class Expression 
{
public:
    Expression(){}
    Expression(std::shared_ptr<Type> t, std::shared_ptr<Expression> c1, std::shared_ptr<Expression> c2, ExpressionType expt): 
        type(t), child1(c1), child2(c2), exp(expt),r(-1) {}
    Expression(std::shared_ptr<Type> t, std::function<std::string (int)> c, ExpressionType e):
        type(t), optCode(c), exp(e), r(-1) {}
    Expression(std::shared_ptr<Type> t, int reg, std::function<std::string (int)> c, ExpressionType e):
        type(t), r(reg), exp(e), optCode(c){}
    std::shared_ptr<Type> type;
    int getValue() {return value;}
    void setValue(int x) { value = x; }
    int r;
    virtual bool isLiteral() { return false; }
    std::string emit();
    ExpressionType exp;
    std::function<std::string (int)> optCode;
protected:
    std::shared_ptr<Expression> child1;
    std::shared_ptr<Expression> child2;
    int value = 0;
};

class Literal : public Expression {
public:
    Literal(char c): Expression(BuiltInType::getChar(), nullptr, nullptr, ExpressionType::CHRCONST){ value = static_cast<int>(c); }
    Literal(int i): Expression(BuiltInType::getInt(), nullptr, nullptr, ExpressionType::INTCONST) { value = i; }
    Literal(bool b): Expression(BuiltInType::getBoolean(), nullptr, nullptr, ExpressionType::BOOLEANCONST) { value = b; }
    Literal(char* c): Expression(BuiltInType::getString(), nullptr, nullptr, ExpressionType::STRINGCONST) { value = StringTable::instance()->addString(c); }
    std::string emit();
    bool isLiteral() { return true; }
};

class LValExpression : public Expression {
public:
    LValExpression(std::string access, std::shared_ptr<Type> t) {
        val = access;
        type = t;
    }
    std::string val;
    std::string emit();
};

#endif
