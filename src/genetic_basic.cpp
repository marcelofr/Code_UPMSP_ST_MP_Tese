#include "genetic_basic.h"

using namespace std;

/*
 * Método para gerar a população inicial com tamanho POPULATION_SIZE
 */
void GenerateInitialPopulation(vector<GASolution*> &population, unsigned tam_population)
{

    /*Gerar uma população inicial*/
    /*Gerar uma solução gulosa considerando o objetivo do makespan*/
    population.push_back(new GASolution());
    population.back()->GenerateGreedySolutionMakespan();

    /*Gerar uma solução gulosa considerando o objetivo do TEC*/
    population.push_back(new GASolution());
    population.back()->GenerateGreedySolutionTEC3();

    /*Gerar o restante dos indivíduos aleatoriamente*/
    for (unsigned i = 0; i < tam_population-2; ++i) {

        population.push_back(new GASolution());
        population.back()->RandomInitialSolution();

    }
}

/*
 * Método para imprimir um vetor de soluções
 */
void PrintPopulation(vector<GASolution*> &population)
{
    for(auto it = population.begin(); it != population.end(); ++it){
        (*it)->Print();
    }
}

/*
 * Cruzamento para gerar TAM_CROSSOVER indivíduos
 * Uso de torneio binário para selecionar os indivíduos para o cruzamento
 */
void Crossover(vector<GASolution*> &population, vector<GASolution*> &new_population, unsigned population_size)
{
    GASolution *parent1, *parent2, *offspring1, *offspring2;
    unsigned ind1, ind2;
    unsigned size = population.size();

    new_population.clear();

    //Gerar novos indivíduos com o cruzamento
    for (unsigned i = 0; i < population_size/2; ++i) {

        //Seleção de indivíduos por torneio binário

        //Escolher dois indivíduos aleatoriamente
        ind1 = rand()%size;
        ind2 = rand()%size;
        //Escolher o melhor indivíduo para ser o pai 1
        if(population[ind1] < population[ind2]){
            parent1 = population[ind1];
        }
        else{
            parent1 = population[ind2];
        }

        //Escolher dois indivíduos aleatoriamente
        ind1 = rand()%size;
        ind2 = rand()%size;
        //Escolher o melhor indivíduo para ser o pai 2
        if(population[ind1] < population[ind2]){
            parent2 = population[ind1];
        }
        else{
            parent2 = population[ind2];
        }

        //Realizar o cruzamento
        offspring1 = new GASolution();
        offspring2 = new GASolution();
        GenerateOffspring3(parent1, parent2, offspring1, offspring2);

        //Adicionar os filhos gerados à nova população
        new_population.push_back(offspring1);
        new_population.push_back(offspring2);

    }

}

/*
 * Método para adicionar nova solução gerada a partir de uma mutação
 */
void Mutation(vector<GASolution*> &population, vector<GASolution*> &new_population, unsigned prob_mutation)
{
    GASolution *individual;
    size_t ind1;
    unsigned op;

    unsigned prob;

    individual = nullptr;


    for (unsigned i = 0; i < population.size(); ++i) {

        //Gerar um número entre zero e cem
        prob = rand()%100;

        //Se o número gerado é menor que a probabilidade definida, fazer a mutação
        if(prob < prob_mutation){

            individual = new GASolution;

            //Escolher o indivídua para fazer a mutação
            ind1 = rand()%population.size();

            //Fazer uma cópia do indivíduo em outro lugar
            (*individual) = *(population[ind1]);

            //Escolher um tipo de mutação
            op = rand()%1;

            switch (op) {
                case 0:
                    MutationSwapInside(*individual);
                    break;
                case 1:
                    MutationSwapOutside(*individual);
                    break;
                case 2:
                    MutationInsertInside(*individual);
                    break;
                case 3:
                    MutationInsertOutside(*individual);
                    break;
                case 4:
                    MutationChangeOpMode(*individual);
                    break;
                case 5:
                    MutationChangeH(*individual);
                    break;
                default:
                    break;
            }

            if(op < 4){
                individual->CalculateShorterTimeHorizon();
            }

            individual->CalculateObjective();

            new_population.push_back(individual);

        }
    }
}

/*
 * Gerar filhos usando o método do Ruiz
 */
void GenerateOffspring3(GASolution *parent1, GASolution *parent2,
                        GASolution *offspring1, GASolution *offspring2)
{

    //Vetores para os filhos
    vector<bool> o1(Instance::num_jobs+1, false), o2(Instance::num_jobs+1, false);

    unsigned size;

    unsigned op = rand()%2;

    //Para cada máquina, a primeira parte das tarefas do pai 1 é herdada pelo filho 1
    //A segunda parte é herdada pelo filho 2
    for (unsigned i = 1; i <= Instance::num_machine; ++i) {

        if(parent1->scheduling[i].size() > 0){
            //O filho 1 herda a primeira parte das tarefas do pai 1, na máquina i
            size = rand()%parent1->scheduling[i].size();
            for(auto it = parent1->scheduling[i].begin(); it != parent1->scheduling[i].begin()+size; ++it){
                offspring1->scheduling[i].push_back(*it);
                o1[*it] = true;

                //O filho 1 herda o modo de operação do pai 1
                offspring1->job_mode_op[*it] = parent1->job_mode_op[*it];
            }

            //O filho 2 herda a segunda parte das tarefas do pai 1, na máquina i
            size = parent1->scheduling[i].size() - size;
            for(auto it = parent1->scheduling[i].begin(); it != parent1->scheduling[i].begin()+size; ++it){
                offspring2->scheduling[i].push_back(*it);
                o2[*it] = true;

                //O filho 2 herda o modo de operação do pai 1
                offspring2->job_mode_op[*it] = parent1->job_mode_op[*it];
            }
        }

    }

    //As tarefas restantes dos filhos 1 e 2 são herdadas de acordo com característica do pai 2
    for (unsigned i = 1; i <= Instance::num_machine; ++i) {
        for(auto it = parent2->scheduling[i].begin(); it != parent2->scheduling[i].end(); ++it){

            //Se a tarefa it ainda não está no filho 1, então ela deve ser adicionada
            if(!o1[*it]){

                //Herdar o modo de operação do pai 2
                //offspring1.job_mode_op[*it] = parent2.job_mode_op[*it];

                //Inserir a nova tarefa, na melhor posição considerando um dos objetivos
                if(op == 0){
                //if(true){
                    //offspring1.scheduling[i].push_back(*it);
                    offspring1->AddJobGreedyMakespanMachine(i, *it, parent2->job_mode_op[*it]);
                }
                else{
                    //offspring1.AddJobGreedyTECMachine(i, *it, parent2.job_mode_op[*it]);
                    offspring1->AddJobGreedyTECMachine3(i, *it, parent2->job_mode_op[*it]);
                }

            }

            //Se a tarefa it não está no filho 2, então ela deve ser adicionada
            if(!o2[*it]){

                //Herdar o modo de operação do pai 2
                //offspring2.job_mode_op[*it] = parent2.job_mode_op[*it];

                //Inserir a nova tarefa, na melhor posição considerando um dos objetivos
                if(op == 0){
                //if(true){
                    //offspring2.scheduling[i].push_back(*it);
                    offspring2->AddJobGreedyMakespanMachine(i, *it, parent2->job_mode_op[*it]);
                }
                else{
                    //offspring2.AddJobGreedyTECMachine(i, *it, parent2.job_mode_op[*it]);
                    offspring2->AddJobGreedyTECMachine3(i, *it, parent2->job_mode_op[*it]);
                }

            }
        }
    }

    if(op == 0){
        //Definir o instance inicial de cada tarefa presente na sequência
        //considerando o menor valor para o tempo de término em cada máquina
        offspring1->CalculateShorterTimeHorizon();
        offspring2->CalculateShorterTimeHorizon();
    }
    else{
        //Definir o instance inicial de cada tarefa presente na sequência
        //considerando o menor valor para o custo de energia
        offspring1->CalculateHorizonAvoidingPeak();
        offspring2->CalculateHorizonAvoidingPeak();
    }


    //Calcular a função objetivo
    offspring1->CalculateObjective();
    offspring2->CalculateObjective();

    o1.clear();
    o2.clear();

}

void SolutionListToVector(GASolution s, vector<unsigned> &v_solution)
{

    //Criar a representação em forma de vetor
    unsigned index = 0;
    for (unsigned i = 1; i <= Instance::num_machine; ++i) {
        v_solution[index] = s.scheduling[i][0];
        for (unsigned j = 0; j < s.scheduling[i].size()-1; ++j) {
            index = v_solution[index];
            v_solution[index] = s.scheduling[i][j+1];
        }
        index = v_solution[index];
    }
}


void MutationSwapInside(GASolution &individual)
{
    unsigned machine, pos_job1, pos_job2;

    //Escolher aleatoriamente a máquina na qual a troca será realizada
    machine = 1 + rand()%Instance::num_machine;

    //Caso não tenha pelo menos duas tarefas, não deve realizar a mutação
    if(individual.scheduling[machine].size() < 2){
        return;
    }

    //Escolher a tarefa
    pos_job1 = rand()%individual.scheduling[machine].size();
    do{
        pos_job2 = rand()%individual.scheduling[machine].size();

    }while(pos_job1 != pos_job2);

    individual.SwapInside(machine, pos_job1, pos_job2);
}

/*
 * Método para realizar mutação em um indivíduo
 * A mutação consiste em trocar duas tarefas de posição em diferentes máquinas
 */
void MutationSwapOutside(GASolution &individual)
{
    unsigned machine1, machine2, pos_job1, pos_job2;

    //Caso não tenha pelo menos duas máquinas, não deve realizar a mutação
    if(Instance::num_machine < 2){
        return;
    }

    //Escolher aleatoriamente a máquina na qual a troca será realizada
    machine1 = 1 + rand()%Instance::num_machine;

    //Caso não tenha pelo menos uma tarefa, não deve realizar a mutação
    if(individual.scheduling[machine1].size() < 1){
        return;
    }

    //Escolher aleatoriamente a segunda máquina, desde que seja diferente da primeira
    do{
        machine2 = 1 + rand()%Instance::num_machine;
    }while(machine1 != machine2);

    //Caso não tenha pelo menos uma tarefa, não deve realizar a mutação
    if(individual.scheduling[machine2].size() < 1){
        return;
    }

    //Escolher a primeira tarefa
    pos_job1 = rand()%individual.scheduling[machine1].size();

    //Escolher a segunda tarefa
    pos_job2 = rand()%individual.scheduling[machine2].size();

    //Realizar a troca
    individual.SwapOutside(machine1, pos_job1, machine2, pos_job2);
}

/*
 * Método para realizar mutação em um indivíduo
 * A mutação consiste em tirar uma tarefa de posição e inserí-la
 * em outra posição da mesma máquina
 */
void MutationInsertInside(GASolution &individual)
{
    unsigned machine, pos1, pos2;

    //Escolher aleatoriamente a máquina na qual a troca será realizada
    machine = 1 + rand()%Instance::num_machine;

    //Caso não tenha pelo menos duas tarefas, não deve realizar a mutação
    if(individual.scheduling[machine].size() < 2){
        return;
    }

    //Escolher a primeira posição, deve haver alguma tarefa nessa posição
    pos1 = rand()%individual.scheduling[machine].size();
    do{
        //Escolher a segunda posição, que pode ser antes de qualquer tarefa ou após a última tarefa
        pos2 = rand()%(individual.scheduling[machine].size()+1);

    }while(pos1 != pos2);

    //Realizar a inserção
    individual.InsertInside(machine, pos1, pos2);
}

/*
 * Método para realizar mutação em um indivíduo
 * A mutação consiste em tirar uma tarefa de posição de uma maquina
 * e inserí-la em alguma posição de outra máquina
 */
void MutationInsertOutside(GASolution &individual)
{
    unsigned machine1, machine2, pos1, pos2;

    //Caso não tenha pelo menos duas máquinas, não deve realizar a mutação
    if(Instance::num_machine < 2){
        return;
    }

    //Escolher aleatoriamente a máquina na qual a troca será realizada
    machine1 = 1 + rand()%Instance::num_machine;

    //Caso não tenha pelo menos uma tarefa, não deve realizar a mutação
    if(individual.scheduling[machine1].size() < 1){
        return;
    }

    //Escolher aleatoriamente a segunda máquina, desde que seja diferente da primeira
    do{
        machine2 = 1 + rand()%Instance::num_machine;
    }while(machine1 != machine2);

    //Caso não tenha pelo menos uma tarefa, não deve realizar a mutação
    if(individual.scheduling[machine2].size() < 1){
        return;
    }

    //Escolher a primeira posição, deve haver alguma tarefa nessa posição
    pos1 = rand()%individual.scheduling[machine1].size();

    //Escolher a segunda posição, que pode ser antes de qualquer tarefa ou após a última tarefa
    pos2 = rand()%(individual.scheduling[machine2].size()+1);

    //Realizar a inserção
    individual.InsertOutside(machine1, pos1, machine2, pos2);
}

/*
 * Método para realizar mutação em um indivíduo
 * A mutação consiste mudar o modo de operação de uma tarefa
 */
void MutationChangeOpMode(GASolution &individual)
{
    unsigned machine, pos_job, job, op_mode;

    //Escolher aleatoriamente a máquina na qual a troca será realizada
    machine = 1 + rand()%Instance::num_machine;

    //Caso não tenha pelo menos duas tarefas, não deve realizar a mutação
    if(individual.scheduling[machine].size() < 1){
        return;
    }

    //Escolher a tarefa
    pos_job = rand()%individual.scheduling[machine].size();

    job = individual.scheduling[machine][pos_job];

    do{
        //Selecionar um novo modo de operação
        op_mode = 1+rand()%Instance::num_mode_op;

    }while(op_mode != individual.job_mode_op[job]);

    individual.ChangeModeOpJob(machine, pos_job, op_mode);
}

/*
 * Método para realizar mutação em um indivíduo
 * A mutação consiste mudar o instante de início de uma tarefa de uma tarefa
 */
void MutationChangeH(GASolution &individual)
{
    unsigned machine, pos_job, job, add_h;

    //Escolher aleatoriamente a máquina na qual a troca será realizada
    machine = 1 + rand()%Instance::num_machine;

    //Caso não tenha pelo menos duas tarefas, não deve realizar a mutação
    if(individual.scheduling[machine].size() < 1){
        return;
    }

    //Escolher a tarefa
    pos_job = rand()%individual.scheduling[machine].size();
    job = individual.scheduling[machine][pos_job];

    unsigned h_day = floor(individual.H1[job]/(double)Instance::num_planning_horizon);

    //O tempo adicional será no máximo o tamanho do horário de pico
    add_h = rand()%(Instance::v_peak_end[h_day] - Instance::v_peak_start[h_day]);

    //Realizar a mudança de h
    individual.ChangeHJob(machine, pos_job, add_h);
}

void PopulationAddIndividual(vector<GASolution*> &population, GASolution *individual)
{
    bool add = true;


    for(auto it = population.begin(); it != population.end(); ++it){
        //Se já tem um indivíduo igual na população
        if(individual->makeSpan == (*it)->makeSpan && abs(individual->TEC - (*it)->TEC) < EPS){
            //Não adicionar
            add = false;
            break;
        }
    }

    if(add){
        population.push_back(individual);
    }
}

/*
 * População @R recebe a união entre @P e @Q
 */
void UnionPopulation(vector<GASolution*> &R, vector<GASolution*> &P, vector<GASolution*> &Q)
{

    R.clear();

    for(auto it = P.begin(); it != P.end(); ++it){
        PopulationAddIndividual(R, *it);
    }

    P.clear();

    for(auto it = Q.begin(); it != Q.end(); ++it){
        PopulationAddIndividual(R, *it);
    }

    Q.clear();
}
void SortByFitness(vector<GASolution *> &set_solution)
{
    sort(set_solution.begin(), set_solution.end(), CompareFitness);
}

bool CompareFitness(GASolution * l, GASolution * r) //(2)
{
    if(l->fitness < r->fitness){
        return true;
    }
    else{
        return false;
    }
}
