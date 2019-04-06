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
#include "FormalDecl.hpp"

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
            fe->blocks.clear();
        }
	return fe;
    }

    void push_ST(){
        symbolTable = std::make_shared<SymbolTable>(symbolTable, "$sp");
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
    NodeList<std::vector<std::shared_ptr<Expression>>> arguments;
    NodeList<std::vector<std::shared_ptr<LValue>>> lValList;
    NodeList<std::vector<std::string>> identList;
    NodeList<Parameter> parameter;
    NodeList<std::vector<int>> parameters;
    std::vector<std::string> statements;
    std::vector<std::string> statementSequence;
    std::vector<std::vector<std::pair<int,int>>> elif;
    std::vector<std::string> blocks;
    
private:
    static std::shared_ptr<FrontEnd> fe;
    std::shared_ptr<SymbolTable> symbolTable = std::make_shared<SymbolTable>(nullptr, "$gp");
    std::shared_ptr<StringTable> stringTable;
    std::string code = "";
};

static RegAlloc reg;
RegAlloc getReg() { return reg; }
std::shared_ptr<FrontEnd> FrontEnd::fe;
int _label = 0;
int _block = 0;

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
    for (int i = 0; i < FrontEnd::instance()->blocks.size(); i++) {
        out += FrontEnd::instance()->blocks[i];
    }
    out += ".data\n";
    out += printStringTable();
    out += ".align 2\n";
    out += "GA:\n";


    std::cout << out << std::endl;
}

void push_ST() {
    FrontEnd::instance()->push_ST();
}
void pop_ST() {
    FrontEnd::instance()->pop_ST();
}

#pragma region Expressions
int StringExpr(char* x){
    auto exp = std::make_shared<Literal>(x, reg.getRegister());
    auto l = FrontEnd::instance()->expressions.add(exp); 
    return l;
}
int IntExpr(int x){ 
    auto exp = std::make_shared<Literal>(x, reg.getRegister());
    auto l = FrontEnd::instance()->expressions.add(exp);
    return l;
}
int CharExpr(char x){ 
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
        auto z = fe->expressions.add(std::make_shared<Expression>(lval->type, r, out, ExpressionType::LVAL));
        return z;
    }
}

int CallFunction(char* id, int argList){
    auto fe = FrontEnd::instance();
    auto st = fe->getSymbolTable();
    auto r = reg.getRegister();
    auto params = st->findSymbol(id);
    auto argsoff = 0;
    if (params->getSub() != "Parameter") {
        std::cout << "ERROR: this is not a function." << std::endl;
        return EXIT_FAILURE;
    }
    auto psym = std::dynamic_pointer_cast<ParameterSymbol>(params);
    auto fd = psym->params;
    // std::cout << fd->params.size() << std::endl;
    auto ps = fd->params;
    auto args = fe->arguments.get(argList);
    if (ps.size() != args->size()) {
        std::cout << "ERROR: function contains an improper number of arguments. Expected " + std::to_string(ps.size()) + " but got " + std::to_string(args->size()) << std::endl;
        return EXIT_FAILURE;
    }
    std::string out = "";
    // check args match params
    out += "\tsw \t$ra, -4($sp)\n";
    out += "\tsw \t$fp, -8($sp)\n";
    out += "\taddi \t$sp, $sp, -8\n";
    // load parameters
    for (int i = 0; i < args->size(); i++) {
        if (args->at(i)->type->name() != ps[i]->type->name()){
            std::cout << "ERROR: expected type " + ps[i]->type->name() + " but got " + args->at(i)->type->name();
            return EXIT_FAILURE;
        }
        out += args->at(i)->emit();
        argsoff += args->at(i)->type->size();
        out += "\tsw \t$t" + std::to_string(args->at(i)->r) + ", -" + std::to_string(argsoff) + "($sp)\n";
    }

    // fp = sp, sp = args offset
    out += "\tmove \t$fp, $sp\n";
    out += "\taddi \t$sp, $sp, -" + std::to_string(argsoff) + "\n";
    // jal
    out += "\tjal BLOCK" + std::to_string(params->m_value) + "\n";
    // sp <- args offset
    out += "\taddi \t$sp, $sp, " + std::to_string(argsoff) + "\n";
    // sp <- -8
    out += "\taddi \t$sp, $sp, 8\n";
    // grab old fp and ra
    out += "\tlw \t$ra, -4($sp)\n";
    out += "\tlw \t$fp, -8($sp)\n";
    // difference here between Procedure and Function
    out += "\tmove \t$t" + std::to_string(r) + ", $v0\n";
    // weird stuff with addi sp, sp, 0

    auto z = fe->expressions.add(std::make_shared<Expression>(psym->getType(), r, out, ExpressionType::FUNCCALL));
    return z;
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

int StackArguments(int list, int expr){
    auto fe = FrontEnd::instance();
    auto e = fe->expressions.get(expr);
    if (list == -1) {
        list = fe->arguments.add(std::make_shared<std::vector<std::shared_ptr<Expression>>>());
    }
    auto args = fe->arguments.get(list);
    args->emplace_back(e);
    return list;
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
    st->addSymbol(id, std::make_shared<VarSymbol>(BuiltInType::getInt(), st->getLoc(), st->getOffset()));
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
    out += (toDownTo?"\tbltz \t$t":"\tbgtz \t$t") + std::to_string(e->r) + ", AF" + std::to_string(label) + "\n";
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
    fe->statements.emplace_back(out);
    return fe->statements.size()-1;
}
int ReturnStmt(int expr){
   auto fe = FrontEnd::instance();
   auto e = fe->expressions.get(expr);
   std::string out = "";
   if (expr != -1) {
       out += e->emit();
       out += "\tmove \t$v0, $t" + std::to_string(e->r) +  "\n";
        
   }
    out += "\tjr \t$ra\n";
    fe->statements.emplace_back(out);
    return fe->statements.size()-1;
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
        vec->emplace_back(exp);
        return fe->writeArgs.add(vec);
    }
    else {
        fe->writeArgs.get(list)->emplace_back(exp);
        return list;
    }
};
int ProcCall(char* id, int argList){
    auto fe = FrontEnd::instance();
    auto st = fe->getSymbolTable();
    auto params = st->findSymbol(id);
    auto argsoff = 0;
    if (params->getSub() != "Parameter") {
        std::cout << "ERROR: this is not a procecure." << std::endl;
        return EXIT_FAILURE;
    }
    auto psym = std::dynamic_pointer_cast<ParameterSymbol>(params);
    auto fd = psym->params;
    // std::cout << fd->params.size() << std::endl;
    auto ps = fd->params;
    auto args = fe->arguments.get(argList);
    if (ps.size() != args->size()) {
        std::cout << "ERROR: function contains an improper number of arguments. Expected " + std::to_string(ps.size()) + " but got " + std::to_string(args->size()) << std::endl;
        return EXIT_FAILURE;
    }
    std::string out = "";
    // check args match params
    out += "\tsw \t$ra, -4($sp)\n";
    out += "\tsw \t$fp, -8($sp)\n";
    out += "\taddi \t$sp, $sp, -8\n";
    // load parameters
    for (int i = 0; i < args->size(); i++) {
        if (args->at(i)->type->name() != ps[i]->type->name()){
            std::cout << "ERROR: expected type " + ps[i]->type->name() + " but got " + args->at(i)->type->name();
            return EXIT_FAILURE;
        }
        out += args->at(i)->emit();
        argsoff += args->at(i)->type->size();
        out += "\tsw \t$t" + std::to_string(args->at(i)->r) + ", -" + std::to_string(argsoff) + "($sp)\n";
    }

    // some sort of addi $sp, sp, 0...

    // store fp and ra, advance sp -8

    // copy args to stack pointer
//     for (int i = 0; i < args->size(); i++) {

// // TODO: HERE!!!! parameters were added to symbol table, but it has since been popped, need a new system
//         auto offset = st->findSymbol(ps[i]->name)->getMemLoc();
        
//     }
    // fp = sp, sp = args offset
    out += "\tmove \t$fp, $sp\n";
    out += "\taddi \t$sp, $sp, -" + std::to_string(argsoff) + "\n";
    // jal
    out += "\tjal BLOCK" + std::to_string(params->m_value) + "\n";
    // sp <- args offset
    out += "\taddi \t$sp, $sp, " + std::to_string(argsoff) + "\n";
    // sp <- -8
    out += "\taddi \t$sp, $sp, 8\n";
    // grab old fp and ra
    out += "\tlw \t$ra, -4($sp)\n";
    out += "\tlw \t$fp, -8($sp)\n";
    // difference here between Procedure and Function
    // weird stuff with addi sp, sp, 0
    fe->statements.emplace_back(out);
    return fe->statements.size()-1;
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
        st->addSymbol(vec->at(i), std::make_shared<VarSymbol>(t, st->getLoc(), st->getOffset()));
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

int addParameter(int id, int type, int ref){
    auto fe = FrontEnd::instance();
    auto t = fe->types.get(type);
    auto il = fe->identList.get(id);
    auto v = std::make_shared<std::vector<int>>();
    auto st = fe->getSymbolTable();
    for(int i = 0; i < il->size(); i++) {
        // if (t->name() == "string") {
        //      //should these take strings? "It should do what his does"
        // }
        st->addParam(il->at(i), std::make_shared<VarSymbol>(t, "$fp", st->getParamOffset() ));
        int z = fe->parameter.add(std::make_shared<Parameter>(il->at(i), t, static_cast<bool>(ref)));
        v->emplace_back(z);
    }
    return fe->parameters.add(v);
}

int stackParameters(int list, int p) {
    auto fe = FrontEnd::instance();
    auto ps = fe->parameters.get(p);
    if (list == -1) {
        return p;
    }
    else {
        for (int i = 0; i < ps->size(); i++) { // take everything on list p and add to list list
            fe->parameters.get(list)->emplace_back(ps->at(i));
        }
        return list;
    }
}

void addProcedure(char* id, int params, int body) {

    auto fe = FrontEnd::instance();
    auto p = std::shared_ptr<std::vector<int>>();
    auto fd = std::shared_ptr<FormalDecl>();
    auto label = _block++;
    auto proc = fe->getSymbolTable()->findSymbol(id);
    if (fe->getSymbolTable()->findSymbol(id) != nullptr) {
        // if symbol already exists -> should exist, otherwise problem
        if (proc->getSub() != "Parameter") {
            std::cout << "ERROR: " + std::string(id) + " is symbol other than type Procedure." << std::endl;
            EXIT_FAILURE;
        }
        else {
            if (params != -1) {
                auto fp = std::vector<std::shared_ptr<Parameter>>();
                p = fe->parameters.get(params);
                for (int i = 0; i < p->size(); i++) {
                    fp.emplace_back(fe->parameter.get(p->at(i)));
                }
                fd = std::make_shared<FormalDecl>(fp);
            }
            if (body == -1) { // forward
                if (!std::dynamic_pointer_cast<ParameterSymbol>(proc)->forward) {
                    std::cout << "ERROR: " + std::string(id) + " has already been declared." << std::endl;
                    EXIT_FAILURE;
                }
            
                auto sym = std::make_shared<ParameterSymbol>(fd, label);
                sym->forward = true;
                if (std::dynamic_pointer_cast<ParameterSymbol>(proc)->function!=false) {
                    std::cout << "ERROR: " + std::string(id) +  " is a function and  has been previously declared as a parameter.";
                    EXIT_FAILURE;
                }
                fe->getSymbolTable()->modSymbol(id, sym);
            }
            
            else {
                proc->m_value = label;
                std::dynamic_pointer_cast<ParameterSymbol>(proc)->forward = false;

                fe->getSymbolTable()->modSymbol(id, std::make_shared<ParameterSymbol>(fd, label));

            }
        }

    }
    else {
        // error with linkProc
    }


    if (body != -1) {
        std::string out = "";
        out += "BLOCK" + std::to_string(label) + ":\n";
        out += fe->statementSequence[body];
        out += "\tjr $ra\n";
        fe->blocks.emplace_back(out);
        pop_ST();
    }
}

char* linkProcedure (char* id) {
    auto st = FrontEnd::instance()->getSymbolTable();
    auto sym = std::make_shared<ParameterSymbol>(false);
    st->addSymbol(id, sym);
    FrontEnd::instance()->push_ST();
    return id;
}

char* linkFunction (char* id) {
    auto st = FrontEnd::instance()->getSymbolTable();
    auto sym = std::make_shared<ParameterSymbol>(true);
    st->addSymbol(id, sym);
    FrontEnd::instance()->push_ST();
    return id;
}

void addFunction(char* id, int params,  int type, int body){
    auto fe = FrontEnd::instance();
    auto p = std::shared_ptr<std::vector<int>>();
    auto fd = std::shared_ptr<FormalDecl>();
    auto label = _block++;
    auto func = fe->getSymbolTable()->findSymbol(id);
    auto t = fe->types.get(type);
    if (fe->getSymbolTable()->findSymbol(id) != nullptr) {
        // if symbol already exists -> should exist, otherwise problem
        if (func->getSub() != "Parameter") {
            std::cout << "ERROR: " + std::string(id) + " is symbol other than type Function." << std::endl;
            EXIT_FAILURE;
        }
        else {
            if (params != -1) {
                auto fp = std::vector<std::shared_ptr<Parameter>>();
                p = fe->parameters.get(params);
                for (int i = 0; i < p->size(); i++) {
                    fp.emplace_back(fe->parameter.get(p->at(i)));
                }
                fd = std::make_shared<FormalDecl>(fp);
            }
            if (body == -1) { // forward
                if (!std::dynamic_pointer_cast<ParameterSymbol>(func)->forward ) {
                    std::cout << "ERROR: " + std::string(id) + " has already been declared." << std::endl;
                    EXIT_FAILURE;
                }
            
                auto sym = std::make_shared<ParameterSymbol>(t, fd, label);
                sym->forward = true;
                if (std::dynamic_pointer_cast<ParameterSymbol>(func)->function!=true) {
                    std::cout << "ERROR: " + std::string(id) +  " is a function and  has been previously declared as a parameter.";
                    EXIT_FAILURE;
                }
                fe->getSymbolTable()->modSymbol(id, sym);
            }
            
            else {
                func->m_value = label;
                std::dynamic_pointer_cast<ParameterSymbol>(func)->forward = false;

                fe->getSymbolTable()->modSymbol(id, std::make_shared<ParameterSymbol>(t, fd, label));
            }
        }
    }
    else {
        // error with linkProc
    }


    if (body != -1) {
        std::string out = "";
        out += "BLOCK" + std::to_string(label) + ":\n";
        out += fe->statementSequence[body];
        out += "\tjr $ra\n";
        fe->blocks.emplace_back(out);
        pop_ST();
    }
}


