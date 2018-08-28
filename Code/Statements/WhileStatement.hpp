#ifndef WHILESTATEMENT_HPP
#define WHILESTATEMENT_HPP

#include "Statement.hpp"
#include "../Expressions/Expression.hpp"
#include <vector>
#include <memory>

class WhileStatement : public Statement
{
public:


private:
    std::shared_ptr<Expression> condition;
    std::vector<std::shared_ptr<Statement>> statements;
};

#endif