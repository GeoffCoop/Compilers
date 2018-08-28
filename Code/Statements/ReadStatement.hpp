#ifndef READSTATEMENT_HPP
#define READSTATEMENT_HPP

#include "Statement.hpp"
#include "../Expressions/Expression.hpp"
#include "../LValue.hpp"
#include <vector>
#include <memory>

class ReadStatement : public Statement
{
    public:


    private:
    std::vector<std::shared_ptr<LValue>> lValues;
};

#endif