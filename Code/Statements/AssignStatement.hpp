#ifndef ASSIGNMENTSTATEMENT_HPP
#define ASSIGNMENTSTATEMENT_HPP

#include "Statement.hpp"
#include "../LValue.hpp"
#include "../Expressions/Expression.hpp"
#include <memory>


class AssignStatement : public Statement 
{
public:
    AssignStatement();

private:
    std::shared_ptr<LValue> left;
    std::shared_ptr<Expression> right;
};

#endif