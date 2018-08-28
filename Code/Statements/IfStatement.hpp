#ifndef IFSTATEMENT_HPP
#define IFSTATEMENT_HPP

#include <memory>
#include <vector>
#include "Statement.hpp"
#include "../Expressions/Expression.hpp"

class IfStatement : public Statement
{
public:
    typedef std::pair<std::shared_ptr<Expression>, std::vector<std::shared_ptr<Statement>>> conditionalStatement;
    IfStatement(std::vector<conditionalStatement> a, std::vector<std::shared_ptr<Statement>> b) : ifs(a), elseStatement(b)
    {}
private:
    std::vector<std::shared_ptr<Statement>> elseStatement;
    std::vector<conditionalStatement> ifs;
};

#endif