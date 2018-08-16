#ifndef DIV_HPP
#define DIV_HPP

#include "Expression.hpp"
#include <memory>

class Div : public Expression
{

	Div(std::shared_ptr<Expression>, std::shared_ptr<Expression>

private:
	std::shared_ptr<Expression> m_right;
	std::shared_ptr<Expression> m_left;
}


#endif
