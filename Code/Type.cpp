#include "Type.hpp"
#include <iostream>

void RecordType::addMember(std::string id, std::shared_ptr<Type> type){
    members.push_back(std::pair<std::string,std::shared_ptr<Type>>(id, type));
    if (offsetArray.size() == 0) {
        offsetArray.push_back(type->size());
    }
    else {
        int size = offsetArray[offsetArray.size() -1] + type->size();
        offsetArray.push_back(size);
    }
}

int RecordType::size(){
    return offsetArray[offsetArray.size() -1];
}

int RecordType::getOffset(std::string id) {
    for (int i = 0; i < offsetArray.size(); i++){
        if (members[i].first == id) {
            if (i == 0) return 0;
            else return offsetArray[i-1];
        }
    }
    std::cout << "ERROR: CANNOT FIND MEMBER " + std::string(id) + ". IT DOES NOT EXIST FOR THIS CLASS." << std::endl;
    return -1;
}

std::shared_ptr<Type> RecordType::getType(std::string id) {
    for (int i = 0; i < offsetArray.size(); i++){
        if (members[i].first == id) return members[i].second;
    }
}