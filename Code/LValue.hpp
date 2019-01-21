#ifndef LVALUE_HPP
#define LVALUE_HPP

#include <string>
#include "Expressions/Expression.hpp"
#include "Type.hpp"
#include "SymbolTable.hpp"

class LValue
{
public:
    LValue(std::shared_ptr<SymbolTable> t): table(t) {}
    std::string id;
    int base;
    int expr;
    std::shared_ptr<Type> type;
    virtual bool isConst(){};
    std::shared_ptr<SymbolTable> table;
};

class IDLValue : public LValue
{
public:
    IDLValue(char* ident, std::shared_ptr<Type> t, std::shared_ptr<SymbolTable> st): LValue(st) {
        id = ident;
	type = t;
    }
    bool isConst() {
    	return false; // TODO complete this!!!
    }

private:
    
};

class MemberLValue : public LValue
{
    MemberLValue(LValue base, char* ident, std::shared_ptr<SymbolTable> t): LValue(t){};
    bool isConst() {
	return false;
    }
};

class ArrayAccessLValue : public LValue
{
public:
    ArrayAccessLValue(std::shared_ptr<LValue> base, std::shared_ptr<Expression> expr, std::shared_ptr<Type> type, std::shared_ptr<SymbolTable> t):LValue(t){};
    bool isConst() {
	return false;
    }
};

#endif
