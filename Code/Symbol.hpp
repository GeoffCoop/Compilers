#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include <string>
#include "Type.hpp"

class Symbol{
public:
	std::string id;
	std::shared_ptr<Type> type;
	
	Symbol(std::string& id, std::shared_ptr<Type> const& type):
		id(id),
		type (type)

}

#endif
