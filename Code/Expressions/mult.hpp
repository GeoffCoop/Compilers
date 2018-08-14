#ifndef MULT_HPP
#define MULT_HPP

class Mult : public Expression
{
	Mult(std::shared_ptr<Expression>, std::shared_ptr<Expression>);

private:
	std::shared_ptr<Expression> m_right;
	std::shared_ptr<Expression> m_left;
}


#endif
