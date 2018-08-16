#ifndef TYPE_HPP
#define TYPE_HPP

#include <string>

class Type
{
	//Types of Symbol  int bool char string 
	virtual std::string name() = 0;	
	virtual int size() = 0;
};

class IntType : public Type
{
	std::string name() {	return "int";		}
};

class BoolType : public Type
{
	std::string name() {	return "bool";		}
};

class CharType : public Type
{
	std::string name() { 	return "char";		}
};

class StringType : public Type
{
	std::string name() {	return "string"; 	}
};

class RecordType : public Type
{};

class ArrayType : public Type
{};

#endif
