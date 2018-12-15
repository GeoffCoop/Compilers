#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include <string>
#include "Type.hpp"
#include <memory>
class Symbol{
public:
	Symbol () {}
	// Symbol(std::string& id, std::shared_ptr<Type> const& type, int memoryLocation):
	// 	id(id), 		//lexeme
	// 	type (type),	//type
	// 	m_memoryLocation(memoryLocation)
	// 	{}
	int getMemLoc() { return m_memoryLocation; }
protected:
	std::string id;
	std::shared_ptr<Type> m_type;
	int m_memoryLocation;

};

class VarSymbol: public Symbol {
	VarSymbol(std::shared_ptr<Type> type, int memoryLocation)
	{
		m_type = type;
		m_memoryLocation = memoryLocation;
	}
};

class ConstSymbol: public Symbol {

};

class TypeSymbol: public Symbol {
	TypeSymbol(std::shared_ptr<Type> type, int memoryLocation){
		m_type = type;
		m_memoryLocation = memoryLocation;
	}
}

#endif
