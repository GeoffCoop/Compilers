#ifndef LTE_HPP
#define LTE_HPP 

class Lte : public Expression
{
	Lte(std::shared_ptr<Expression>, std::shared_ptr<Expression>);

private:	
	std::shared_ptr<Expression> m_right;
	std::shared_ptr<Expression> m_left;
}


#endif
