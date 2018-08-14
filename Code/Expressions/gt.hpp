#ifndef GT_HPP
#define GT_HPP

class GT : public Expression
{
	GT(std::shared_ptr<Expression>, std::shared_ptr<Expression>);

private:
	std::shared_ptr<Expression> m_right;
	std::shared_ptr<Expression> m_left;
}


#endif
