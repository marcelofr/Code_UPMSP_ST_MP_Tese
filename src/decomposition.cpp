#include "decomposition.h"

/*
 * Método que gerar vizinhos através da realização de  trocas de tarefas em uma máquina.
 * Ele gera novos vizinhos e para quanto encontrar um que possa ser incluído
 * no conjunto não-dominado (first improvement), ou quando percorrer toda a vizinhança
 * Ele retorna verdadeiro, caso consiga gerar um vizinho melhor
 */
bool SwapInsideLSMonoFI(MonoSolution *neighbor_solution, const MonoSolution *best_solution)
{
    unsigned long num_job_maq;
    double best_objective_funtion;

    //Para cada máquina i de 1 à n
    //O TEC não muda em troca dentro da mesma máquina
    //O makespan só muda no máquina onde está o makespan
    unsigned i = neighbor_solution->GetMakespanMachine();
    //for (unsigned i = 1; i <= Instance::num_machine; i++) {

        num_job_maq = neighbor_solution->scheduling[i].size();

        if(num_job_maq < 2){

            //continue;
            return false;
        }

        const MonoSolution *neighbor_sol_ref = new MonoSolution(*neighbor_solution);

        //Para cada tarefa j da máquina i
        for (unsigned j = 0; j < num_job_maq-1 ; j++) {

            //Para cada tarefa k da máquina i
            for (unsigned k = j+1; k < num_job_maq ; ++k) {

                //Gerar vizinho com a troca de tarefas em uma máquina
                neighbor_solution->SwapInside(i, j, k);
                neighbor_solution->CalculateShorterTimeHorizonMachine(i);
                neighbor_solution->CalculateObjectiveMachine(i);
                neighbor_solution->CalculeMonoObjectiveTchebycheff();

                best_objective_funtion = best_solution->weights.first*(double(best_solution->makeSpan)/double(Z_STAR::makespan))+
                                        best_solution->weights.second*(double(best_solution->TEC)/double(Z_STAR::TEC));

                //Verificar se houve melhora global
                if(neighbor_solution->objective_funtion - best_objective_funtion < -EPS){

                    delete neighbor_sol_ref;

                    return true;
                }
                else if(neighbor_solution->makeSpan > Instance::v_peak_start[0]){
                    neighbor_solution->CalculateHorizonAvoidingPeak();
                    neighbor_solution->CalculateObjective();
                    neighbor_solution->CalculeMonoObjectiveTchebycheff();

                    best_objective_funtion = best_solution->weights.first*(double(best_solution->makeSpan)/double(Z_STAR::makespan))+
                                            best_solution->weights.second*(double(best_solution->TEC)/double(Z_STAR::TEC));

                    if(neighbor_solution->objective_funtion - best_objective_funtion < -EPS){

                        delete neighbor_sol_ref;

                        return true;

                    }
                }

                *neighbor_solution = *neighbor_sol_ref;
            }
        }
    //}
    delete neighbor_sol_ref;

    //Retorna falso, caso não consiga encontrar um vizinho melhor
    return false;
}

/*
 * Método que gerar vizinhos através da realização de  trocas de tarefas em uma máquina.
 * Ele gera novos vizinhos e para quanto encontrar um que possa ser incluído
 * no conjunto não-dominado (first improvement), ou quando percorrer toda a vizinhança
 * Ele retorna verdadeiro, caso consiga gerar um vizinho melhor
 */
bool SwapInsideLSMonoBI(MonoSolution *neighbor_solution)
{
    unsigned long num_job_maq;
    bool improve = false;

    //Para cada máquina i de 1 à n
    //O TEC não muda em troca dentro da mesma máquina
    //O makespan só muda no máquina onde está o makespan
    unsigned i = neighbor_solution->GetMakespanMachine();
    //for (unsigned i = 1; i <= Instance::num_machine; i++) {

        //unsigned i = neighbor_sol->GetMakespanMachine();
        num_job_maq = neighbor_solution->scheduling[i].size();

        if(num_job_maq < 2){
            //continue;
            return false;
        }

        const MonoSolution *neighbor_sol_ref = new MonoSolution(*neighbor_solution);
        MonoSolution *neighbor_sol_best = new MonoSolution(*neighbor_solution);

        //Para cada tarefa j da máquina i
        for (unsigned j = 0; j < num_job_maq-1 ; j++) {

            //Para cada tarefa k da máquina i
            for (unsigned k = j+1; k < num_job_maq ; ++k) {

                //Gerar vizinho com a troca de tarefas em uma máquina
                neighbor_solution->SwapInside(i, j, k);
                neighbor_solution->CalculateShorterTimeHorizonMachine(i);
                neighbor_solution->CalculateObjectiveMachine(i);
                neighbor_solution->CalculeMonoObjectiveTchebycheff();

                neighbor_sol_best->CalculeMonoObjectiveTchebycheff();

                //Verificar se houve melhora local
                if(neighbor_solution->objective_funtion - neighbor_sol_best->objective_funtion < -EPS){

                    improve = true;
                    *neighbor_sol_best = *neighbor_solution;
                }
                if(neighbor_solution->makeSpan > Instance::v_peak_start[0]){

                    neighbor_solution->CalculateHorizonAvoidingPeak();
                    neighbor_solution->CalculateObjective();
                    neighbor_solution->CalculeMonoObjectiveTchebycheff();

                    neighbor_sol_best->CalculeMonoObjectiveTchebycheff();

                    if(neighbor_solution->objective_funtion - neighbor_sol_best->objective_funtion < -EPS){

                        improve = true;
                        *neighbor_sol_best = *neighbor_solution;

                    }
                }

                *neighbor_solution = *neighbor_sol_ref;
            }
        }
    //}

    if(improve)
        *neighbor_solution = *neighbor_sol_best;

    delete neighbor_sol_ref;
    delete neighbor_sol_best;

    //Retorna falso, caso não consiga encontrar um vizinho melhor
    return improve;
}

/*
 * Método que gerar vizinhos através da realização de  trocas de tarefas entre máquinas
 * Ele gera novos vizinhos e para quanto encontrar um que possa ser incluído
 * no conjunto não-dominado (first improvement), ou quando percorrer toda a vizinhança
 * Ele retorna verdadeiro, caso consiga gerar um vizinho melhor
 * neighbor_solution é a solução que será modificada
 * best_solution é a solução de referência
 */
bool SwapOutsideLSMonoFI(MonoSolution *neighbor_solution, const MonoSolution *best_solution)
{
    unsigned long num_job_maq1, num_job_maq2;
    double best_objective_funtion;

    const MonoSolution *neighbor_sol_ref = new MonoSolution(*neighbor_solution);

    //Para cada máquina i1 de 1 à n
    for (unsigned i1 = 1; i1 <= Instance::num_machine; i1++) {

        num_job_maq1 = neighbor_sol_ref->scheduling[i1].size();

        if(num_job_maq1<1){
            continue;
        }

        //Para cada máquina i2 de i1+1 à n
        for (unsigned i2 = i1+1; i2 <= Instance::num_machine; i2++) {

            num_job_maq2 = neighbor_sol_ref->scheduling[i2].size();

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
                    neighbor_solution->CalculeMonoObjectiveTchebycheff();

                    best_objective_funtion = best_solution->weights.first*(double(best_solution->makeSpan)/double(Z_STAR::makespan))+
                                            best_solution->weights.second*(double(best_solution->TEC)/double(Z_STAR::TEC));

                    //Verificar se houve melhora global
                    if(neighbor_solution->objective_funtion - best_objective_funtion < -EPS){

                        delete neighbor_sol_ref;

                        return true;
                    }
                    else if(neighbor_solution->makeSpan > Instance::v_peak_start[0]){
                        neighbor_solution->CalculateHorizonAvoidingPeak();
                        neighbor_solution->CalculateObjective();
                        neighbor_solution->CalculeMonoObjectiveTchebycheff();

                        best_objective_funtion = best_solution->weights.first*(double(best_solution->makeSpan)/double(Z_STAR::makespan))+
                                                best_solution->weights.second*(double(best_solution->TEC)/double(Z_STAR::TEC));

                        if(neighbor_solution->objective_funtion - best_objective_funtion < -EPS){

                            delete neighbor_sol_ref;

                            return true;

                        }
                    }

                    *neighbor_solution = *neighbor_sol_ref;
                }

            }

        }
    }

    delete neighbor_sol_ref;

    //Retorna falso, caso não consiga encontrar um vizinho melhor
    return false;
}

/*
 * Método que gerar vizinhos através da realização de  trocas de tarefas entre máquinas
 * Ele gera novos vizinhos e para quanto encontrar um que possa ser incluído
 * no conjunto não-dominado (first improvement), ou quando percorrer toda a vizinhança
 * Ele retorna verdadeiro, caso consiga gerar um vizinho melhor
 * neighbor_solution é a solução que será modificada
 * best_solution é a solução de referência
 */
bool SwapOutsideLSMonoBI(MonoSolution *neighbor_solution)
{
    unsigned long num_job_maq1, num_job_maq2;
    bool improve = false;

    const MonoSolution *neighbor_sol_ref = new MonoSolution(*neighbor_solution);
    MonoSolution *neighbor_sol_best = new MonoSolution(*neighbor_solution);

    //Para cada máquina i1 de 1 à n
    for (unsigned i1 = 1; i1 <= Instance::num_machine; i1++) {

        num_job_maq1 = neighbor_sol_ref->scheduling[i1].size();

        if(num_job_maq1<1){
            continue;
        }

        //Para cada máquina i2 de i1+1 à n
        for (unsigned i2 = 1; i2 <= Instance::num_machine; i2++) {

            num_job_maq2 = neighbor_sol_ref->scheduling[i2].size();

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
                    neighbor_solution->CalculeMonoObjectiveTchebycheff();

                    neighbor_sol_best->CalculeMonoObjectiveTchebycheff();

                    //Verificar se houve melhora local
                    if(neighbor_solution->objective_funtion - neighbor_sol_best->objective_funtion < -EPS){

                        improve = true;
                        *neighbor_sol_best = *neighbor_solution;
                    }
                    else if(neighbor_solution->makeSpan > Instance::v_peak_start[0]){
                        neighbor_solution->CalculateHorizonAvoidingPeakMachine(i1);
                        neighbor_solution->CalculateHorizonAvoidingPeakMachine(i2);
                        neighbor_solution->CalculateObjectiveMachine(i1);
                        neighbor_solution->CalculateObjectiveMachine(i2);
                        neighbor_solution->CalculeMonoObjectiveTchebycheff();

                        neighbor_sol_best->CalculeMonoObjectiveTchebycheff();

                        if(neighbor_solution->objective_funtion - neighbor_sol_best->objective_funtion < -EPS){

                            improve = true;
                            *neighbor_sol_best = *neighbor_solution;

                        }
                    }

                    *neighbor_solution = *neighbor_sol_ref;
                }

            }

        }
    }

    if(improve)
        *neighbor_solution = *neighbor_sol_best;

    delete neighbor_sol_ref;
    delete neighbor_sol_best;

    //Retorna falso, caso não consiga encontrar um vizinho melhor
    return improve;
}

/*
 * Método que gerar vizinhos através da realização de inserção de tarefas em uma máquina.
 * Ele gera novos vizinhos e para quanto encontrar um que possa ser incluído
 * no conjunto não-dominado (first improvement), ou quando percorrer toda a vizinhança
 * Ele retorna verdadeiro, caso consiga gerar um vizinho melhor
 */
bool InsertInsideLSMonoFI(MonoSolution *neighbor_solution, const MonoSolution *best_solution)
{
    unsigned long num_job_maq;
    double best_objective_funtion;



    //Para cada máquina i de 1 à n
    //O TEC não muda em troca dentro da mesma máquina
    //O makespan só muda no máquina onde está o makespan
    unsigned i = neighbor_solution->GetMakespanMachine();
    //for (unsigned i = 1; i <= Instance::num_machine; i++) {

        num_job_maq = neighbor_solution->scheduling[i].size();

        if(num_job_maq < 2){

            //continue;
            return false;
        }

        const MonoSolution *neighbor_sol_ref = new MonoSolution(*neighbor_solution);

        //Para cada tarefa j da máquina i
        for (unsigned j = 0; j < num_job_maq-1; j++) {

            //Para cada tarefa k da máquina i
            for (unsigned k = j+1; k < num_job_maq; ++k) {

                //Gerar vizinho com a troca de tarefas em uma máquina
                neighbor_solution->InsertInside(i, j, k);
                neighbor_solution->CalculateShorterTimeHorizonMachine(i);
                neighbor_solution->CalculateObjectiveMachine(i);
                neighbor_solution->CalculeMonoObjectiveTchebycheff();

                best_objective_funtion = best_solution->weights.first*(double(best_solution->makeSpan)/double(Z_STAR::makespan))+
                                        best_solution->weights.second*(double(best_solution->TEC)/double(Z_STAR::TEC));

                //Verificar se houve melhora global
                if(neighbor_solution->objective_funtion - best_objective_funtion < -EPS){

                    delete neighbor_sol_ref;

                    return true;
                }
                else if(neighbor_solution->makeSpan > Instance::v_peak_start[0]){
                    neighbor_solution->CalculateHorizonAvoidingPeak();
                    neighbor_solution->CalculateObjective();
                    neighbor_solution->CalculeMonoObjectiveTchebycheff();

                    best_objective_funtion = best_solution->weights.first*(double(best_solution->makeSpan)/double(Z_STAR::makespan))+
                                            best_solution->weights.second*(double(best_solution->TEC)/double(Z_STAR::TEC));

                    if(neighbor_solution->objective_funtion - best_objective_funtion < -EPS){

                        delete neighbor_sol_ref;

                        return true;

                    }
                }

                *neighbor_solution = *neighbor_sol_ref;
            }
        }
    //}

    delete neighbor_sol_ref;

    //Retorna falso, caso não consiga encontrar um vizinho melhor
    return false;
}

/*
 * Método que gerar vizinhos através da realização de inserção de tarefas em uma máquina.
 * Ele gera novos vizinhos e para quanto encontrar um que possa ser incluído
 * no conjunto não-dominado (first improvement), ou quando percorrer toda a vizinhança
 * Ele retorna verdadeiro, caso consiga gerar um vizinho melhor
 */
bool InsertInsideLSMonoBI(MonoSolution *neighbor_solution)
{
    unsigned long num_job_maq;
    bool improve = false;



    //Para cada máquina i de 1 à n
    //O TEC não muda em troca dentro da mesma máquina
    //O makespan só muda no máquina onde está o makespan
    unsigned i = neighbor_solution->GetMakespanMachine();
    //for (unsigned i = 1; i <= Instance::num_machine; i++) {

        num_job_maq = neighbor_solution->scheduling[i].size();

        if(num_job_maq<2){
            //continue;
            return false;
        }

        const MonoSolution *neighbor_sol_ref = new MonoSolution(*neighbor_solution);
        MonoSolution *neighbor_sol_best = new MonoSolution(*neighbor_solution);

        //Para cada tarefa j da máquina i
        for (unsigned j = 0; j < num_job_maq-1; j++) {

            //Para cada tarefa k da máquina i
            for (unsigned k = j+1; k < num_job_maq; ++k) {

                //Gerar vizinho com a troca de tarefas em uma máquina
                neighbor_solution->InsertInside(i, j, k);
                neighbor_solution->CalculateShorterTimeHorizon();
                neighbor_solution->CalculateObjective();
                neighbor_solution->CalculeMonoObjectiveTchebycheff();

                neighbor_sol_best->CalculeMonoObjectiveTchebycheff();

                //Verificar se houve melhora local
                if(neighbor_solution->objective_funtion - neighbor_sol_best->objective_funtion < -EPS){

                    improve = true;
                    *neighbor_sol_best = *neighbor_solution;
                }
                else if(neighbor_solution->makeSpan > Instance::v_peak_start[0]){
                    neighbor_solution->CalculateHorizonAvoidingPeak();
                    neighbor_solution->CalculateObjective();
                    neighbor_solution->CalculeMonoObjectiveTchebycheff();

                    neighbor_sol_best->CalculeMonoObjectiveTchebycheff();

                    if(neighbor_solution->objective_funtion - neighbor_sol_best->objective_funtion < -EPS){

                        improve = true;
                        *neighbor_sol_best = *neighbor_solution;

                    }
                }

                *neighbor_solution = *neighbor_sol_ref;
            }
        }
    //}

    if(improve)
        *neighbor_solution = *neighbor_sol_best;

    delete neighbor_sol_ref;
    delete neighbor_sol_best;

    //Retorna falso, caso não consiga encontrar um vizinho melhor
    return improve;
}

/*
 * Método que gerar vizinhos através da realização de inserção de tarefas entre máquinas
 * Ele gera novos vizinhos e para quanto encontrar um que possa ser incluído
 * no conjunto não-dominado (first improvement), ou quando percorrer toda a vizinhança
 * Ele retorna verdadeiro, caso consiga gerar um vizinho melhor
 * InsertOutsideLSMono_FI(neighbor_solution, best_solution);
 */
bool InsertOutsideLSMonoFI(MonoSolution *neighbor_solution, const MonoSolution* best_solution)
{
    unsigned num_job_maq1, num_job_maq2;
    double best_objective_funtion;

    const MonoSolution *neighbor_sol_ref = new MonoSolution(*neighbor_solution);

    for (unsigned i1 = 1; i1 <= Instance::num_machine; i1++) {

        num_job_maq1 = neighbor_sol_ref->scheduling[i1].size();

        if(num_job_maq1 < 1){
            continue;
        }

        //Para cada máquina i2 de i1+1 à n
        for (unsigned i2 = 1; i2 <= Instance::num_machine; i2++) {

            if(i1 == i2){
                continue;
            }

            num_job_maq2 = neighbor_sol_ref->scheduling[i2].size();

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
                    neighbor_solution->CalculeMonoObjectiveTchebycheff();

                    best_objective_funtion = best_solution->weights.first*(double(best_solution->makeSpan)/double(Z_STAR::makespan))+
                                            best_solution->weights.second*(double(best_solution->TEC)/double(Z_STAR::TEC));

                    //Verificar se houve melhora global
                    if(neighbor_solution->objective_funtion - best_objective_funtion < -EPS){

                        delete neighbor_sol_ref;

                        return true;
                    }
                    else if(neighbor_solution->makeSpan > Instance::v_peak_start[0]){
                        neighbor_solution->CalculateHorizonAvoidingPeak();
                        neighbor_solution->CalculateObjective();
                        neighbor_solution->CalculeMonoObjectiveTchebycheff();

                        best_objective_funtion = best_solution->weights.first*(double(best_solution->makeSpan)/double(Z_STAR::makespan))+
                                                best_solution->weights.second*(double(best_solution->TEC)/double(Z_STAR::TEC));

                        if(neighbor_solution->objective_funtion - best_objective_funtion < -EPS){

                            delete neighbor_sol_ref;

                            return true;

                        }
                    }

                    *neighbor_solution = *neighbor_sol_ref;

                }

            }
        }
    }

    delete neighbor_sol_ref;

    //Retorna falso, caso não consiga encontrar um vizinho melhor
    return false;
}


/*
 * Método que gerar vizinhos através da realização de inserção de tarefas entre máquinas
 * Ele gera novos vizinhos e para quanto encontrar um que possa ser incluído
 * no conjunto não-dominado (first improvement), ou quando percorrer toda a vizinhança
 * Ele retorna verdadeiro, caso consiga gerar um vizinho melhor
 * InsertOutsideLSMono_FI(neighbor_solution, best_solution);
 */
bool InsertOutsideLSMonoBI(MonoSolution *neighbor_solution)
{
    unsigned num_job_maq1, num_job_maq2;
    bool improve = false;

    const MonoSolution *neighbor_sol_ref = new MonoSolution(*neighbor_solution);
    MonoSolution *neighbor_sol_best = new MonoSolution(*neighbor_solution);

    for (unsigned i1 = 1; i1 <= Instance::num_machine; i1++) {

        num_job_maq1 = neighbor_sol_ref->scheduling[i1].size();

        if(num_job_maq1 < 1){
            continue;
        }

        //Para cada máquina i2 de i1+1 à n
        for (unsigned i2 = 1; i2 <= Instance::num_machine; i2++) {

            if(i1 == i2){
                continue;
            }

            num_job_maq2 = neighbor_sol_ref->scheduling[i2].size();

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
                    neighbor_solution->CalculeMonoObjectiveTchebycheff();

                    neighbor_sol_best->CalculeMonoObjectiveTchebycheff();

                    //Verificar se houve melhora local
                    if(neighbor_solution->objective_funtion - neighbor_sol_best->objective_funtion < -EPS){

                        improve = true;
                        *neighbor_sol_best = *neighbor_solution;
                    }
                    else if(neighbor_solution->makeSpan > Instance::v_peak_start[0]){
                        neighbor_solution->CalculateHorizonAvoidingPeakMachine(i1);
                        neighbor_solution->CalculateHorizonAvoidingPeakMachine(i2);
                        neighbor_solution->CalculateObjectiveMachine(i1);
                        neighbor_solution->CalculateObjectiveMachine(i2);
                        neighbor_solution->CalculeMonoObjectiveTchebycheff();

                        neighbor_sol_best->CalculeMonoObjectiveTchebycheff();

                        if(neighbor_solution->objective_funtion - neighbor_sol_best->objective_funtion < -EPS){

                            improve = true;
                            *neighbor_sol_best = *neighbor_solution;

                        }
                    }

                    *neighbor_solution = *neighbor_sol_ref;

                }

            }
        }
    }

    if(improve)
        *neighbor_solution = *neighbor_sol_best;

    delete neighbor_sol_ref;
    delete neighbor_sol_best;

    //Retorna falso, caso não consiga encontrar um vizinho melhor
    return improve;
}


/*
 * Método que gerar vizinhos através da realização da troca do modo de operação das tarefas
 * Ele tenta gerar novos vizinhos até encontrar um que possa ser incluído
 * no conjunto não-dominado (first improvement), ou até percorrer toda a vizinhança
 * Ele retorna verdadeiro, caso consiga adicionar um vizinho ao conjunto não-dominado
 */
bool ChangeOpModeLSMonoFI(MonoSolution *neighbor_solution, const MonoSolution *best_solution)
{
    unsigned long num_job_maq;
    double best_objective_funtion;

    const MonoSolution *neighbor_sol_ref = new MonoSolution(*neighbor_solution);

    //Para cada máquina i de 1 à n
    for (unsigned i = 1; i <= Instance::num_machine; i++) {

        num_job_maq = neighbor_solution->scheduling[i].size();

        //Para cada tarefa j da máquina i
        for (unsigned j = 0; j < num_job_maq; j++) {

            //Para cada modo de operração k
            for (unsigned k = 1; k <= Instance::num_mode_op; ++k) {

                auto job = neighbor_sol_ref->scheduling[i][j];
                if(k == neighbor_sol_ref->job_mode_op[job]){
                    continue;
                }

                neighbor_solution->ChangeModeOpJob(i, j, k);
                neighbor_solution->CalculateShorterTimeHorizonMachine(i);
                neighbor_solution->CalculateObjectiveMachine(i);
                neighbor_solution->CalculeMonoObjectiveTchebycheff();

                best_objective_funtion = best_solution->weights.first*(double(best_solution->makeSpan)/double(Z_STAR::makespan))+
                                        best_solution->weights.second*(double(best_solution->TEC)/double(Z_STAR::TEC));

                //Verificar se houve melhora global
                if(neighbor_solution->objective_funtion - best_objective_funtion < -EPS){

                    delete neighbor_sol_ref;

                    return true;
                }
                else if(neighbor_solution->makeSpan > Instance::v_peak_start[0]){
                    neighbor_solution->CalculateHorizonAvoidingPeak();
                    neighbor_solution->CalculateObjective();
                    neighbor_solution->CalculeMonoObjectiveTchebycheff();

                    best_objective_funtion = best_solution->weights.first*(double(best_solution->makeSpan)/double(Z_STAR::makespan))+
                                            best_solution->weights.second*(double(best_solution->TEC)/double(Z_STAR::TEC));

                    if(neighbor_solution->objective_funtion - best_objective_funtion < -EPS){

                        delete neighbor_sol_ref;

                        return true;

                    }
                }

                *neighbor_solution = *neighbor_sol_ref;
            }
        }
    }

    delete neighbor_sol_ref;

    //Retorna falso, caso não consiga encontrar um vizinho melhor
    return false;
}

/*
 * Método que gerar vizinhos através da realização da troca do modo de operação das tarefas
 * Ele tenta gerar novos vizinhos até encontrar um que possa ser incluído
 * no conjunto não-dominado (first improvement), ou até percorrer toda a vizinhança
 * Ele retorna verdadeiro, caso consiga adicionar um vizinho ao conjunto não-dominado
 */
bool ChangeOpModeLSMonoBI(MonoSolution *neighbor_solution)
{
    unsigned long num_job_maq;
    bool improve = false;

    const MonoSolution *neighbor_sol_ref = new MonoSolution(*neighbor_solution);
    MonoSolution *neighbor_sol_best = new MonoSolution(*neighbor_solution);

    //Para cada máquina i de 1 à n
    for (unsigned i = 1; i <= Instance::num_machine; i++) {

        num_job_maq = neighbor_solution->scheduling[i].size();

        //Para cada tarefa j da máquina i
        for (unsigned j = 0; j < num_job_maq; j++) {

            //Para cada modo de operração k
            for (unsigned k = 1; k <= Instance::num_mode_op; ++k) {

                auto job = neighbor_sol_ref->scheduling[i][j];
                if(k == neighbor_sol_ref->job_mode_op[job]){
                    continue;
                }

                neighbor_solution->ChangeModeOpJob(i, j, k);
                neighbor_solution->CalculateShorterTimeHorizonMachine(i);
                neighbor_solution->CalculateObjectiveMachine(i);
                neighbor_solution->CalculeMonoObjectiveTchebycheff();

                neighbor_sol_best->CalculeMonoObjectiveTchebycheff();

                //Verificar se houve melhora local
                if(neighbor_solution->objective_funtion - neighbor_sol_best->objective_funtion < -EPS){

                    improve = true;
                    *neighbor_sol_best = *neighbor_solution;
                }
                else if(neighbor_solution->makeSpan > Instance::v_peak_start[0]){
                    neighbor_solution->CalculateHorizonAvoidingPeak();
                    neighbor_solution->CalculateObjective();
                    neighbor_solution->CalculeMonoObjectiveTchebycheff();

                    neighbor_sol_best->CalculeMonoObjectiveTchebycheff();

                    if(neighbor_solution->objective_funtion - neighbor_sol_best->objective_funtion < -EPS){

                        improve = true;
                        *neighbor_sol_best = *neighbor_solution;

                    }
                }

                *neighbor_solution = *neighbor_sol_ref;
            }
        }
    }

    if(improve)
        *neighbor_solution = *neighbor_sol_best;

    delete neighbor_sol_ref;
    delete neighbor_sol_best;

    //Retorna falso, caso não consiga encontrar um vizinho melhor
    return improve;
}


bool CompareMakespanMonoSolution(MonoSolution *& l, MonoSolution *& r) //(2)
{

    if(l->makeSpan < r->makeSpan){
        return true;
    }
    else if (l->makeSpan > r->makeSpan){
        return false;
    }
    else{
        if(l->TEC < r->TEC){
            return true;
        }
        else{
            return false;
        }
    }

}

void SortByMakespanMonoSolution(vector<MonoSolution*> &set_solution)
{
    //sort(set_solution.begin(), set_solution.end(), CompareMakespanMonoSolution);
    sort(set_solution.begin(), set_solution.end(), CompareMakespanMonoSolution);
    //sort(set_solution.begin(), set_solution.end());
}

void SetWeights(vector<MonoSolution *> &set_solution){

    vector<pair<double, double>> Weigths;
    unsigned qtd_solutions;
    qtd_solutions = set_solution.size();

    GenerateWeightVector(Weigths, qtd_solutions);

    unsigned i=0;
    for(auto it: set_solution){
        it->weights = Weigths[i++];
    }

}


void GenerateWeightVector(vector<pair<double, double>> &Weigths, unsigned num_weights){

    Weigths.resize(num_weights);

    unsigned x,y;
    x = num_weights;
    y = 0;

    Weigths[0].first = 1-EPS;
    Weigths[0].second = EPS;

    x--;
    y++;

    for(unsigned h=1;h<num_weights-1 ;h++){
        Weigths[h].first = double(x-1)/double(num_weights-1);
        Weigths[h].second = double(y)/double(num_weights-1);
        x--;
        y++;
    }

    Weigths[num_weights-1].first = EPS;
    Weigths[num_weights-1].second = 1-EPS;
    x--;
    y++;

}

void Destruction(MonoSolution * my_solution, unsigned level){

    //unsigned job_position, max_ct, machine_max_ct, min_ct, machine_min_ct;
    unsigned num_jobs;
    unsigned machine, job_position;
    vector<unsigned> removed_jobs;

    MonoSolution * new_solution = new MonoSolution(*my_solution);

    unsigned i;
    num_jobs = 0;
    for (i=1;i <= Instance::num_machine;i++ ) {
        num_jobs += new_solution->scheduling[i].size();
    }

    level = min(level, num_jobs);

    auto machine_makespan = new_solution->GetMakespanMachine();
    for (i=0;i < level && new_solution->scheduling[machine_makespan].size()>0 ;i++ ) {

        //job_position = rand()%solution->scheduling[machine_makespan].size();
        //solution->RemovingJob(machine_makespan, job_position);

        job_position = rand()%new_solution->scheduling[machine_makespan].size();

        new_solution->RemovingJob(machine_makespan, job_position);

    }

    for (;i < level;i++ ) {

        do{
            machine = 1 + rand()%Instance::num_machine;
        }while(new_solution->scheduling[machine].size() < 1);

        job_position = rand()%new_solution->scheduling[machine].size();

        new_solution->RemovingJob(machine, job_position);

    }

    new_solution->CalculateShorterTimeHorizon();
    new_solution->CalculateObjective();

    *my_solution = *new_solution;

    delete new_solution;

}

void Construction(MonoSolution * ini_partial_solution, vector<unsigned> removed_jobs){


    MonoSolution *best_partial_solution = new MonoSolution();
    MonoSolution *new_partial_solution = new MonoSolution();

    unsigned size;
    unsigned best_op_mode;

    *best_partial_solution = *ini_partial_solution;

    while (removed_jobs.size() > 0) {

        auto best_it_job = removed_jobs.begin();

        //A primeira será criada na máquina 1 e posição 0
        best_partial_solution->AddJob(*best_it_job, 1, 0, 0);
        best_op_mode = 1;
        best_partial_solution->job_mode_op[*best_it_job] = best_op_mode;

        best_partial_solution->CalculateShorterTimeHorizon();
        best_partial_solution->CalculateObjective();
        best_partial_solution->CalculeMonoObjectiveTchebycheff();

        for(auto it_job = removed_jobs.begin(); it_job != removed_jobs.end(); ++it_job){

            //Para cada máquina
            for(unsigned i=1; i<=Instance::num_machine; i++){

                size = ini_partial_solution->scheduling[i].size();

                for(unsigned pos=0; pos<=size; pos++ ){

                    *new_partial_solution = *ini_partial_solution;
                    new_partial_solution->AddJob(*it_job, i, pos, 0);

                    //Para cada modo de operação
                    for(unsigned op_mode = 1; op_mode <= Instance::num_mode_op; op_mode++ ){

                        new_partial_solution->job_mode_op[*it_job] = op_mode;

                        new_partial_solution->CalculateShorterTimeHorizon();
                        new_partial_solution->CalculateObjective();
                        new_partial_solution->CalculeMonoObjectiveTchebycheff();

                        best_partial_solution->CalculeMonoObjectiveTchebycheff();

                        //if(*new_partial_solution < *best_partial_solution){
                        if(new_partial_solution->objective_funtion - best_partial_solution->objective_funtion < -EPS){
                            *best_partial_solution = *new_partial_solution;

                            best_op_mode = op_mode;

                            best_it_job = it_job;
                        }

                    }
                }
            }
        }

        *ini_partial_solution =  *best_partial_solution;

        ini_partial_solution->job_mode_op[*best_it_job] = best_op_mode;

        removed_jobs.erase(best_it_job);
    }

    ini_partial_solution->CalculateShorterTimeHorizon();
    ini_partial_solution->CalculateObjective();
    ini_partial_solution->CalculeMonoObjectiveTchebycheff();

#ifdef DEBUG
    ini_partial_solution->Check();
#endif

    delete best_partial_solution;
    delete new_partial_solution;

}

void Construction2(MonoSolution * partial_solution, vector<unsigned> removed_jobs){

    MonoSolution * best_partial_solution = new MonoSolution() ;
    MonoSolution * new_partial_solution = new MonoSolution() ;


    *best_partial_solution = *partial_solution;
    *new_partial_solution = *partial_solution;

    for(unsigned it_1=0; it_1<removed_jobs.size(); it_1++){

        for(unsigned it_2=it_1+1; it_2<removed_jobs.size(); it_2++){


            best_partial_solution->AddJob(it_1, 1, 0, 0);
            best_partial_solution->job_mode_op[1] = 1;
            best_partial_solution->CalculateShorterTimeHorizon();
            best_partial_solution->CalculateObjective();

            //Para cada modo de operação
            for(unsigned op_mode = 1; op_mode <= Instance::num_mode_op; op_mode++ ){

                //Para cada máquina
                for(unsigned i=1; i<=Instance::num_machine; i++){

                    for(unsigned pos=0; pos <partial_solution->scheduling[i].size(); pos++){
                        new_partial_solution->AddJob(it_2, i, pos, 0);
                        new_partial_solution->CalculateShorterTimeHorizon();
                        new_partial_solution->CalculateObjective();
                        new_partial_solution->CalculeMonoObjectiveTchebycheff();

                        best_partial_solution->CalculeMonoObjectiveTchebycheff();

                        if(new_partial_solution->objective_funtion < best_partial_solution->objective_funtion){
                            *best_partial_solution = *new_partial_solution;
                        }
                    }
                }
            }

        }
    }

    *partial_solution = *best_partial_solution;

    delete best_partial_solution;
    delete new_partial_solution;
}

bool IntesificationArroyo(MonoSolution *current_solution, unsigned level){

    bool improve = false;
    MonoSolution *partial_solution;

    partial_solution = new MonoSolution(*current_solution);

    Destruction(partial_solution, level);

    vector<unsigned> removed_jobs(partial_solution->removed_jobs);

    partial_solution->removed_jobs.clear();


    Construction(partial_solution, removed_jobs);
    //Construction2(partial_solution, removed_jobs);

    *current_solution = *partial_solution;
    /*if(partial_solution->objective_funtion - current_solution->objective_funtion < - EPS){

    }*/

    delete partial_solution;

    return improve;

}

void GenNeighborSol(MonoSolution *my_solution, unsigned op_neighbor)
{

    unsigned i1, i2, j1, j2, mode_op;

    unsigned cont =0;

    MonoSolution *new_solution = new MonoSolution(*my_solution);

    //op_mov
    switch (op_neighbor) {
        //Troca na mesma máquina
        case 0:

            if(Instance::num_machine < 1 ){
                break;
            }

            //Selecionar a máquina i1 com pelo menos 2 tarefas
            cont =0;
            do {
                i1 = 1+rand()%Instance::num_machine;
                cont++;
            } while (new_solution->scheduling[i1].size() < 2 && cont < Instance::num_machine*10);

            if(new_solution->scheduling[i1].size() < 2){
                break;
            }

            //Selecionar a primeira tarefa
            j1 = rand()%new_solution->scheduling[i1].size();

            //Selecionar a segunda tarefa, diferente da primeira
            cont =0;
            do{
                j2 = rand()%new_solution->scheduling[i1].size();
                cont++;
            }while(j1==j2 && cont < Instance::num_jobs*10);

            if(j1==j2){
                break;
            }

            //Realizar troca
            new_solution->SwapInside(i1, j1, j2);
            break;

        //Troca em máquinas diferentes
        case 1:

            if(Instance::num_machine < 2 ){
                break;
            }

            //Selecionar máquina i1 com pelo menos 1 tarefa
            cont =0;
            do {
                i1 = 1+rand()%Instance::num_machine;
                cont++;
            } while (new_solution->scheduling[i1].size() < 1 && cont < Instance::num_machine*10);

            if(new_solution->scheduling[i1].size() < 1){
                break;
            }

            //Selecionar máquina i2 com pelo menos 1 tarefa
            cont =0;
            do {
                i2 = 1+rand()%Instance::num_machine;
                cont++;
            } while ((i1 == i2 || new_solution->scheduling[i2].size() < 1) && cont < Instance::num_machine*10);

            if(new_solution->scheduling[i2].size() < 1){
                break;
            }

            //Selecionar a primeira tarefa
            j1 = rand()%new_solution->scheduling[i1].size();

            //Selecionar a segunda tarefa
            j2 = rand()%new_solution->scheduling[i2].size();

            //Realizar troca
            new_solution->SwapOutside(i1, j1, i2, j2);

            break;

        //Inserção na mesma máquina
        case 2:

            if(Instance::num_machine < 1 ){
                break;
            }

            //Selecionar a máquina i1 com pelo menos 2 tarefas
            cont =0;
            do {
                i1 = 1+rand()%Instance::num_machine;
                cont++;
            } while (new_solution->scheduling[i1].size() < 2 && cont < Instance::num_machine*10);

            if(new_solution->scheduling[i1].size() < 2){
                break;
            }

            //Selecionar a primeira tarefa
            j1 = rand()%new_solution->scheduling[i1].size();

            //Selecionar a segunda tarefa, diferente da primeira
            cont=0;
            do{
                j2 = rand()%new_solution->scheduling[i1].size();
                cont++;
            }while(j1==j2 && cont < Instance::num_jobs*10);

            if(j1==j2){
                break;
            }

            //Realizar inserção
            new_solution->InsertInside(i1, j1, j2);

            break;

        //Inserção em máquinas diferentes
        case 3:

            if(Instance::num_machine < 2 ){
                break;
            }

            //Selecionar máquina i1 com pelo menos 1 tarefa
            cont =0;
            do {
                i1 = 1+rand()%Instance::num_machine;
                cont++;
            } while (new_solution->scheduling[i1].size() < 1 && cont < Instance::num_machine*10);


            if(new_solution->scheduling[i1].size() < 1){
                break;
            }

            //Selecionar máquina i2 com pelo menos 1 tarefa
            cont =0;
            do {
                i2 = 1+rand()%Instance::num_machine;
                cont++;
            } while ((i1 == i2 || new_solution->scheduling[i2].size() < 1) && (cont < Instance::num_machine*10));

            if(new_solution->scheduling[i2].size() < 1 || i1 == i2){
                break;
            }

            //Selecionar a primeira tarefa
            j1 = rand()%new_solution->scheduling[i1].size();

            //Selecionar a segunda tarefa
            j2 = rand()%new_solution->scheduling[i2].size();

            //Realizar inserção
            new_solution->InsertOutside(i1, j1, i2, j2);

            break;

        //Troca de modo de operação
        case 4:

            if(Instance::num_machine < 1){
                break;
            }

            //Selecionar máquina i1 com pelo menos 1 tarefa
            cont =0;
            do {
                i1 = 1+rand()%Instance::num_machine;
                cont++;
            } while (new_solution->scheduling[i1].size() < 1 && cont < Instance::num_machine*10);

            if(new_solution->scheduling[i1].size() < 1){
                break;
            }

            //Selecionar uma tarefa
            j1 = rand()%new_solution->scheduling[i1].size();

            //Selecionar modo de operação
            mode_op = 1+rand()%Instance::num_mode_op;

            //Realizar troca do modo de operação
            new_solution->ChangeModeOpJob(i1, j1, mode_op);

            break;
    }

#ifdef DEBUG
    my_solution->Check();
#endif

    *my_solution = *new_solution;

    delete new_solution;

}


MonoSolution * Shaking(MonoSolution *cur_solution, unsigned op_neighbor, unsigned level)
{

    for(unsigned i=0; i<level; i++){
        //Gerar uma solução vizinha aleatoriamente
        GenNeighborSol(cur_solution, op_neighbor);
    }

    return cur_solution;
}

void Shaking(MonoSolution *cur_solution, unsigned level)
{

    unsigned op_neighbor;

    MonoSolution *new_solution = new MonoSolution(*cur_solution);

    for(unsigned i=0; i<level; i++){
        op_neighbor = rand()%QTD_NEIGHBOR;
        //Gerar uma solução vizinha aleatoriamente
        GenNeighborSol(new_solution, op_neighbor);
    }

    *cur_solution = *new_solution;

    cur_solution->CalculateShorterTimeHorizon();
    cur_solution->CalculateObjective();
    cur_solution->CalculeMonoObjectiveTchebycheff();

    delete new_solution;

}
/*
 * Local search monobjective with best improvement
 */
bool LS_Mono_BI(MonoSolution *my_solution, unsigned op_neighbor){

    MonoSolution *current_solution = new MonoSolution();

    *current_solution = *my_solution;

    bool improve = false;

    switch (op_neighbor) {
        case 0:
            SwapInsideLSMonoBI(current_solution);
            my_solution->CalculeMonoObjectiveTchebycheff();
            current_solution->CalculeMonoObjectiveTchebycheff();
            if(current_solution->objective_funtion < my_solution->objective_funtion){
                *my_solution = *current_solution;
                improve = true;
            }
        break;
        case 1:
            SwapOutsideLSMonoBI(current_solution);
            my_solution->CalculeMonoObjectiveTchebycheff();
            current_solution->CalculeMonoObjectiveTchebycheff();
            if(current_solution->objective_funtion < my_solution->objective_funtion){
                *my_solution = *current_solution;
                improve = true;
            }
        break;
        case 2:
            InsertInsideLSMonoBI(current_solution);
            my_solution->CalculeMonoObjectiveTchebycheff();
            current_solution->CalculeMonoObjectiveTchebycheff();
            if(current_solution->objective_funtion < my_solution->objective_funtion){
                *my_solution = *current_solution;
                improve = true;
            }
        break;
        case 3:
            InsertOutsideLSMonoBI(current_solution);
            my_solution->CalculeMonoObjectiveTchebycheff();
            current_solution->CalculeMonoObjectiveTchebycheff();
            if(current_solution->objective_funtion < my_solution->objective_funtion){
                *my_solution = *current_solution;
                improve = true;
            }
        break;
        case 4:
            ChangeOpModeLSMonoBI(current_solution);
            my_solution->CalculeMonoObjectiveTchebycheff();
            current_solution->CalculeMonoObjectiveTchebycheff();
            if(current_solution->objective_funtion < my_solution->objective_funtion){
                *my_solution = *current_solution;
                improve = true;
            }
        break;

    }

    return improve;

}

/*
 * Local search monobjective with first improvement
 */
bool LS_Mono_FI(MonoSolution *my_solution, unsigned op_neighbor){

    MonoSolution *current_solution = new MonoSolution();

    my_solution->CalculeMonoObjectiveTchebycheff();

    *current_solution = *my_solution;

    switch (op_neighbor) {
        case 0:
            SwapInsideLSMonoFI(current_solution, my_solution);
            my_solution->CalculeMonoObjectiveTchebycheff();
            current_solution->CalculeMonoObjectiveTchebycheff();
            if(current_solution->objective_funtion < my_solution->objective_funtion){
                *my_solution = *current_solution;
                return true;
            }
        break;
        case 1:
            SwapOutsideLSMonoFI(current_solution, my_solution);
            my_solution->CalculeMonoObjectiveTchebycheff();
            current_solution->CalculeMonoObjectiveTchebycheff();
            if(current_solution->objective_funtion < my_solution->objective_funtion){
                *my_solution = *current_solution;
                return true;
            }
        break;
        case 2:
            InsertInsideLSMonoFI(current_solution, my_solution);
            my_solution->CalculeMonoObjectiveTchebycheff();
            current_solution->CalculeMonoObjectiveTchebycheff();
            if(current_solution->objective_funtion < my_solution->objective_funtion){
                *my_solution = *current_solution;
                return true;
            }
        break;
        case 3:
            InsertOutsideLSMonoFI(current_solution, my_solution);
            my_solution->CalculeMonoObjectiveTchebycheff();
            current_solution->CalculeMonoObjectiveTchebycheff();
            if(current_solution->objective_funtion < my_solution->objective_funtion){
                *my_solution = *current_solution;
                return true;
            }
        break;
        case 4:
            ChangeOpModeLSMonoFI(current_solution, my_solution);
            my_solution->CalculeMonoObjectiveTchebycheff();
            current_solution->CalculeMonoObjectiveTchebycheff();
            if(current_solution->objective_funtion < my_solution->objective_funtion){
                *my_solution = *current_solution;
                return true;
            }
        break;
        case 5:

        break;

    }

    return false;

}


void MOVNS_D(NDSetSolution<MonoSolution *> &non_dominated_set, algorithm_data alg_data, Timer *t1)
{

    MonoSolution * neighbor_solution = new MonoSolution();
    MonoSolution * best_solution;
    //MonoSolution * cur_solution = new MonoSolution();

    unsigned op_neighboor, intensification_level;
    bool improve;
    long index;
    unsigned num_neighboor;


    UpdateZ_STAR(non_dominated_set.set_solution);

    for(auto &it_sol:non_dominated_set.set_solution){
        it_sol->CalculeMonoObjectiveTchebycheff();
    }

    //---------------------Manter atualizado
    //Número de vizinhanças
    num_neighboor=5;

    //Nível da perturbação
    intensification_level = 3 + ceil(double(Instance::num_jobs)/double(750)*7);

    index = 0;

    //auto perturbation_level = 4;

    //---------------------

    while (t1->getElapsedTimeInMilliSec() < alg_data.time_limit) {

        auto s_index = index%non_dominated_set.set_solution.size();

        //Escolher a próxima solução a ser explorada
        best_solution = non_dominated_set.set_solution[s_index];
        best_solution->CalculeMonoObjectiveTchebycheff();

        //cur_solution guarda a solução perturbada
        //*cur_solution = *best_solution;
        //*cur_solution = *non_dominated_set.set_solution[s_index];

        //Shaking(cur_solution, op_shake, perturbation_level);
        //Shaking(cur_solution, perturbation_level);

        /*best_solution->CalculeMonoObjectiveTchebycheff();
        if(cur_solution->objective_funtion - best_solution->objective_funtion < -EPS){
            continue;
        }*/

        op_neighboor = 0;

        unsigned OP_BUSCA = 0;

        while (op_neighboor < num_neighboor && t1->getElapsedTimeInMilliSec() < alg_data.time_limit) {

            improve=false;
            *neighbor_solution = *best_solution;
            //*neighbor_solution = *cur_solution;

            switch (op_neighboor) {
                case 0:
                    //Explorar a solução escohida em relação a vizinhança de troca entre máquinas
                    if(OP_BUSCA==0)
                        improve=SwapInsideLSMonoFI(neighbor_solution, best_solution);
                    else
                        improve=SwapInsideLSMonoBI(neighbor_solution);
                    break;
                case 1:
                    if(OP_BUSCA==0)
                        improve=InsertInsideLSMonoFI(neighbor_solution, best_solution);
                    else
                        improve=InsertInsideLSMonoBI(neighbor_solution);
                    break;
                case 2:
                    //Explorar a solução escohida em relação a vizinhança de troca entre máquinas
                    if(OP_BUSCA==0)
                        improve=SwapOutsideLSMonoFI(neighbor_solution, best_solution);
                    else
                        improve=SwapOutsideLSMonoBI(neighbor_solution);
                    break;
                case 3:
                    if(OP_BUSCA==0)
                        improve=InsertOutsideLSMonoFI(neighbor_solution, best_solution);
                    else
                        improve=InsertOutsideLSMonoBI(neighbor_solution);
                    break;
                case 4:
                    //Explorar a solução escohida em relação a vizinhança de mudança de modo de operação
                    if(OP_BUSCA==0)
                        improve=ChangeOpModeLSMonoFI(neighbor_solution, best_solution);
                    else
                        improve=ChangeOpModeLSMonoBI(neighbor_solution);
                    break;
            }

            //Se a busca é first improvement
            if(OP_BUSCA==0){
                //Verificar se houve melhora
                if(improve){
                    CheckDominatedSolution(non_dominated_set.set_solution, neighbor_solution);
                    *best_solution = *neighbor_solution;
                }
            }//Se a busca é best improvement
            else{
                //Verificar se houve melhora
                best_solution->CalculeMonoObjectiveTchebycheff();
                if(neighbor_solution->objective_funtion - best_solution->objective_funtion < -EPS){
                    CheckDominatedSolution(non_dominated_set.set_solution, neighbor_solution);
                    *best_solution = *neighbor_solution;
                }
            }

            //Seguir para próxima vizinhança
            op_neighboor++;

            //Se não tem próxima vizinhança, fazer a intensificação
            if(op_neighboor == num_neighboor){

                *neighbor_solution = *best_solution;

                IntesificationArroyo(neighbor_solution, intensification_level);

                best_solution->CalculeMonoObjectiveTchebycheff();

                //Verificar se houve melhora
                if(neighbor_solution->objective_funtion - best_solution->objective_funtion < -EPS){
                    CheckDominatedSolution(non_dominated_set.set_solution, neighbor_solution);
                    *best_solution = *neighbor_solution;
                }

                //Se alguma tarefa está sendo alocada dentro do horário de pico
                if(neighbor_solution->makeSpan > Instance::v_peak_start[0]){
                    neighbor_solution->CalculateHorizonAvoidingPeak();
                    neighbor_solution->CalculateObjective();
                    neighbor_solution->CalculeMonoObjectiveTchebycheff();

                    best_solution->CalculeMonoObjectiveTchebycheff();

                    if(neighbor_solution->objective_funtion - best_solution->objective_funtion < -EPS){

                        CheckDominatedSolution(non_dominated_set.set_solution, neighbor_solution);
                        *best_solution = *neighbor_solution;

                    }
                }

            }

            t1->stop();
        }

        UpdateZ_STAR(non_dominated_set.set_solution);

        index++;
        t1->stop();
    }


    //delete best_solution;
    delete neighbor_solution;
    //delete cur_solution;
}

void CheckDominatedSolution(vector<MonoSolution *> set_solution, MonoSolution * my_solution){

    for(auto &it_sol:set_solution){
        /*if(neighbor_solution->makeSpan > it_sol->makeSpan &&
                neighbor_solution->TEC > it_sol->TEC){*/
        if(*it_sol < *my_solution){

            auto weights_bkp = my_solution->weights;
            //*it_sol = *best_solution;
            *my_solution = *it_sol;
            my_solution->weights = weights_bkp;

            break;

        }
    }
}

void UpdateZ_STAR(vector<MonoSolution *> &set_solution){

    Z_STAR::makespan = Z_STAR::TEC = 0;

    for(auto it:set_solution){
        if(it->makeSpan > Z_STAR::makespan){
            Z_STAR::makespan = it->makeSpan;
        }

        if(it->TEC > Z_STAR::TEC){
            Z_STAR::TEC = it->TEC;
        }
    }
}

void MOVNS_D_Vivian(NDSetSolution<MonoSolution *> &non_dominated_set, algorithm_data alg_data, Timer *t1)
{

    MonoSolution * neighbor_solution = new MonoSolution();
    MonoSolution * best_solution;
    MonoSolution * cur_solution = new MonoSolution();

    unsigned op_neighboor, intensification_level, perturbation_level;
    bool improve;
    long index;
    unsigned num_neighboor;

    UpdateZ_STAR(non_dominated_set.set_solution);

    //---------------------Manter atualizado
    //Número de vizinhanças
    num_neighboor=5;

    //Nível da perturbação
    intensification_level = 3 + ceil(double(Instance::num_jobs)/double(750)*7);

    index = 0;

    perturbation_level = 4;

    //---------------------

    vector<pair<double, double>> Weights;
    GenerateWeightVector(Weights, alg_data.num_weights);

    //Calcular as distâncias entre os pesos par a par
    vector<vector<double>>distance_weighted(alg_data.num_weights, vector<double>(alg_data.num_weights));
    for(unsigned i=0; i<alg_data.num_weights; i++){
        for(unsigned j=0; j<alg_data.num_weights; j++){
            distance_weighted[i][j] = CalcEuclideanDistance(Weights[i], Weights[j]);
        }
    }

    //Associar as soluções mais próximas para cada peso
    unsigned best_index;
    double best_distance;
    vector<vector<unsigned>>solution_neighboor(alg_data.num_weights, vector<unsigned>(alg_data.num_group));

    //Associar cada peso a um conjunto j de pesos
    for(unsigned i=0; i<alg_data.num_weights; i++){
        for(unsigned k=0; k<alg_data.num_group; k++){
            best_distance = INT_MAX;
            best_index = 0;
            for(unsigned j=0; j<alg_data.num_weights; j++){
                if(distance_weighted[i][j] < best_distance){
                    best_distance = distance_weighted[i][j];
                    best_index = j;
                }
            }
            solution_neighboor[i][k] = best_index;
            distance_weighted[i][best_index] = INT_MAX;
        }
    }

    while (t1->getElapsedTimeInMilliSec() < alg_data.time_limit) {

        auto s_index = index%non_dominated_set.set_solution.size();

        //Escolher a próxima solução a ser explorada
        best_solution = non_dominated_set.set_solution[s_index];
        best_solution->CalculeMonoObjectiveTchebycheff();

        //cur_solution guarda a solução perturbada
        *cur_solution = *best_solution;

        //Shaking(cur_solution, op_shake, perturbation_level);
        Shaking(cur_solution, perturbation_level);

        op_neighboor = 0;

        unsigned op_busca = 0;

        while (op_neighboor < num_neighboor && t1->getElapsedTimeInMilliSec() < alg_data.time_limit) {

            improve=false;
            *neighbor_solution = *cur_solution;

            switch (op_neighboor) {
                case 0:
                    //Explorar a solução escohida em relação a vizinhança de troca entre máquinas
                    if(op_busca==0)
                        improve=SwapInsideLSMonoFI(neighbor_solution, best_solution);
                    else
                        improve=SwapInsideLSMonoBI(neighbor_solution);
                    break;
                case 1:
                    if(op_busca==0)
                        improve=InsertInsideLSMonoFI(neighbor_solution, best_solution);
                    else
                        improve=InsertInsideLSMonoBI(neighbor_solution);
                    break;
                case 2:
                    //Explorar a solução escohida em relação a vizinhança de troca entre máquinas
                    if(op_busca==0)
                        improve=SwapOutsideLSMonoFI(neighbor_solution, best_solution);
                    else
                        improve=SwapOutsideLSMonoBI(neighbor_solution);
                    break;
                case 3:
                    if(op_busca==0)
                        improve=InsertOutsideLSMonoFI(neighbor_solution, best_solution);
                    else
                        improve=InsertOutsideLSMonoBI(neighbor_solution);
                    break;
                case 4:
                    //Explorar a solução escohida em relação a vizinhança de mudança de modo de operação
                    if(op_busca==0)
                        improve=ChangeOpModeLSMonoFI(neighbor_solution, best_solution);
                    else
                        improve=ChangeOpModeLSMonoBI(neighbor_solution);
                    break;
            }

            //Verificar se houve melhora
            if(improve){
                *best_solution = *neighbor_solution;
                //op_neighboor = 0;
            }
            else{

                //Se não houve melhora, seguir para próxima vizinhança
                op_neighboor++;

                //Se não tem próxima vizinhança, fazer a intensificação
                if(op_neighboor == num_neighboor){

                    *neighbor_solution = *cur_solution;

                    IntesificationArroyo(neighbor_solution, intensification_level);

                    best_solution->CalculeMonoObjectiveTchebycheff();

                    //Verificar se houve melhora
                    if(neighbor_solution->objective_funtion - best_solution->objective_funtion < -EPS){
                        *best_solution = *neighbor_solution;
                    }
                    else{
                        neighbor_solution->CalculateHorizonAvoidingPeak();
                        neighbor_solution->CalculateObjective();
                        neighbor_solution->CalculeMonoObjectiveTchebycheff();

                        best_solution->CalculeMonoObjectiveTchebycheff();

                        if(neighbor_solution->objective_funtion - best_solution->objective_funtion < -EPS){

                            *best_solution = *neighbor_solution;

                        }
                    }

                }
            }

            t1->stop();
        }

        index++;
        t1->stop();
    }


    unsigned index_n;

    //while (t1->getElapsedTimeInMilliSec() < alg_data.time_limit) {
    for(unsigned i=0; i<solution_neighboor.size() && t1->getElapsedTimeInMilliSec() < alg_data.time_limit;i++){
    //while (t1->getElapsedTimeInMilliSec() < alg_data.time_limit) {

        auto s_index = index%non_dominated_set.set_solution.size();

        //Escolher a próxima solução a ser explorada
        best_solution = non_dominated_set.set_solution[s_index];
        best_solution->CalculeMonoObjectiveTchebycheff();

        //cur_solution guarda a solução perturbada
        *cur_solution = *best_solution;

        //Shaking(cur_solution, op_shake, perturbation_level);
        Shaking(cur_solution, perturbation_level);

        op_neighboor = 0;

        unsigned op_busca = 0;

        while (op_neighboor < num_neighboor && t1->getElapsedTimeInMilliSec() < alg_data.time_limit) {

            improve=false;
            *neighbor_solution = *cur_solution;

            switch (op_neighboor) {
                case 0:
                    //Explorar a solução escohida em relação a vizinhança de troca entre máquinas
                    if(op_busca==0)
                        improve=SwapInsideLSMonoFI(neighbor_solution, best_solution);
                    else
                        improve=SwapInsideLSMonoBI(neighbor_solution);
                    break;
                case 1:
                    if(op_busca==0)
                        improve=InsertInsideLSMonoFI(neighbor_solution, best_solution);
                    else
                        improve=InsertInsideLSMonoBI(neighbor_solution);
                    break;
                case 2:
                    //Explorar a solução escohida em relação a vizinhança de troca entre máquinas
                    if(op_busca==0)
                        improve=SwapOutsideLSMonoFI(neighbor_solution, best_solution);
                    else
                        improve=SwapOutsideLSMonoBI(neighbor_solution);
                    break;
                case 3:
                    if(op_busca==0)
                        improve=InsertOutsideLSMonoFI(neighbor_solution, best_solution);
                    else
                        improve=InsertOutsideLSMonoBI(neighbor_solution);
                    break;
                case 4:
                    //Explorar a solução escohida em relação a vizinhança de mudança de modo de operação
                    if(op_busca==0)
                        improve=ChangeOpModeLSMonoFI(neighbor_solution, best_solution);
                    else
                        improve=ChangeOpModeLSMonoBI(neighbor_solution);
                    break;
            }

            //Verificar se houve melhora
            if(improve){
                *best_solution = *neighbor_solution;
                //op_neighboor = 0;
            }
            else{

                //Se não houve melhora, seguir para próxima vizinhança
                op_neighboor++;

                //Se não tem próxima vizinhança, fazer a intensificação
                if(op_neighboor == num_neighboor){

                    *neighbor_solution = *cur_solution;

                    IntesificationArroyo(neighbor_solution, intensification_level);

                    best_solution->CalculeMonoObjectiveTchebycheff();

                    //Verificar se houve melhora
                    if(neighbor_solution->objective_funtion - best_solution->objective_funtion < -EPS){
                        *best_solution = *neighbor_solution;
                    }
                    else{
                        neighbor_solution->CalculateHorizonAvoidingPeak();
                        neighbor_solution->CalculateObjective();
                        neighbor_solution->CalculeMonoObjectiveTchebycheff();

                        best_solution->CalculeMonoObjectiveTchebycheff();

                        if(neighbor_solution->objective_funtion - best_solution->objective_funtion < -EPS){

                            *best_solution = *neighbor_solution;

                        }
                    }

                }
            }

            t1->stop();
        }

        //for(auto *it_j:non_dominated_set.set_solution){
        //for(auto it_j = non_dominated_set.set_solution.begin(); it_j != non_dominated_set.set_solution.end(); it_j++){
        for(unsigned it_j = 0; it_j < solution_neighboor.size(); it_j++){
            auto w_j = non_dominated_set.set_solution[it_j]->weights;
            //auto obj_i = max(w_j.first*(non_dominated_set.set_solution[index_s]->makeSpan - Z_STAR::makespan),
            //                w_j.second*(non_dominated_set.set_solution[index_s]->TEC - Z_STAR::TEC));
            //auto obj_j = max(w_j.first*(non_dominated_set.set_solution[it_j]->makeSpan - Z_STAR::makespan),
            //                w_j.second*(non_dominated_set.set_solution[it_j]->TEC - Z_STAR::TEC));

            auto obj_i = w_j.first*(double(best_solution->makeSpan) / double(Z_STAR::makespan)) +
                    w_j.second*(double(best_solution->TEC) / double(Z_STAR::TEC));
            auto obj_j = w_j.first*(double(non_dominated_set.set_solution[it_j]->makeSpan) / double(Z_STAR::makespan)) +
                    w_j.second*(double(non_dominated_set.set_solution[it_j]->TEC) / double(Z_STAR::TEC));

            //if(obj_i < obj_j){
            if(obj_i - obj_j < -EPS){
                *non_dominated_set.set_solution[it_j] = *best_solution;
                non_dominated_set.set_solution[it_j]->weights = w_j;
            }
        }


        index++;
        t1->stop();
    }

    delete neighbor_solution;
    delete cur_solution;

}

/*
 * Cruzamento para gerar TAM_CROSSOVER indivíduos
 * Uso de torneio binário para selecionar os indivíduos para o cruzamento
 */
void Crossover(vector<MonoSolution*> &population, vector<MonoSolution*> &new_population, unsigned population_size)
{
    MonoSolution *parent1, *parent2, *offspring1, *offspring2;
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
        offspring1 = new MonoSolution();
        offspring2 = new MonoSolution();
        GenerateOffspring3(parent1, parent2, offspring1, offspring2);

        //Adicionar os filhos gerados à nova população
        new_population.push_back(offspring1);
        new_population.push_back(offspring2);

    }

}

/*
 * Gerar filhos usando o método do Ruiz
 */
void GenerateOffspring3(MonoSolution *parent1, MonoSolution *parent2,
                        MonoSolution *offspring1, MonoSolution *offspring2)
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

    offspring1->weights = parent1->weights;
    offspring2->weights = parent2->weights;


    //Calcular a função objetivo
    offspring1->CalculateObjective();
    offspring2->CalculateObjective();

    o1.clear();
    o2.clear();

}
