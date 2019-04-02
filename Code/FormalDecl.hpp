#ifndef FORMALDECL_HPP
#define FORMALDECL_HPP

#include "./Type.hpp"
#include <map>
#include <vector>
#include <iostream>

class Parameter {
public:
    Parameter (std::string n, std::shared_ptr<Type> t, bool rf): name(n), type(t), ref(rf)
    {}

    std::string name;
    std::shared_ptr<Type> type;
    bool ref;
};

class FormalDecl {
public:
    FormalDecl(std::vector<std::shared_ptr<Parameter>> p): params(p) {}
    FormalDecl() {
        params = std::vector<std::shared_ptr<Parameter>>();
    }

    std::vector<std::shared_ptr<Parameter>> params;
};

#endif