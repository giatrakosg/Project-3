//
//  Vector.cpp
//  Project-Cluster
//
//  Created by <author> on 18/11/2019.
//
//

#include "Vector.hpp"

Vector::Vector(std::string id,std::vector<double> coordinates) : Item(id) , coordinates(coordinates) {
}
Vector::Vector(std::string id) : Item(id) {}
bool Vector::isEqual(Item &q ) {
    Vector &p = dynamic_cast<Vector &> (q) ;
    bool idcmp = p.getId() == this->id ;
    // TODO implement point for point comparison
    return idcmp ;
}
void Vector::addPoint(double p) {
    coordinates.push_back(p);
}
double Vector::distance(Item *q) {
    // manhattan distance of the 2 vectors
    Vector *p = dynamic_cast<Vector *> (q) ;
    std::vector<double> qcoordinates = p->getCoordinates();
    double sum = 0 ;
    for (size_t i = 0; i < coordinates.size(); i++) {
        sum += abs(coordinates[i] - qcoordinates[i]);
    }
    return sum ;
}
Item * Vector::clone(void) {
    // manhattan distance of the 2 vectors
    Vector *p = new Vector(this->getId()) ;
    for (size_t i = 0; i < coordinates.size(); i++) {
        p->addPoint(coordinates[i]);
    }
    return p ;
}
std::vector<double>& Vector::getCoordinates()  {
    return coordinates ;
}
void Vector::print(std::ostream &out) {
    out << this->getId() << ":";
    out << "[" ;
    for (size_t i = 0; i < coordinates.size(); i++) {
        out << coordinates[i] << "," ;
    }
    out << "]\n" ;
}
double Vector::getmax(void) {
    return * std::max_element(coordinates.begin(),coordinates.end());
}
int Vector::getDimension(void) {
    return coordinates.size();
}
void Vector::init_0(int n) {
    for (int i = 0; i < n; i++) {
        this->addPoint(0);
    }
}
Vector::~Vector() {}
