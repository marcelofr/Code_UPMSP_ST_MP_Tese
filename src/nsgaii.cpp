#include "nsgaii.h"

void nsgaii(algorithm_data alg_data, vector<GASolution*> &P, Timer *t1){

    vector<GASolution*> Q, R;
    vector<vector<GASolution*>> F;

    unsigned i, j;

    j = 0;


    t1->stop();
    while (t1->getElapsedTimeInMilliSec() < alg_data.time_limit) {

        //R_t <- P_t U Q_t
        UnionPopulation(R, P, Q);

        //Criar as frentes F[1], F[2], ...
        FastNonDominatedSort(F, R);

        i = 0;

        P.clear();
        while (P.size() + F[i].size() < alg_data.param.u_population_size && i < F.size()) {

            //Calcular crowding distance para a frente i
            ComputeCrowdingDistance(F[i]);

            //P_{t+1} <- P_{t+1} U F[i]
            move(F[i].begin(), F[i].end(), back_inserter(P));

            i++;
        }

        unsigned tam = 0;

        //Se F[i] não está vazia
        if(i < F.size()){
            //Calcular crowding distance para a frente i
            ComputeCrowdingDistance(F[i]);

            //Ordenar a frente i, de acordo com a crowding distance
            Sort(F[i]);

            //P_{t+1} <- P_{t+1} U F[j][1:(N - |P|)]
            //P.resize(POPULATION_SIZE);
            tam = alg_data.param.u_population_size-unsigned(P.size());
            move(F[i].begin(), F[i].begin()+tam, back_inserter(P));
        }

        //Apagar as soluções que não foram incluídas na próxima população
        for(unsigned k = tam; k < F[i].size() && i < F.size(); k++){
            delete (GASolution*)F[i][k];
        }

        //Apagar as soluções das outras frentes
        for(i++; i<F.size(); i++){
            while(!F[i].empty()) {
                delete (GASolution*)F[i].back();
                F[i].pop_back();
            }
            F[i].clear();
        }

        //Criar um população Q_{t+1} de tamanho N
        Crossover(P, Q, alg_data.param.u_population_size);
        Mutation(P, Q, alg_data.param.u_prob_mutation);

        j++;

        //Registar a contagem do tempo
        t1->stop();

    }

    //R_t <- P_t U Q_t
    UnionPopulation(R, P, Q);

    //Criar as frentes F[1], F[2], ...
    FastNonDominatedSort(F, R);

    P.clear();
    P.assign(F[0].begin(), F[0].end());

}

/*
 * Método para formar cada frente F[i] a partir da população R
 */
void FastNonDominatedSort(vector<vector<GASolution*> > &F, vector<GASolution*> &P)
{

    //Conjunto auxiliar
    vector<GASolution*> Q;
    Q.clear();

    for(unsigned k = 0; k < F.size(); k++){
        F[k].clear();
    }
    F.clear();

    //Gerar cada frente F[i]
    for(auto p = P.begin(); p != P.end(); ++p){

        //Apagar todo o conjunto de soluções dominadas por p
        (*p)->set_solution_dominated.clear();
        //Zerar o contador de soluções não-dominadas por p
        (*p)->counter_solution_non_dominated = 0;
        for(auto q = P.begin(); q != P.end(); ++q){

            //Se p e q são soluções diferentes
            if(!((**p) == (**q))){
                //Se p domina q
                if((**p) < (**q)){
                    //Adicionar q no conjunto de soluções dominadas por p
                    (*p)->set_solution_dominated.push_back(*q);
                }
                else if ((**q) < (**p)){
                    //Incrementtar o contador de soluções não-dominadas por p
                    (*p)->counter_solution_non_dominated++;
                }
            }
        }
        //Se p não é dominado por nenhuma outra solução
        if((*p)->counter_solution_non_dominated == 0){
            (*p)->rank = 0;
            //Adicionar p na primeira frente
            Q.push_back(*p);
            //F[0].push_back(*p);
        }
    }

    //Formar a primeira frente F[0]
    F.push_back(Q);

    unsigned i = 0;
    while (Q.size() > 0) {
        Q.clear();

        //Para cada solução p da frente i
        for(auto p = F[i].begin(); p != F[i].end(); ++p){

            //Para cada solução q dominada por p
            for(auto q = (*p)->set_solution_dominated.begin(); q != (*p)->set_solution_dominated.end(); ++q){

                //Decrementar o número de soluções não-dominadas de cada solução q
                (*q)->counter_solution_non_dominated--;
                //Se o número de soluções não-dominadas por q é igual a zero
                if((*q)->counter_solution_non_dominated == 0){
                    (*q)->rank = i+1;
                    //A solução q será adicionada na próxima frente
                    Q.push_back(*q);
                }
            }
        }
        i++;
        //Se alguma solução foi selecionada para fazer parte da próxima frente F[i]
        if(Q.size() > 0){
            //Formar a frente F[i]
            F.push_back(Q);
        }
    }

    for (unsigned i = 0; i < P.size();i++) {
        P[i]->set_solution_dominated.clear();
    }

    //P.clear();
    Q.clear();

}

/*
 * Calcular crowding distance das soluções de F[i]
 */
void ComputeCrowdingDistance(vector<GASolution*> &F_i)
{
    unsigned size, f_max, f_min;

    size = F_i.size();

    for(auto p = F_i.begin(); p != F_i.end(); ++p){
        (*p)->crowding_distance = 0;
    }

    //Ordenar makespan
    SortByMakespan(F_i);

    F_i[0]->crowding_distance = F_i[size-1]->crowding_distance =  INT_MAX;

    f_min = F_i[0]->makeSpan;
    f_max = F_i[size-1]->makeSpan;

    for(unsigned i = 1; i < size-1; i++){

        F_i[i]->crowding_distance += double(F_i[i+1]->makeSpan - F_i[i-1]->makeSpan)/(f_max-f_min);
    }

    //Ordenar TEC
    SortByTEC(F_i);

    F_i[0]->crowding_distance = F_i[size-1]->crowding_distance =  INT_MAX;

    f_min = F_i[0]->TEC;
    f_max = F_i[size-1]->TEC;

    for(unsigned i = 1; i < size-1; i++){

        F_i[i]->crowding_distance +=
                double(F_i[i+1]->TEC - F_i[i-1]->TEC)/(f_max-f_min);
    }

}

/*
 * Método para compara duas soluções l e r
 * se
 */
bool CompareCrowdingDistance(GASolution * l, GASolution * r) //(2)
{
    return (l->rank < r->rank) ||
            (l->rank == r->rank && l->crowding_distance > r->crowding_distance);
}

/*
 * Ordenar as soluções da frente F[i] de acordo com a crowding distance
 */
void Sort(vector<GASolution*> & F_i)
{
    try
    {
        sort(F_i.begin(), F_i.end(), CompareCrowdingDistance);
    }
    catch (bad_alloc & exception)
    {
        cerr << "bad_alloc detected: " << exception.what();
    }
}
