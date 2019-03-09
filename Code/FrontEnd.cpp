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

//#include "Expressions/stringExpression.hpp"
//#include "Expressions/intExpression.hpp"
//#include "Expressions/charExpression.hpp"
//#include "Expressions/add.hpp"
//#include "Expressions/and.hpp"
//#include "Expressions/chr.hpp"
//#include "Expressions/div.hpp"
//#include "Expressions/eq.hpp"
//#include "Expressions/functionCall.hpp"
//#include "Expressions/gt.hpp"
//#include "Expressions/gte.hpp"
//#include "Expressions/lt.hpp"
//#include "Expressions/lte.hpp"
//// #include "Expressions/lvalueexpr.hpp"
//#include "Expressions/mod.hpp"
//#include "Expressions/mult.hpp"
//#include "Expressions/ne.hpp"
//#include "Expressions/not.hpp"
//#include "Expressions/or.hpp"
//#include "Expressions/ord.hpp"
//#include "Expressions/pred.hpp"
//#include "Expressions/sub.hpp"
//#include "Expressions/succ.hpp"
//#include "Expressions/umin.hpp"

// #include "Statements/AssignStatement.hpp"
// #include "Statements/ForStatement.hpp"
// #include "Statements/IfStatement.hpp"
// #include "Statements/ProcCall.hpp"
// #include "Statements/ReadStatement.hpp"
// #include "Statements/RepeatStatement.hpp"
// #include "Statements/ReturnStatement.hpp"
// #include "Statements/StopStatement.hpp"
// #include "Statements/WhileStatement.hpp"
// #include "Statements/WriteStatement.hpp"

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
            fe->getSymbolTable()->addType("integer", BuiltInType::getInt());
            fe->getSymbolTable()->addType("INTEGER", BuiltInType::getInt());
            fe->getSymbolTable()->addType("char", BuiltInType::getChar());
            fe->getSymbolTable()->addType("CHAR", BuiltInType::getChar());
            fe->getSymbolTable()->addType("boolean", BuiltInType::getBoolean());
            fe->getSymbolTable()->addType("BOOLEAN", BuiltInType::getBoolean());
            fe->getSymbolTable()->addType("string", BuiltInType::getString());
            fe->getSymbolTable()->addType("STRING", BuiltInType::getString());
            fe->getSymbolTable()->addSymbol("true", std::make_shared<ConstSymbol>(BuiltInType::getBoolean(), static_cast<int>(true)));
            fe->getSymbolTable()->addSymbol("TRUE", std::make_shared<ConstSymbol>(BuiltInType::getBoolean(), static_cast<int>(true)));
            fe->getSymbolTable()->addSymbol("false", std::make_shared<ConstSymbol>(BuiltInType::getBoolean(), static_cast<int>(false)));
            fe->getSymbolTable()->addSymbol("FALSE", std::make_shared<ConstSymbol>(BuiltInType::getBoolean(), static_cast<int>(false)));
        }
	return fe;
    }

    void push_ST(){
        symbolTable = std::make_shared<SymbolTable>(symbolTable, symbolTable->getLoc());
    }
    void pop_ST(){
        symbolTable = symbolTable->getParent();
    }

    std::shared_ptr<SymbolTable> getSymbolTable()   {
        return symbolTable;
    }

    void addCode(std::string newCode){
        code += newCode;
    }

    std::string getCode() {
        return code;
    }

    
    //some list of expressions
    NodeList<Expression> expressions;
    NodeList<LValue> lValues;
    NodeList<std::vector<int>> writeArgs;
    NodeList<Type> types;
    NodeList<std::pair<int, int>> members;
    NodeList<std::vector<int>> membersList;
    //NodeList<std::vector<std::shared_ptr<Expression>>> arguments;
    //NodeList<Statement> statements;
    NodeList<std::vector<std::shared_ptr<LValue>>> lValList;
    //NodeList<std::vector<std::shared_ptr<Statement>>> slist;
    //NodeList<ThreeAddressInstruction> instructions;
    NodeList<std::vector<std::string>> identList;
    //typedef NodeList<std::pair<
        //std::shared_ptr<Expression>,
        //std::vector<std::shared_ptr<Statement>>>>
        //    condStmt;
    //condStmt conditional;
    //typedef NodeList<std::vector<std::shared_ptr<std::pair<
        //std::shared_ptr<Expression>,
        //std::vector<std::shared_ptr<Statement>>>>>>
        //    condList;
    //condList conditionalList;
    std::vector<std::string> statements;
    std::vector<std::string> statementSequence;
    std::vector<std::vector<std::pair<int,int>>> elif;
    
private:
    static std::shared_ptr<FrontEnd> fe;
    std::shared_ptr<SymbolTable> symbolTable = std::make_shared<SymbolTable>(nullptr,0);
    std::shared_ptr<StringTable> stringTable;
    std::string code = "";
};

static RegAlloc reg;
RegAlloc getReg() { return reg; }
std::shared_ptr<FrontEnd> FrontEnd::fe;
int _label = 0;

std::string printStringTable(){
    auto st = StringTable::instance()->getTable();
    std::string out = "";
    if (st.size() > 0)
    for (auto e: st){
        out += "STR" + std::to_string(e.second) + ": \t .asciiz " + std::string(e.first) + "\n";
    }
    return out;
}

std::string initMIPS() {
    std::string s = "# Code generated by CS5300 compiler project. \n#\tKyleCooper\n";
    s += "\t.text\n";
    s += "\t.globl main\n";
    s += "main:\n";
    s += "\tla \t$gp, GA\n";
    s += "\taddi \t$fp, $sp, 0\n\n"; 
    s += "BB1:\n"; 
    return s;
}

void emitMIPS(int ss) {
    std::string out;
    out = initMIPS();
    //blocks go here
    out += FrontEnd::instance()->statementSequence[ss];
    // what to do with this?
    out += "\tli \t$v0, 10\n\tsyscall\n";
    out += ".data\n";
    out += printStringTable();
    out += ".align 2\n";
    out += "GA:\n";


    std::cout << out << std::endl;
}

#pragma region Expressions
int StringExpr(char* x){
    auto exp = std::make_shared<Literal>(x, reg.getRegister());
    auto l = FrontEnd::instance()->expressions.add(exp); 
    return l;
}
int IntExpr(int x){ //TODO: Add Literal Expressions for const
    auto exp = std::make_shared<Literal>(x, reg.getRegister());
    auto l = FrontEnd::instance()->expressions.add(exp);
    return l;
}
int CharExpr(char x){ //TODO: Add Literal Expressions for const
    auto exp = std::make_shared<Literal>(x, reg.getRegister());
    auto l = FrontEnd::instance()->expressions.add(exp);
    return l;
}
// The following need to retrieve other expressions first
int SuccExpr(int x){
    auto exp = FrontEnd::instance()->expressions.get(x);
    auto z = FrontEnd::instance()->expressions.add(std::make_shared<Expression>(exp->type, exp->r, exp, nullptr, ExpressionType::SUCC));
    return z;
}
int PredExpr(int x){
    auto exp = FrontEnd::instance()->expressions.get(x);
    auto z = FrontEnd::instance()->expressions.add(std::make_shared<Expression>(exp->type, exp->r, exp, nullptr, ExpressionType::PRED));
    return z;
}
int ChrExpr(int x){
    auto fe = FrontEnd::instance();
    auto exp = fe->expressions.get(x);
    return fe->expressions.add(std::make_shared<Expression>(BuiltInType::getChar(), exp->r, exp, nullptr, ExpressionType::CHR));
    
}
int OrdExpr(int x){
    auto fe = FrontEnd::instance();
    auto exp = fe->expressions.get(x);
    return fe->expressions.add(std::make_shared<Expression>(BuiltInType::getInt(), exp->r, exp, nullptr, ExpressionType::ORD));
    
}
int UMinusExpr(int x){
    auto exp = FrontEnd::instance()->expressions.get(x);
    auto z = FrontEnd::instance()->expressions.add(std::make_shared<Expression>(BuiltInType::getInt(), exp->r, exp, nullptr, ExpressionType::UMIN));
    return z;
}
int NotExpr(int x){
    auto exp = FrontEnd::instance()->expressions.get(x);
    auto z = FrontEnd::instance()->expressions.add(std::make_shared<Expression>(BuiltInType::getBoolean(), exp->r, exp, nullptr, ExpressionType::NOT));
    return z;
}
//////////////////////////////// Need to grab both expressions from list
int ModExpr(int x, int y){
    auto fe = FrontEnd::instance();
    int r = reg.getRegister();
    auto expx = fe->expressions.get(x);
    auto expy = fe->expressions.get(y);
    reg.release(expx->r);
    reg.release(expy->r);

    if (fe->expressions.get(x)->isLiteral() && fe->expressions.get(y)->isLiteral()) {
        auto val = fe->expressions.get(x)->getValue() % fe->expressions.get(y)->getValue();
        auto z = fe->expressions.add(std::make_shared<Literal>(val, r));
        return z;
    }
    else {
        auto z = fe->expressions.add(std::make_shared<Expression>(BuiltInType::getInt(), r, expx, expy, ExpressionType::MODULO));
        return z;
    }
}
int DivExpr(int x, int y){
    auto fe = FrontEnd::instance();
    int r = reg.getRegister();
    auto expx = fe->expressions.get(x);
    auto expy = fe->expressions.get(y);
    reg.release(expx->r);
    reg.release(expy->r);

    if (fe->expressions.get(x)->isLiteral() && fe->expressions.get(y)->isLiteral()) {
        auto val = fe->expressions.get(x)->getValue() / fe->expressions.get(y)->getValue();
        auto z = fe->expressions.add(std::make_shared<Literal>(val, r));
        return z;
    }
    else {
        auto z = fe->expressions.add(std::make_shared<Expression>(BuiltInType::getInt(), r, expx, expy, ExpressionType::DIV));
        return z;
    }
}
int MultExpr(int x, int y){
    auto fe = FrontEnd::instance();
    int r = reg.getRegister();
    auto expx = fe->expressions.get(x);
    auto expy = fe->expressions.get(y);
    reg.release(expx->r);
    reg.release(expy->r);
    
    if (fe->expressions.get(x)->isLiteral() && fe->expressions.get(y)->isLiteral()) {
        auto val = fe->expressions.get(x)->getValue() * fe->expressions.get(y)->getValue();
        auto z = fe->expressions.add(std::make_shared<Literal>(val, r));
        return z;
    }
    else {
        auto z = fe->expressions.add(std::make_shared<Expression>(BuiltInType::getInt(), r, expx, expy, ExpressionType::MULT));
        return z;
    }
}
int MinExpr(int x, int y){
    auto fe = FrontEnd::instance();
    int r = reg.getRegister();
    auto expx = fe->expressions.get(x);
    auto expy = fe->expressions.get(y);
    reg.release(expx->r);
    reg.release(expy->r);
    
    if (fe->expressions.get(x)->isLiteral() && fe->expressions.get(y)->isLiteral()) {
        auto val = fe->expressions.get(x)->getValue() - fe->expressions.get(y)->getValue();
        auto z = fe->expressions.add(std::make_shared<Literal>(val, r));
        return z;
    }
    else {
        auto z = fe->expressions.add(std::make_shared<Expression>(BuiltInType::getInt(), r, expx, expy, ExpressionType::MINUS));
        return z;
    }
}
int PlusExpr(int x, int y){
    auto fe = FrontEnd::instance();
    int r = reg.getRegister();
    auto expx = fe->expressions.get(x);
    auto expy = fe->expressions.get(y);
    reg.release(expx->r);
    reg.release(expy->r);
    
    // TODO: Maybe refactor this part? Idk, it might work, it might not...
    if (fe->expressions.get(x)->isLiteral() && fe->expressions.get(y)->isLiteral()) {
        auto val = fe->expressions.get(x)->getValue() + fe->expressions.get(y)->getValue();
        auto z = fe->expressions.add(std::make_shared<Literal>(val, r));
        return z;
    }
    else {
        auto z = fe->expressions.add(std::make_shared<Expression>(BuiltInType::getInt(), r, expx, expy, ExpressionType::PLUS));
        return z;
    }
}
int GTExpr(int x, int y){
    auto fe = FrontEnd::instance();
    int r = reg.getRegister();
    auto expx = fe->expressions.get(x);
    auto expy = fe->expressions.get(y);
    reg.release(expx->r);
    reg.release(expy->r);

    if (fe->expressions.get(x)->isLiteral() && fe->expressions.get(y)->isLiteral()) {
        auto val = fe->expressions.get(x)->getValue() >= fe->expressions.get(y)->getValue();
        auto z = fe->expressions.add(std::make_shared<Literal>(val, r));
        return z;
    }
    else {
        auto z = fe->expressions.add(std::make_shared<Expression>(BuiltInType::getBoolean(), r, expx, expy, ExpressionType::GT));
        return z;
    }
}
int LTExpr(int x, int y){
    auto fe = FrontEnd::instance();
    int r = reg.getRegister();
    auto expx = fe->expressions.get(x);
    auto expy = fe->expressions.get(y);
    reg.release(expx->r);
    reg.release(expy->r);
    auto z = fe->expressions.add(std::make_shared<Expression>(BuiltInType::getBoolean(), r, expx, expy, ExpressionType::LT));
    return z;
}
int GTEExpr(int x, int y){
    auto fe = FrontEnd::instance();
    int r = reg.getRegister();
    auto expx = fe->expressions.get(x);
    auto expy = fe->expressions.get(y);
    reg.release(expx->r);
    reg.release(expy->r);
    auto z = fe->expressions.add(std::make_shared<Expression>(BuiltInType::getBoolean(), r, expx, expy, ExpressionType::GTE));
    return z;
}
int LTEExpr(int x, int y){
    auto fe = FrontEnd::instance();
    int r = reg.getRegister();
    auto expx = fe->expressions.get(x);
    auto expy = fe->expressions.get(y);
    reg.release(expx->r);
    reg.release(expy->r);
    auto z = fe->expressions.add(std::make_shared<Expression>(BuiltInType::getBoolean(), r, expx, expy, ExpressionType::LTE));
    return z;
}
int NEExpr(int x, int y){
    auto fe = FrontEnd::instance();
    int r = reg.getRegister();
    auto expx = fe->expressions.get(x);
    auto expy = fe->expressions.get(y);
    reg.release(expx->r);
    reg.release(expy->r);
    auto z = fe->expressions.add(std::make_shared<Expression>(BuiltInType::getBoolean(), r, expx, expy, ExpressionType::NOTEQUAL));
    return z;
}
int EQExpr(int x, int y){
    auto fe = FrontEnd::instance();
    int r = reg.getRegister();
    auto expx = fe->expressions.get(x);
    auto expy = fe->expressions.get(y);
    reg.release(expx->r);
    reg.release(expy->r);
    auto z = fe->expressions.add(std::make_shared<Expression>(BuiltInType::getBoolean(), r, expx, expy, ExpressionType::EQUAL));
    return z;
}
int AndExpr(int x, int y){
    auto fe = FrontEnd::instance();
    int r = reg.getRegister();
    auto expx = fe->expressions.get(x);
    auto expy = fe->expressions.get(y);
    reg.release(expx->r);
    reg.release(expy->r);
    auto z = fe->expressions.add(std::make_shared<Expression>(BuiltInType::getBoolean(), r, expx, expy, ExpressionType::AND));
    return z;
}
int OrExpr(int x, int y){
    auto fe = FrontEnd::instance();
    int r = reg.getRegister();
    auto expx = fe->expressions.get(x);
    auto expy = fe->expressions.get(y);
    reg.release(expx->r);
    reg.release(expy->r);
    auto z = fe->expressions.add(std::make_shared<Expression>(BuiltInType::getBoolean(), r, expx, expy, ExpressionType::OR));
    return z;
}
int LValueExpr(int x){ // x is a # in lValue
    auto fe = FrontEnd::instance();
    auto r = reg.getRegister();
    auto lval = fe->lValues.get(x);
    //if lValID
    auto sym = fe->getSymbolTable()->findSymbol(lval->id);
    if (sym->getType()->name() == "string") {
        auto z = fe->expressions.add(std::make_shared<Expression>(lval->type, r, nullptr, nullptr, ExpressionType::LVAL));
        fe->expressions.get(z)->setValue(sym->m_value);
        return z;
    }
    else if (sym->getSub() == "Const") {
        int z = -1;
        if (sym->getType()->name() == "bool"){
            z = fe->expressions.add(std::make_shared<Literal>(static_cast<bool>(sym->m_value), r));
        }
        else if (sym->getType()->name() == "char") {
            z = fe->expressions.add(std::make_shared<Literal>(static_cast<char>(sym->m_value), r));
        }
        else {
            z = fe->expressions.add(std::make_shared<Literal>(sym->m_value, r));
        }
        return z;
    }
    else {
        std::string out = "";
        out += lval->getMemLoc(r);
        out += "\tlw \t$t" + std::to_string(r) + ", 0($t" + std::to_string(r) + ")\n" ;
        // fe->addCode(out);
        auto z = fe->expressions.add(std::make_shared<Expression>(lval->type, r, out));
        return z;
    }
}

int CallFunction(char* x, int y){
//    auto fe = FrontEnd::instance();

}

#pragma endregion

#pragma region LValues
int LValID(char* id){
    auto fe = FrontEnd::instance();
    auto symbol_ptr = fe->getSymbolTable()->findSymbol(id);
    int i = -1;
    if (symbol_ptr != nullptr) {
        i = fe->lValues.add(std::make_shared<IDLValue>(id, symbol_ptr->getType(), fe->getSymbolTable()));
    }
    else {
        //throw error about id not existing;
        std::cout << "id " + std::string(id) + " doesn't exist." << std::endl;
    }
    return i;
}
int LValMemberAccess(int base, char* ident){
    auto fe = FrontEnd::instance();
    auto b = fe->lValues.get(base);
    if (b->type->name() != "record") {
        std::cout << "ERROR: THIS VARIABLE IS NOT A RECORD." << std::endl;
        return -1;
    }
    return fe->lValues.add(std::make_shared<MemberLValue>(b, ident, fe->getSymbolTable()));

}
int LValArrayAccess(int base, int access){ //base is the LValue returned from LValID, acces is expression
    auto fe = FrontEnd::instance();
    auto b = fe->lValues.get(base);
    auto e = fe->expressions.get(access);
    auto z = fe->lValues.add(std::make_shared<ArrayAccessLValue>(b, e, fe->getSymbolTable())); 
    return z;
}

int StackLVal(int list, int lVal){
    auto fe = FrontEnd::instance();
    if (list == -1) {
        list = (fe->lValList.add(std::make_shared<std::vector<std::shared_ptr<LValue>>>()));
    }
    fe->lValList.get(list)->emplace_back(fe->lValues.get(lVal));
    return list;
    
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
int AssignStmt(int lval, int expr){ // Assign is addi/sw    
    auto fe = FrontEnd::instance();
    auto l = fe->lValues.get(lval);
	// if const, throw error

    auto r = reg.getRegister();
    auto e = fe->expressions.get(expr);
    auto sym = fe->getSymbolTable()->findSymbol(l->id);
    std::string out = "";

    if (sym->getType()->name() == "string") {
        sym->m_value = e->getValue();
    }
    else {
    // does not take into consideration type or location
        out += e->emit();
        out += l->getMemLoc(r);
        out += "\tsw \t$t" + std::to_string(e->r) + ", 0($t" + std::to_string(r) + ")\n";
    }
    
    reg.release(r);
    reg.release(e->r);
    fe->statements.emplace_back(out);
    //fe->addCode(out);
	return fe->statements.size()-1;
}
int IfStmt(int exp, int iss, int elifList, int optElse){
    auto fe = FrontEnd::instance();
    int label = _label++;
    auto e = fe->expressions.get(exp);
    auto type = e->type;
    auto elifs = fe->elif[0];
    if (elifList != -1) elifs = fe->elif[elifList];
    std::string out = "";
    while (type->name() == "array") {
        type = std::dynamic_pointer_cast<ArrayType>(type)->type;
    }
    if (type->name() != "bool"){
        std::cout << "ERROR WHILE TRYING TO EVALUATE IF EXPRESSION. TYPE DID NOT RESOLVE TO TYPE BOOLEAN.";
    }
    out += "";
    out += e->emit();
    out += "\tbne \t$t" + std::to_string(e->r) + ", $zero, ISS" + std::to_string(label) + "\n";
    if (elifList != -1) { out += "\tj EI" + std::to_string(label) + "_0\n"; }
    else {out += "\tj AS" + std::to_string(label) + "\n"; }
    out += "ISS" + std::to_string(label) + ":\n";
    out += fe->statementSequence[iss];
    out += "\tj AI" + std::to_string(label) + "\n";

    if (elifList != -1) {
        for(int i = 0; i < elifs.size(); i++) {
            auto ifexp = fe->expressions.get(elifs[i].first);
            out += "EI" + std::to_string(label) + "_" + std::to_string(i) + ":\n";
            out += ifexp->emit();
            out += "\tbne \t$t" + std::to_string(ifexp->r) + ", $zero, EISS" + std::to_string(label) + "_" + std::to_string(i) + "\n";
            out += "\tj EI" + std::to_string(label) + "_" + std::to_string(i+1) + "\n";
            out += "EISS" + std::to_string(label) + "_" + std::to_string(i) + ":\n";
            out += fe->statementSequence[elifs[i].second];
            out += "\tj AI" + std::to_string(label) + "\n";
        }
    }
    out += "EI" + std::to_string(label) + "_" + std::to_string(elifs.size()) + ":\n";
    if (optElse != -1) {
        out += fe->statementSequence[optElse];
    }
    out += "\tj AI" + std::to_string(label) + "\n";
    out += "AI" + std::to_string(label) + ":\n";

    fe->statements.emplace_back(out);
    return fe->statements.size()-1;
}
int StackElif(int list, int exp, int ss){
    auto fe = FrontEnd::instance();
    if (list == -1) {
        auto v = std::vector<std::pair<int,int>>();
        v.emplace_back(std::pair<int,int> (exp, ss));
        fe->elif.emplace_back(v);
        list = fe->elif.size()-1;
        return list;
    }
    else   {
        fe->elif[list].emplace_back(std::pair<int,int>(exp,ss));
        return list;
    }
}
int WhileStmt(int cond, int stmts){
    auto fe = FrontEnd::instance();
    int label = _label++;
    auto e = fe->expressions.get(cond);
    auto type = e->type;
    while (type->name() == "array") {
        type = std::dynamic_pointer_cast<ArrayType>(type)->type;
    }
    if (type->name() != "bool") {
        std::cout << "ERROR WHILE TRYING TO EVALUATE WHILE STATE CONDITION. TYPE EVALUATED NOT TO BOOL." << std::endl;
    }
    std::string out = "";
    out += "\tj WCD" + std::to_string(label) + "\n";
    out += "WCD" + std::to_string(label) + ":\n";
    out += e->emit();
    out += "\tbne \t$t" + std::to_string(e->r) + ", $zero, WCNT" + std::to_string(label) + "\n";
    out += "\tj AW" + std::to_string(label) + "\n";
    out += "WCNT" + std::to_string(label) + " :\n";
    out += fe->statementSequence[stmts];
    out += "\tj WCD" + std::to_string(label) + "\n";
    out += "AW" + std::to_string(label) + ":\n";

    fe->statements.emplace_back(out);
    return fe->statements.size()-1;
}
int RepeatStmt(int stmts, int cond){
    auto fe = FrontEnd::instance();
    int label = _label++;
    auto e = fe->expressions.get(cond);
    auto type = e->type;
    while (type->name() == "array") {
        type = std::dynamic_pointer_cast<ArrayType>(type)->type;
    }
    if (type->name() != "bool") {
        std::cout << "ERROR WHILE TRYING TO EVALUATE REPEAT STATE CONDITION. TYPE NOT EVALUATED TO BOOL." << std::endl;
    }
    std::string out = "";
    out += "\tj RCNT" + std::to_string(label) + "\n";
    out += "RCD" + std::to_string(label) + ":\n";
    out += e->emit();
    out += "\tbeq \t$t" + std::to_string(e->r) + ", $zero, RCNT" + std::to_string(label) + "\n";
    out += "\tj AR" + std::to_string(label) + "\n";
    out += "RCNT" + std::to_string(label) + " :\n";
    out += fe->statementSequence[stmts];
    out += "\tj RCD" + std::to_string(label) + "\n";
    out += "AR" + std::to_string(label) + ":\n";

    fe->statements.emplace_back(out);
    return fe->statements.size()-1;
}
void ForAddToST(char* id) {
    auto fe = FrontEnd::instance();
    auto st = fe->getSymbolTable();
    st->addSymbol(id, std::make_shared<VarSymbol>(BuiltInType::getInt(), st->getOffset()));
}
int ForStmt(char* id, int begin, bool toDownTo, int end, int stmts){
    auto fe = FrontEnd::instance();
    auto st = fe->getSymbolTable();
    auto b = fe->expressions.get(begin);
    auto e = fe->expressions.get(end);
    auto l = std::make_shared<IDLValue>(id, BuiltInType::getInt(), st);
    auto r = reg.getRegister();
    auto r2 = reg.getRegister();
    auto label = _label++;
    std::string out = "";
    out += b->emit();
    out += l->getMemLoc(r);
    out += "\tsw \t$t" + std::to_string(b->r) + ", 0($t" + std::to_string(r) + ")\n"; // id = begin
    out += "F" + std::to_string(label) + ":\n";
    out += e->emit();
    out += l->getMemLoc(r);
    out += "\tlw \t$t" + std::to_string(r) + ", 0($t" + std::to_string(r) + ")\n"; 
    out += "\tsub \t$t" + std::to_string(e->r) + ", $t" + std::to_string(r) + ", $t" + std::to_string(e->r) + "\n";
    out += (toDownTo?"\tblez \t$t":"\tbgez \t$t") + std::to_string(e->r) + ", AF" + std::to_string(label) + "\n";
    out += fe->statementSequence[stmts];
    out += l->getMemLoc(r);
    out += "\tlw \t$t" + std::to_string(r2) + ", 0($t" + std::to_string(r) + ")\n";
    out += "\taddi \t$t" + std::to_string(r2) + ", $t" + std::to_string(r2) + (toDownTo?", -1 \n":", 1 \n");
    out += "\tsw \t$t" + std::to_string(r2) + ", 0($t" + std::to_string(r) + ")\n";
    out += "\tj F" + std::to_string(label) + "\n";
    out += "AF" + std::to_string(label) + ":\n";

    fe->statements.emplace_back(out);   
    
    reg.release(r);
    st->removeSymbol(id);
    return fe->statements.size()-1;
}
int StopStmt(){
    auto fe = FrontEnd::instance();
    std::string out = "";
    out += "\tli \t$v0, 10\n";
    out += "\tsyscall\n";
    fe->addCode(out);
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
    auto fe = FrontEnd::instance();
    auto list = fe->lValList.get(lvals);
    std::string out = "";
    for (int i = 0; i < list->size(); i++) {
        auto type = list->at(i)->type;
        while (type->name()=="array") {
            type = std::dynamic_pointer_cast<ArrayType>(type)->type;
        }
        if (type->name()=="record"){
            auto l = std::dynamic_pointer_cast<MemberLValue>(list->at(i))->member;
            type = std::dynamic_pointer_cast<RecordType>(type)->getType(l);
        }
        auto r = reg.getRegister();
        out += list->at(i)->getMemLoc(r);
        if (type->name()=="int") {
            out += "\tli \t$v0, 5\n";
            out += "\tsyscall # Read Int\n";
            out += "\tsw \t$v0, 0($t" + std::to_string(r) + ") \n";
        }
        else if (type->name()=="char") {
            out += "\tli \t$v0, 12\n";
            out += "\tsyscall # Read Char\n";
            out += "\tsw \t$v0, 0($t" + std::to_string(r) + ")\n";
        }
        //fe->addCode(out);
        reg.release(r);
    }
    fe->statements.emplace_back(out);
    return fe->statements.size()-1;
}

int WriteStmt(int argList){
    auto fe = FrontEnd::instance();
    auto list = fe->writeArgs.get(argList);
    std::string out = "";
    for (int i = 0; i < list->size(); i++){ // for each expression in list
        auto exp = fe->expressions.get(list->at(i));
        auto type = exp->type;
        while (type->name()=="array") {
            type = std::dynamic_pointer_cast<ArrayType>(type)->type;
        }
        auto r = exp->r;
        out += exp->emit();
	    if (exp->isLiteral() && (type->name() == "int" || type->name() == "char")) out += "\tli \t$a0, "+ std::to_string(exp->getValue()) + "\n";
        if (type->name() == "int") { 
            out += "\tli \t$v0, 1\n";
            if (!exp->isLiteral()) out += "\tmove \t$a0, $t" + std::to_string(r) + " # write int\n";
            out += "\tsyscall\n";
        }
        else if (type->name() == "char") {
            out += "\tli \t$v0, 11\n";
            if (!exp->isLiteral()) out += "\tmove \t$a0, $t" + std::to_string(r) + " # write char\n";
            out += "\tsyscall\n";
        }
        else if (type->name() == "string") {
            out += "\tli \t$v0, 4\n";
            out += "\tla \t$a0, STR" + std::to_string(exp->getValue()) + " # write string\n";
            out += "\tsyscall\n";
        }
        else if (type->name() == "bool") { //Potential error with reg alloc... Don't think so, though...
            out += "\tli \t$v0, 1\n";
            //if (exp->isLiteral()) out += "\tsne \t$t" + std::to_string(r) + ", " + std::to_string(exp->getValue()) + ", $zero # bool const\n";
            out += "\tsne \t$t" + std::to_string(r) + ", $t" + std::to_string(r) + ", $zero\n";
            out += "\tmove \t$a0, $t" + std::to_string(r) + " # write boolean\n";
            out += "\tsyscall\n";
        }
        else {} // non-printable type
        //fe->addCode(out);
    }
    fe->statements.emplace_back(out);
    return fe->statements.size()-1;

}

int StackWriteArgs(int list, int exp) {
    auto fe = FrontEnd::instance();
    if(list == -1){
        auto vec = std::make_shared<std::vector<int>>();
        vec->push_back(exp);
        return fe->writeArgs.add(vec);
    }
    else {
        fe->writeArgs.get(list)->push_back(exp);
        return list;
    }
};
int ProcCall(char* id, int argList){}
int NewStatementSequence(int stmt){
 //   auto fe = FrontEnd::instance();
 //   std::vector<std::shared_ptr<Statement>> vec;
 //   vec.push_back(fe->statements.get(stmt));
 //   return fe->slist.add(std::make_shared<std::vector<std::shared_ptr<Statement>>>(vec));
}
int StackStatementSequence(int list, int stmt){
    auto fe = FrontEnd::instance(); 
    if (stmt == -1) {
        return list;
    }
    if (list == -1) {
        fe->statementSequence.emplace_back(fe->statements[stmt]);
        int z = fe->statementSequence.size() -1;
        return z;
    }
    else {
        fe->statementSequence[list] += fe->statements[stmt];
        return list;
    }
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


int addVar(int list, int type) {
    auto fe = FrontEnd::instance();
    auto vec = fe->identList.get(list);
    auto st = fe->getSymbolTable();
    auto t = fe->types.get(type);
    for (int i = 0; i < vec->size(); i++){
        st->addSymbol(vec->at(i), std::make_shared<VarSymbol>(t, st->getOffset()));
    }
}

#pragma endregion
int addConst(char* id, int expr) {
    auto fe = FrontEnd::instance();
    auto st = fe->getSymbolTable();
    auto e = fe->expressions.get(expr);
    st->addSymbol(id, std::make_shared<ConstSymbol>(e->type, e->getValue()));
}

int addType(char* id, int newType) {
    auto fe = FrontEnd::instance();
    auto st = fe->getSymbolTable();
    st->addType(id, fe->types.get(newType));
}

int lookupType(char* ident) { // pass type name through if it exist
    auto fe = FrontEnd::instance();
    auto type = fe->getSymbolTable()->findType(ident);
    if (type == nullptr){
        std::cout << "ERROR WITH TYPE " << std::string(ident)<< ". Type non-existant." << std::endl;
    }
    return fe->types.add(type);
}

int addArrayType(int exp1, int exp2, int type) {
    auto fe = FrontEnd::instance();
    auto t = fe->types.get(type);
    auto e1 = fe->expressions.get(exp1);
    if (e1->isLiteral() == false) {
        std::cout << "Array could not be created. Error with parameter 1. Use constants when setting the bounds of an array." << std::endl;
        EXIT_FAILURE;
   }
    auto e2 = fe->expressions.get(exp2);
    if (e1->isLiteral() == false) {
        std::cout << "Array could not be created. Error with parameter 2. Use constants when setting the bounds of an array." << std::endl;
        EXIT_FAILURE;
    }
    return fe->types.add(std::make_shared<ArrayType>(t, e1->getValue(), e2->getValue()));
}

int addRecordType(int rec) {
    auto fe = FrontEnd::instance();
    auto membersList = fe->membersList.get(rec);
    auto record = std::make_shared<RecordType>();
    for (int i = 0; i < membersList->size(); i++){
        auto members = fe->members.get(membersList->at(i));
        auto identList = fe->identList.get(members->first);
        for (int j = 0; j < identList->size(); j++){
            record->addMember(identList->at(j), fe->types.get(members->second));
        }
    }
    return fe->types.add(record);
}

int addRecords(int idList, int t) {
    auto fe = FrontEnd::instance();
    return fe->members.add(std::make_shared<std::pair<int, int>>(idList, t));
}

int stackRecords(int list, int rec) {
    auto fe = FrontEnd::instance();
    if (list == -1) {
        int l = fe->membersList.add(std::make_shared<std::vector<int>>());
        fe->membersList.get(l)->emplace_back(rec);
        return l;
    }
    else {
        fe->membersList.get(list)->emplace_back(rec);
        return list;
    }
}
