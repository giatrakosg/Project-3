//
//  Database.hpp
//  Project-Cluster
//
//  Created by <author> on 18/11/2019.
//
//

#ifndef Database_hpp
#define Database_hpp

#include <stdio.h>
#include "Item.hpp"
#include <string>
#include <vector>
#include <map>

using namespace std ;

class Database {
private:
    std::vector<Item *> items;
    std::map<string, int> map; // We map each string to an int for accessing the database through indexing
public:
    Database();
    void printItem(std::string ); // Prints the item stored with that string id
    void addItem(Item *); // Adds item to database
    Item * getItem(std::string ); // Get a pointer to the Item stored by string
    Item * getItem(int ); // Gets the item at index
    int getSize(void) ; // Get size of database
    int getDimensions() ;
    int getIndex(Item *); //Returns index of item in db 
    ~Database();
protected:

};


#endif /* Database_hpp */
