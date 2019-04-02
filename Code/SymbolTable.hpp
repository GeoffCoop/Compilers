	#ifndef SYMBOLTABLE_HPP
#define SYMBOLTABLE_HPP

#include <string>
#include <map>
#include "Symbol.hpp"

class SymbolTable
{
public:
	SymbolTable(std::shared_ptr<SymbolTable> parent, std::string memLoc):m_parent(parent), memoryLocation(memLoc)  {}
	std::shared_ptr<SymbolTable> getParent() { return m_parent; }

	void addSymbol(std::string key, std::shared_ptr<Symbol> symbol);
	void modSymbol(std::string key, std::shared_ptr<Symbol> symbol);
	void addParam(std::string key, std::shared_ptr<Symbol> symbol);
	void addType(std::string, std::shared_ptr<Type>);
	std::shared_ptr<Symbol> findSymbol(std::string);
	std::shared_ptr<Type> findType(std::string);
	int getOffset() { return memoryOffset; }
	int getParamOffset() { return paramOffset; }
	std::string getLoc() { return memoryLocation; }
	void resetOffset() { memoryOffset = 0;}

	// only for use in For Statement
	void removeSymbol(std::string key) { m_symbols.erase(key); }
private:
	std::shared_ptr<SymbolTable> m_parent;
	std::map<std::string, std::shared_ptr<Symbol>> m_symbols;
	std::map<std::string, std::shared_ptr<Type>> m_types;
	std::string memoryLocation;
	int memoryOffset = 0;
	int paramOffset = 0;

};

#endif
