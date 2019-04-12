#include "Expression.hpp"
#include "RegAllocation.hpp"
#include <iostream>
#include <string>

std::string Expression::emit(){ // take a known register/location and do the logic
    //check children types?

    std::string out = "";
    auto c2 = false;
    if (child1) { 
        if (child1->child1 || child1->child2){
            out += child1->emit();
        }
        else if (child2 && (child2->child1||child2->child2)){
            out += child2->emit();
            c2 = true;
            out += child1->emit();
        }
        else {
            out += child1->emit(); 
        }
    }
    if (child2) { 
        if (!c2) {
            out += child2->emit(); 
        }
    }
    if (r == -1) {
        r = RegAlloc::instance()->getRegister();
    }
    switch (exp) {
        case ExpressionType::AND : 
            out += "\tand \t$t" + std::to_string(r) + ", $t" + std::to_string(child1->r) + ", $t" + std::to_string(child2->r) + "\n";
        break;
        case ExpressionType::OR : 
            out+= "\tor \t$t" + std::to_string(r) + ", $t" + std::to_string(child1->r) + ", $t" + std::to_string(child2->r) + "\n";
        break;
        case ExpressionType::EQUAL : 
            out+= "\tseq \t$t" + std::to_string(r) + ", $t" + std::to_string(child1->r) + ", $t" + std::to_string(child2->r) + "\n";
        break;
        case ExpressionType::NOTEQUAL : 
            out+= "\tsne \t$t" + std::to_string(r) + ", $t" + std::to_string(child1->r) + ", $t" + std::to_string(child2->r) + "\n";
        break;
        case ExpressionType::LTE : 
            out+= "\tsle \t$t" + std::to_string(r) + ", $t" + std::to_string(child1->r) + ", $t" + std::to_string(child2->r) + "\n";
        break;
        case ExpressionType::GTE : 
            out+= "\tsge \t$t" + std::to_string(r) + ", $t" + std::to_string(child1->r) + ", $t" + std::to_string(child2->r) + "\n";
        break;
        case ExpressionType::LT : 
            out+= "\tslt \t$t" + std::to_string(r) + ", $t" + std::to_string(child1->r) + ", $t" + std::to_string(child2->r) + "\n";
        break;
        case ExpressionType::GT : 
            out+= "\tsgt \t$t" + std::to_string(r) + ", $t" + std::to_string(child1->r) + ", $t" + std::to_string(child2->r) + "\n";
        break;
        case ExpressionType::MINUS : 
            out+= "\tsub \t$t" + std::to_string(r) + ", $t" + std::to_string(child1->r) + ", $t" + std::to_string(child2->r) + "\n";
        break;
        case ExpressionType::PLUS : 
            out+= "\tadd \t$t" + std::to_string(r) + ", $t" + std::to_string(child1->r) + ", $t" + std::to_string(child2->r) + "\n";
        break;
        case ExpressionType::MULT : 
            out+= "\tmult \t$t" + std::to_string(child1->r) + ", $t" + std::to_string(child2->r) + "\n";
            out += "\tmflo \t$t" + std::to_string(r) + "\n";
        break;
        case ExpressionType::DIV : 
            out+= "\tdiv \t$t" + std::to_string(child1->r) + ", $t" + std::to_string(child2->r) + "\n";
            out += "\tmflo \t$t" + std::to_string(r) + "\n";
        break;
        case ExpressionType::MODULO : 
            out+= "\tdiv \t$t" + std::to_string(child1->r) + ", $t" + std::to_string(child2->r) + "\n";
            out +="\tmfhi \t$t" + std::to_string(r) + "\n";
        break;
        //only valid because child and parent will share register with child
        case ExpressionType::NOT :  
            out+= "\tseq \t$t" + std::to_string(r) + ", $t" + std::to_string(child1->r) + ", $zero\n";
        break;
        case ExpressionType::UMIN : 
            out+= "\tsub \t$t" + std::to_string(r) + ", $0, $t" + std::to_string(child1->r) + "\n";
        break;
        case ExpressionType::FUNCCALL : 
            out += optCode(r);
        break;
        case ExpressionType::PRED : 
            out+= "\taddi \t$t" + std::to_string(r) + ", $t" + std::to_string(child1->r) + ", -1\n";
        break;
        case ExpressionType::SUCC : 
            out+= "\taddi \t$t" + std::to_string(r) + ", $t" + std::to_string(child1->r) + ", 1\n";
        break;
        case ExpressionType::ORD : 
        case ExpressionType::CHR :
            RegAlloc::instance()->release(r);
            r = child1->r;
        break;
        case ExpressionType::LVAL : 
            // might do nothing, who knows? But most logic will be done in LValue Print functions
            if (type->name() == "string") {
                out += "\tla \t$t"+ std::to_string(r) + ", STR" + std::to_string(value) + " # Load string address\n";
            }
            else {
                out += optCode(r); 
                out += "\tlw \t$t" + std::to_string(r) + ", 0($t" + std::to_string(r) + ")\n" ;
            }
        break;
        // if literal?
        case ExpressionType::CHRCONST : 
            out+= "\tli \t$t" + std::to_string(r) + ", " + std::to_string(value) + " # Load char const\n";
        break;
        case ExpressionType::INTCONST : 
            out+= "\tli \t$t" + std::to_string(r) + ", " + std::to_string(value) + " # Load int const\n";
        break;
        case ExpressionType::STRINGCONST : 
            out+= "\tla \t$t"+ std::to_string(r) + ", STR" + std::to_string(value) + " # Load string address\n";
        break;
        case ExpressionType::BOOLEANCONST :
            out += "\tli \t$t" +std::to_string(r) + ", " + std::to_string(value) + "\n";
            out += "\tsne \t$t" + std::to_string(r) + ", $t" + std::to_string(r) + ", $zero\n";
        break;
    }
    RegAlloc::instance()->release(r);
    return out;
};

std::string LValExpression::emit() {
    return val;
}

std::string Literal::emit() {
    std::string out = "";
    switch (exp) {
        case ExpressionType::CHRCONST : 
            out = "\tli \t$t" + std::to_string(r) + ", " + std::to_string(value) + "\n";
        break;
        case ExpressionType::INTCONST : 
            out = "\tli \t$t" + std::to_string(r) + ", " + std::to_string(value) + "\n";
        break;
        case ExpressionType::STRINGCONST : 
            out = "\tla \t$t"+ std::to_string(r) + ", STR" + std::to_string(value) + "\n";
        break;
        case ExpressionType::BOOLEANCONST :
            out += "\tli \t$t" +std::to_string(r) + ", " + std::to_string(value) + "\n";
            out = "\tsne \t$t" + std::to_string(r) + ", $t" + std::to_string(r) + ", $zero\n";
        break;
    }
    return out;
}