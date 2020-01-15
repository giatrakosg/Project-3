//
//  Item.hpp
//  Project-Cluster
//
//  Created by <author> on 18/11/2019.
//
//

#ifndef Item_hpp
#define Item_hpp

#include <stdio.h>
#include <string>
#include <iostream>

// Virtual function that represents items stored in the database

class Item {
private:
public:
    Item(std::string );
    std::string getId(void) ;
    virtual bool isEqual(Item &) = 0 ;// Returns if 2 items are equal
    virtual double distance(Item *) = 0 ; // Returns the distance between two Items
    virtual Item * clone(void) = 0 ; // Deep copy of given Item pointer to this
    virtual double getmax(void) = 0 ; // Get max point in item
    virtual void print(std::ostream &) = 0;
    virtual int getDimension(void) = 0 ;
    virtual ~Item();
protected:
    std::string id ; // Item id
};


#endif /* Item_hpp */
