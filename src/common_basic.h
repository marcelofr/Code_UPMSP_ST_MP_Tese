#ifndef COMMON_BASIC_H
#define COMMON_BASIC_H


#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <iostream>

//#define RELEASE
#define DEBUG
//#define IRACE


#define EPS 1e-8

using namespace std;

void MakeTrace();
string itos(int i);
string itos(size_t i);
string itos(unsigned i);
string dtos(double i);

void init(int uf[], int N ) ;
void init(vector<int> uf);

/* Find root of set containing x */
int Find( int uf[], int x ) ;

/* Union of sets containing x and y */
void Union( int uf[],int x, int y ) ;

//int myrandom (int i);

bool AddPoint(pair<unsigned, double> point, vector<pair<unsigned, double>> &non_dominated_set);
void SortByMakespan(vector<pair<unsigned, double>> &set_solution);
double CalcEuclideanDistance(pair<double, double> point, pair<double, double> cof_line);

class Timer{
    public:
        inline Timer();
        inline void start();
        inline void stop();
        inline double getElapsedTimeInMilliSec();
        inline void printElapsedTimeInMilliSec();
    private:
        double t1, t2;
};

Timer::Timer(){
    this->t1 = this->t2 = 0;
}

/*
 * Registra o tempo inicial de relogio
 */
void Timer::start(){
    this->t1 = clock();
}

/*
 * Registra o tempo final de relogio
 */
void Timer::stop(){
    this->t2 = clock();
}


/*
 * Retorna o tempo em milisecundos
 */
double Timer::getElapsedTimeInMilliSec(){
    double elapsedTime;
    elapsedTime = ((this->t2 - this->t1)/CLOCKS_PER_SEC)*1000;
    return elapsedTime;
}

/*
 * Imprime o tempo que passou em ms
 */
void Timer::printElapsedTimeInMilliSec(){
    cout << getElapsedTimeInMilliSec() << " ms" << endl;
}

struct parameters{
    string instance_folder;
    string instance_file;
    string instance_extension;
    string instance_name;
    string algorithm_name;
    string file_solution;
    string folder_solution;
    unsigned u_seed;
    unsigned u_max_time_factor;
    double d_alpha;
    unsigned u_prob_mutation;
    unsigned u_population_size;
    string s_seed, s_max_time_factor, s_alpha, s_prob_mutation, s_population_size;
    string s_destruction_factor;
    unsigned u_destruction_factor;
    string s_decomposition_size, s_decomposition_neighboor_size;
    unsigned u_decomposition_size, u_decomposition_neighboor_size;

};

struct algorithm_data{
    double time_limit;
    double elapsed_time_sec;
    vector<pair<unsigned, double>>non_dominated_set;
    bool is_optimal;
    parameters param;
    unsigned qtd_neighbor;
    unsigned max_shake_level;
    unsigned num_weights;
    unsigned num_group;
};

#endif // COMMON_BASIC_H
