#include <StringTable.hpp>

#include <string>
#include <map>
#include <iostream>

std::shared_ptr<StringTable> StringTable::singleton;
int label() {
    static int count = 0;
    return count++;
}


int StringTable::addString(std::string s){
    auto f = s_table.find(s);
    if (f != s_table.end()) {
        return f->second;
    }
    int i = label();
    s_table[s] = i;
    id_table[i] = s;
    return i;
}

std::string StringTable::lookupString(int i){
    return id_table[i];
}
