#ifndef ADD_HPP
#define ADD_HPP

#include "Expression.hpp"

class Add : public Expression
{
	Add(std::shared_ptr<Expression>, std::shared_ptr<Expression>);

private:
	std::shared_ptr<Expression> m_right;
	std::shared_ptr<Expression> m_left;
}


#endif
