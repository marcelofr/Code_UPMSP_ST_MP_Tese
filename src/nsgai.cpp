#include "nsgai.h"

void nsga_i(algorithm_data alg_data, vector<GASolution*> &p, Timer *t1){

    t1->stop();

    pair<double,double> w;
    vector<GASolution*> new_p;
    int random;
    vector<vector<GASolution*>> F;

    while (t1->getElapsedTimeInMilliSec() < alg_data.time_limit) {


        ComputeFitness(p, F);
        /*ComputeFitness(new_p, w);

        Selection(p, new_p, alg_data.param.u_population_size);*/

        Crossover(p, new_p, alg_data.param.u_population_size);
        Mutation(p, new_p, alg_data.param.u_prob_mutation);

        random = rand()%100;
        w.first = double(random)/double(100);
        w.second = double(100-random)/double(100);




        //Registar a contagem do tempo
        t1->stop();

    }

}

void ComputeFitness(vector<GASolution *> &p, vector<vector<GASolution*>> &F){

    vector<GASolution*> temp;

    while (p.size() > 0) {

        //Calcular o strengthValue
        for (unsigned i = 0; i < p.size(); i++) {
            p[i]->strength_value = 0;
            //p[i]->set_solution_dominated.clear();
            for (unsigned j = 0; j < p.size(); j++) {
                if(i != j){
                    //if(set[i].Dominate(set[j])){
                    if(*p[j] < *p[i]){
                    //if(*all[j] <= *all[i]){
                        p[i]->strength_value++;
                        //p[i]->set_solution_dominated.push_back(p[j]);
                    }
                }
            }
        }

        temp.clear();
        //for (auto &it_p : p) {
        for (auto it_p=p.begin();it_p!=p.end() ; ) {
            if((*it_p)->strength_value == 0){
                temp.push_back(*it_p);
                it_p=p.erase(it_p);
            }
            else{
                it_p++;;
            }
        }
        F.push_back(temp);
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
    }*/

}
