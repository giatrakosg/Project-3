//
//  Database.cpp
//  Project-Cluster
//
//  Created by <author> on 18/11/2019.
//
//

#include "Database.hpp"

Database::Database()  {}
void Database::addItem(Item *t) {
    int index = items.size() ; // We get the index of where  the item will be added
    items.push_back(t);
    map.insert(std::pair<string,int> (t->getId(),index));
}
Item * Database::getItem(std::string qid) {
    int index = map[qid];
    return items[index];
}
Item * Database::getItem(int qind) {
    return items[qind];
}
int Database::getSize(void) {
    return items.size();
}
int Database::getDimensions(void) {
    int max = -1 ;
    for (size_t i = 0; i < items.size(); i++) {
        if (items[i]->getDimension() > max) {
            max = items[i]->getDimension() ;
        }
    }
    return max;
}
int Database::getIndex(Item *q){
    for (int i = 0; i < this->getSize(); i++) {
        if (items[i]->getId() == q->getId()) {
            return i ;
        }
    }
    return -1 ;
}
Database::~Database() {
    for (size_t i = 0; i < items.size(); i++) {
        delete items[i] ;
    }
}
