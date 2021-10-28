#include "moga.h"

void moga(algorithm_data alg_data, vector<GASolution*> &p, Timer *t1){

    t1->stop();

    pair<double,double> w;
    vector<GASolution*> new_p;
    int random;

    while (t1->getElapsedTimeInMilliSec() < alg_data.time_limit) {



        Crossover(p, new_p, alg_data.param.u_population_size);
        Mutation(p, new_p, alg_data.param.u_prob_mutation);

        random = rand()%100;
        w.first = double(random)/double(100);
        w.second = double(100-random)/double(100);

        ComputeFitness(p, w);
        ComputeFitness(new_p, w);

        Selection(p, new_p, alg_data.param.u_population_size);


        //Registar a contagem do tempo
        t1->stop();

    }
}

void Selection(vector<GASolution *> &p, vector<GASolution *> &new_p,
                            unsigned population_size){

    vector<GASolution *> all;
    unsigned size;

    //Remover duplicidade
    for (auto &it_p: p) {
        for (auto it_new_p = new_p.begin(); it_new_p != new_p.end();) {
            if(it_p->makeSpan == (*it_new_p)->makeSpan
                    && abs(it_p->TEC - (*it_new_p)->TEC) < EPS){
                delete *it_new_p;
                it_new_p = new_p.erase(it_new_p);
            }
            else{
                ++it_new_p;
            }
        }
    }

    copy(p.begin(), p.end(), back_inserter(all));
    copy(new_p.begin(), new_p.end(), back_inserter(all));

    size = all.size();

    //Remover os indivíduos com menor aptidão
    if(size > population_size){
        SortByFitness(all);
        while (all.size() > population_size) {
            delete all.back();
            all.pop_back();
        }
    }

    new_p.clear();
    p.clear();

    copy(all.begin(), all.end(), back_inserter(p));

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
                if(*p[i] < *p[j]){
                //if(*all[j] <= *all[i]){
                    p[i]->strength_value++;
                    p[i]->set_solution_dominated.push_back(p[j]);
                }
            }
        }
    }

    //Calcular o rawFitness
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
    }

    unsigned max_makespan, min_makespan;
    double max_tec, min_tec;
    max_makespan = max_tec = 0;
    min_makespan = min_tec = UINT_MAX;

    for (auto it : p) {
        if(it->makeSpan < min_makespan)
            min_makespan = it->makeSpan;
        else if(it->makeSpan > max_makespan){
            max_makespan = it->makeSpan;
        }

        if(it->TEC < min_tec)
            min_tec = it->TEC;
        else if(it->TEC > max_tec){
            max_tec = it->TEC;
        }
    }

    //Calcular o fitness
    for (int i = 0; i < size; i++) {
        //Calcular o fitness
        p[i]->fitness = p[i]->raw_fitness
                + double((p[i]->makeSpan-min_makespan)/(max_makespan-min_makespan))*w.first
                + double((p[i]->TEC-min_tec)/(max_tec-min_tec))*w.second ;
    }

}
