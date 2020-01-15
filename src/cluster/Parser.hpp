//
//  Parser.hpp
//  Project-Cluster
//
//  Created by <author> on 18/11/2019.
//
//

#ifndef Parser_hpp
#define Parser_hpp

#include "Database.hpp"
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include "Vector.hpp"
#include "Curve.hpp"
using namespace std ;
// Pure virtual class that describes an interface


class Parser {
private:
public:
    Parser(Database *);
    bool parseFile(std::string &) ; // Parses the given file
    ~Parser();
protected:
    Database *db ; // Database class that the data is stored to

};


#endif /* Parser_hpp */
