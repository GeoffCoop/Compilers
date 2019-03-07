#include "LValue.hpp"
#include <string>
#include "Type.hpp"
    

// std::string ArrayAccessLValue::emit(int r) { 
//     // TODO: find the fix for when it's not $gp (Some sort of location variable)
//     //       Also add logic to bypass long print statements for consts.
//     // get mem_loc
//     auto reg = getReg();
//     auto fe = FrontEnd::instance();

//     auto sym = table->findSymbol(id);

//     int loc = sym->getMemLoc();
//     std::string out = "\taddi \t$t" + std::to_string(r) + ", $gp, 0\n"; // get mem loc
//     //// get E-low
//     // get E   -> er
//     int er = expr->r;
//     expr->emit();
//     //get low    -> rlow
//     int rlow = reg.getRegister();
//     auto aType = std::dynamic_pointer_cast<ArrayType>(sym->getType());
//     out += "\tli \t$t" + std::to_string(rlow) + ", " + std::to_string(aType->lower) + "\n";
//     // E - low    -> rlow
//     out += "\tsub \t$t" + std::to_string(rlow) + ", $t" + std::to_string(er) + ", $t" + std::to_string(rlow) + "\n";
//     // get array element size     ->er
//     out += "\tli \t$t" + std::to_string(er) + ", " + std::to_string(type->size()) + "\n";
//     // mult size * (e-low)      -> rlow
//     out += "\tmult \t$t" + std::to_string(rlow) + ", $t" + std::to_string(er) + "\n";
//     out += "\tmflo \t$t" + std::to_string(rlow) + "\n";
//     // add last to memloc
//     out += "\tadd \t$t" + std::to_string(r) + ", " + std::to_string(r) + ", " + std::to_string(rlow) + "\n";

//     reg.release(rlow);
//     reg.release(er);

//     return out;
// }

std::string IDLValue::getMemLoc(int r) {
    int memLoc = table->findSymbol(id)->getMemLoc();
    std::string out = "";
    out += "\taddi \t$t" + std::to_string(r) + ", $gp, " + std::to_string(memLoc) + "\n";
    return out;
}

std::string MemberLValue::getMemLoc(int r) {
    int memLoc = table->findSymbol(id)->getMemLoc();
    memLoc+=offset;
    std::string out = "";
    out += "\taddi \t$t" + std::to_string(r) + ", $gp, " + std::to_string(memLoc) + "\n";
    return out;
}

 /**
  * get memory location and store it in register r.
  */
std::string ArrayAccessLValue::getMemLoc(int r) {
    int memLoc = table->findSymbol(id)->getMemLoc();
    int lower = std::dynamic_pointer_cast<ArrayType>(type)->lower;
    int size = std::dynamic_pointer_cast<ArrayType>(type)->type->size();
    std::string out = "";
    out += "";
    if (expr->isLiteral()) {
        int offset = (expr->getValue() - lower)
            * size;
        memLoc += offset;
        out += "\taddi \t$t" + std::to_string(r) + ", $gp, " + std::to_string(memLoc) + "\n";
    }
    else {
        out += "\taddi \t$t" + std::to_string(r) + ", $gp, " + std::to_string(memLoc) + "\n";
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