#ifndef RETURNSTATEMENT_HPP
#define RETURNSTATEMENT_HPP

#include "Statement.hpp"
#include "../Expressions/Expression.hpp"
#include <memory>

class ReturnStatement : public Statement
{
    public:


    private:
    std::shared_ptr<Expression> expression;
};

#endif