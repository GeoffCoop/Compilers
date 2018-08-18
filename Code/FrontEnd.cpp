#include "FrontEnd.hpp"

#include <memory>
#include <vector>

#include "SymbolTable.hpp"
#include "Symbol.hpp"
#include "Expressions/Expression.hpp"
#include "LValue.hpp"


#include "Expressions/stringExpression.hpp"
#include "Expressions/intExpression.hpp"
#include "Expressions/charExpression.hpp"
#include "Expressions/add.hpp"
#include "Expressions/and.hpp"
#include "Expressions/chr.hpp"
#include "Expressions/div.hpp"
#include "Expressions/eq.hpp"
#include "Expressions/functionCall.hpp"
#include "Expressions/gt.hpp"
#include "Expressions/gte.hpp"
#include "Expressions/lt.hpp"
#include "Expressions/lte.hpp"
#include "Expressions/lvalueexpr.hpp"
#include "Expressions/mod.hpp"
#include "Expressions/mult.hpp"
#include "Expressions/ne.hpp"
#include "Expressions/not.hpp"
#include "Expressions/or.hpp"
#include "Expressions/ord.hpp"
#include "Expressions/pred.hpp"
#include "Expressions/sub.hpp"
#include "Expressions/succ.hpp"
#include "Expressions/umin.hpp"

template <typename T>
class NodeList
{
    public:
    NodeList() : list() {};
    int add(std::shared_ptr<T> node){
        list.push_back(node);
        return list.size() - 1;
    }
    std::shared_ptr<T> get(int i){
        if ( i < 0 || i > list.size() + 1) return nullptr;
        else return list[i];
    }

    private:
    std::vector<std::shared_ptr<T>> list;
};

class FrontEnd
{
    public:
    static std::shared_ptr<FrontEnd> instance(){
        if (fe) { return fe; }
        else { fe = std::make_shared<FrontEnd>(); }
    }

    void push_ST(){
        symbolTable = std::make_shared<SymbolTable>(symbolTable);
    }
    void pop_ST(){
        symbolTable = symbolTable->getParent();
    }

    std::shared_ptr<SymbolTable> getSymbolTable()   {
        return symbolTable;
    }

    //some list of expressions
    NodeList<Expression> expressions;
    NodeList<LValue> lValues;
    
    private:
    static std::shared_ptr<FrontEnd> fe;
    std::shared_ptr<SymbolTable> symbolTable;
};


#pragma region Expressions
int FE::StringExpr(char* x){
    auto fe = FrontEnd::instance();
    return fe->expressions.add(std::make_shared<StringExpression>(x));
}
int FE::IntExpr(int x){
    auto fe = FrontEnd::instance();
    return fe->expressions.add(std::make_shared<IntExpression>(x));
}
int FE::CharExpr(char x){
    auto fe = FrontEnd::instance();
    return fe->expressions.add(std::make_shared<CharExpression>(x));
}
// The following need to retrieve other expressions first
int FE::SuccExpr(int x){
    auto fe = FrontEnd::instance();
    auto exp = fe->expressions.get(x);
    return fe->expressions.add(std::make_shared<Succ>(exp));
}
int FE::PredExpr(int x){
    auto fe = FrontEnd::instance();
    auto exp = fe->expressions.get(x);
    return fe->expressions.add(std::make_shared<Pred>(exp));
}
int FE::ChrExpr(int x){
    auto fe = FrontEnd::instance();
    auto exp = fe->expressions.get(x);
    return fe->expressions.add(std::make_shared<Chr>(exp));
}
int FE::OrdExpr(int x){
    auto fe = FrontEnd::instance();
    auto exp = fe->expressions.get(x);
    return fe->expressions.add(std::make_shared<Ord>(exp));
}
int FE::UMinusExpr(int x){
    auto fe = FrontEnd::instance();
    auto exp = fe->expressions.get(x);
    return fe->expressions.add(std::make_shared<UMin>(exp));
}
int FE::NotExpr(int x){
    auto fe = FrontEnd::instance();
    auto exp = fe->expressions.get(x);
    return fe->expressions.add(std::make_shared<Not>(exp));
}
//////////////////////////////// Need to grab both expressions from list
int FE::ModExpr(int x, int y){
    auto fe = FrontEnd::instance();
    auto a = fe->expressions.get(x);
    auto b = fe->expressions.get(y);
    return fe->expressions.add(std::make_shared<Mod>(a,b));
}
int FE::DivExpr(int x, int y){
    auto fe = FrontEnd::instance();
    auto a = fe->expressions.get(x);
    auto b = fe->expressions.get(y);
    return fe->expressions.add(std::make_shared<Div>(a,b));
}
int FE::MultExpr(int x, int y){
    auto fe = FrontEnd::instance();
    auto a = fe->expressions.get(x);
    auto b = fe->expressions.get(y);
    return fe->expressions.add(std::make_shared<Mult>(a,b));
}
int FE::MinExpr(int x, int y){
    auto fe = FrontEnd::instance();
    auto a = fe->expressions.get(x);
    auto b = fe->expressions.get(y);
    return fe->expressions.add(std::make_shared<Sub>(a,b));
}
int FE::GTExpr(int x, int y){
    auto fe = FrontEnd::instance();
    auto a = fe->expressions.get(x);
    auto b = fe->expressions.get(y);
    return fe->expressions.add(std::make_shared<GT>(a,b));

}
int FE::PlusExpr(int x, int y){
    auto fe = FrontEnd::instance();
    auto a = fe->expressions.get(x);
    auto b = fe->expressions.get(y);
    return fe->expressions.add(std::make_shared<Add>(a,b));
}
int FE::LTExpr(int x, int y){
    auto fe = FrontEnd::instance();
    auto a = fe->expressions.get(x);
    auto b = fe->expressions.get(y);
    return fe->expressions.add(std::make_shared<Lt>(a,b));
}
int FE::GTEExpr(int x, int y){
    auto fe = FrontEnd::instance();
    auto a = fe->expressions.get(x);
    auto b = fe->expressions.get(y);
    return fe->expressions.add(std::make_shared<Gte>(a,b));
}
int FE::LTEExpr(int x, int y){
    auto fe = FrontEnd::instance();
    auto a = fe->expressions.get(x);
    auto b = fe->expressions.get(y);
    return fe->expressions.add(std::make_shared<Lte>(a,b));
}
int FE::NEExpr(int x, int y){
    auto fe = FrontEnd::instance();
    auto a = fe->expressions.get(x);
    auto b = fe->expressions.get(y);
    return fe->expressions.add(std::make_shared<NE>(a,b));
}
int FE::EQExpr(int x, int y){
    auto fe = FrontEnd::instance();
    auto a = fe->expressions.get(x);
    auto b = fe->expressions.get(y);
    return fe->expressions.add(std::make_shared<EQ>(a,b));
}
int FE::AndExpr(int x, int y){
    auto fe = FrontEnd::instance();
    auto a = fe->expressions.get(x);
    auto b = fe->expressions.get(y);
    return fe->expressions.add(std::make_shared<And>(a,b));
}
int FE::OrExpr(int x, int y){
    auto fe = FrontEnd::instance();
    auto a = fe->expressions.get(x);
    auto b = fe->expressions.get(y);
    return fe->expressions.add(std::make_shared<Or>(a,b));
}
int FE::LValueExpr(int x){
    auto fe = FrontEnd::instance();
    auto lval = fe->lValues.get(x);
    return fe->expressions.add(std::make_shared<LValueExpression>(lval));
}

int FE::CallFunction(char* x, int y){
    auto fe = FrontEnd::instance();

}

#pragma endregion

#pragma region LValues
int FE::LValID(char* id){
    auto fe = FrontEnd::instance();
    return fe->lValues.add(std::make_shared<IDLValue>(id));
}
int FE::LValMemberAccess(int base, char* ident){
    auto fe = FrontEnd::instance();
    auto lval = fe->lValues.get(base);
    return fe->lValues.add(std::make_shared<MemberLValue>(lval, ident));
}
int FE::LValArrayAccess(int base, int access){
    auto fe = FrontEnd::instance();
    auto lval = fe->lValues.get(base);
    auto expr = fe->expressions.get(access);
    return fe->lValues.add(std::make_shared<ArrayAccesLValue>(lval, expr));
}
#pragma endregion


