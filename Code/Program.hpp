#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "SymbolTable.hpp"

class Program
{
public:
	Program():m_table(Program::initSymbolTable()) {}

	// TODO: add predefined types to init function
	static std::shared_ptr<SymbolTable> initSymbolTable() {
		auto table = std::make_shared<SymbolTable>() //constructor
		// add predefined types integer boolean char etc.
		return table;
	}

	std::shared_ptr<SymbolTable> m_table
	
}


#endif
