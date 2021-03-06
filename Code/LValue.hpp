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
    std::shared_ptr<LValue> base;
    std::shared_ptr<Expression> expr;
    std::shared_ptr<Type> type;
    virtual std::string getMemLoc(int r) { return "ERROR LVALUE GETMEMLOC!!!"; };
    virtual std::string emit() {};
    std::shared_ptr<SymbolTable> table;
};

class IDLValue : public LValue
{
public:
    IDLValue(char* ident, std::shared_ptr<Type> t, std::shared_ptr<SymbolTable> st): LValue(st) {
        id = ident;
	    type = t;
    }
    std::string getMemLoc(int r);
    std::string emit() {
        return "";
    }

private:
    
};

class MemberLValue : public LValue
{
public:
    MemberLValue(std::shared_ptr<LValue> b, char* ident, std::shared_ptr<SymbolTable> t): LValue(t){
        id = b->id;
        base = b;
        member = std::string(ident);
        if (base->type->name() == "array") {

            type = std::dynamic_pointer_cast<RecordType>(std::dynamic_pointer_cast<ArrayType>(b->type)->type)->getType(std::string(ident));
            offset = std::dynamic_pointer_cast<RecordType>(std::dynamic_pointer_cast<ArrayType>(b->type)->type)->getOffset(member);
        }
        else {
            type = std::dynamic_pointer_cast<RecordType>(b->type)->getType(std::string(ident));
            offset = std::dynamic_pointer_cast<RecordType>(b->type)->getOffset(member);
        }
    };
    std::string getMemLoc(int r);
    std::string emit() {
        return "";
    }
    std::string member;
    int offset;
};

class ArrayAccessLValue : public LValue
{
public:
    ArrayAccessLValue(std::shared_ptr<LValue> & b, std::shared_ptr<Expression> e, std::shared_ptr<SymbolTable> st): LValue(st)
    {
        id = b->id;
        type = b->type;
        base = b;
        expr = e;
    };
    std::string getMemLoc(int r);
    std::string emit(int) {return "";};
    
};

#endif
