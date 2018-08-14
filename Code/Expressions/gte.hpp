#ifndef GTE_HPP
#define GTE_HPP

class Gte : public Expression
{
	Gte(std::shared_ptr<Expression>, std::shared_ptr<Expression>);

private:
	std::shared_ptr<Expression> m_right;
	std::shared_ptr<Expression> m_left;
}


#endif
