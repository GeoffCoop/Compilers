#include "FrontEnd.hpp"

#include <memory>
#include <vector>
#include <iostream>
#include <string>

#include "StringTable.hpp"
#include "SymbolTable.hpp"
#include "Symbol.hpp"
#include "Expressions/Expression.hpp"
#include "Statements/Statement.hpp"
#include "LValue.hpp"
#include "ThreeAddressInstruction.hpp"
#include "RegAllocation.hpp"
#include "Type.hpp"

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

#include "Statements/AssignStatement.hpp"
#include "Statements/ForStatement.hpp"
#include "Statements/IfStatement.hpp"
#include "Statements/ProcCall.hpp"
#include "Statements/ReadStatement.hpp"
#include "Statements/RepeatStatement.hpp"
#include "Statements/ReturnStatement.hpp"
#include "Statements/StopStatement.hpp"
#include "Statements/WhileStatement.hpp"
#include "Statements/WriteStatement.hpp"

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
        if (!fe) { 
            fe = std::make_shared<FrontEnd>(); 
            fe->getSymbolTable()->addEntry("integer", std::make_shared<TypeSymbol>(BuiltInType::getInt(), 0));
            fe->getSymbolTable()->addEntry("INTEGER", std::make_shared<TypeSymbol>(BuiltInType::getInt(), 0));
            fe->getSymbolTable()->addEntry("char", std::make_shared<TypeSymbol>(BuiltInType::getChar(), 0));
            fe->getSymbolTable()->addEntry("CHAR", std::make_shared<TypeSymbol>(BuiltInType::getChar(), 0));
            fe->getSymbolTable()->addEntry("boolean", std::make_shared<TypeSymbol>(BuiltInType::getBoolean(), 0));
            fe->getSymbolTable()->addEntry("BOOLEAN", std::make_shared<TypeSymbol>(BuiltInType::getBoolean(), 0));
            fe->getSymbolTable()->addEntry("string", std::make_shared<TypeSymbol>(BuiltInType::getString(), 0));
            fe->getSymbolTable()->addEntry("STRING", std::make_shared<TypeSymbol>(BuiltInType::getString(), 0));
            fe->getSymbolTable()->addEntry("true", std::make_shared<TypeSymbol>(BuiltInType::getBoolean(), 0));
            fe->getSymbolTable()->addEntry("TRUE", std::make_shared<TypeSymbol>(BuiltInType::getBoolean(), 0));
            fe->getSymbolTable()->addEntry("false", std::make_shared<TypeSymbol>(BuiltInType::getBoolean(), 0));
            fe->getSymbolTable()->addEntry("FALSE", std::make_shared<TypeSymbol>(BuiltInType::getBoolean(), 0));
        }
	return fe;
    }

    void push_ST(){
        symbolTable = std::make_shared<SymbolTable>(symbolTable, 0, 0);
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
    NodeList<std::vector<std::shared_ptr<Expression>>> arguments;
    NodeList<Statement> statements;
    NodeList<std::vector<std::shared_ptr<LValue>>> lValList;
    NodeList<std::vector<std::shared_ptr<Statement>>> slist;
    NodeList<ThreeAddressInstruction> instructions;
    NodeList<std::vector<std::string>> identList;
    typedef NodeList<std::pair<
        std::shared_ptr<Expression>,
        std::vector<std::shared_ptr<Statement>>>>
            condStmt;
    condStmt conditional;
    typedef NodeList<std::vector<std::shared_ptr<std::pair<
        std::shared_ptr<Expression>,
        std::vector<std::shared_ptr<Statement>>>>>>
            condList;
    condList conditionalList;
    
private:
    static std::shared_ptr<FrontEnd> fe;
    std::shared_ptr<SymbolTable> symbolTable = std::make_shared<SymbolTable>(nullptr,0,0);
    std::shared_ptr<StringTable> stringTable;
};

std::shared_ptr<FrontEnd> FrontEnd::fe;
RegAlloc reg;

std::string initMIPS() {
    std::string s =  "\t.data\n";
    s += "\t.globl main\n";
    s += "\t.text\n";
    s += "main:\n";
    return s;
}
#pragma region Expressions
int StringExpr(char* x){
    auto st = StringTable::instance();
    int i = st->addString(std::string(x)); 
    int r = reg.getRegister();
    std::string out = "li \t$t"+ std::to_string(r) + std::string(", st") + std::to_string(i);
    std::cout << out << std::endl;
    return r;
}
int IntExpr(int x){
    auto fe = FrontEnd::instance();
    int r = reg.getRegister();
    std::string out = "li \t$t" + std::to_string(r) + std::string(", ") + std::to_string(x);
    std::cout << out << std::endl;
    return r;
}
int CharExpr(char x){
    auto fe = FrontEnd::instance();
    int r = reg.getRegister();
    std::string out = "li \t$t" + std::to_string(r) + std::string(", ") + std::to_string(x);
    std::cout << out << std::endl;
    return r;
}
// The following need to retrieve other expressions first
int SuccExpr(int x){
    int r = reg.getRegister();
    std::string out = "addi \t$t" + std::to_string(r) + ", $" + std::to_string(x) + ", 1";
    reg.release(x);
    std::cout << out << std::endl;
    return r;
}
int PredExpr(int x){
    int r = reg.getRegister();
    std::string out = "addi \t$t" + std::to_string(r) + ", $" + std::to_string(x) + ", -1";
}
int ChrExpr(int x){
//    auto fe = FrontEnd::instance();
//    auto exp = fe->expressions.get(x);
//    return fe->expressions.add(std::make_shared<Chr>(exp));
}
int OrdExpr(int x){
//    auto fe = FrontEnd::instance();
//    auto exp = fe->expressions.get(x);
//    return fe->expressions.add(std::make_shared<Ord>(exp));
}
int UMinusExpr(int x){
    int r = reg.getRegister();
    std::string out = "sub \t$t" + std::to_string(r) + ", $0" + std::to_string(x);
    reg.release(x);
    std::cout << out << std::endl;
    return r;
}
int NotExpr(int x){
    int r = reg.getRegister();
    std::string out = "not \t$t" + std::to_string(r) + ", $t" + std::to_string(x);
    reg.release(x);
    std::cout << out << std::endl;
    return r;
}
//////////////////////////////// Need to grab both expressions from list
int ModExpr(int x, int y){
    int r = reg.getRegister();
    std::string out = "div \t$t" + std::to_string(x) + ", $t" + std::to_string(y);
    std::string out2 ="mfhi \t$t" + std::to_string(r);
    reg.release(x);
    reg.release(y);
    std::cout << out << std::endl;
    std::cout << out2 << std::endl;
    return r;
}
int DivExpr(int x, int y){
    int r = reg.getRegister();
    std::string out = "div \t$t" + std::to_string(x) + ", $t" + std::to_string(y);
    std::string out2 = "mflo \t$t" + std::to_string(r);
    reg.release(x);
    reg.release(y);
    std::cout << out << std::endl;
    std::cout << out2 << std::endl;
    return r;
}
int MultExpr(int x, int y){ //THIS IS INCOMPLETE, NEED HI/LO LOGIC IN HERE
    int r = reg.getRegister();
    std::string out = "mult \t$t" + std::to_string(x) + ", $t" + std::to_string(y);
    std::string out2 = "mflo \t$t" + std::to_string(r);
    reg.release(x);
    reg.release(y);
    std::cout << out << std::endl;
    std::cout << out2 << std::endl;
    return r;
}
int MinExpr(int x, int y){
    auto r = reg.getRegister();
	x = 1; //TEMP SO IT WORKS ON HANOI FOR NOW
    std::string out = "sub \t$t" + std::to_string(r) + ", $t" + std::to_string(x) + ", $t" + std::to_string(y);
    reg.release(x);
    reg.release(y);
    std::cout << out <<std::endl;
    return r;
}
int GTExpr(int x, int y){
    auto r = reg.getRegister();
    std::string out = "sgt \t$t" + std::to_string(r) + ", $t" + std::to_string(x) + ", $t" + std::to_string(y);
    reg.release(x);
    reg.release(y);
    std::cout << out << std::endl;
    return r;
}
int PlusExpr(int x, int y){
    auto r = reg.getRegister();
    std::string out = "add \t$t" + std::to_string(r) + ", $t" + std::to_string(x) + ", $t" + std::to_string(y);
    reg.release(x);
    reg.release(y);
    std::cout << out << std::endl;
    return r;
}
int LTExpr(int x, int y){
    auto r = reg.getRegister();
    std::string out = "slt \t$t" + std::to_string(r) + ", $t" + std::to_string(x) + ", $t" + std::to_string(y);
    reg.release(x);
    reg.release(y);
    std::cout << out << std::endl;
    return r;
}
int GTEExpr(int x, int y){
    auto r = reg.getRegister();
    std::string out = "sge \t$t" + std::to_string(r) + ", $t" + std::to_string(x) + ", $t" + std::to_string(y);
    reg.release(x);
    reg.release(y);
    std::cout << out << std::endl;
    return r;
}
int LTEExpr(int x, int y){
    auto r = reg.getRegister();
    std::string out = "sle \t$t" + std::to_string(r) + ", $t" + std::to_string(x) + ", $t" + std::to_string(y);
    reg.release(x);
    reg.release(y);
    std::cout << out << std::endl;
    return r;
}
int NEExpr(int x, int y){
    auto r = reg.getRegister();
    std::string out = "sne \t$t" + std::to_string(r) + ", $t" + std::to_string(x) + ", $t" + std::to_string(y);
    reg.release(x);
    reg.release(y);
    std::cout << out << std::endl;
    return r;
}
int EQExpr(int x, int y){
    auto r = reg.getRegister();
    std::string out = "seq \t$t" + std::to_string(r) + ", $t" + std::to_string(x) + ", $t" + std::to_string(y);
    reg.release(x);
    reg.release(y);
    std::cout << out << std::endl; 
    return r;
}
int AndExpr(int x, int y){
    auto r = reg.getRegister();
    std::string out = "and \t$t" + std::to_string(r) + ", $t" + std::to_string(x) + ", $t" + std::to_string(y);
    reg.release(x);
    reg.release(y);
    std::cout << out << std::endl;
    return r;
}
int OrExpr(int x, int y){
    auto r = reg.getRegister();
    std::string out = "or \t$t" + std::to_string(r) + ", $t" + std::to_string(x) + ", $t" + std::to_string(y);
    reg.release(x);
    reg.release(y);
    std::cout << out << std::endl;
    return r;
}
int LValueExpr(int x){
    auto r = reg.getRegister();
    // symbolTable->findEntry
}

int CallFunction(char* x, int y){
//    auto fe = FrontEnd::instance();

}

#pragma endregion

#pragma region LValues
int LValID(char* id){
    auto fe = FrontEnd::instance();
    auto symbol_ptr = fe->getSymbolTable()->findEntry(id);
    auto r = 0;
    if (symbol_ptr != nullptr) {
        r = reg.getRegister();
        std::string out = "lw \t$t" + std::to_string(r); + ", " + std::to_string(symbol_ptr->getMemLoc());
    }
    else {
        //throw error about id not existing;
        std::cout << "id " + std::string(id) + " doesn't exist." << std::endl;
    }
    return r;
}
int LValMemberAccess(int base, char* ident){
//    auto fe = FrontEnd::instance();
//    auto lval = fe->lValues.get(base);
//    return fe->lValues.add(std::make_shared<MemberLValue>(lval, ident));
}
int LValArrayAccess(int base, int access){
//    auto fe = FrontEnd::instance();
//    auto lval = fe->lValues.get(base);
//    auto expr = fe->expressions.get(access);
//    return fe->lValues.add(std::make_shared<ArrayAccesLValue>(lval, expr));
}
int NewLValList(int lVal){
//    auto fe = FrontEnd::instance();
//    std::vector<std::shared_ptr<LValue>> vec;
//    vec.push_back(fe->lValues.get(lVal));
//    return fe->lValList.add(std::make_shared<std::vector<std::shared_ptr<LValue>>>(vec));
}
int StackLVal(int list, int lVal){
//    auto fe = FrontEnd::instance();
//    auto vec = fe->lValList.get(list);
//    vec->push_back(fe->lValues.get(lVal));
//    return list;
}
#pragma endregion

#pragma region arguments
int NewArguments(int expr){
//    auto fe = FrontEnd::instance();
//    std::vector<std::shared_ptr<Expression>> vec;
//    vec.push_back(fe->expressions.get(expr));
//    return fe->arguments.add(std::make_shared<std::vector<std::shared_ptr<Expression>>>(vec));
}
int StackArguments(int list, int expr){
//    auto fe = FrontEnd::instance();
//    auto vec = fe->arguments.get(list); //vec is list of arugments
//    vec->push_back(fe->expressions.get(expr)); // add expression to list  of args
//    return list;        // return list number
}
#pragma endregion

#pragma region statements
int AssignStmt(int lval, int expr){
//    auto fe = FrontEnd::instance();
//    auto lv = fe->lValues.get(lval);
//    auto e = fe-> expressions.get(expr);
//    return fe->statements.add(std::make_shared<AssignStatement>(lv, e));
}
int MergeConditional(int expr, int stmts){
//    auto fe = FrontEnd::instance();
//    auto e = fe->expressions.get(expr);
//    auto s = fe->slist.get(stmts);
//    return fe->conditional.add(std::make_shared<std::pair<std::shared_ptr<Expression>, std::vector<std::shared_ptr<Statement>>>>(e,s));
}
int IfStmt(int ifList, int elifList, int optElse){
//    auto fe = FrontEnd::instance();
//    auto i = fe->conditional.get(ifList);
//    auto eil = fe->conditionalList.get(elifList);
//    auto e = fe->slist;
//    return fe->statements.add(std::make_shared<IfStatement>(i,eil,e));
}
int StackElif(int list, int elif){
//    auto fe = FrontEnd::instance();
    
    //new elif list
/*    if (list == -1){
        std::vector<std::shared_ptr<std::pair<
            std::shared_ptr<Expression>,
            std::vector<std::shared_ptr<Statement>>>>> vec;
        vec.push_back(fe->conditional.get(elif));
        return fe->conditionalList.add(std::make_shared<
            std::vector<std::shared_ptr<std::pair<
                std::shared_ptr<Expression>,
                std::vector<std::shared_ptr<Statement>>>>>>
                (vec));
    }
    else {
        auto vec = fe->conditionalList.get(list);
        vec->push_back(fe->conditional.get(elif));
        return list;
    }*/
}
int WhileStmt(int cond, int stmts){
//    auto fe = FrontEnd::instance();
//    auto c = fe->expressions.get(cond);
//    auto s = fe->slist.get(stmts);
//    return fe->statements.add(std::make_shared<WhileStatement>(c,s));
}
int RepeatStmt(int stmts, int cond){
//    auto fe = FrontEnd::instance();
//    auto s = fe->slist.get(stmts);
//    auto c = fe->expressions.get(cond);
//    return fe->statements.add(std::make_shared<RepeatStatement>(s,c));
}
int ForStmt(char* id, int begin, bool toDownTo, int end, int stmts){
//    auto fe = FrontEnd::instance();
    // these need to pull values from wherever first
//    auto b = fe->expressions.get(begin);
//    auto e = fe->expressions.get(end);
//    auto s = fe->slist.get(stmts);
//    return fe->statements.add(std::make_shared<ForStatement>(id, b, toDownTo, e, s));
}
int StopStmt(){
//    auto fe = FrontEnd::instance();
//    return fe->statements.add(std::make_shared<StopStatement>());
}
int ReturnStmt(int expr){
//    auto fe = FrontEnd::instance();
//    if (expr == -1) {
//        return fe->statements.add(std::make_shared<ReturnStatement>(nullptr));
//    }
//    else {
//        return fe->statements.add(std::make_shared<ReturnStatement>(fe->expressions.get(expr)));
//    }
}
int ReadStmt(int lvals){
//    auto fe = FrontEnd::instance();
//    return fe->statements.add(std::make_shared<ReadStatement>(fe->lValList.get(lvals)));
}
int WriteStmt(int argList){
//    auto fe = FrontEnd::instance();
//    return fe->statements.add(std::make_shared<WriteStatement>(fe->arguments.get(argList)));
}
int ProcCall(char* id, int argList){}
int NewStatementSequence(int stmt){
 //   auto fe = FrontEnd::instance();
 //   std::vector<std::shared_ptr<Statement>> vec;
 //   vec.push_back(fe->statements.get(stmt));
 //   return fe->slist.add(std::make_shared<std::vector<std::shared_ptr<Statement>>>(vec));
}
int StackStatementSequence(int list, int stmt){
 //   auto fe = FrontEnd::instance();
//    auto ss = fe->slist.get(list);
//    ss->push_back(fe->statements.get(stmt));
//    return list;
}

#pragma endregion

#pragma region declarations
int stackIdentList(int list, char* ident) {
    auto fe = FrontEnd::instance();
    if (list == -1) {
        auto temp = std::make_shared<std::vector<std::string>>();
        temp->push_back(ident);
        return fe->identList.add(temp);
    }
    else {
        auto vec = fe->identList.get(list);
        vec->push_back(ident);
        return list;
    }
}

char* lookupType(char* ident) { // pass type name through if it exists
    auto fe = FrontEnd::instance();
    if (fe->getSymbolTable()->findEntry(ident) == nullptr){
        std::cout << "ERROR WITH TYPE " << std::string(ident) << ". Type non-existant." << std::endl;
    }
    return ident;
}

int addVar(int list, char* ident) {
    auto fe = FrontEnd::instance();
    auto vec = fe->identList.get(list);
    auto type = fe->getSymbolTable()->findEntry(ident)->getType();
    for (int i = 0; i < vec->size(); i++){
        fe->getSymbolTable()->addEntry(vec->at(i), std::make_shared<VarSymbol>(type, 0));
    }
}

#pragma endregion