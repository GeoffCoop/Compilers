#ifndef NE_HPP
#define NE_HPP

class NE : public Expression
{
	NE(std::shared_ptr<Expression>, std::shared_ptr<Expression>);

private:
	std::shared_ptr<Expression> m_right;
	std::shared_ptr<Expression> m_left;
}


#endif
