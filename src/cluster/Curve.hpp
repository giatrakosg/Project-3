//
//  Curve.hpp
//  Project-Cluster
//
//  Created by <author> on 18/11/2019.
//
//

#ifndef Curve_hpp
#define Curve_hpp

#include <algorithm>
#include <stdio.h>
#include "Item.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <math.h>
#include <random>

#include "Vector.hpp"

using namespace std ;

struct Point {
    double x ;
    double y ;
    Point(double x,double y) : x(x) , y(y) {}
    double dist(Point *p) {
        return sqrt((abs(p->x - x)*abs(p->x - x) + abs(p->y - y)*abs(p->y - y))) ; // euclidian distance
        // see https://eclass.uoa.gr/modules/forum/viewtopic.php?course=DI352&topic=22350&forum=33439
    }
    friend bool operator==(const Point& lhs, const Point& rhs){
        return (lhs.x == rhs.x) && (lhs.y == rhs.y) ;
    }
};


class Curve : public Item {
private:
    std::vector<Point *> points;
public:
    Curve(std::string );
    friend ostream& operator<<(ostream& os, const Curve& v) {
    os << "[ " << v.id << "|" ;
    for (size_t i = 0; i < v.points.size(); i++) {
        os << "(" << v.points[i]->x << "," << v.points[i]->y << "),"  ;
    }
    os << "]\n" ;
    return os ;
    }
    Point * getPoint(int );
    bool isEqual(Item &q) ;
    bool isEqual(Curve *);
    void addPoint(double ,double );
    void addPoint(Point *);
    int getSize();
    Curve * random_subsequence(int ); // We produce a random subsequence of this curve with length
    double distance(Item *q) ;
    Item * clone() ;
    double getmax();
    std::vector<std::pair<int,int>> dtwBestTraversal(Curve *q); // Returns the pairs of indices of the best traversal calculated with dtw
    void clear(void); // Clear points from Curve
    void print(std::ostream &out);
    int getDimension(void);
    // Turn curve to vector of length n . Fill with max point
    Vector * toVector(int n);
    ~Curve();
protected:

};


#endif /* Curve_hpp */
