#include "spea2.h"


void spea2(algorithm_data alg_data, vector<GASolution*> &p, Timer *t1)
{

    //Arquivo externo vazio
    vector<GASolution*> a, new_a, new_p;
    vector<GASolution*> parents;

    t1->stop();

    int i=0;

    while (t1->getElapsedTimeInMilliSec() < alg_data.time_limit) {


        //Atribuir valor de aptidão para o conjunto P e A
        FitnessAssignment(p, a, new_a);

        //Selecionar N indivíduos que formarão o novo conjunto externo
        EnvironmentalSelection(new_a, alg_data.param.u_population_size);

        //m <- new_a
        //copy(new_a.begin(), new_a.end(),back_inserter(m));
        BinaryTournamentSelection(new_a, parents, alg_data.param.u_population_size);
        //copy(p.begin(), p.end(),back_inserter(m));*/

        //Criar um população Q_{t+1} de tamanho N
        Crossover(parents, new_p, alg_data.param.u_population_size);
        /*m.clear();
        //copy(new_p.begin(), new_p.end(),back_inserter(m));*/
        Mutation(parents, new_p, alg_data.param.u_prob_mutation);

        //t = t+1
        //P = newP;

        //copy(new_p.begin(), new_p.end(),back_inserter(p));
        for (auto it : new_p) {
            p.push_back(it);
        }

        new_p.clear();

        //archive = newArchice;
        //copy(new_a.begin(), new_a.end(),back_inserter(a));
        for (auto it : new_a) {
            a.push_back(it);
        }

        new_a.clear();

        //Registar a contagem do tempo
        t1->stop();


        i++;

    }


    p.clear();

    for (auto it : a) {
        //if(it->fitness >= 1)
        //    break;
        //else
            p.push_back(it);
    }

}

void FitnessAssignment(vector<GASolution *> &p, vector<GASolution *> &a, vector<GASolution *> &new_a){

    int size, k;

    //for (auto &it_a: a) {
    for(auto &it_a : a){
        new_a.push_back(it_a);
    }

    vector<GASolution *> temp;

    for(auto &it_p : p){
        if(!PopulationAddIndividual(new_a, it_p)){
            temp.push_back(it_p);
        }
    }

    for (auto it_t : temp) {
        delete (GASolution *)it_t;
    }
    temp.clear();

    size = new_a.size();

    vector<double> distance(size);

    //Calcular o strengthValue
    for (int i = 0; i < size; i++) {
        new_a[i]->strength_value = 0;
        new_a[i]->set_solution_dominated.clear();
        for (int j = 0; j < size; j++) {
            if(i != j){
                //if(set[i].Dominate(set[j])){
                if(*new_a[i] < *new_a[j]){
                //if(*all[j] <= *all[i]){
                    new_a[i]->strength_value++;
                    new_a[i]->set_solution_dominated.push_back(new_a[j]);
                }
            }
        }
    }

    //Calcular o rawFitness
    for (int i = 0; i < size; i++) {
        new_a[i]->raw_fitness = 0;
        for (int j = 0; j < size; j++) {
            if(i != j){
                //if(set[j].Dominate(set[i])){
                if(*new_a[j] < *new_a[i]){
                //if(*all[i] < *all[j]){
                    new_a[i]->raw_fitness += new_a[j]->strength_value;
                }
            }
        }
    }

    k = sqrt(new_a.size());

    //Calcular o density
    for (int i = 0; i < size; i++) {
        distance[i] = 0;
        for (int j = 0; j < size; j++) {            
            if(i != j){
                distance[j] = new_a[i]->CalcEuclideanDistance(new_a[j]);
            }
            else{
                distance[j] = INT_MAX;
            }
        }
        sort(distance.begin(), distance.end());
        new_a[i]->density = double(1)/double(distance[k] + 2);
        //Calcular o fitness
        new_a[i]->fitness = new_a[i]->raw_fitness + new_a[i]->density;
    }

    p.clear();
    a.clear();
}

void EnvironmentalSelection(vector<GASolution *> &new_a, unsigned population_size){


    //Adequar o tamanho do novo arquivo
    unsigned size = new_a.size();

    //Se o novo arquivo (new_a) é maior que population_size
    //Então, reduzir o tamanho do novo arquivo (new_a)
    //Remover os indivíduos com menor aptidão
    if(size > population_size){
        SortByFitness(new_a);
        while (new_a.size() > population_size) {
            delete new_a.back();
            new_a.pop_back();
        }
    }

}

void BinaryTournamentSelection(vector<GASolution *> p, vector<GASolution *> &parents, int population_size){

    int x, y, size_a;

    size_a = p.size();

    /*for (auto it_parents : parents) {
        delete it_parents;
    }*/
    parents.clear();

    for (int i = 0; i < population_size; i++) {
        x = random()%size_a;
        y = random()%size_a;
        //if(*a_new[x] < *a_new[y]){
        if(p[x]->fitness < p[y]->fitness){
            //parents.push_back(new GASolution(*p[x]));
            parents.push_back(p[x]);
        }
        else{
            //parents.push_back(new GASolution(*p[y]));
            parents.push_back(p[y]);
        }
    }

}
