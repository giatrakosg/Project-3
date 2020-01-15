//
//  Vector.hpp
//  Project-Cluster
//
//  Created by <author> on 18/11/2019.
//
//

#ifndef Vector_hpp
#define Vector_hpp

#include <stdio.h>
#include "Item.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std ;

class Vector : public Item {
private:
    std::vector<double> coordinates ;
public:
    Vector(std::string ,std::vector<double>);
    Vector(std::string ); // Init an empty vector
    friend ostream& operator<<(ostream& os, const Vector& v) {
    os << "[ " << v.id << "|" ;
    for (size_t i = 0; i < v.coordinates.size(); i++) {
        os << v.coordinates[i]<< ","  ;
    }
    os << "]\n" ;
    return os ;
    }
    bool isEqual(Item &q) ;

    double distance(Item *);
    Item * clone(void);
    void addPoint(double );
    std::vector<double>& getCoordinates() ;
    double getmax(void) ;
    Vector & operator += (Vector &other)  {
        auto m = other.getCoordinates();
        for (size_t i = 0; i < m.size(); i++) {
            coordinates[i] += m[i];
        }
        return *this ;
    }
    Vector & operator /= (double x) {
        for (size_t i = 0; i < coordinates.size(); i++) {
            coordinates[i] /= x ;
        }
        return *this ;
    }
    void print(std::ostream &);
    int getDimension();
    void init_0(int n);// Initializes the n positions of the vector to 0
    ~Vector();
protected:

};


#endif /* Vector_hpp */
