#ifndef STRINGTABLE_HPP
#define STRINGTABLE_HPP

#include <map>
#include <string>
#include <memory>
#include <iostream>


class StringTable {
    public:
    static std::shared_ptr<StringTable> instance() {
        if (!singleton){
            singleton = std::shared_ptr<StringTable>(new StringTable());
        }
        return singleton;
    }

    int addString(std::string);
    std::string lookupString(int);
    std::map<std::string, int> getTable() { return s_table; }
private:
    static std::shared_ptr<StringTable> singleton;
    std::map<std::string, int> s_table;
    std::map<int, std::string> id_table;
};


#endif
