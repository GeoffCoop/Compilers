#include <StringTable.hpp>

#include <string>
#include <map>

int label() {
    static int count = 0;
    return count++;
}

std::shared_ptr<StringTable> StringTable::singleton;

int StringTable::addString(std::string s){
    auto f = s_table.find(s);
    if (f == s_table.end()) {
        return f->second;
    }
    int i = label();
    s_table[i] = s;
    id_table[s] = i;
    return i;
}

std::string StringTable::lookupString(int i){
    return id_table[i];
}
