#ifndef TYPE_HPP
#define TYPE_HPP

#include <string>
#include <memory>
#include <vector>

class Type
{
	//Types of Symbol  int bool char string 
public:
	virtual std::string name() = 0;	
	virtual int size() = 0;
};

class IntType : public Type
{
	std::string name() {	return "int";		}
	int size() { return 4; }
};

class BooleanType : public Type
{
	std::string name() {	return "bool";		}
	int size() { return 4;	}
};

class CharType : public Type
{
	std::string name() { 	return "char";		}
	int size() { return 4; }
};

class StringType : public Type
{
	std::string name() {	return "string"; 	}
	int size() { return 0; }
};

class RecordType : public Type
{
public:
	void addMember(std::string, std::shared_ptr<Type> );
	int getOffset(std::string);
	std::shared_ptr<Type> getType(std::string);
	std::vector<std::pair<std::string, std::shared_ptr<Type>>> members;
	std::vector<int> offsetArray;
	std::string name() { return "record"; }
	int size();
};

class ArrayType : public Type
{	
public:
	ArrayType(std::shared_ptr<Type> t, int l, int u): 
		type(t), 
		lower(l), 
		upper(u)
		{}
	std::shared_ptr<Type> type;
	int lower;
	int upper;
	std::string name() { return "array"; }
	int size() { return (upper-lower+1) * type->size(); }
	
};

class BuiltInType {
public:
	static std::shared_ptr<Type> getInt() { return std::make_shared<IntType>(); };
	static std::shared_ptr<Type> getBoolean() { return std::make_shared<BooleanType>(); };
	static std::shared_ptr<Type> getChar() { return std::make_shared<CharType>(); };
	static std::shared_ptr<Type> getString() { return std::make_shared<StringType>(); };
};

#endif
