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
	std::shared_ptr<Type> getType() { return m_type; }
	virtual std::string getSub() { return ""; }
	int m_value = 0;
protected:
	std::string id;
	std::shared_ptr<Type> m_type;
	int m_memoryLocation;
};

class VarSymbol: public Symbol {
public:
	VarSymbol(std::shared_ptr<Type> type, int memoryLocation)
	{
		m_type = type;
		m_memoryLocation = memoryLocation;
	}
	std::string getSub() { return "Var"; }
};

class ConstSymbol: public Symbol {
public:
	ConstSymbol(std::shared_ptr<Type> type, int value){
		m_type = type;
		m_value = value;
	}
//	int m_value;
	std::string getSub() { return "Const"; }
};

class TypeSymbol: public Symbol {
public:
	TypeSymbol(std::shared_ptr<Type> type, int memoryLocation){
		m_type = type;
		m_memoryLocation = memoryLocation;
	}
	std::string getSub() { return "Type"; }
};

#endif
