//
//  Curve.cpp
//  Project-Cluster
//
//  Created by <author> on 18/11/2019.
//
//

#include "Curve.hpp"

Curve::Curve(std::string id) : Item(id)  {}
bool Curve::isEqual(Item &q ) {
    Curve &p = dynamic_cast<Curve &> (q) ;
    bool idcmp = p.getId() == this->id ;
    // TODO implement point for point comparison
    return idcmp ;
}
bool Curve::isEqual(Curve *q) {
    //std::cout << "isEqual" << std::endl ;
    if (q->getSize() != this->getSize()) {
        return false ;
    }
    for (int i = 0; i < q->getSize(); i++) {
        if (q->getPoint(i) != this->getPoint(i)) {
            return false ;
        }
    }
    return true ;
}
void Curve::addPoint(double px,double py) {
    Point *p = new Point(px,py);
    points.push_back(p);
}
void Curve::addPoint(Point *m) {
    points.push_back(m);
}
int Curve::getSize() {
    return points.size();
}
Point * Curve::getPoint(int i) {
    return points[i] ;
}
double min3(double x ,double y , double z) {
    return std::min({x, y, z});
}
Curve * Curve::random_subsequence(int l) {
    std::uniform_int_distribution<int> dis(0,this->getSize() - 1);
    std::default_random_engine generator;
    Curve *Sl = new Curve("rs");
    for (int i = 0; i < l; i++) {
        int index = dis(generator);
        Sl->addPoint(this->points[index]->x,this->points[index]->y);
    }
    return Sl ;

}
double Curve::distance(Item *p) {
    Curve *q = dynamic_cast<Curve *> (p) ;
    int n = this->getSize();
    int m = q->getSize() ;
    double DTW[n+1][m+1] ; // Array with distances
    for (int i = 0; i < n+1; i++) {
        for (int j = 0; j < m+1; j++) {
            DTW[i][j] = INFINITY ;
        }
    }
    DTW[0][0] = 0 ;
    for (int i = 1; i < n+1; i++) {
        for (int j = 1; j < m+1; j++) {
            double cost = (q->getPoint(j-1))->dist((points[i-1]));
            DTW[i][j] = cost + min3(DTW[i-1][j],DTW[i][j-1],DTW[i-1][j-1]);
        }
    }
    return DTW[n][m];

}
Item * Curve::clone(void) {
    Curve *p = new Curve(this->getId());
    for (int i = 0; i < this->getSize(); i++) {
        Point * m = this->getPoint(i);
        p->addPoint(m->x,m->y);
    }
    return p ;
}
// See https://nipunbatra.github.io/blog/2014/dtw.html
// Section [19]
std::vector<std::pair<int,int>> Curve::dtwBestTraversal(Curve *q) {
    /*
        path = [[len(x)-1, len(y)-1]]
        i = len(y)-1
        j = len(x)-1
        while i>0 and j>0:
            if i==0:
                j = j - 1
            elif j==0:
                i = i - 1
            else:
                if accumulated_cost[i-1, j] == min(accumulated_cost[i-1, j-1], accumulated_cost[i-1, j], accumulated_cost[i, j-1]):
                    i = i - 1
                elif accumulated_cost[i, j-1] == min(accumulated_cost[i-1, j-1], accumulated_cost[i-1, j], accumulated_cost[i, j-1]):
                    j = j-1
                else:
                    i = i - 1
                    j= j- 1
            path.append([j, i])
        path.append([0,0])
    */
    int n = this->getSize();
    int m = q->getSize() ;
    double DTW[n+1][m+1] ; // Array with distances
    for (int i = 0; i < n+1; i++) {
        for (int j = 0; j < m+1; j++) {
            DTW[i][j] = INFINITY ;
        }
    }
    DTW[0][0] = 0 ;
    for (int i = 1; i < n+1; i++) {
        for (int j = 1; j < m+1; j++) {
            double cost = (q->getPoint(j-1))->dist((points[i-1]));
            DTW[i][j] = cost + min3(DTW[i-1][j],DTW[i][j-1],DTW[i-1][j-1]);
        }
    }



    std::vector<std::pair<int,int>> path;
    int i = this->getSize() - 1;
    int j = q->getSize() - 1;
    while((i > 0) && (j > 0)) {
        if (i == 0) {
            j--;
        } else if(j == 0) {
            i--;
        } else {
            if (DTW[i-1][j] == min3(DTW[i-1][j-1],DTW[i-1][j],DTW[i][j-1])) {
                i--;
            } else if(DTW[i][j-1] == min3(DTW[i-1][j-1],DTW[i-1][j],DTW[i][j-1])) {
                j--;
            } else {
                i--;
                j--;
            }
        }
        path.push_back(pair<int,int>(j,i));
    }
    return path ;
}
void Curve::clear(void) {

    for (size_t i = 0; i < points.size(); i++) {
        delete points[i] ;
    }

    points.clear();
}
double Curve::getmax(void) {
    return 0 ;
}
void Curve::print(std::ostream &out) {
    out << this->getId() << ":";
    out << "[" ;
    for (size_t i = 0; i < points.size(); i++) {
        out << "(" << points[i]->x << "," << points[i]->y << ")"  << "," ;
    }
    out << "]\n" ;

}
int Curve::getDimension(void) {
    return this->getSize() ;
}
Vector * Curve::toVector(int n) {
    Vector *v1 = new Vector(this->getId());
    double max = -1 * INFINITY ;
    for (int i = 0;( i < points.size()) && (i < n); i++) {
        double x = points[i]->x ;
        double y = points[i]->y ;
        if (x > max) {
            max = x;
        }
        if (y > max) {
            max = y;
        }
        v1->addPoint(x);
        v1->addPoint(y);
    }
    for (int i = v1->getDimension(); i < n; i+=2) {
        v1->addPoint(max);
        v1->addPoint(max);

    }
    return v1 ;
}
Curve::~Curve() {
    for (size_t i = 0; i < points.size(); i++) {
        delete points[i] ;
    }
}
