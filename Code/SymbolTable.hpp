#ifndef SYMBOLTABLE_HPP
#define SYMBOLTABLE_HPP

#include <string>
#include <map>
#include "Symbol.hpp"

class SymbolTable
{
public:
	SymbolTable(std::shared_ptr<SymbolTable> parent):m_parent(parent)  {}
	std::shared_ptr<SymbolTable> getParent() { return m_parent; }

	void addEntry(std::string key, std::shared_ptr<Symbol> symbol);
	std::shared_ptr<Symbol> findEntry(std::string s);
private:
	std::shared_ptr<SymbolTable> m_parent;
	std::map<std::string, std::shared_ptr<Symbol>> m_symbols;

};

#endif
