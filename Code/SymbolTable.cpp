
#include "SymbolTable.hpp"

void SymbolTable::addEntry (std::string key, std::shared_ptr<Symbol> symbol){
    auto found = m_symbols.find(key);
    if (found == m_symbols.end()){
        m_symbols[key] = symbol;
    }
    if (symbol->getMemLoc() != -1)
        memoryOffset += symbol->getType()->size();
}

std::shared_ptr<Symbol> SymbolTable::findEntry(std::string key){
    auto found = m_symbols.find(key);
    if(found == m_symbols.end()){
        if (m_parent == nullptr) return nullptr;
        else return m_parent->findEntry(key);
    }
    return m_symbols[key];
}
