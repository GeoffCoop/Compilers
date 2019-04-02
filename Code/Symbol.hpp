#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include <string>
#include "Type.hpp"
#include <memory>
#include "FormalDecl.hpp"

class Symbol{
public:
	Symbol () {}
	// Symbol(std::string& id, std::shared_ptr<Type> const& type, int memoryLocation):
	// 	id(id), 		//lexeme
	// 	type (type),	//type
	// 	m_memoryLocation(memoryLocation)
	// 	{}
	int getMemLoc() { return m_offset; }
	std::string getLoc() { return m_memoryLocation; }
	std::shared_ptr<Type> getType() { return m_type; }
	virtual std::string getSub() { return ""; }
	int m_value = 0;
protected:
	std::string id;
	std::shared_ptr<Type> m_type;
	std::string m_memoryLocation;
	int m_offset;
};

class VarSymbol: public Symbol {
public:
	VarSymbol(std::shared_ptr<Type> type, std::string memoryLocation, int offset)
	{
		m_type = type;
		m_memoryLocation = memoryLocation;
		m_offset = offset;
	}
	std::string getSub() { return "Var"; }
};

class ConstSymbol: public Symbol {
public:
	ConstSymbol(std::shared_ptr<Type> type, int value){
		m_type = type;
		m_value = value;
	}
	ConstSymbol(std::shared_ptr<Type> type, char* str){}
//	int m_value;
	std::string getSub() { return "Const"; }
};

class TypeSymbol: public Symbol {
public:
	TypeSymbol(std::shared_ptr<Type> type){
		m_type = type;
	}
	std::string getSub() { return "Type"; }
};

class ParameterSymbol: public Symbol {
public:
	ParameterSymbol(std::shared_ptr<Type> t, std::shared_ptr<FormalDecl> p, int block) {
		m_type = t;
		params = p;
		m_value = block;
		function = true;
	}
	ParameterSymbol(std::shared_ptr<FormalDecl> p, int block) {
		params = p;
		m_value = block;
		function = false;
	}
	ParameterSymbol(bool f) : forward(f) {}
	bool forward = 0;
	std::shared_ptr<FormalDecl> params;
	std::string getSub() { return "Parameter"; }
	bool function;
};

#endif
