#ifndef SUB_HPP
#define SUB_HPP

class Sub : public Expression
{
	Sub(std::shared_ptr<Expression>, std::shared_ptr<Expression>);

private:
	std::shared_ptr<Expression> m_right;
	std::shared_ptr<Expression> m_left;
}


#endif
