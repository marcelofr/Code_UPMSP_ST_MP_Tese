#include "nsgai.h"

void nsga_i(algorithm_data alg_data, vector<GASolution*> &p, Timer *t1){

    t1->stop();

    pair<double,double> w;
    vector<vector<GASolution*>> front;
    vector<GASolution*> parents, offspring;
    int random;
    unsigned i=0;

    while (t1->getElapsedTimeInMilliSec() < alg_data.time_limit) {


        random = rand()%100;
        w.first = double(random)/double(100);
        w.second = double(100-random)/double(100);

        NonDominatedSort(front, p);

        ComputeFitness(front, p.size());

        Selection(front, p, alg_data.param.u_population_size);

        BinaryTournamentSelection(p, parents, alg_data.param.u_population_size);

        Crossover(parents, offspring, alg_data.param.u_population_size);
        Mutation(parents, offspring, alg_data.param.u_prob_mutation);

        Union(p, offspring);

        //Registar a contagem do tempo
        t1->stop();

        i++;

    }

}

/*
 * Método para formar cada frente F[i] a partir da população R
 */
void NonDominatedSort(vector<vector<GASolution*> > &front, vector<GASolution*> &pop)
{

    //Conjunto auxiliar
    vector<GASolution*> pop_aux;
    pop_aux.clear();

    //Gerar cada frente F[i]
    for(auto p = pop.begin(); p != pop.end(); ++p){

        //Zerar o contador de soluções não-dominadas por p
        (*p)->counter_solution_non_dominated = 0;
        for(auto q = pop.begin(); q != pop.end(); ++q){

            //Se p e q são soluções diferentes
            if(!((**p) == (**q))){
                if ((**q) < (**p)){
                    //Incrementtar o contador de soluções não-dominadas por p
                    (*p)->counter_solution_non_dominated++;
                }
            }
        }
        //Se p não é dominado por nenhuma outra solução
        if((*p)->counter_solution_non_dominated == 0){
            (*p)->rank = 0;
            //Adicionar p na primeira frente
            pop_aux.push_back(*p);
            //F[0].push_back(*p);
        }
    }

    //Formar a primeira frente F[0]
    front.push_back(pop_aux);

    for(unsigned i=1;i<pop.size();i++){
        pop_aux.clear();
        for(auto p = pop.begin(); p != pop.end(); ++p){
            if((*p)->counter_solution_non_dominated == i){
                (*p)->rank = i;
                //Adicionar p na primeira frente
                pop_aux.push_back(*p);
                //F[0].push_back(*p);
            }
        }
        if(pop_aux.size()>0)
            front.push_back(pop_aux);
    }
}


void ComputeFitness(vector<vector<GASolution*>> &front, unsigned population_size){


    //pair<double,double> d1, d2;
    double m_max, t_max, m_min, t_min;
    m_max = m_min = front[0][0]->makeSpan;
    t_max = t_min = front[0][0]->TEC;
    for(unsigned i = 0; i<front[0].size(); i++){
        front[0][i]->fitness = population_size;
        if(front[0][i]->makeSpan < m_min)
            m_min = front[0][i]->makeSpan;
        if(front[0][i]->makeSpan > m_max)
            m_max = front[0][i]->makeSpan;
        if(front[0][i]->TEC < t_min)
            t_min = front[0][i]->TEC;
        if(front[0][i]->TEC > t_max)
            t_max = front[0][i]->TEC;
    }

    double distance;
    double sh;
    double a, b;


    for (auto &it_front : front) {

        for (unsigned i = 0; i < it_front.size(); i++) {
            //d1.first = it_front[i]->makeSpan;
            //d1.second = it_front[i]->TEC;
            sh=0;
            for(unsigned j = i+1; j<it_front.size(); j++){
                //d2.first = it_front[j]->makeSpan;
                //d2.second = it_front[j]->TEC;
                a = double(int(it_front[i]->makeSpan - it_front[j]->makeSpan))/double(m_max - m_min);
                b = double(it_front[i]->TEC-it_front[j]->TEC)/double(t_max - t_min);
                distance = double(sqrt(pow(a, 2) + pow(b, 2)));
                if(distance < SHARE){
                    sh += 1-pow(double(distance)/double(SHARE),2);
                }
            }
            it_front[i]->fitness = double(it_front[i]->fitness)/sh;
        }

    }

}

void Selection(vector<vector<GASolution*>> &front, vector<GASolution*> &pop, unsigned population_size){


    pop.clear();

    unsigned i,j;

    //for (auto &it_i : front) {
    for(i=0; i<front.size();i++){

        //for (auto &it_ij : it_i) {
        for(j=0; j<front[i].size();j++){
            if(pop.size() < population_size)
                pop.push_back(front[i][j]);
            else
                delete front[i][j];
        }
    }

    for(unsigned k = 0; k < front.size(); k++){
        front[k].clear();
    }
    front.clear();

}
