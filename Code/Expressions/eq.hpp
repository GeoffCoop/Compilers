#ifndef EQ_HPP
#define EQ_HPP

class EQ : public Expression
{
	EQ(std::shared_ptr<Expression>, std::shared_ptr<Expression>);

private:	
	std::shared_ptr<Expression> m_right;
	std::shared_ptr<Expression> m_left;
}


#endif
