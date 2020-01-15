//
//  Clustering.cpp
//  Project-Cluster
//
//  Created by <author> on 19/11/2019.
//
//

#include "Clustering.hpp"

Clustering::Clustering(Database *db,bool isCurve , int num_of_clusters ,int init_f,int assign_f,int update_f) : db(db) , k(num_of_clusters) , isCurve(isCurve){
    flags[0] = init_f ;
    flags[1] = assign_f ;
    flags[2] = update_f ;
    std::cout << "Started clustering with \n" <<
    "k=" << k << std::endl ;
        std::cout << "Calculating pairwise distances...." ;
        for (int i = 0; i < db->getSize(); i++) {
            for (int j = 0; j < db->getSize(); j++) {
                double d = db->getItem(i)->distance(db->getItem(j));
                dist[std::pair<int,int> (i,j)] = d ;
            }
        }
        std::cout << "done" << std::endl ;

    if (flags[1] == 1) {
        std::cout << "initializing hashtable..." ;
        r_rs = 1.2 ;
        if (isCurve) {
            toVectors();
            ht  = new Hash(10,1,(dbvc->getSize()/64),dbvc->getDimensions(),dbvc);
            ht->insert_Database();
            std::cout << "done" << std::endl ;
            ht->printBuckets();
        } else {
            ht  = new Hash(4,1,(db->getSize()/16),db->getDimensions(),db);
            ht->insert_Database();
            std::cout << "done" << std::endl ;
            ht->printBuckets();
        }
    }
}

void Clustering::random_init(void) {
    // We produce k distinct integers in the [0,n) range that
    // are the selected initial centers of the clusters

    std::uniform_int_distribution<int> distribution(0,db->getSize() - 1) ; // Uniform distribution used for
    // random select

    std::set<int> used ;
    int selected = 0 ;
    while (selected < k) {
        int index = distribution(this->generator);
        if (!(used.find(index) == used.end())) {
            continue ;
        }
        used.insert(index);
        Item *m = db->getItem(index)->clone();
        //m->deepCopy(db->getItem(index));
        // Add the the generated object to the representative map
        representative.insert(std::pair<int,Item *> (selected,m));
        selected++;
        // In case of medoid representation we store the index of the selected
        if (flags[2] == 0) {
            medoid_repr[selected] = index ;
        }
    }

}
Curve * Clustering::init_dba(std::vector<Curve *> &Sn) {
    double mean_length = 0 ;
    int num_curves = Sn.size() ;
    // Empty cluster
    if (num_curves == 0) {
        return NULL ;
    }
    for (size_t i = 0; i < Sn.size(); i++) {
        mean_length += Sn[i]->getSize() / (double) num_curves ;
    }
    mean_length = floor(mean_length);
    // We count the number of curves with length greater that the mean
    int num_gt_mean = std::count_if(Sn.begin(),Sn.end(),
    [mean_length](Curve *c) {
        return c->getSize() >= mean_length;
    });
    // Sort the vector of Curves by descending order
    std::sort(Sn.begin(),Sn.end(),
    [] (Curve *a,Curve *b) {
        return a->getSize() > b->getSize() ;
    });
    // We produce the index of the randomly selected curve
    std::uniform_int_distribution<int> dis(0,num_gt_mean-1);
    int rind = dis(generator);
    Curve * s0 = Sn[rind]->random_subsequence(mean_length);
    return s0 ;
    // We have selected a random sequence whose length is greater or equal to the mean length of all curves
    // Now we must create a random subsequence of s0 with length = mean_length

}
Point * Clustering::meanPoint(std::vector<Point *> m) {
    double sumx = 0;
    double sumy = 0;
    int n = m.size();
    for (size_t i = 0; i < m.size(); i++) {
        sumx += m[i]->x;
        sumy += m[i]->y;
    }
    Point *k = new Point(sumx/n,sumy/n);
    return k ;
}
Curve * Clustering::dba(std::vector<Curve *> Sn) {
    Curve *C = init_dba(Sn);
    if (C == NULL) {
        return NULL ;
    }
    int lambda = C->getSize();
    int iterations = MAX_MEAN_ITER ;
    while (iterations>0) {
        Curve *C_prime = new Curve(C->getId()) ;
        for (int i = 0; i < C->getSize(); i++) {
            Point *m = C->getPoint(i);
            C_prime->addPoint(m->x,m->y);
        }
        std::vector<std::vector<Point *>> A;
        for (int i = 0; i < lambda; i++) {
            A.push_back(std::vector<Point *>());
        }

        for (size_t i = 0; i < Sn.size(); i++) {
            auto lpairs = C->dtwBestTraversal(Sn[i]);
            for (auto &p : lpairs) {
                /*
                if (p.second >= Sn[i]->getSize()) {
                    printf("Error\n");
                }
                */
                A[p.second].push_back(Sn[i]->getPoint(p.first));
            }
        }
        C->clear();
        //Curve *C2 = new Curve("c2") ;
        for (size_t i = 0; i < A.size(); i++) {
            Point * np = meanPoint(A[i]);
            C->addPoint(np->x,np->y);
        }
        //C->clear();
        //C = C2 ;
        if (C->isEqual(C_prime)) {
            delete C_prime ;
            break ;
        }
        delete C_prime ;
        iterations--;
    }
    return C ;
}
Vector * Clustering::meanVector(std::vector<Vector *> vectors) {
        if (vectors.empty()) {
            return NULL ;
        }
        Vector *mean = new Vector("m");
        int d = vectors[0]->getDimension();
        mean->init_0(d);
        for (size_t i = 0; i < vectors.size(); i++) {
            *mean += *vectors[i];
        }
        *mean /= vectors.size();
        return mean ;
}
double Clustering::D(Item *t){
    double min_distance = t->distance(representative[0]); //vazw ws minimum distance enos Item tin apostasi apo to prwto centroid pou
    for (size_t i = 1; i < representative.size(); i++){ //vrisketai sta representatives
        double dist = t->distance(representative[i]);
        if (dist < min_distance){
            min_distance = dist;
        }
    }
    return min_distance;
}
    std::pair<double,int> Clustering::closest_rep(Item *t) {
    double min_distance = INFINITY ;
    int min_index = -1 ;
    for (size_t i = 0; i < representative.size(); i++) {
        double dist = t->distance(representative[i]);
        if (dist < min_distance) {
            min_distance = dist ;
            min_index = i ;
        }
    }
    return std::pair<double,int>(min_distance,min_index);
}
std::pair<double,int> Clustering::closest_rep(int qi) {
    double min_distance = INFINITY ;
    int min_index = -1 ;
    Item *t = db->getItem(qi);
    for (size_t i = 0; i < representative.size(); i++) {
        if (representative[i] == NULL) {
            continue ;
        }
        double dist = t->distance(representative[i]);
        if (dist < min_distance) {
            min_distance = dist ;
            min_index = i ;
        }
    }
    return std::pair<double,int>(min_distance,min_index);
}

int Clustering::find_new_centroid(set<int> &used){
    vector< std::pair<int,double> > partial_sum_array; //pinakas tetragwnwn merikwn a8roismatwn
    partial_sum_array.push_back(std::pair<int,double> (-1,0.0)) ; //vazw ws prwto item sto pinaka to -1,0 giati o ari8mos -1
    double min_distance; //den anikei sto set mas kai i apostasi 0.0 gia na arxikopoiisoume to a8roisma twn merikwn apostasewn
    //uparxei periptwsi to Item 0 na einai centroid
    for (int i = 0; i < db->getSize(); i++){ //ftiaxnei to pinaka twn tetragwnwn twn merikwn a8roismatwn
        if (!(used.find(i) == used.end())){ //an to Item einai sta used diladi einai centroid tote de vriskei tin minimum apostasi tou
            continue;
        }
        min_distance = D(db->getItem(i)) / 1.2 ; //vriskw tin minimum distance enos simeiou apo to centroid tou kai tin apo8ikeuw
        std::pair<int,double> p = partial_sum_array[partial_sum_array.size() - 1] ; //ftiaxnw ena kainourio pair p wste na krataw ti
        //meriki apostasi tou proigoumenou Item tou pinaka pou mou xreiazetai gia to Item pou exw
        partial_sum_array.push_back(std::pair<int,double> (i,p.second + min_distance)) ; //pros8etw ti metriki apostasi tou proigoumenou
        //item me tou trexontos wste na ftia3w ti meriki apostasi tou trexontos item
    }
    std::uniform_int_distribution<int> distribution(0,partial_sum_array[partial_sum_array.size() - 1].second) ;
    double x = distribution(this-> generator) ; //pairnw ena random ari8mo anamesa sto 0 kai ti megisti metriki apostasi
    //pou einai i metriki apostasi tou item tou teleutaiou antikeimenou tou pinaka partial_sum_arrat
    int new_centroid_position = Binary_search(partial_sum_array,x,partial_sum_array[1].first,partial_sum_array[partial_sum_array.size() - 1].first);
    //kanw binary search gia na vrw ti 8esi tou kainouriou item
    return new_centroid_position ;
}


//epistrefei ti 8esi tou neou centroid sto db
int Clustering::Binary_search(vector< std::pair<int,double> > &partial_sum_array,double x,int l,int r){ //to l einai to prwto kai to r einai to teleutaio stoixeio
    if (r >= 1){
        int mid = (l + (r - 1)) / 2;
        double var1 = abs(x - partial_sum_array[mid].second ) ; //to var1 einai i diafora meta3u tou ari8mou pou diale3ame tuxaia kai tou mid
        //einai to middle tou ka8e upopinaka
        double var2 = abs(partial_sum_array[mid].second - partial_sum_array[mid - 1].second ) ;
        //to var2 einai i diafora meta3u tou ari8mou pou diale3ame tuxaia kai tou mid-1 pou einai o proigoumenos ari8mos apo to middle
        double var3 = abs(partial_sum_array[mid].second - partial_sum_array[mid + 1].second ) ;
        //antistoixa gia ta mid+1
        if (var1 <= var2 && var1<=var3 )
            return partial_sum_array[mid].first;
        if (x < partial_sum_array[mid].second )
            return Binary_search(partial_sum_array,x, l , mid - 1);
        return Binary_search(partial_sum_array,x, mid + 1, r );
    }
    return -1;
}


void Clustering::kmeans_init(void) {
   std::uniform_int_distribution<int> distribution(0,db->getSize() - 1) ; // Uniform distribution used for

    int index = distribution(this -> generator); //to index einai i 8esi pou exei to item sti vasi
    int selected = 0;
    std::set<int> used ;
    used.insert(index) ;
    Item *m = db->getItem(index)->clone();
    representative.insert(std::pair<int,Item *> (selected,m));
    selected++ ;
    while (selected < k){
        index = find_new_centroid(used) ;
        used.insert(index) ;
        m = db->getItem(index)->clone() ;
        representative.insert(std::pair<int,Item *> (selected,m)) ;
        selected++ ;
    }
}

void Clustering::lloyd_assign(void) {
    // Clear out previous assignments
    for (auto & x : assigned)
    {
        x.second.clear();
    }

    for (int i = 0; i < db->getSize(); i++) {
        double min_dist = INFINITY ;
        int min_index = -1 ;
        for (int j = 0; j < k; j++) {
            if (representative[j] == NULL) {
                continue ;
            }
            double d_to_c = db->getItem(i)->distance(representative[j]);
            if (d_to_c < min_dist) {
                min_index = j ;
                min_dist = d_to_c ;
            }
        }
        assigned[min_index].push_back(i) ;
    }
}
void Clustering::range_search_assign(void) {
    // Tuple (db index,is assigned,cluster,dist)
    std::vector<std::tuple<bool,int,double>> cassn ;
    for (int i = 0; i < db->getSize(); i++) {
        auto t = std::make_tuple(false,-1,-1);
        cassn.push_back(t);
    }
    // We cycle through each representative and call range_search
    for (auto &r : representative) {
        Vector *rep ;
        if (r.second == NULL) {
            continue ;
        }
        if (isCurve) {
            // We turn the representative to a vector and then do the range search
            rep = dynamic_cast<Curve *>(r.second)->toVector(dbvc->getDimensions());
        }else {
            rep = dynamic_cast<Vector *>(r.second);
        }
        auto ball = ht->range_search(rep,c_rs,r_rs);
        // For each item the range search returns we update the entry on the cassn vector
        std::cout << "rep=" << r.first << "|range =" << ball.size() << std::endl;
        for (auto &bp : ball) {
            int index = db->getIndex(bp.second);
            if (std::get<0>(cassn[index]) == false) {
                cassn[index] = std::make_tuple(true,r.first,bp.first);
            } else {
                if (std::get<2>(cassn[index]) > bp.first) {
                    cassn[index] = std::make_tuple(true,r.first,bp.first);
                } else {
                    continue ;
                }
            }
        }
    }
    for (size_t i = 0; i < cassn.size(); i++) {
        if (std::get<0>(cassn[i]) == false) {
            auto closest = closest_rep(i);
            cassn[i] = std::make_tuple(true,closest.second,closest.first);
        }
    }
    for(auto &assn: assigned) {
        assn.second.clear();
    }
    for (size_t i = 0; i < cassn.size(); i++) {
        int cluster = std::get<1>(cassn[i]) ;
        assigned[cluster].push_back(i);
    }
    r_rs *= 2.0;

}
void Clustering::pam_update(void) {
    // We iterate through each cluster and update the medoid
    for (auto & x : assigned)
    {
        std::vector<int> &items = x.second ;
        double min_total_dist = INFINITY ;
        int min_index = -1 ;
        for (size_t i = 0; i < items.size(); i++) {
            double total_dist = 0 ;
            int index1 = items[i];
            for (size_t j = 0; j < items.size(); j++) {
                int index2 = items[j] ;
                total_dist += dist[pair<int,int>(index1,index2)] ; // We have already calculated the pairwise distances
            }
            if (total_dist < min_total_dist) {
                min_total_dist = total_dist ;
                min_index = index1 ;
            }
        }
        // Empty cluster
        if (items.size() == 0) {
            representative[x.first] = NULL;
            continue ;
        }
        representative[x.first] = db->getItem(min_index);
        medoid_repr[x.first] = min_index ;
    }

}
// We convert the curves to vectors for the lsh
void Clustering::toVectors(void) {
    dbvc = new Database();
    int d = db->getDimensions();
    for (int i = 0; i < db->getSize(); i++) {
        Curve * curve = dynamic_cast<Curve *>(db->getItem(i));
        Vector * vector = curve->toVector(2*d);
        dbvc->addItem(vector);
    }
}
void Clustering::mean_update(void){
    if (isCurve) {
        for (auto &cluster : assigned) {
            std::vector<Curve *> curves;
            for (size_t i = 0; i < cluster.second.size(); i++) {
                curves.push_back(dynamic_cast<Curve *> (db->getItem(cluster.second.at(i)))) ;
            }
            auto nm = dba(curves);
            delete representative[cluster.first] ;
            representative[cluster.first] = nm ;
        }
    } else {
        for (auto &cluster : assigned) {
            std::vector<Vector *> vectors;
            for (size_t i = 0; i < cluster.second.size(); i++) {
                vectors.push_back(dynamic_cast<Vector *> (db->getItem(cluster.second.at(i)))) ;
            }
            auto nm = meanVector(vectors);
            delete representative[cluster.first] ;
            representative[cluster.first] = nm ;
        }

    }
}
void Clustering::init_c_rs(void) {
    // We init the c_rs variable to min(dist between centers)/2
    double min = INFINITY;
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            if (i == j) {
                continue ;
            }
            double d = representative[i]->distance(representative[j]);
            if (d < min ) {
                min = d;
            }
        }
    }
    c_rs = min / 2.0 ;

}
void Clustering::init(void) {
    if (flags[0] == 0) {
        random_init();
    } else if (flags[0] == 1) {
        kmeans_init();
    }
    init_c_rs();
}
void Clustering::assign(void) {
    if (flags[1] == 0) {
        lloyd_assign();
    } else if (flags[1] == 1) {
        range_search_assign();
    }
}
void Clustering::update(void) {
    if (flags[2] == 0) {
        pam_update();
    } else if (flags[2] == 1) {
        mean_update();
    }
}
void Clustering::runClustering(void) {
    clock_t begin = clock();
    init();
    for (size_t i = 0; i < MAX_ITERATIONS; i++) {
        assign();
        update();
    }
    clock_t end = clock();
    elapsed_secs = double(end -begin) / CLOCKS_PER_SEC ;


}
void Clustering::printRepresentatives(void) {
    for (auto const& x : representative)
    {
        //Item * x_it = x.second;
        std::cout << "cluster=" << x.first << " " ;
        if (x.second == NULL) {
            std::cout << "Empty cluster" << std::endl ;
            continue ;
        }
        std::cout << "representative id=" << x.second->getId() << " " ;
        int na = assigned[x.first].size();
        std::cout << "number of points in cluster =" << na << std::endl ;
    }
}
Clustering::~Clustering() {
    if (flags[2] == 0) {
        /* code */
    }
    if (flags[2] == 1) {
        // We delete the represenentatives if mean update since they do not belong to the db an wont be deleted
        // Else , since the representatives are in the dataset they will be deleted by the db
        for(auto &p : representative) {
            delete p.second ;
        }
    }
}


int Clustering::nearest_cluster(int cluster){
//epistrefei to kontinotero cluster apo ena simeio
    Item * centroid1 = representative[cluster];
    //Item * centroid2 ;
    int n_cluster ;
    double min_dist = INFINITY;
    /*
    if (cluster > 0){
        centroid2 = representative[0];
        n_cluster = 0 ;
    }
    else{
        centroid2 = representative[1];
        n_cluster = 1 ;
    }
    min_dist = centroid1->distance(centroid2);
*/
    for (auto & x : representative){
        if (x.first == cluster){
            continue;
        }
        double dist = centroid1->distance(x.second);
        if (dist < min_dist){
            min_dist = dist;
            n_cluster = x.first ;
        }
    }
    return n_cluster;
}

double Clustering::Silhouette_point(int cluster,int point,int nearest_cluster){
    std::vector<int> cluster_points = assigned[cluster];
    std::vector<int> nearest_cluster_points = assigned[nearest_cluster];

    double ai = 0;
    double bi = 0;
    for (int i = 0; i < (int)cluster_points.size(); ++i)
    {
        if (cluster_points[i] == point)
            continue;
        ai += dist[pair<int,int>(point,cluster_points[i])];
    }
    for (unsigned i = 0; i < nearest_cluster_points.size(); ++i)
    {
        bi += dist[pair<int,int>(point,nearest_cluster_points[i])];
    }
    ai = ai / cluster_points.size();
    bi = bi / nearest_cluster_points.size();
    //std::cout << ai << " "<< bi << std::endl;
    double max = ai > bi ? ai : bi;
    return (bi - ai)/max;
}
double Clustering::Silhouette_cluster(int c){
    double total_si = 0 ;
    int n_cluster ; //o ari8mos tou cluster pou vrisketai to item, to kontinotero cluster sto item, kai to item to idio
    //for (int j = 0; j < db->getSize(); j++){ //pairnw ta index olwn twn item tis vasis
        for (auto & x : assigned[c]){ //mpainw sto assigned gia na vrw se poio cluster anoikei to item
          n_cluster = nearest_cluster(c);
          total_si += Silhouette_point(c,x,n_cluster);
        }
    //}
    total_si /= db->getSize() ;
    return total_si ;
}

double Clustering::Silhouette(){
    double total_si = 0; //8a krataw edw tin a3iologisi tou si pou einai o ari8mos pou 8a epistrefei i silhouette
    int cluster,n_cluster ; //o ari8mos tou cluster pou vrisketai to item, to kontinotero cluster sto item, kai to item to idio
    for (int j = 0; j < db->getSize(); j++){ //pairnw ta index olwn twn item tis vasis
        for (auto & x : assigned){ //mpainw sto assigned gia na vrw se poio cluster anoikei to item
        std::vector<int> &items = x.second ; //vazw stin items ton vector me ta index twn items tou cluster
            for (size_t i = 0; i < items.size(); i++) { //diasxizw twn vector me ta items
                if (items[i] == j){ //an to item[i] to opoio einai to index enos ari8mou einai to idio me to j pou einai to item
                    cluster = x.first ; //pou exoume twra einai idia simainei oti to item auto vrisketai se auto to cluster
                }
            }
        }
        n_cluster = nearest_cluster(cluster);
        total_si += Silhouette_point(cluster,j,n_cluster);
    }
    total_si = total_si / (db->getSize()) ;
    return total_si ;
}

void Clustering::printResults(std::ostream &out,bool complete) {
    double stotal = Silhouette();
    double scluster ;
    if (!isCurve) {
        out << "Algorithm: I" << flags[0] << "A" << flags[1] << "U" << flags[2] << std::endl ;
        for (int i = 0; i < k; i++) {
            int cluster_size = assigned[i].size();
            out << "CLUSTER-" << i + 1<< "{size: " << cluster_size << "," ;
            // Centroid
            if (complete) {
                out << "{" ;
                for (size_t j = 0; j < assigned[i].size(); j++) {
                    out << db->getItem(assigned[i][j])->getId() << "," ;
                }
                out <<"}\n";
            }
            if (flags[2] == 0) {
                Item *centroid = db->getItem(medoid_repr[i]) ;
                out << "centroid :" << centroid->getId() << "}" ;
            } else {
                Vector *centroid = dynamic_cast<Vector *>(representative[i]);
                out << *centroid ;
            }
            out << std::endl ;
        }
        out << "clustering_time :" << elapsed_secs << std::endl ;
        out << "Silhouette : [" ;
        for (size_t i = 0; i < k; i++) {
            scluster = Silhouette_cluster(i);
            out << scluster << "," ;
        }
        out << stotal << "]"<< std::endl ;
    } else {
        out << "Algorithm: I" << flags[0] << "A" << flags[1] << "U" << flags[2] << std::endl ;
        for (int i = 0; i < k; i++) {
            int cluster_size = assigned[i].size();
            out << "CLUSTER-" << i + 1<< "{size: " << cluster_size << "," ;
            // Centroid
            if (complete) {
                out << "{" ;
                for (size_t j = 0; j < assigned[i].size(); j++) {
                    out << db->getItem(assigned[i][j])->getId() << "," ;
                }
                out <<"}\n";
            }

            if (flags[2] == 0) {
                Item *centroid = db->getItem(medoid_repr[i]) ;
                out << "centroid :" << centroid->getId() << "}" ;
            } else {
                Curve *centroid = dynamic_cast<Curve *>(representative[i]);
                if (centroid == NULL) {
                    out << "centroid : none\n" ;
                    continue ;
                }
                out << *centroid ;
            }
            out << std::endl ;
        }
        out << "clustering_time :" << elapsed_secs << std::endl ;
        out << "Silhouette : [" ;
        for (size_t i = 0; i < k; i++) {
            scluster = Silhouette_cluster(i);
            out << scluster << "," ;
        }
        out << stotal << "]"<< std::endl ;


    }
}
