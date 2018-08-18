#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include <string>
#include "Type.hpp"

class Symbol{
public:

	Symbol(std::string& id, std::shared_ptr<Type> const& type, int memoryLocation):
		id(id), 		//lexeme
		type (type),	//type
		m_memoryLocation(memoryLocation)
		{}
private:
	std::string id;
	std::shared_ptr<Type> type;
	int m_memoryLocation;

};

#endif
