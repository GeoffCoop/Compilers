#ifndef FORSTATEMENT_HPP
#define FORSTATEMENT_HPP

#include "Statement.hpp"
#include "../Expressions/Expression.hpp"
#include <memory>
#include <vector>


class ForStatement : public Statement
{
public:
    ForStatement();

private:
    //entry in ST
    bool toDownTo; //downTo = true/1; to = false/0;
    std::shared_ptr<Expression> start;
    std::shared_ptr<Expression> end;
    std::vector<std::shared_ptr<Statement>> statements;

};

#endif