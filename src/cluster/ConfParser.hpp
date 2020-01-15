//
//  ConfParser.hpp
//  Project-Cluster
//
//  Created by <author> on 18/11/2019.
//
//

#ifndef ConfParser_hpp
#define ConfParser_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std ;

class ConfParser {
private:
    string &config_file ;
public:
    ConfParser(string &);
    void parseConfig(int &,int &,int &,int &) ;
    ~ConfParser();
protected:

};


#endif /* ConfParser_hpp */
