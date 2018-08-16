#ifndef OR_HPP
#define OR_HPP

#include "Expression.hpp"
#include <memory>

class Or : public Expression
{
public:
    Or(std::shared_ptr<Expression>, std::shared_ptr<Expression>);

private:
    std::shared_ptr<Expression> m_right;
    std::shared_ptr<Expression> m_left;
};

#endif