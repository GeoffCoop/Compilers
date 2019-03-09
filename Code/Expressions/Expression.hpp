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
    Expression(std::shared_ptr<Type> t, int reg, std::shared_ptr<Expression> c1, std::shared_ptr<Expression> c2, ExpressionType expt): 
        r(reg), type(t), child1(c1), child2(c2), exp(expt) {}
    Expression(std::shared_ptr<Type> t, int reg, std::string c):
        r(reg), type(t), optCode(c), exp(ExpressionType::LVAL) {}
    std::shared_ptr<Type> type;
    int getValue() {return value;}
    void setValue(int x) { value = x; }
    int r;
    virtual bool isLiteral() { return false; }
    std::string emit();
protected:
    std::shared_ptr<Expression> child1;
    std::shared_ptr<Expression> child2;
    int value = 0;
    ExpressionType exp;
    std::string optCode = "";
};

class Literal : public Expression {
public:
    Literal(char c, int r): Expression(BuiltInType::getChar(), r, nullptr, nullptr, ExpressionType::CHRCONST){ value = static_cast<int>(c); }
    Literal(int i, int r): Expression(BuiltInType::getInt(), r, nullptr, nullptr, ExpressionType::INTCONST) { value = i; }
    Literal(bool b, int r): Expression(BuiltInType::getBoolean(), r, nullptr, nullptr, ExpressionType::BOOLEANCONST) { value = b; }
    Literal(char* c, int r): Expression(BuiltInType::getString(), r, nullptr, nullptr, ExpressionType::STRINGCONST) { value = StringTable::instance()->addString(c); }
    std::string emit();
    bool isLiteral() { return true; }
};

class LValExpression : public Expression {
public:
    LValExpression(std::string access, std::shared_ptr<Type> t, int reg) {
        val = access;
        type = t;
        r = reg;
    }
    std::string val;
    std::string emit();
};

#endif
