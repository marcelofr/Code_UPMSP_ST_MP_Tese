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
        FitnessAssignment(p, a);

        //Selecionar N indivíduos que formarão o novo conjunto externo
        EnvironmentalSelection(new_a, p, a, alg_data.param.u_population_size);

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

        //archive = newArchice;
        //copy(new_a.begin(), new_a.end(),back_inserter(a));
        for (auto it : new_a) {
            a.push_back(it);
        }

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

void FitnessAssignment(vector<GASolution *> &p, vector<GASolution *> &a){

    int size, k;
    vector<GASolution *> all;

    /*//Remover duplicidade
    for (auto &it_a: a) {
        for (auto it_p = p.begin(); it_p != p.end();) {
            if(it_a->makeSpan == (*it_p)->makeSpan
                    && abs(it_a->TEC - (*it_p)->TEC) < EPS){
                delete *it_p;
                it_p = p.erase(it_p);
            }
            else{
                ++it_p;
            }
        }
    }

    copy(p.begin(), p.end(), back_inserter(all));
    copy(a.begin(), a.end(), back_inserter(all));*/


    all.clear();

    for (auto &it_a: a) {
        PopulationAddIndividual(all, it_a);
    }

    //for (auto &it_a: a) {
    for(auto it_p=p.begin();it_p != p.end();){
        if(!PopulationAddIndividual(all, *it_p)){
            delete (GASolution *)*it_p;
            it_p = p.erase(it_p);
        }
        else{
            it_p++;
        }
    }

    size = all.size();

    vector<double> distance(size);

    //Calcular o strengthValue
    for (int i = 0; i < size; i++) {
        all[i]->strength_value = 0;
        all[i]->set_solution_dominated.clear();
        for (int j = 0; j < size; j++) {
            if(i != j){
                //if(set[i].Dominate(set[j])){
                if(*all[i] < *all[j]){
                //if(*all[j] <= *all[i]){
                    all[i]->strength_value++;
                    all[i]->set_solution_dominated.push_back(all[j]);
                }
            }
        }
    }

    //Calcular o rawFitness
    for (int i = 0; i < size; i++) {
        all[i]->raw_fitness = 0;
        for (int j = 0; j < size; j++) {
            if(i != j){
                //if(set[j].Dominate(set[i])){
                if(*all[j] < *all[i]){
                //if(*all[i] < *all[j]){
                    all[i]->raw_fitness += all[j]->strength_value;
                }
            }
        }
    }

    k = sqrt(all.size());

    //Calcular o density
    for (int i = 0; i < size; i++) {
        distance[i] = 0;
        for (int j = 0; j < size; j++) {            
            if(i != j){
                distance[j] = all[i]->CalcEuclideanDistance(all[j]);
            }
            else{
                distance[j] = INT_MAX;
            }
        }
        sort(distance.begin(), distance.end());
        all[i]->density = double(1)/double(distance[k] + 2);
        //Calcular o fitness
        all[i]->fitness = all[i]->raw_fitness + all[i]->density;
    }
}

void EnvironmentalSelection(vector<GASolution *> &new_a, vector<GASolution *> &p,
                            vector<GASolution *> &a, unsigned population_size){


    //Apagar todos os indivíduos do novo arquivo (new_a)
    new_a.clear();

    //Copiar as soluções não-dominadas do arquivo (a) no novo arquivo (new_a)
    //As soluções não-dominadas tem fitness menor que 1
    for (const auto &it : a) {
        if(new_a.size() >= population_size)
            break;
        if(it->fitness <= 1)
            new_a.push_back(it);
    }

    //Copiar as soluções não-dominadas da população (p) para o novo arquivo (new_a)
    //As soluções não-dominadas tem fitness menor que 1
    for (auto &it : p) {
        if(new_a.size() >= population_size)
            break;
        if(it->fitness <= 1)
            new_a.push_back(it);
    }


    //Adequar o tamanho do novo arquivo
    unsigned size = new_a.size();
    vector<GASolution *> temp;

    //Se o novo arquivo (new_a) é maior que population_size
    //Então, reduzir o tamanho do novo arquivo (new_a)
    //Remover os indivíduos com menor aptidão
    if(size > population_size){
        SortByFitness(new_a);
        while (new_a.size() > population_size) {
            //delete new_a.back();
            new_a.pop_back();
        }
    }
    else if (size < population_size){
        //Senão, aumentar o tamanho do novo arquivo (new_a)
        //Adicionar indivíduos dominados com maior aptidão
        for (auto it : p) {
            if(it->fitness > 1)
                temp.push_back(it);
        }
        for (auto it : a) {
            if(it->fitness > 1)
                temp.push_back(it);
        }
        //Indivíduos com menor fitness tem melhor aptidão
        SortByFitness(temp);
        unsigned i=0;
        while (new_a.size() < population_size) {
            new_a.push_back(temp[i]);
            i++;
        }
    }

    vector<GASolution *> temp2;

    for (auto &it_new_a : new_a) {
        temp2.push_back(it_new_a);
    }

    for (auto &it_temp : temp) {
        if(PopulationAddIndividual(temp2, it_temp))
            delete it_temp;
    }

    a.clear();
    p.clear();
    temp.clear();
    temp2.clear();

}

void BinaryTournamentSelection(vector<GASolution *> p, vector<GASolution *> &parents, int population_size){

    int x, y, size_a;

    size_a = p.size();

    for (auto it_parents : parents) {
        delete it_parents;
    }
    parents.clear();

    for (int i = 0; i < population_size; i++) {
        x = random()%size_a;
        y = random()%size_a;
        //if(*a_new[x] < *a_new[y]){
        if(p[x]->fitness < p[y]->fitness){
            parents.push_back(new GASolution(*p[x]));
        }
        else{
            parents.push_back(new GASolution(*p[y]));
        }
    }

}
