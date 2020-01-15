//
//  Parser.cpp
//  Project-Cluster
//
//  Created by <author> on 18/11/2019.
//
//

#include "Parser.hpp"

Parser::Parser(Database *db) : db(db) {}
bool Parser::parseFile(std::string &input_file) {
    std::ifstream file(input_file);
    //std::istringstream str(file);

    // We extract the first line to determine if it is a vector or curve file
    std::string         flag;
    std::getline(file, flag);
    if (flag.back() == '\r') {
        flag.pop_back();
    }
    std::cout << "Parsing vector file " << std::endl ;
    while (!file.eof()) {
        std::vector<std::string>    m_data;

        std::string         line;
        std::getline(file, line);

        std::stringstream   lineStream(line);
        std::string         cell;

        m_data.clear();
        while(std::getline(lineStream, cell, '\t'))
        {
            m_data.push_back(cell);
        }
        // This checks for a trailing comma with no data after it.
        //if (!lineStream && cell.empty())
        //{
            // If there was a trailing comma then add an empty element.
        //    m_data.push_back("");
        //}
        // We have reached the eof
        if (m_data.size() == 0) {
            continue ;
        }
        // We have stored the data in a vector of strings so we have to add them to our database
        string id = m_data[0] ;
        Vector *v = new Vector(id);
        m_data.erase(m_data.begin());
        if (m_data[m_data.size() - 1].compare("\r") == 0) {
            m_data.pop_back() ;
            //std::cout << "is r" << std::endl ;
        }
        for (size_t i = 0; i < m_data.size(); i++) {
            v->addPoint(std::stod(m_data[i]));
        }
        //m_data.pop_back();
        db->addItem(v);

        //std::cout << std::endl ;
    }
    return false ;
}
Parser::~Parser() {}
