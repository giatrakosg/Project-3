//
//  CMDParser.hpp
//  Project-Cluster
//
//  Created by <author> on 18/11/2019.
//
//

#ifndef CMDParser_hpp
#define CMDParser_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <getopt.h>

class CMDParser {
private:
public:
    CMDParser();
    void getArgs(int ,char **,std::string &,std::string &,std::string &,bool &); // Receives the argc , argv and string for the input , configuration and output file and  parses the command line arguments
    ~CMDParser();
protected:

};


#endif /* CMDParser_hpp */
