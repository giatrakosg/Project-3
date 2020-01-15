#include "Hashtable.h"



Hash::Hash(int k,float rad,int size,int d , Database *dbS)  // Constructor
{
    db = dbS ;
    this->k=k;
    radius=rad;
    hsize=size;
    w= W_CONST;
    this->d=d;
    for (int i=0;i<size;i++){
    	htable.push_back(new vector<Item *>);
    }
    m = ((uint64_t) pow(2,32)) -  5;

   	create_Starting_Points();
}

// Different implementation of starting Items
void Hash::create_Starting_Points(void) { //dimiourgia starting Items
    std::uniform_int_distribution<int> dis(0,w-1);
    std::default_random_engine generator;

    for (int i = 0; i < k; i++) { //ta starting Items einai ta simeia twn opoiwn tis diastaseis tou si xrisimopoioume gia
        Vector *sp = new Vector("sp") ; //ton upologismo tou a
        for (int j = 0; j < d; j++) {
            float floatPart = ((float)rand()/RAND_MAX) ;
            float intPart = dis(generator);
            sp->addPoint(intPart + floatPart);
        }
        start_points.push_back(sp);
    }


}
void Hash::insert_Database(void){ //kanoume to hashing mesw tis g kai analoga me to ti epistefei ta vazoume sto antistoixo bucket
	for (int i=0;i<db->getSize();i++){
        Item *x = db->getItem(i);
		int bucket_number=g(x);
		htable[bucket_number]->push_back(x);
	}
}

unsigned int Hash::h(Item *x,Item *s){ //ulopoiisi twn h(k) pou xrisimopoiountai apo tin g
    vector<int> a ;
    //std::vector<unsigned int>  sumTerms ;
    unsigned int sum = 0 ;
    Vector *vx = dynamic_cast<Vector *>(x);
    Vector *vs = dynamic_cast<Vector *>(s);
    auto xcoords = vx->getCoordinates();
    auto scoords = vs->getCoordinates();

    for (int i = 0; i < d; i++) {
        int temp = floor ( (xcoords[i] - scoords[i]) / w );
        a.push_back(temp );
    }
    for (int i = 0; i < d; i++) {
        sum += calculate_mod(m,i,256,a[a.size() - i - 1]) ;

    }

    return sum % 256 ;
}
unsigned int Hash::calculate_mod(unsigned int m,int power,unsigned int M,int a) { //kanei ti pra3i (ai+m*ai+...+m^(n-1)*ai)%M
    if (a == 0) {
        return 0 ;
    }
    unsigned int c = 1 ;
    for (int i = 0; i < power; i++) {
        // c = (c * m) % M ; ==
        c = ((c) * (m)) % M ;
    }
    unsigned int number = ((((unsigned int )a) ) * (c )) % M ;
    return number ;
}
unsigned int Hash::g(Item *x){ //i ulopoisi tis g
	unsigned int sum=0;
	int i;
	for (i=0;i<k;i++){
        unsigned int r = h(x,start_points[i]) ;
        r = r << ((k - i - 1)*(32 / k)) ; // Move the result to the left
		sum += r ;
	}
	return ( sum) % ((unsigned int ) hsize);
}
void Hash::printBuckets(void) {
    for (size_t i = 0; i < htable.size(); i++) {
        std::cout << "Bucket " << i << ":" << htable[i]->size() << '\n';
    }
}
std::pair<int,Item *> Hash::search_query_neighbour(Item *q,int maxR){ //pairnei ena query apo to arxeio pou mas exoun dwsei,kanei to hashing kai vriskei to pio kontino .
    // To maxR antistoixei sto 3L stis diafaneies
	//simeio se auto me vasi ti manhattan distance
	int bucket_number=g(q);
    // Se periptosi pou einai adeio to htable . Allios yperxe error me outofbounds
    if (htable[bucket_number]->size() == 0) {
        cout << "Empty bucket" << std::endl ;
        std::pair<int,Item *> ret(INFTY,NULL) ;
        return ret ;
    }
	int min_distance= q->distance(htable[bucket_number]->at(0)); //krataei ti minimum distance,vazw ws default timi tin apostasi apo to
	//prwto simeio tou bucket
	int distance; //krataei ti manhattan distance twn simeiwn kai an einai mikroteri tis min distance 8a tin antika8istw
	Item * neighbour=htable[bucket_number]->at(0); //krataei ton pio kontino geitona,vazw ws default timi to stringId tou prwtou stoixeiou
	for (size_t i=0;i<htable[bucket_number]->size();i++){ //htable[bucket_number].size() einai to mege8os tou bucket pou mpike to query,pou ousiastika
		//einai to pli8os twn simeiwn tis geitonias
		distance= q->distance(htable[bucket_number]->at(i));
		if (distance<min_distance){
			min_distance=distance;
			neighbour=htable[bucket_number]->at(i);
		}
        if (((int) i) > maxR) {
            break ;
        }
	}
    std::pair<int,Item *> ret(min_distance,neighbour);
    return ret ;
}

std::vector<std::pair<double,Item *>> Hash::range_search(Item *q,double c,double r) {
    std::vector<std::pair<double,Item *>> ret ;
    int bucket_number = g(q) ;
    // We access  the bucket of the q Item and iterate through each Item
    for (size_t i = 0; i < htable[bucket_number]->size(); i++) {
        Item *p = htable[bucket_number]->at(i) ;
        double dist = q->distance(p) ;
        if (dist < (double ) r*c) {
            ret.push_back(std::pair<int,Item *> (dist,p)) ;
        }
    }
    return ret ;
}

unsigned int Hash::mod_Calculator(unsigned int m,unsigned int power,unsigned int M,int ai){  //to xrisimopoiw gia na upologisw to upoloipw twn dunamewn tou m pou xrisimopoiountai stin h(x)
	unsigned int i;
	unsigned int numberm=1;
	unsigned int numbera=1;
	unsigned int number=1;
	for (i=0;i<power;i++){
		numberm = (numberm  * m ) % M ;
	}
	numberm=numberm%M; //dunami tou m mod M
	numbera=ai%M; //a mod M
	number=(numberm*numbera); //(m * a) mod M

	return number;
}
//https://stackoverflow.com/questions/11641629/generating-a-uniform-distribution-of-integers-/in-c
int Hash::uniform_distribution(int rangeLow, int rangeHigh) {
    double myRand = rand()/(1.0 + RAND_MAX);
    int range = rangeHigh - rangeLow + 1;
    int myRand_scaled = (myRand * range) + rangeLow;
    return myRand_scaled;
}


Hash::~Hash(){ //destructor tou hashtable
	for (int i=0;i<hsize;i++){
		delete htable[i];
	}
    for (size_t i = 0; i < start_points.size(); i++) {
        delete start_points[i];
    }
}
