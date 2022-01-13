#include "localsearch_basic.h"


/*
 * Método para adicionar uma nova solução ao conjunto não-dominado, caso possíveis:
 * 1 - A nova solução é dominada então ela não será adicionada
 * 2 - A nova solução é não-dominada então ela será adicionada
 * 3 - A solução domina então ela será adicionada e as outras serão removidas
 * Se conseguiu adicionar my_solution, então retorna verdadeiro
 */
/*bool AddSolution(LSSolution *my_solution, vector<LSSolution*> &non_dominated_set)
{

    //caso 1
    //bool is_dominated = false;
    for(auto it_sol = non_dominated_set.begin(); it_sol != non_dominated_set.end(); ++it_sol){
        //Se my_solution é dominada por alguma solução do conjunto
        if(*it_sol < my_solution || *it_sol == my_solution){
            return false;
        }
    }

    //Caso 3
    for(auto it_sol = non_dominated_set.begin(); it_sol != non_dominated_set.end();){
        //Se my_solution domina alguma solução do conjunto
        if(my_solution < *it_sol){
            //Remover essa solução do conjunto
            it_sol = non_dominated_set.erase(it_sol);
        }
        else{
            ++it_sol;
        }
    }
    //Caso 2 e 3
    non_dominated_set.push_back(my_solution);
    return true;
}*/

#include<numeric>

LSSolution *SelectNonDomintatedSolution(NDSetSolution<LSSolution *> &non_dominated_set)
{

    unsigned qtd_nao_visitada = 0;

    LSSolution *cur_solution = NULL;

    vector<int> v(non_dominated_set.set_solution.size());
    iota (std::begin(v), std::end(v), 0);
    random_shuffle( v.begin(), v.end() );

    //for(auto it_sol = non_dominated_set->set_solution.begin(); it_sol != non_dominated_set->set_solution.end();it_sol++){
    //for(auto it_sol : non_dominated_set.set_solution){
    for(auto it: v){
        //if(!((it_sol)->was_visited)){
        if(!(non_dominated_set.set_solution[it]->was_visited)){
            //cout << "Solução não visitada" << endl;

            //Selecionar solução não-visitada
            //cur_solution = (LSSolution*)(*it_sol);
            //cur_solution = it_sol;
            cur_solution = non_dominated_set.set_solution[it];
            break;

        }
        else{
            //cout << "Solução visitada" << endl;
            qtd_nao_visitada++;
        }
    }

    //Se todas as solução já foram visitadas
    if(qtd_nao_visitada == non_dominated_set.set_solution.size()){
        //Marcar todas como não-visitadas
        for(auto it_sol = non_dominated_set.set_solution.begin(); it_sol != non_dominated_set.set_solution.end();it_sol++){
            (*it_sol)->was_visited = false;
        }
        //Selecionar a primeira para continuar a busca
        //cur_solution = *(non_dominated_set.set_solution.begin());
        cur_solution = non_dominated_set.set_solution[v[0]];
    }

    return cur_solution;
}



unsigned SelectNonVisitedSolution(NDSetSolution<LSSolution *> non_dominated_set)
{

    vector<int> v(non_dominated_set.set_solution.size());
    iota (std::begin(v), std::end(v), 0);
    srand(Instance::seed);
    random_shuffle( v.begin(), v.end() );
    for(auto it: v){
        if(!(non_dominated_set.set_solution[it]->was_visited)){
            return it;
        }
    }

    return non_dominated_set.set_solution.size()+1;
}


unsigned ExistNonDomintatedSolution(NDSetSolution<LSSolution *> non_dominated_set)
{

    unsigned qtd_nao_visitada = 0;

    for(auto it_sol : non_dominated_set.set_solution){
        if(!it_sol->was_visited){

            return ++qtd_nao_visitada;

        }
    }

    return qtd_nao_visitada;
}

LSSolution *GenRandomNeighborSol(LSSolution *my_solution)
{

    LSSolution *neighbor_sol = new LSSolution();

    *neighbor_sol = *my_solution;

    unsigned op_mov;
    unsigned i1, i2, j1, j2, mode_op;

    op_mov = rand()%5;

    switch (op_mov) {
        //Troca na mesma máquina
        case 0:

            //Selecionar a máquina i1 com pelo menos 2 tarefas
            do {
                i1 = 1+rand()%Instance::num_machine;
            } while (neighbor_sol->scheduling[i1].size() < 2);

            //Selecionar a primeira tarefa
            j1 = rand()%neighbor_sol->scheduling[i1].size();

            //Selecionar a segunda tarefa, diferente da primeira
            do{
                j2 = rand()%neighbor_sol->scheduling[i1].size();
            }while(j1==j2);

            //Realizar troca
            neighbor_sol->SwapInside(i1, j1, j2);

            neighbor_sol->CalculateShorterTimeHorizon();
            neighbor_sol->CalculateObjective();

            break;

        //Troca em máquinas diferentes
        case 1:

            //Selecionar máquina i1 com pelo menos 1 tarefa
            do {
                i1 = 1+rand()%Instance::num_machine;
            } while (neighbor_sol->scheduling[i1].size() < 1);

            //Selecionar máquina i2 com pelo menos 1 tarefa
            do{
                i2 = 1+rand()%Instance::num_machine;
            }while(i1==i2 || neighbor_sol->scheduling[i2].size() < 1);

            //Selecionar a primeira tarefa
            j1 = rand()%neighbor_sol->scheduling[i1].size();

            //Selecionar a segunda tarefa
            j2 = rand()%neighbor_sol->scheduling[i2].size();

            //Realizar troca
            neighbor_sol->SwapOutside(i1, j1, i2, j2);

            neighbor_sol->CalculateShorterTimeHorizon();
            neighbor_sol->CalculateObjective();

            break;

        //Inserção na mesma máquina
        case 2:

            //Selecionar a máquina i1 com pelo menos 2 tarefas
            do {
                i1 = 1+rand()%Instance::num_machine;
            } while (neighbor_sol->scheduling[i1].size() < 2);

            //Selecionar a primeira tarefa
            j1 = rand()%neighbor_sol->scheduling[i1].size();

            //Selecionar a segunda tarefa, diferente da primeira
            do{
                j2 = rand()%neighbor_sol->scheduling[i1].size();
            }while(j1==j2);

            //Realizar inserção
            neighbor_sol->InsertInside(i1, j1, j2);

            neighbor_sol->CalculateShorterTimeHorizon();
            neighbor_sol->CalculateObjective();

            break;

        //Inserção em máquinas diferentes
        case 3:

            //Selecionar máquina i1 com pelo menos 1 tarefa
            do {
                i1 = 1+rand()%Instance::num_machine;
            } while (neighbor_sol->scheduling[i1].size() < 1);

            //Selecionar máquina i2 com pelo menos 1 tarefa
            do{
                i2 = 1+rand()%Instance::num_machine;
            }while(i1==i2 || neighbor_sol->scheduling[i2].size() < 1);


            //Selecionar a primeira tarefa
            j1 = rand()%neighbor_sol->scheduling[i1].size();

            //Selecionar a segunda tarefa
            j2 = rand()%neighbor_sol->scheduling[i2].size();

            //Realizar inserção
            neighbor_sol->InsertOutside(i1, j1, i2, j2);

            neighbor_sol->CalculateShorterTimeHorizon();
            neighbor_sol->CalculateObjective();

            break;

        //Troca de modo de operação
        case 4:

            //Selecionar máquina i1 com pelo menos 1 tarefa
            do {
                i1 = 1+rand()%Instance::num_machine;
            } while (neighbor_sol->scheduling[i1].size() < 1);

            //Selecionar uma tarefa
            j1 = rand()%neighbor_sol->scheduling[i1].size();

            //Selecionar modo de operação
            mode_op = 1+rand()%Instance::num_mode_op;

            //Realizar troca do modo de operação
            neighbor_sol->ChangeModeOpJob(i1, j1, mode_op);

            neighbor_sol->CalculateShorterTimeHorizon();
            neighbor_sol->CalculateObjective();

            break;
    }

    return neighbor_sol;
}

LSSolution *GenNeighborSol(LSSolution *my_solution, unsigned op_neighbor)
{

    unsigned i1, i2, j1, j2, mode_op;

    //op_mov
    switch (op_neighbor) {
        //Troca na mesma máquina
        case 0:

            //Selecionar a máquina i1 com pelo menos 2 tarefas
            do {
                i1 = 1+rand()%Instance::num_machine;
            } while (my_solution->scheduling[i1].size() < 2);

            //Selecionar a primeira tarefa
            j1 = rand()%my_solution->scheduling[i1].size();

            //Selecionar a segunda tarefa, diferente da primeira
            //do{
                j2 = rand()%my_solution->scheduling[i1].size();
            //}while(j1==j2);

            //Realizar troca
            my_solution->SwapInside(i1, j1, j2);

            break;

        //Troca em máquinas diferentes
        case 1:

            //Selecionar máquina i1 com pelo menos 1 tarefa
            do {
                i1 = 1+rand()%Instance::num_machine;
            } while (my_solution->scheduling[i1].size() < 1);

            //Selecionar máquina i2 com pelo menos 1 tarefa
            do{
                i2 = 1+rand()%Instance::num_machine;
            }while(my_solution->scheduling[i2].size() < 1);

            //Selecionar a primeira tarefa
            j1 = rand()%my_solution->scheduling[i1].size();

            //Selecionar a segunda tarefa
            j2 = rand()%my_solution->scheduling[i2].size();

            //Realizar troca
            my_solution->SwapOutside(i1, j1, i2, j2);

            break;

        //Inserção na mesma máquina
        case 2:

            //Selecionar a máquina i1 com pelo menos 2 tarefas
            do {
                i1 = 1+rand()%Instance::num_machine;
            } while (my_solution->scheduling[i1].size() < 2);

            //Selecionar a primeira tarefa
            j1 = rand()%my_solution->scheduling[i1].size();

            //Selecionar a segunda tarefa, diferente da primeira
            //do{
                j2 = rand()%my_solution->scheduling[i1].size();
            //}while(j1==j2);

            //Realizar inserção
            my_solution->InsertInside(i1, j1, j2);

            break;

        //Inserção em máquinas diferentes
        case 3:

            //Selecionar máquina i1 com pelo menos 1 tarefa
            do {
                i1 = 1+rand()%Instance::num_machine;
            } while (my_solution->scheduling[i1].size() < 1);

            //Selecionar máquina i2 com pelo menos 1 tarefa
            do{
                i2 = 1+rand()%Instance::num_machine;
            }while(my_solution->scheduling[i2].size() < 1);

            //Selecionar a primeira tarefa
            j1 = rand()%my_solution->scheduling[i1].size();

            //Selecionar a segunda tarefa
            j2 = rand()%my_solution->scheduling[i2].size();

            //Realizar inserção
            if(i1 != i2)
                my_solution->InsertOutside(i1, j1, i2, j2);
            else
                my_solution->InsertInside(i1, j1, j2);

            break;

        //Troca de modo de operação
        case 4:

            //Selecionar máquina i1 com pelo menos 1 tarefa
            do {
                i1 = 1+rand()%Instance::num_machine;
            } while (my_solution->scheduling[i1].size() < 1);

            //Selecionar uma tarefa
            j1 = rand()%my_solution->scheduling[i1].size();

            //Selecionar modo de operação
            mode_op = 1+rand()%Instance::num_mode_op;

            //Realizar troca do modo de operação
            my_solution->ChangeModeOpJob(i1, j1, mode_op);

            break;
    }

    my_solution->CalculateShorterTimeHorizon();
    my_solution->CalculateObjective();

    return my_solution;
}


void Shaking(LSSolution *cur_solution, unsigned op_neighbor, unsigned level)
{

    for(unsigned i=0; i<level; i++){
        //Gerar uma solução vizinha aleatoriamente
        cur_solution = GenNeighborSol(cur_solution, op_neighbor);
    }

    cur_solution->CalculateShorterTimeHorizon();
    cur_solution->CalculateObjective();

}

void Shaking(LSSolution *cur_solution, unsigned level)
{

    unsigned op_neighbor;

    LSSolution *new_solution = new LSSolution(*cur_solution);

    for(unsigned i=0; i<level; i++){
        op_neighbor = rand()%QTD_NEIGHBOR;
        //Gerar uma solução vizinha aleatoriamente
        GenNeighborSol(new_solution, op_neighbor);
    }

    *cur_solution = *new_solution;

    cur_solution->CalculateShorterTimeHorizon();
    cur_solution->CalculateObjective();

#ifdef DEBUG
    cur_solution->Check();
#endif

    delete new_solution;

}

LSSolution * Destruction(LSSolution * solution, unsigned level){

    //unsigned job_position, max_ct, machine_max_ct, min_ct, machine_min_ct;
    unsigned machine_makespan;
    unsigned machine, job_position;
    vector<unsigned> removed_jobs;


    //Remover as tarefas da máquina com o makespan
    unsigned i;
    i=0;
    machine_makespan = solution->GetMakespanMachine();
    for (;i < level && solution->scheduling[machine_makespan].size()>0 ;i++ ) {

        //job_position = rand()%solution->scheduling[machine_makespan].size();
        //solution->RemovingJob(machine_makespan, job_position);

        solution->RemovingJob(machine_makespan, solution->scheduling[machine_makespan].size()-1);

    }

    //solution->CalculateShorterTimeHorizon();
    //solution->CalculateObjective();


    for (;i < level;i++ ) {

        do{
            machine = 1 + rand()%Instance::num_machine;
        }while(solution->scheduling[machine].size() == 0);

        job_position = rand()%solution->scheduling[machine].size();

        solution->RemovingJob(machine, job_position);

    }

    solution->CalculateShorterTimeHorizon();
    solution->CalculateObjective();

    return solution;
}

bool Construction(LSSolution * partial_solution, NDSetSolution<LSSolution *> &non_dominated_set,
                  vector<unsigned> removed_jobs){

    LSSolution *reconstructed_solution;
    unsigned size;
    bool improve;
    NDSetSolution<LSSolution *> non_dominated_set_partial;
    NDSetSolution<LSSolution *> non_dominated_set_partial_next;
    reconstructed_solution = new LSSolution();

    non_dominated_set_partial.AddSolution(partial_solution);

    //Para cada tarefa removida
    for(auto it1 : removed_jobs){
        //Para cada solução parcial
        for (auto it : non_dominated_set_partial_next.set_solution) {
            delete it;
        }
        non_dominated_set_partial_next.set_solution.clear();
        for(auto it2 : non_dominated_set_partial.set_solution){
            //Para cada máquina
            for(unsigned i=1; i<=Instance::num_machine; i++){
                size = it2->scheduling[i].size();
                //Para cada posição
                for(unsigned p=0; p<=size; p++ ){

                    *reconstructed_solution = *it2;
                    reconstructed_solution->AddJob(it1, i, p, 0);

                    //Para cada modo de operação
                    for(unsigned o = 1; o <= Instance::num_mode_op; o++ ){

                        reconstructed_solution->job_mode_op[it1] = o;
                        //reconstructed_solution->CalculateShorterTimeHorizon();
                        reconstructed_solution->CalculateShorterTimeHorizonMachine(i);
                        //reconstructed_solution->CalculateObjective();
                        reconstructed_solution->CalculateObjectiveMachine(i);
                        non_dominated_set_partial_next.AddSolution(reconstructed_solution);

                    }
                }
            }
        }

        for (auto it : non_dominated_set_partial.set_solution) {
            delete it;
        }
        non_dominated_set_partial.set_solution.clear();

        for(auto it: non_dominated_set_partial_next.set_solution){
            non_dominated_set_partial.AddSolution(it);
        }
    }

    improve=false;
    for(auto it: non_dominated_set_partial.set_solution){
        if(non_dominated_set.AddSolution(it))
            improve=true;
    }

    for (auto it : non_dominated_set_partial.set_solution) {
        delete it;
    }
    non_dominated_set_partial.set_solution.clear();

    delete reconstructed_solution;

    return improve;

}

bool IntesificationArroyo(LSSolution *current_solution, NDSetSolution<LSSolution *> &non_dominated_set, unsigned level){

    bool improve = false;
    LSSolution *partial_solution;

    partial_solution = new LSSolution();

    *partial_solution = *current_solution;

    partial_solution = Destruction(partial_solution, level);

    vector<unsigned> removed_jobs(partial_solution->removed_jobs);

    partial_solution->removed_jobs.clear();

    improve=Construction(partial_solution, non_dominated_set, removed_jobs);

    delete partial_solution;

    return improve;

}

void SortByMakespanLSSolution(vector<LSSolution*> &set_solution)
{
    //sort(set_solution.begin(), set_solution.end(), CompareMakespanMonoSolution);
    sort(set_solution.begin(), set_solution.end(), CompareMakespan);
    //sort(set_solution.begin(), set_solution.end());
}


/*
 * Método que gerar vizinhos através da realização de  trocas de tarefas em uma máquina.
 * Ele tenta gerar novos vizinhos até encontrar um que possa ser incluído
 * no conjunto não-dominado (first improvement), ou até percorrer toda a vizinhança
 * Ele retorna verdadeiro, caso consiga adicionar um vizinho ao conjunto não-dominado
 */
bool SwapInsideLS_FI(LSSolution *ini_solution, NDSetSolution<LSSolution*> &non_dominated_set)
{

    LSSolution *neighbor_solution = new LSSolution(*ini_solution);

    unsigned long num_job_maq;
    //double best_objective_funtion;

    //Para cada máquina i de 1 à n
    //O TEC não muda em troca dentro da mesma máquina
    //O makespan só muda no máquina onde está o makespan
    unsigned i = ini_solution->GetMakespanMachine();
    //for (unsigned i = 1; i <= Instance::num_machine; i++) {

        num_job_maq = ini_solution->scheduling[i].size();

        if(num_job_maq < 2){

            delete neighbor_solution;

            //continue;
            return false;
        }

        //Para cada tarefa j da máquina i
        for (unsigned j = 0; j < num_job_maq-1 ; j++) {

            //Para cada tarefa k da máquina i
            for (unsigned k = j+1; k < num_job_maq ; ++k) {

                //Gerar vizinho com a troca de tarefas em uma máquina
                neighbor_solution->SwapInside(i, j, k);
                neighbor_solution->CalculateShorterTimeHorizonMachine(i);
                neighbor_solution->CalculateObjectiveMachine(i);

                //Verificar se houve melhora global
                if(non_dominated_set.AddSolution(neighbor_solution)){

                    delete neighbor_solution;

                    return true;
                }
                else if(neighbor_solution->makeSpan > Instance::v_peak_start[0]){
                    neighbor_solution->CalculateHorizonAvoidingPeak();
                    neighbor_solution->CalculateObjective();

                    if(non_dominated_set.AddSolution(neighbor_solution)){

                        delete neighbor_solution;

                        return true;

                    }
                }

                //*ini_solution = *neighbor_sol_ref;
            }
        }
    //}
    delete neighbor_solution;

    //Retorna falso, caso não consiga encontrar um vizinho melhor
    return false;
}

/*
 * Método que gera vizinhos através da realização de  trocas de tarefas em uma máquina.
 * Ele gera todos os vizinhos e tentar o melhor no conjunto não-dominado
 * (best improvement)
 */
bool SwapInsideLS_BI(LSSolution *my_solution, NDSetSolution<LSSolution*> &non_dominated_set_out)
{
    unsigned long num_job_maq;

    LSSolution *neighbor_sol = new LSSolution () ;

    bool improve = false;

    //Criar uma cópia da solução
    *neighbor_sol = *my_solution;

    //Para cada máquina i de 1 à n
    //for (unsigned i = 1; i <= Instance::num_machine; i++) {
    unsigned i = neighbor_sol->GetMakespanMachine();
        num_job_maq = neighbor_sol->scheduling[i].size();

        //Para cada tarefa j da máquina i
        for (unsigned j = 0; j < num_job_maq-1; j++) {

            //Para cada tarefa k da máquina i
            for (unsigned k = j+1; k < num_job_maq; ++k) {

                //Criar uma cópia da solução
                //*neighbor_sol = *my_solution;

                //Gerar vizinho com a troca de tarefas em uma máquina
                //neighbor_sol->SwapInsideDelta(i, j, k);
                neighbor_sol->SwapInside(i, j, k);

                neighbor_sol->CalculateShorterTimeHorizonMachine(i);
                neighbor_sol->CalculateObjectiveMachine(i);

                if(non_dominated_set_out.AddSolution(neighbor_sol)){
                //if(neighbor_sol < best_solution){
                    //*best_solution = *neighbor_sol;
                    improve=true;
                }

                neighbor_sol->SwapInsideDelta(i, k, j);
            }

            neighbor_sol->CalculateShorterTimeHorizonMachine(i);
            neighbor_sol->CalculateObjectiveMachine(i);
        }
    //}

    /*if(non_dominated_set_out.AddSolution(best_solution)){
        improve=true;
    }*/

    delete neighbor_sol;

    return improve;
}

/*
 * Método que gerar vizinhos através da realização de  trocas de tarefas entre máquina.
 * Ele tenta gerar novos vizinhos até encontrar um que possa ser incluído
 * no conjunto não-dominado (first improvement), ou até percorrer toda a vizinhança
 * Ele retorna verdadeiro, caso consiga adicionar um vizinho ao conjunto não-dominado
 */
bool SwapOutsideLS_FI(LSSolution* ini_solution, NDSetSolution<LSSolution*> &non_dominated_set)
{
    unsigned long num_job_maq1, num_job_maq2;

    LSSolution *neighbor_solution = new LSSolution(*ini_solution);

    //Para cada máquina i1 de 1 à n
    for (unsigned i1 = 1; i1 <= Instance::num_machine; i1++) {

        num_job_maq1 = ini_solution->scheduling[i1].size();

        if(num_job_maq1<1){
            continue;
        }

        //Para cada máquina i2 de i1+1 à n
        for (unsigned i2 = i1+1; i2 <= Instance::num_machine; i2++) {

            num_job_maq2 = ini_solution->scheduling[i2].size();

            if(num_job_maq2<1 || i1 == i2){
                continue;
            }

            //Para cada tarefa j da máquina i1
            for (unsigned j = 0; j < num_job_maq1; j++) {

                //Para cada tarefa k da máquina i2
                for (unsigned k = 0; k < num_job_maq2; ++k) {

                    //Gerar vizinho com a troca de tarefas entre máquinas
                    neighbor_solution->SwapOutside(i1, j, i2, k);
                    neighbor_solution->CalculateShorterTimeHorizonMachine(i1);
                    neighbor_solution->CalculateShorterTimeHorizonMachine(i2);
                    neighbor_solution->CalculateObjectiveMachine(i1);
                    neighbor_solution->CalculateObjectiveMachine(i2);

                    //Verificar se houve melhora global
                    if(non_dominated_set.AddSolution(neighbor_solution)){

                        delete neighbor_solution;

                        return true;
                    }
                    else if(neighbor_solution->makeSpan > Instance::v_peak_start[0]){
                        neighbor_solution->CalculateHorizonAvoidingPeak();
                        neighbor_solution->CalculateObjective();

                        if(non_dominated_set.AddSolution(neighbor_solution)){

                            delete neighbor_solution;

                            return true;

                        }
                    }

                    *neighbor_solution = *ini_solution;
                }

            }

        }
    }

    delete neighbor_solution;

    //Retorna falso, caso não consiga encontrar um vizinho melhor
    return false;
}

/*
 * Método que gerar vizinhos através da realização de  trocas de tarefas entre máquina.
 * Ele gera todos os vizinhos e tenta incluir o melhor no conjunto não-dominado
 * (best improvement)
 */
bool SwapOutsideLS_BI(LSSolution* my_solution, NDSetSolution<LSSolution*> &non_dominated_set_out)
{
    unsigned long num_job_maq1, num_job_maq2;
    LSSolution *neighbor_sol = new LSSolution();
    bool improve = false;

    *neighbor_sol = *my_solution;

    //Para cada máquina i1 de 1 à n
    //for (unsigned i1 = 1; i1 <= Instance::num_machine-1; i1++) {
    unsigned i1 = neighbor_sol->GetMakespanMachine();
        num_job_maq1 = neighbor_sol->scheduling[i1].size();

        //Para cada máquina i2 de i1+1 à n
        for (unsigned i2 = 1; i2 <= Instance::num_machine; i2++) {

            if(i1 == i2){
                continue;
            }

            num_job_maq2 = neighbor_sol->scheduling[i2].size();

            //Para cada tarefa j da máquina i1
            for (unsigned j = 0; j < num_job_maq1; j++) {

                //Para cada tarefa k da máquina i2
                for (unsigned k = 0; k < num_job_maq2; ++k) {

                    //*neighbor_sol = *my_solution;

                    //Gerar vizinho com a troca de tarefas entre máquinas
                    neighbor_sol->SwapOutsideDelta(i1, j, i2, k);
                    /*neighbor_sol->SwapOutside(i1, j, i2, k);

                    neighbor_sol->CalculateShorterTimeHorizon();
                    neighbor_sol->CalculateObjective();*/

                    if(non_dominated_set_out.AddSolution(neighbor_sol)){
                    //if(neighbor_sol < best_solution){
                        //*best_solution = *neighbor_sol;
                        improve = true;
                    }

                    neighbor_sol->SwapOutsideDelta(i2, k, i1, j);

                }

                neighbor_sol->CalculateShorterTimeHorizonMachine(i1);
                neighbor_sol->CalculateObjectiveMachine(i1);

                neighbor_sol->CalculateShorterTimeHorizonMachine(i2);
                neighbor_sol->CalculateObjectiveMachine(i2);
            }
        }
    //}

    /*if(non_dominated_set_out.AddSolution(best_solution)){
        improve=true;
    }*/

    delete neighbor_sol;
    //delete best_solution;

    return improve;
}

/*
 * Método que gerar vizinhos através da realização de inserções de tarefas em uma máquina.
 * Ele tenta gerar novos vizinhos até encontrar um que possa ser incluído
 * no conjunto não-dominado (first improvement), ou até percorrer toda a vizinhança
 * Ele retorna verdadeiro, caso consiga adicionar um vizinho ao conjunto não-dominado
 */
bool InsertInsideLS_FI(LSSolution* ini_solution, NDSetSolution<LSSolution *> &non_dominated_set)
{

    LSSolution *neighbor_solution = new LSSolution(*ini_solution);

    unsigned long num_job_maq;

    //Para cada máquina i de 1 à n
    //O TEC não muda em troca dentro da mesma máquina
    //O makespan só muda no máquina onde está o makespan
    unsigned i = ini_solution->GetMakespanMachine();
    //for (unsigned i = 1; i <= Instance::num_machine; i++) {

        num_job_maq = ini_solution->scheduling[i].size();

        if(num_job_maq < 2){

            delete neighbor_solution;

            //continue;
            return false;
        }

        //Para cada tarefa j da máquina i
        for (unsigned j = 0; j < num_job_maq-1; j++) {

            //Para cada tarefa k da máquina i
            for (unsigned k = j+1; k < num_job_maq; ++k) {

                //Gerar vizinho com a troca de tarefas em uma máquina
                neighbor_solution->InsertInside(i, j, k);
                neighbor_solution->CalculateShorterTimeHorizonMachine(i);
                neighbor_solution->CalculateObjectiveMachine(i);

                //Verificar se houve melhora global
                if(non_dominated_set.AddSolution(neighbor_solution)){

                    delete neighbor_solution;

                    return true;
                }
                else if(neighbor_solution->makeSpan > Instance::v_peak_start[0]){
                    neighbor_solution->CalculateHorizonAvoidingPeak();
                    neighbor_solution->CalculateObjective();

                    if(non_dominated_set.AddSolution(neighbor_solution)){

                        delete neighbor_solution;

                        return true;

                    }
                }

                *neighbor_solution = *ini_solution;
            }
        }
    //}

    delete neighbor_solution;

    //Retorna falso, caso não consiga encontrar um vizinho melhor
    return false;
}

/*
 * Método que gerar vizinhos através da realização de inserções de tarefas em uma máquina.
 * Ele gera todos os vizinhos e tenta inserir o melhor no conjunto não-dominado
 * (best improvement)
 */
bool InsertInsideLS_BI(LSSolution* my_solution, NDSetSolution<LSSolution *> &non_dominated_set_out)
{
    unsigned long num_job_maq;
    LSSolution *neighbor_sol = new LSSolution();
    //LSSolution *best_solution = new LSSolution();
    bool improve = false;

    //Criar uma cópia da solução
    *neighbor_sol = *my_solution;

    //Para cada máquina i de 1 à n
    //for (unsigned i = 1; i <= Instance::num_machine; i++) {
    unsigned i = neighbor_sol->GetMakespanMachine();
        num_job_maq = neighbor_sol->scheduling[i].size();

        //Para cada tarefa j da máquina i
        for (unsigned j = 0; j < num_job_maq; j++) {

            //Para cada tarefa k da máquina i
            for (unsigned k = 0; k < num_job_maq; ++k) {

                if(j != k){

                    //Criar uma cópia da solução
                    //*neighbor_sol = *my_solution;

                    //Realizar a inserção
                    neighbor_sol->InsertInsideDelta(i, j, k);
                    /*neighbor_sol->InsertInside(i, j, k);

                    neighbor_sol->CalculateShorterTimeHorizon();
                    neighbor_sol->CalculateObjective();*/

                    if(non_dominated_set_out.AddSolution(neighbor_sol)){
                    //if(neighbor_sol < best_solution){
                        //*best_solution = *neighbor_sol;
                        improve = true;
                    }

                    neighbor_sol->InsertInsideDelta(i, k, j);
                }

                neighbor_sol->CalculateShorterTimeHorizonMachine(i);
                neighbor_sol->CalculateObjectiveMachine(i);

            }
        }
    //}

    //delete best_solution;
    delete neighbor_sol;

    return improve;

}

/*
 * Método que gerar vizinhos através da realização de inserções de tarefas entre máquinas.
 * Ele tenta gerar novos vizinhos até encontrar um que possa ser incluído
 * no conjunto não-dominado (first improvement), ou até percorrer toda a vizinhança
 * Ele retorna verdadeiro, caso consiga adicionar um vizinho ao conjunto não-dominado
 */
bool InsertOutsideLS_FI(LSSolution *ini_solution, NDSetSolution<LSSolution *> &non_dominated_set)
{
    unsigned num_job_maq1, num_job_maq2;

    LSSolution *neighbor_solution = new LSSolution(*ini_solution);

    for (unsigned i1 = 1; i1 <= Instance::num_machine; i1++) {

        num_job_maq1 = ini_solution->scheduling[i1].size();

        if(num_job_maq1 < 1){
            continue;
        }

        //Para cada máquina i2 de i1+1 à n
        for (unsigned i2 = 1; i2 <= Instance::num_machine; i2++) {

            if(i1 == i2){
                continue;
            }

            num_job_maq2 = ini_solution->scheduling[i2].size();

            //Para cada tarefa j da máquina i1
            for (unsigned j = 0; j < num_job_maq1; j++) {

                //Para cada tarefa k da máquina i2
                for (unsigned k = 0; k <= num_job_maq2; ++k) {

                    //Gerar vizinho com a troca de tarefas entre máquinas
                    neighbor_solution->InsertOutside(i1, j, i2, k);
                    neighbor_solution->CalculateShorterTimeHorizonMachine(i1);
                    neighbor_solution->CalculateShorterTimeHorizonMachine(i2);
                    neighbor_solution->CalculateObjectiveMachine(i1);
                    neighbor_solution->CalculateObjectiveMachine(i2);

                    //Verificar se houve melhora global
                    if(non_dominated_set.AddSolution(neighbor_solution)){

                        delete neighbor_solution;

                        return true;
                    }
                    else if(neighbor_solution->makeSpan > Instance::v_peak_start[0]){
                        neighbor_solution->CalculateHorizonAvoidingPeak();
                        neighbor_solution->CalculateObjective();

                        if(non_dominated_set.AddSolution(neighbor_solution)){

                            delete neighbor_solution;

                            return true;

                        }
                    }

                    *neighbor_solution = *ini_solution;

                }

            }
        }
    }

    delete neighbor_solution;

    //Retorna falso, caso não consiga encontrar um vizinho melhor
    return false;
}

/*
 * Método que gerar vizinhos através da realização de inserções de tarefas entre máquinas.
 * Ele gera todos os vizinhos e tenta inserir o melhor no conjunto não-dominado
 * (best improvement)
 */
bool InsertOutsideLS_BI(LSSolution *my_solution, NDSetSolution<LSSolution *> &non_dominated_set_out)
{
    unsigned long num_job_maq1, num_job_maq2;

    LSSolution *neighbor_sol = new LSSolution();
    //LSSolution *best_solution = new LSSolution();
    bool improve = false;

    *neighbor_sol = *my_solution;

    //Para cada máquina i1 de 1 à n
    //for (unsigned i1 = 1; i1 <= Instance::num_machine; i1++) {
    unsigned i1 = neighbor_sol->GetMakespanMachine();
        num_job_maq1 = neighbor_sol->scheduling[i1].size();

        //Para cada máquina i2 de i1+1 à n
        for (unsigned i2 = 1; i2 <= Instance::num_machine; i2++) {
            num_job_maq2 = neighbor_sol->scheduling[i2].size();

            if(i1 != i2){
                //Para cada tarefa j da máquina i1
                for (unsigned j = 0; j < num_job_maq1; j++) {

                    //Para cada tarefa k da máquina i2
                    for (unsigned k = 0; k <= num_job_maq2; ++k) {

                        //Criar uma cópia da solução
                        //*neighbor_sol = *my_solution;

                        //Realizar a troca
                        neighbor_sol->InsertOutsideDelta(i1, j, i2, k);
                        /*neighbor_sol->InsertOutside(i1, j, i2, k);

                        neighbor_sol->CalculateShorterTimeHorizon();
                        neighbor_sol->CalculateObjective();*/

                        if(non_dominated_set_out.AddSolution(neighbor_sol)){
                        //if(neighbor_sol < best_solution){
                            //*best_solution = *neighbor_sol;
                            improve = true;
                        }

                        neighbor_sol->InsertOutsideDelta(i2, k, i1, j);

                    }

                    neighbor_sol->CalculateShorterTimeHorizonMachine(i1);
                    neighbor_sol->CalculateObjectiveMachine(i1);

                    neighbor_sol->CalculateShorterTimeHorizonMachine(i2);
                    neighbor_sol->CalculateObjectiveMachine(i2);
                }
            }
        }
    //}

    /*if(non_dominated_set_out.AddSolution(best_solution)){
        improve=true;
    }*/

    //delete best_solution;
    delete neighbor_sol;

    return improve;
}

/*
 * Método que gerar vizinhos através da realização da troca do modo de operação das tarefas
 * Ele tenta gerar novos vizinhos até encontrar um que possa ser incluído
 * no conjunto não-dominado (first improvement), ou até percorrer toda a vizinhança
 * Ele retorna verdadeiro, caso consiga adicionar um vizinho ao conjunto não-dominado
 */
bool ChangeOpModeLS_FI(LSSolution *ini_solution, NDSetSolution<LSSolution *> &non_dominated_set)
{
    unsigned long num_job_maq;

    LSSolution *neighbor_solution = new LSSolution(*ini_solution);

    //Para cada máquina i de 1 à n
    for (unsigned i = 1; i <= Instance::num_machine; i++) {

        num_job_maq = ini_solution->scheduling[i].size();

        //Para cada tarefa j da máquina i
        for (unsigned j = 0; j < num_job_maq; j++) {

            //Para cada modo de operração k
            for (unsigned k = 1; k <= Instance::num_mode_op; ++k) {

                auto job = ini_solution->scheduling[i][j];
                if(k == ini_solution->job_mode_op[job]){
                    continue;
                }

                neighbor_solution->ChangeModeOpJob(i, j, k);
                neighbor_solution->CalculateShorterTimeHorizonMachine(i);
                neighbor_solution->CalculateObjectiveMachine(i);

                //Verificar se houve melhora global
                if(non_dominated_set.AddSolution(neighbor_solution)){

                    delete neighbor_solution;

                    return true;
                }
                else if(neighbor_solution->makeSpan > Instance::v_peak_start[0]){
                    neighbor_solution->CalculateHorizonAvoidingPeak();
                    neighbor_solution->CalculateObjective();

                    if(non_dominated_set.AddSolution(neighbor_solution)){

                        delete neighbor_solution;

                        return true;

                    }
                }

                *neighbor_solution = *ini_solution;
            }
        }
    }

    delete neighbor_solution;

    //Retorna falso, caso não consiga encontrar um vizinho melhor
    return false;
}

/*
 * Método que gerar vizinhos através da realização da troca do modo de operação das tarefas
 * Ele gera todos os vizinhos e tenta inserir o melhor no conjunto não-dominado
 * (best improvement)
 */
bool ChangeOpModeLS_BI(LSSolution *my_solution, NDSetSolution<LSSolution *> &non_dominated_set_out)
{
    unsigned long num_job_maq, job, old_op;

    LSSolution *neighbor_sol = new LSSolution();
    //LSSolution *best_solution = new LSSolution();
    bool improve = false;

    //Criar uma cópia da solução
    *neighbor_sol = *my_solution;

    //Para cada máquina i de 1 à n
    for (unsigned i = 1; i <= Instance::num_machine; i++) {
        num_job_maq = neighbor_sol->scheduling[i].size();

        //Para cada tarefa j da máquina i
        for (unsigned j = 0; j < num_job_maq; j++) {

            job = neighbor_sol->scheduling[i][j];

            old_op = neighbor_sol->job_mode_op[job];

            //Para cada modo de operação k
            for (unsigned k = 1; k <= Instance::num_mode_op; ++k) {

                if(old_op != k){

                    //Criar uma cópia da solução
                    //*neighbor_sol = *my_solution;

                    neighbor_sol->ChangeModeOpJobDelta(i, j, k);
                    /*neighbor_sol->ChangeModeOpJob(i, j, k);

                    neighbor_sol->CalculateShorterTimeHorizon();
                    neighbor_sol->CalculateObjective();*/

                    if(non_dominated_set_out.AddSolution(neighbor_sol)){
                    //if(neighbor_sol < best_solution){
                        //*best_solution = *neighbor_sol;
                        improve = true;
                    }

                    neighbor_sol->ChangeModeOpJobDelta(i, j, old_op);
                }

                neighbor_sol->CalculateShorterTimeHorizonMachine(i);
                neighbor_sol->CalculateObjectiveMachine(i);

            }
        }
    }

    delete neighbor_sol;
    //delete best_solution;

    return improve;
}

//bool ChangeHLS(LSSolution my_solution, vector<LSSolution> &non_dominated_set)
bool ChangeHLS(LSSolution , vector<LSSolution> &)
{
    return false;
}

/*
 * Método que gerar vizinhos através da realização de inserções de tarefas entre máquinas.
 * Ele tenta gerar novos vizinhos até encontrar um que possa ser incluído
 * no conjunto não-dominado (first improvement), ou até percorrer toda a vizinhança
 * Ele retorna verdadeiro, caso consiga adicionar um vizinho ao conjunto não-dominado
 */
//bool InsertOutsideDuoLS_FI(LSSolution *my_solution, NDSetSolution<LSSolution *> &non_dominated_set)
//{
//    unsigned long num_job_maq1, num_job_maq2;

//    LSSolution *neighbor_sol = new LSSolution();


//    *neighbor_sol = *my_solution;

//    //Para cada máquina i1 de 1 à n
//    //for (unsigned i1 = 1; i1 <= Instance::num_machine; i1++) {
//    unsigned i1 = neighbor_sol->GetMakespanMachine();
//        num_job_maq1 = neighbor_sol->scheduling[i1].size();

//        //Para cada máquina i2 de i1+1 à n
//        for (unsigned i2 = 1; i2 <= Instance::num_machine; i2++) {
//            num_job_maq2 = neighbor_sol->scheduling[i2].size();

//            if(i1 != i2){
//                //Para cada tarefa j da máquina i1
//                for (unsigned j1 = 0; j1 < num_job_maq1-1; j1++) {
//                for (unsigned j2 = j1+1; j2 < num_job_maq1-1; j2++) {

//                    //Para cada tarefa k da máquina i2
//                    for (unsigned k1 = 0; k1 <= num_job_maq2-1; ++k1) {
//                    for (unsigned k2 = k1+1; k2 <= num_job_maq2; ++k2) {

//                        //Criar uma cópia da solução
//                        //*neighbor_sol = *my_solution;

//                        //Realizar a troca
//                        neighbor_sol->InsertOutsideDelta(i1, j1, j2, i2, k1, k2);
//                        /*neighbor_sol->InsertOutside(i1, j, i2, k);

//                        neighbor_sol->CalculateShorterTimeHorizon();
//                        neighbor_sol->CalculateObjective();*/

//                        //Tentar adicionar o vizinho gerado no conjunto não-dominado
//                        //Se conseguiu, então retorna true
//                        //if(AddSolution(neighbor_sol, non_dominated_set)){


//                        if(non_dominated_set.AddSolution(neighbor_sol)){

//                            return true;
//                        }


//                        /*if(neighbor_sol->machine_completion_time[i1] > Instance::v_peak_start[0] || neighbor_sol->machine_completion_time[i2] > Instance::v_peak_start[0]){

//                            neighbor_sol->CalculateHorizonAvoidingPeakMachine(i1);
//                            neighbor_sol->CalculateObjectiveMachine(i1);

//                            neighbor_sol->CalculateHorizonAvoidingPeakMachine(i2);
//                            neighbor_sol->CalculateObjectiveMachine(i2);

//                            //Tentar adicionar o vizinho gerado no conjunto não-dominado
//                            //Se conseguiu, então retorna true, senão continua a busca
//                            //if(AddSolution(neighbor_sol, non_dominated_set)){
//                            if(non_dominated_set.AddSolution(neighbor_sol)){
//                                return true;
//                            }
//                        }*/

//                        neighbor_sol->InsertOutsideDelta(i2, k1, k2, i1, j1, j2);
//                        //neighbor_sol->InsertOutside(i2, k, i1, j);
//                    }
//                    }

//                    neighbor_sol->CalculateShorterTimeHorizonMachine(i1);
//                    neighbor_sol->CalculateObjectiveMachine(i1);

//                    neighbor_sol->CalculateShorterTimeHorizonMachine(i2);
//                    neighbor_sol->CalculateObjectiveMachine(i2);
//                }
//                }
//            }
//        //}
//    }

//    delete neighbor_sol;

//    //Retorna falso, caso não consiga encontrar um vizinho melhor
//    return false;
//}
