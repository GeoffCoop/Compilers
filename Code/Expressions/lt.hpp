#ifndef LT_HPP
#define LT_HPP

#include "Expression.hpp"
#include <memory>

class Lt : public Expression
{
	Lt(std::shared_ptr<Expression>, std::shared_ptr<Expression>);

private:
	std::shared_ptr<Expression> m_right;
	std::shared_ptr<Expression> m_left;
}


#endif
