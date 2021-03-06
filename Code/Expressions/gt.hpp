#ifndef GT_HPP
#define GT_HPP

#include "Expression.hpp"
#include <memory>

class GT : public Expression
{
	GT(std::shared_ptr<Expression>, std::shared_ptr<Expression>);

private:
	std::shared_ptr<Expression> m_right;
	std::shared_ptr<Expression> m_left;
};


#endif
