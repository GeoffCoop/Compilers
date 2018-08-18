#ifndef NE_HPP
#define NE_HPP

#include "Expression.hpp"
#include <memory>

class NE : public Expression
{
public:
	NE(std::shared_ptr<Expression>, std::shared_ptr<Expression>);

private:
	std::shared_ptr<Expression> m_right;
	std::shared_ptr<Expression> m_left;
};


#endif
