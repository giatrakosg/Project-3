//
//  Item.cpp
//  Project-Cluster
//
//  Created by <author> on 18/11/2019.
//
//

#include "Item.hpp"

Item::Item(std::string id) : id(id)  {}
std::string Item::getId(void) {
    return id ;
}
Item::~Item() {}
