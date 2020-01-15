//
//  ConfParser.cpp
//  Project-Cluster
//
//  Created by <author> on 18/11/2019.
//
//

#include "ConfParser.hpp"

ConfParser::ConfParser(string &cf) : config_file(cf)  {}
void ConfParser::parseConfig(int &nofc,int &nofg,int &nofvht,int &nofvhf) {
    std::ifstream file(config_file);
    //std::istringstream str(file);

    // We extract the first line to determine if it is a vector or curve file
    std::string         number_of_clusters,number_of_grids,number_of_vector_hash_tables , number_of_vector_hash_functions;
    std::getline(file, number_of_clusters);

    auto pos = number_of_clusters.find(":");
    nofc = stoi(number_of_clusters.substr(pos+1));

    std::getline(file, number_of_grids);
    auto pos2 = number_of_grids.find(":");
    nofg = stoi(number_of_grids.substr(pos2+1));

    std::getline(file, number_of_vector_hash_tables);
    auto pos3 = number_of_vector_hash_tables.find(":");
    nofvht = stoi(number_of_vector_hash_tables.substr(pos3+1));

    std::getline(file, number_of_vector_hash_functions);
    auto pos4 = number_of_vector_hash_functions.find(":");
    nofvhf = stoi(number_of_vector_hash_functions.substr(pos4+1));

    std::cout << "config : " << number_of_clusters << " " << number_of_grids << " " << number_of_vector_hash_tables << " " << number_of_vector_hash_functions << std::endl ;
}
ConfParser::~ConfParser() {}
