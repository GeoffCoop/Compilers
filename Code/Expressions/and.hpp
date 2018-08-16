#ifndef AND_HPP
#define AND_HPP

#include "Expression.hpp"

class And : public Expression
{
	And(std::shared_ptr<Expression>, std::shared_ptr<Expression>);

private:
	std::shared_ptr<Expression> m_right;
	std::shared_ptr<Expression> m_left;
}


#endif
