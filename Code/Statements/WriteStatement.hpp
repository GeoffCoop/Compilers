#ifndef WRITESTATEMENT_HPP
#define WRITESTATEMENT_HPP

#include "Statement.hpp"
#include "../Expressions/Expression.hpp"
#include <vector>
#include <memory>

class WriteStatement : public Statement
{
    public:


private:
    std::vector<std::shared_ptr<Expression>> writeArgs;
};

#endif