#ifndef PROCCALL_HPP
#define PROCCALL_HPP

#include "Statement.hpp"
#include "Expressions/Expression.hpp"
#include <vector>
#include <memory>

class ProcedureCall : public Statement
{
public:


private:
    std::vector<std::shared_ptr<Expression>> args;
};

#endif