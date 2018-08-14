#ifndef SYMBOLTABLE_HPP
#define SYMBOLTABLE_HPP

class SymbolTable
{
public:
	SymbolTable(std::shared_ptr<SymbolTable> parent):m_parent(parent)  {}
	std::shared_ptr<SymbolTable> getParent() { return parent; }

	addEntry(std::string key, Symbol symbol);
	findEntry(std::string s);
private:
	std::shared_ptr<SymbolTable> m_parent;

}

#endif
