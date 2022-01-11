#include "monosolution.h"


MonoSolution::MonoSolution():LSSolution(){

}

MonoSolution::MonoSolution (const MonoSolution &s):LSSolution(s){
    this->objective_funtion = s.objective_funtion;
    this->weights = s.weights;
}

MonoSolution::~MonoSolution(){}

MonoSolution* MonoSolution::Create(){
    return new MonoSolution();
}

MonoSolution& MonoSolution::operator=(const MonoSolution &s){

    LSSolution::operator=(s);

    this->objective_funtion = s.objective_funtion;

    this->weights = s.weights;

    return *this;
}

bool MonoSolution::operator <(const MonoSolution& s){

    return LSSolution::operator <(s);
}

bool MonoSolution::operator ==(const MonoSolution& s){

    return LSSolution::operator ==(s);
}

void MonoSolution::CalculeMonoObjective(){

    this->objective_funtion = this->weights.first*this->makeSpan + this->weights.second*this->TEC;
}

void MonoSolution::CalculeMonoObjectiveTchebycheff(){

    if(this->makeSpan > Z_STAR::makespan){
        Z_STAR::makespan = this->makeSpan;
    }

    if(this->TEC > Z_STAR::TEC){
        Z_STAR::TEC = this->TEC;
    }

    //Minha função objetivo
    this->objective_funtion =
            this->weights.first*(double(this->makeSpan)/double(Z_STAR::makespan)) +
            this->weights.second*(double(this->TEC)/double(Z_STAR::TEC));

}

void MonoSolution::GenerateGreedySolutionWeigth()
{
    vector<unsigned> jobs(Instance::num_jobs+1);
    unsigned machine, position, diff_time;
    unsigned best_machine, best_position, best_diff_time, best_op_mode;
    double obj_job, best_obj_job;
    //vector<unsigned>::iterator it2;

    for(unsigned i = 0; i <= Instance::num_jobs; i++){
        jobs[i] = i;
    }

    //Gerar um vetor com números aleatórios
    srand(Instance::seed);
    random_shuffle(jobs.begin()+1, jobs.end());

    //Inserir cada tarefa através de uma estratégia gulosa
    for(auto it = jobs.begin()+1; it != jobs.end(); ++it){
        best_obj_job = INT_MAX;
        best_machine = 1;
        best_position = 0;
        best_diff_time = 0;
        best_op_mode = 1;
        //for(unsigned id_machine=1; id_machine <= Instance::num_machine ; id_machine++){
            for(unsigned id_op=1; id_op <= Instance::num_mode_op ; id_op++){
                //for(unsigned id_pos=0; id_pos <= scheduling[id_machine].size() ; id_pos++){*/
                    GreedyChoiceWeigth(*it, id_op, machine, position, diff_time, obj_job);
                    if(obj_job - best_obj_job < -EPS){
                        best_machine = machine;
                        best_position = position;
                        best_diff_time = diff_time;
                        best_obj_job = obj_job;
                        best_op_mode = id_op;
                        //it2 = it;
                    }
                //}
            }
        //}
        AddJob(*it, best_machine, best_position, best_diff_time);

        //Definir o modo de operação da nova tarefa
        job_mode_op[*it] = best_op_mode;

        //jobs.erase(it);
    }

    CalculateShorterTimeHorizon();
    CalculateObjective();
    CalculeMonoObjectiveTchebycheff();

#ifdef DEBUG
    Check();
#endif
}

void MonoSolution::GenerateGreedySolutionWeigth_Caro()
{
    vector<unsigned> jobs(Instance::num_jobs+1);
    unsigned machine, position, diff_time;
    unsigned best_machine, best_position, best_diff_time, best_op_mode;
    double obj_job, best_obj_job;
    vector<unsigned>::iterator it2;

    for(unsigned i = 0; i <= Instance::num_jobs; i++){
        jobs[i] = i;
    }

    //Gerar um vetor com números aleatórios
    srand(Instance::seed);
    random_shuffle(jobs.begin()+1, jobs.end());

    //Inserir cada tarefa através de uma estratégia gulosa
    while (jobs.size() > 1) {
        best_obj_job = INT_MAX;
        best_machine = 1;
        best_position = 0;
        best_diff_time = 0;
        best_op_mode = 1;
        for(auto it = jobs.begin()+1; it != jobs.end(); ++it){

            for(unsigned o=1; o<=Instance::num_mode_op; o++){
                obj_job = 0;
                machine = 1;
                GreedyChoiceWeigth(*it, o, machine, position, diff_time, obj_job);
                if(obj_job - best_obj_job < -EPS){
                    best_machine = machine;
                    best_position = position;
                    best_diff_time = diff_time;
                    best_obj_job = obj_job;
                    best_op_mode = o;
                    it2 = it;
                }
            }

        }

        AddJob(*it2, best_machine, best_position, best_diff_time);

        //Definir o modo de operação da nova tarefa
        job_mode_op[*it2] = best_op_mode;

        jobs.erase(it2);

    }

    //CalculateInitialTimeMin();
    CalculateShorterTimeHorizon();
    CalculateObjective();

#ifdef DEBUG
    Check();
#endif
}



/*
 * Escolher a melhor posição
 *
 */
unsigned MonoSolution::FindJobBestPosMacWeigth(unsigned new_job, unsigned machine,
                                 double &smaller_weigth_obj_machine,
                                 unsigned &diff_time_job)
{
    unsigned previos_job, best_position, decreased_time, increased_time;
    unsigned completion_time, new_makespan;
    unsigned h, setup;
    double job_tec;

    smaller_weigth_obj_machine = INT_MAX;

    //Retornar se não definiu o modo de operação de new_job
    if(job_mode_op[new_job] == 0){
        return scheduling[machine].size();
    }

    //A tarefa fictícia zero é anterior à primeira tarefa da máquina i
    previos_job = 0;

    best_position = 0;

    //A tarefa fictícia zero é anterior à primeira tarefa da máquina i
    previos_job = best_position = 0;

    //Tentar inserir antes de cada tarefa presente na máquina
    for(auto j = scheduling[machine].begin(); j != scheduling[machine].end(); ++j){

        //Tempo de processamento que será incrementado
        increased_time = ceil(double(Instance::m_processing_time[machine][new_job]) /
                               double(Instance::v_speed_factor[job_mode_op[new_job]]));

        //Tempo de preparação que será incrementado
        increased_time += Instance::m_setup_time[machine][previos_job][new_job];
        increased_time += Instance::m_setup_time[machine][new_job][*j];


        //Tempo de preparação que será reduzido
        decreased_time = Instance::m_setup_time[machine][previos_job][*j];

        //Tempo de termino da máquina i será igual ao valor atual mais
        //o custo da inserção da tarefa na posição j

        completion_time = this->machine_completion_time[machine] + increased_time - decreased_time;

        if(completion_time > this->makeSpan){
            new_makespan = completion_time;
        }
        else{
            new_makespan = this->makeSpan;
        }

        //A tarefe atual passa a ser a tarefa anterior
        previos_job = *j;

        //######################

        setup = Instance::m_setup_time[machine][previos_job][new_job];

        h = job_end_time1[previos_job] + setup;

        //Mudei
        //machine_tec = CalcPECToJob(machine, *j, h);
        job_tec = CalcPECToJob(machine, new_job, h);


        //Se o novo tempo de término da máquina i é menor
        //que o melhor tempo de termíno encontra até agora,
        //então encontrei uma posição melhor para fazer a inserção
        if((new_makespan*weights.first + job_tec*weights.second) < smaller_weigth_obj_machine){
            best_position = j - scheduling[machine].begin();
            smaller_weigth_obj_machine = (new_makespan*weights.first + job_tec*weights.second);
            diff_time_job = increased_time - decreased_time;
        }

        //A tarefa atual passa a ser a tarefa anterior
        previos_job = *j;
    }

    //---Nessa parte será calculado o custo de inserir a nova tarefa após à última---

    //Tempo de processamento que será incrementado
    increased_time = ceil(double(Instance::m_processing_time[machine][new_job]) /
                           double(Instance::v_speed_factor[job_mode_op[new_job]]));

    //Tempo de preparação que será incrementado
    increased_time += Instance::m_setup_time[machine][previos_job][new_job];

    //Tempo de termino da máquina i será igual ao valor atual mais
    //o custo da inserção da tarefa na posição j
    completion_time = this->machine_completion_time[machine] + increased_time;

    if(completion_time > this->makeSpan){
        new_makespan = completion_time;
    }
    else{
        new_makespan = this->makeSpan;
    }

    //##############
    setup = Instance::m_setup_time[machine][previos_job][new_job];

    h = job_end_time1[previos_job] + setup;

    job_tec = CalcPECToJob(machine, new_job, h);

    //Se o novo tempo de término da máquina i é menor
    //que o melhor tempo de termíno encontra até agora,
    //então encontrei uma posição melhor para fazer a inserção
    if((new_makespan*weights.first + job_tec*weights.second) < smaller_weigth_obj_machine){
        best_position = scheduling[machine].size();
        smaller_weigth_obj_machine = (new_makespan*weights.first + job_tec*weights.second);
        diff_time_job = increased_time;
    }

    return best_position;
}


/*
 * Escolher a máquina e a posição
 * O modo de operação já vem definido
 * */
void MonoSolution::GreedyChoiceWeigth(unsigned new_job, unsigned mode_op, unsigned &best_machine, unsigned &best_position, unsigned &diff_time, double &obj_job)
{
    unsigned best_position_machine, diff_time_machine;
    double new_job_weigth_obj_machine, smaller_new_job_weigth_obj;

    smaller_new_job_weigth_obj = INT_MAX;

    //Definir o modo de operação da nova tarefa
    job_mode_op[new_job] = mode_op;

    best_machine = 1;

    //Percorrer cada máquina
    for (unsigned i = 1; i <= Instance::num_machine; i++) {

        //Encontrar a melhor posição na máquina i
        best_position_machine = FindJobBestPosMacWeigth(new_job, i, new_job_weigth_obj_machine, diff_time_machine);

        //Se encontrou uma posição com menor custo
        //if(new_job_tec < smaller_new_job_tec && completion_time <= Instance::num_planning_horizon){
        if(new_job_weigth_obj_machine < smaller_new_job_weigth_obj){
            smaller_new_job_weigth_obj = new_job_weigth_obj_machine;

            best_machine = i;
            best_position = best_position_machine;

            diff_time = diff_time_machine;
        }

    }

    obj_job = smaller_new_job_weigth_obj;

}

