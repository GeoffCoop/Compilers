#ifndef LVALUEEXPR_HPP
#define LVALUEEXPR_HPP

#include "Expression.hpp"
#include "../LValue.hpp"
#include <memory>

class LValue;

class LValueExpression : public Expression
{
	public:
	LValueExpression(std::shared_ptr<LValue> l) : lValue(l){}


	private:
	std::shared_ptr<LValue> lValue;

}


#endif
