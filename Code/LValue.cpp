#include "LValue.hpp"
#include <string>
#include "Type.hpp"
    

std::string IDLValue::getMemLoc(int r) {
    int memLoc = table->findSymbol(id)->getMemLoc();
    auto sym = table->findSymbol(id);
    std::string out = "";
    std::string loc = "";
    if (sym->getLoc() == "$gp") {
        loc = ", $gp, ";
    }
    else if (sym->getLoc() == "$fp"){
        loc = ", $fp, -";
        memLoc +=4;
    }
    else if (sym->getLoc() == "$sp"){
        loc = ", $sp, -";
        memLoc += 4;
    }
    out += "\taddi \t$t" + std::to_string(r) + loc + std::to_string(memLoc) + "\n";
    return out;
}

std::string MemberLValue::getMemLoc(int r) {
    int memLoc = table->findSymbol(id)->getMemLoc();
    auto sym = table->findSymbol(id);
    memLoc+=offset;
    std::string out = "";
    std::string loc = "";
    if (sym->getLoc() == "$gp") {
        loc = ", $gp, ";
    }
    else if (sym->getLoc() == "$fp"){
        loc = ", $fp, -";
        memLoc +=4;
    }
    else if (sym->getLoc() == "$sp"){
        loc = ", $sp, -";
        memLoc += 4;
    }
    out += "\taddi \t$t" + std::to_string(r) + loc + std::to_string(memLoc) + "\n";
    return out;
}

 /**
  * get memory location and store it in register r.
  */
std::string ArrayAccessLValue::getMemLoc(int r) {
    int memLoc = table->findSymbol(id)->getMemLoc();
    auto sym = table->findSymbol(id);
    int lower = std::dynamic_pointer_cast<ArrayType>(type)->lower;
    int size = std::dynamic_pointer_cast<ArrayType>(type)->type->size();
    std::string out = "";
    std::string loc = "";
    if (sym->getLoc() == "$gp") {
        loc = ", $gp, ";
    }
    else if (sym->getLoc() == "$fp"){
        loc = ", $fp, -";
        memLoc +=4;
    }
    else if (sym->getLoc() == "$sp"){
        loc = ", $sp, -";
        memLoc += 4;
    }
    if (expr->isLiteral()) {
        int offset = (expr->getValue() - lower)
            * size;
        memLoc += offset;
        out += "\taddi \t$t" + std::to_string(r) + loc + std::to_string(memLoc) + "\n";
    }
    else {
        out += "\taddi \t$t" + std::to_string(r) + loc + std::to_string(memLoc) + "\n";
        out += "\tli \t$t8, " + std::to_string(lower) + "\n";
        out += expr->emit();
        out += "\tsub \t$t8, $t" + std::to_string(expr->r) + ", $t8\n";
        out += "\tli \t$t9, " + std::to_string(size) + "\n";
        out += "\tmult \t$t8, $t9 \n";
        out += "\tmflo \t$t8\n";
        out += "\tadd \t$t" + std::to_string(r) + ", $t8, $t" + std::to_string(r) + "\n"; 
    }

    return out;
}