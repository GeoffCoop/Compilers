#include <string>
#include <iostream>
#include "SymbolTable.hpp"

void SymbolTable::addSymbol (std::string key, std::shared_ptr<Symbol> symbol){
    auto found = m_symbols.find(key);
    if (found == m_symbols.end()){
        m_symbols[key] = symbol;
    }
//    if(typeid(symbol) != typeid(findEntry("integer")))
    if (symbol->getSub() != "Parameter" && symbol->getSub() != "Const") {
        memoryOffset += symbol->getType()->size();
    }
}

void SymbolTable::modSymbol (std::string key, std::shared_ptr<Symbol> symbol){
    auto found = m_symbols.find(key);
    if (found != m_symbols.end()) {
        m_symbols[key] = symbol;
    }
    else {
        m_parent->modSymbol(key, symbol);
    }
}
void SymbolTable::addParam (std::string key, std::shared_ptr<Symbol> symbol){
    auto found = m_symbols.find(key);
    if (found == m_symbols.end()){
        m_symbols[key] = symbol;
    }
    if (symbol->getSub() != "Const") {
        paramOffset += symbol->getType()->size();
    }
}

void SymbolTable::addType(std::string key, std::shared_ptr<Type> type) {
    auto found = m_types.find(key);
    if (found == m_types.end()){
        m_types[key] = type;
    }
    else {
        //TODO Complain bitterly. Type name already exists.
    }
}

std::shared_ptr<Symbol> SymbolTable::findSymbol(std::string key){
    auto found = m_symbols.find(key);
    if(found == m_symbols.end()){
        if (m_parent == nullptr) return nullptr;
        else return m_parent->findSymbol(key);
    }
    return m_symbols[key];
}

std::shared_ptr<Type> SymbolTable::findType(std::string key) {
    auto found = m_types.find(key);
    if(found == m_types.end()){
        if (m_parent == nullptr) return nullptr;
        else return m_parent->findType(key);
    }
    return m_types[key];
}
