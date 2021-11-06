#include "moga.h"

void moga(algorithm_data alg_data, vector<GASolution*> &p, Timer *t1){

    t1->stop();

    pair<double,double> w;
    vector<GASolution*> new_p;
    vector<GASolution*> parents, offspring;
    int random;
    unsigned i=0;

    while (t1->getElapsedTimeInMilliSec() < alg_data.time_limit) {


        random = rand()%100;
        w.first = double(random)/double(100);
        w.second = double(100-random)/double(100);

        ComputeFitness(p, w);

        Selection(p, alg_data.param.u_population_size);

        BinaryTournamentSelection(p, parents, alg_data.param.u_population_size);

        Crossover(parents, offspring, alg_data.param.u_population_size);
        Mutation(parents, offspring, alg_data.param.u_prob_mutation);

        //Apagar aqui pra não mudar o método so SPEA
        for (auto &it_parents : parents) {
            delete (GASolution*)it_parents;
        }
        parents.clear();

        Union(p, offspring);

        //Registar a contagem do tempo
        t1->stop();

        i++;

    }
}

void Selection(vector<GASolution *> &p, unsigned population_size){

    unsigned size;

    size = p.size();

    //Remover os indivíduos com menor aptidão
    if(size > population_size){
        SortByFitness(p);
        while (p.size() > population_size) {
            delete (GASolution*)p.back();
            p.pop_back();
        }
    }
}


void Union(vector<GASolution *> &a, vector<GASolution *> &b){


    bool add;
    for (auto &it_b : b) {
        add = PopulationAddIndividual(a, it_b);
        if(!add)
            delete (GASolution *)it_b;
    }

    b.clear();
}


void ComputeFitness(vector<GASolution *> &p, pair<double,double> w){

    int size;

    size = p.size();

    //Calcular o strengthValue
    for (int i = 0; i < size; i++) {
        p[i]->strength_value = 0;
        p[i]->set_solution_dominated.clear();
        for (int j = 0; j < size; j++) {
            if(i != j){
                //if(set[i].Dominate(set[j])){
                if(*p[j] < *p[i]){
                //if(*all[j] <= *all[i]){
                    p[i]->strength_value++;
                    p[i]->set_solution_dominated.push_back(p[j]);
                }
            }
        }
    }

    /*//Calcular o rawFitness
    for (int i = 0; i < size; i++) {
        p[i]->raw_fitness = 0;
        for (int j = 0; j < size; j++) {
            if(i != j){
                //if(set[j].Dominate(set[i])){
                if(*p[j] < *p[i]){
                //if(*all[i] < *all[j]){
                    p[i]->raw_fitness += p[j]->strength_value;
                }
            }
        }
    }*/

    unsigned max_makespan, min_makespan;
    double max_tec, min_tec;
    max_makespan = max_tec = 0;
    min_makespan = min_tec = UINT_MAX;

    for (auto it : p) {
        if(it->makeSpan < min_makespan)
            min_makespan = it->makeSpan;
        if(it->makeSpan > max_makespan){
            max_makespan = it->makeSpan;
        }

        if(it->TEC < min_tec)
            min_tec = it->TEC;
        if(it->TEC > max_tec){
            max_tec = it->TEC;
        }
    }

    //Calcular o fitness
    for (int i = 0; i < size; i++) {
        //Calcular o fitness
        p[i]->fitness = p[i]->strength_value
                + double((max_makespan - p[i]->makeSpan)/(max_makespan-min_makespan))*w.first
                + double((max_tec-p[i]->TEC)/(max_tec-min_tec))*w.second ;
    }

}
