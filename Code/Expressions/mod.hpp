#ifndef MOD_HPP
#define MOD_HPP

class Mod : public Expression
{
	Mod(std::shared_ptr<Expression>, std::shared_ptr<Expression>);

private:
	std::shared_ptr<Expression> m_right;
	std::shared_ptr<Expression> m_left;
}


#endif
