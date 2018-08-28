#ifndef REPEATSTATEMENT_HPP
#define REPEATSTATEMENT_HPP

#include "Statement.hpp"
#include "../Expressions/Expression.hpp"
#include <memory>
#include <vector>

class RepeatStatement : public Statement
{
public:


private:
    std::shared_ptr<Expression> untilCondition;
    std::vector<std::shared_ptr<Statement>> statements;
    
};

#endif