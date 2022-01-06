#include "solution.h"

Solution::Solution()
{
    //Criar uma lista vazia para cada máquina
    scheduling = vector<vector<unsigned>>(Instance::num_machine+1, vector<unsigned>(0));

    //Criar um vetor para armazenar o instante de início de processamento cada tarefa
    H1 = vector<unsigned>(Instance::num_jobs+1);

    //Criar um vetor para armazenar o instante de início de cada tarefa
    job_start_time1 = vector<unsigned>(Instance::num_jobs+1);

    //Criar um vetor para armazenar o instante do final de cada tarefa
    job_end_time1 = vector<unsigned>(Instance::num_jobs+1);

    //Criar um vetor para armazenar o instante de término para cada máquina
    machine_completion_time = vector<unsigned>(Instance::num_machine+1);

    //Criar um vetor para armazenar o instante de término para cada máquina
    machine_TEC = vector<double>(Instance::num_machine+1);

    //Criar um vetor para armazenar o modo de operação de cada tarefa
    job_mode_op = vector<unsigned>(Instance::num_jobs+1);

    makeSpan = 0;
    TEC = 0;

}

Solution::Solution (const Solution &s){

    //Criar uma lista vazia para cada máquina
    scheduling = vector<vector<unsigned>>(Instance::num_machine+1, vector<unsigned>(0));

    //Criar um vetor para armazenar o instante de início de processamento cada tarefa
    H1 = vector<unsigned>(Instance::num_jobs+1);

    //Criar um vetor para armazenar o instante de início de cada tarefa
    job_start_time1 = vector<unsigned>(Instance::num_jobs+1);

    //Criar um vetor para armazenar o instante do final de cada tarefa
    job_end_time1 = vector<unsigned>(Instance::num_jobs+1);

    //Criar um vetor para armazenar o instante de término para cada máquina
    machine_completion_time = vector<unsigned>(Instance::num_machine+1);

    //Criar um vetor para armazenar o instante de término para cada máquina
    machine_TEC = vector<double>(Instance::num_machine+1);

    //Criar um vetor para armazenar o modo de operação de cada tarefa
    job_mode_op = vector<unsigned>(Instance::num_jobs+1);


    this->makeSpan = s.makeSpan;
    this->TEC = s.TEC;
    this->is_optimal = s.is_optimal;

    for(unsigned i = 1; i <= Instance::num_machine; i++){

        for(unsigned j = 0; j < s.scheduling[i].size(); j++){
            this->scheduling[i].push_back(s.scheduling[i][j]);
        }
    }

    for(unsigned i = 0; i <= Instance::num_jobs; i++){

        this->H1[i] = s.H1[i];
        this->job_end_time1[i] = s.job_end_time1[i];
        this->job_start_time1[i] = s.job_start_time1[i];
        this->job_mode_op[i] = s.job_mode_op[i];
    }

    for(unsigned i = 0; i <= Instance::num_machine; i++){
        this->machine_completion_time[i] = s.machine_completion_time[i];
        this->machine_TEC[i] = s.machine_TEC[i];
    }


}

Solution::~Solution()
{

    for(unsigned i = 0; i < this->scheduling.size(); i++){
        this->scheduling[i].clear();
    }

    scheduling.clear();
    H1.clear();
    job_start_time1.clear();
    job_end_time1.clear();
    machine_completion_time.clear();
    machine_TEC.clear();
    job_mode_op.clear();
}


Solution& Solution::operator=(const Solution &s){

    unsigned size_i, size_j;

    if( &s != this ) {

        Solution* ptr_object = NULL;
        if(&s != ptr_object){
            this->makeSpan = s.makeSpan;
            this->TEC = s.TEC;
            this->is_optimal = s.is_optimal;


            size_i = this->scheduling.size();
            for(unsigned i = 0; i < size_i; i++){
                size_j = s.scheduling[i].size();
                this->scheduling[i].resize(size_j);
                for(unsigned j = 0; j < size_j; j++ ){

                    this->scheduling[i][j] = s.scheduling[i][j];

                    //job = this->scheduling[i][j];

                }

                this->machine_completion_time[i] = s.machine_completion_time[i];
                //copy(s.machine_completion_time.begin(), s.machine_completion_time.end(), back_inserter(this->machine_completion_time));
                this->machine_TEC[i] = s.machine_TEC[i];
                //copy(s.machine_TEC.begin(), s.machine_TEC.end(), back_inserter(this->machine_TEC));

            }

            for(unsigned i = 0; i <= Instance::num_jobs; i++){
                this->H1[i] = s.H1[i];
                //copy(s.H1.begin(), s.H1.end(), back_inserter(this->H1));
                this->job_end_time1[i] = s.job_end_time1[i];
                //copy(s.job_end_time1.begin(), s.job_end_time1.end(), back_inserter(this->job_end_time1));
                this->job_start_time1[i] = s.job_start_time1[i];
                //copy(s.job_start_time1.begin(), s.job_start_time1.end(), back_inserter(this->job_start_time1));
                this->job_mode_op[i] = s.job_mode_op[i];
                //copy(s.job_mode_op.begin(), s.job_mode_op.end(), back_inserter(this->job_mode_op));
            }

        }

    }
    return *this;
}

bool Solution::operator <(const Solution& s) {
        return (this->makeSpan < s.makeSpan && (this->TEC - s.TEC < -EPS))
                || (this->makeSpan < s.makeSpan && abs(this->TEC - s.TEC) < EPS) ||
                (this->makeSpan == s.makeSpan && (this->TEC - s.TEC < -EPS));
}
bool Solution::operator <=(const Solution& s) {
        return ((*this < s)
                || (this->makeSpan < s.makeSpan && this->TEC > s.TEC)
                || (this->makeSpan > s.makeSpan && this->TEC < s.TEC));
}

bool Solution::operator ==(const Solution& s) {
        return (this->makeSpan == s.makeSpan && abs(this->TEC - s.TEC) < EPS);
}

void Solution::Print()
{
    for (size_t i = 1; i <= Instance::num_machine; ++i) {
        cout << "Máquina " << i << ": ";
        for (auto it = scheduling[i].begin(); it != scheduling[i].end(); ++it) {
            cout << *it << "[" << job_mode_op[*it] << "]" << " ";
        }
        cout << " (" << machine_completion_time[i] << ") " << endl;
    }
    cout << "Makespan: " << makeSpan << endl;
    cout << "TEC: " << TEC << endl;
    cout << endl;
}

void Solution::DummyInitialSolution()
{
    size_t i = 1;

    //Inserir cada tarefa j na solução inicial
    for (size_t j = 1; j <= Instance::num_jobs; ++j) {
        //Inserir a tarefa j na máquina i
        scheduling[i].push_back(j);

        //Definir o modo de operação de cada tarefa j, aleatoriamente
        job_mode_op[j] = 1 + rand()%(Instance::num_mode_op);

        //Distribuir as tarefas entre as máquinas, incrementar i
        if(j/i >= Instance::num_jobs/Instance::num_machine){

            //Mudar de máquina
            i++;
        }
    }

    CalculateShorterTimeHorizon();
    CalculateObjective();
}

void Solution::RandomInitialSolution()
{

    vector<unsigned> jobs(Instance::num_jobs+1);

    for(unsigned i = 0; i <= Instance::num_jobs; i++){
        jobs[i] = i;
    }

    //Gerar um vetor com números aleatórios
    random_shuffle(jobs.begin()+1, jobs.end());

    //Inserir cada tarefa através de uma estratégia aleatória
    for(auto it = jobs.begin()+1; it != jobs.end(); ++it){

        //Definir o modo de operação de cada tarefa it, aleatoriamente
        job_mode_op[*it] = 1 + rand()%(Instance::num_mode_op);

        //Inserir aleatoriamente a tarefa it
        InsertRandomPosition(*it);
    }

    CalculateShorterTimeHorizon();
    CalculateObjective();

#ifdef DEBUG
    Check();
#endif

}

/*
 * Método para gerar uma solução inicial gulosa,
 * considerando o objetivo do makespan
 */
void Solution::GenerateGreedySolutionMakespan()
{
    vector<unsigned> jobs(Instance::num_jobs+1);
    unsigned machine, position, diff_time;
    unsigned best_machine, best_position, best_diff_time;
    unsigned new_makespan, smaller_makespan;
    unsigned current_makespan =0;
    vector<unsigned>::iterator it2;

    for(unsigned i = 0; i <= Instance::num_jobs; i++){
        jobs[i] = i;
    }

    //Gerar um vetor com números aleatórios
    srand(Instance::seed);
    random_shuffle(jobs.begin()+1, jobs.end());

    //Selecionar o modo de operação, mais rápido
    unsigned best_op_mode = this->GetFastestOpMode();

    //Inserir cada tarefa através de uma estratégia gulosa em todas as posições
    //Enquanto houver tarefa que não foi inserida na solução

    while (jobs.size() > 1) {
        smaller_makespan = INT_MAX;
        best_diff_time = INT_MAX;
        best_machine = 1;
        best_position = 0;
        //Calcular o custo para inserir todas as tarefas em todas as posições
        for(auto it = jobs.begin()+1; it != jobs.end(); ++it){

            GreedyChoiceMakespan(*it, best_op_mode, machine, position, diff_time, new_makespan);

            if(new_makespan > this->makeSpan){
                current_makespan = new_makespan;
            }

            if(current_makespan < smaller_makespan){
                smaller_makespan = new_makespan;
                best_machine = machine;
                best_position = position;
                best_diff_time = diff_time;
                //it2 é a tarefa que gera menor custo
                it2 = it;
            }
            else if(new_makespan == smaller_makespan){
                if(diff_time < best_diff_time){
                    best_machine = machine;
                    best_position = position;
                    best_diff_time = diff_time;
                    //it2 é a tarefa que gera menor custo
                    it2 = it;
                }
            }
        }

        //Inserir a tarefa que gere menor custo
        AddJob(*it2, best_machine, best_position, best_diff_time);

        //Remover it2, pois ela já foi inserida na solução
        jobs.erase(it2);

    }

    CalculateShorterTimeHorizon();
    CalculateObjective();

#ifdef DEBUG
    Check();
#endif
}


/*
 * Método para gerar uma solução inicial gulosa,
 * considerando o objetivo do TEC
 */
void Solution::GenerateGreedySolutionTEC3()
{
    vector<unsigned> jobs(Instance::num_jobs+1);
    unsigned machine, position, diff_time;
    unsigned best_machine, best_position, best_diff_time;
    double pec_job, best_pec_job;
    vector<unsigned>::iterator it2;

    for(unsigned i = 0; i <= Instance::num_jobs; i++){
        jobs[i] = i;
    }

    //Gerar um vetor com números aleatórios
    srand(Instance::seed);
    random_shuffle(jobs.begin()+1, jobs.end());

    //Selecionar o modo de operação que consome menos energia
    unsigned best_op_mode = GetCheapestOpMode();

    //Inserir cada tarefa através de uma estratégia gulosa
    while (jobs.size() > 1) {
        best_pec_job = INT_MAX;
        best_machine = 1;
        best_position = 0;
        for(auto it = jobs.begin()+1; it != jobs.end(); ++it){

            GreedyChoiceTEC(*it, best_op_mode, machine, position, diff_time, pec_job);
            if(pec_job < best_pec_job){
                best_machine = machine;
                best_position = position;
                best_diff_time = diff_time;
                best_pec_job = pec_job;
                it2 = it;
            }
        }

        AddJob(*it2, best_machine, best_position, best_diff_time);

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
 * Método para calcular o instante inicial para cada tarefa
 * considerando um tempo mínimo entre as tarefas
 */
void Solution::CalculateShorterTimeHorizon(bool update_h)
{
    unsigned start;
    unsigned last_job, l;
    unsigned processing_time, setup_time, previous_job;

    for(size_t i = 1; i <= Instance::num_machine; ++i){

        previous_job = 0;
        start = 0;

        //Percorrer as tarefas de cada máquina
        for(auto j = scheduling[i].begin(); j != scheduling[i].end(); ++j){

            job_start_time1[*j] = start;

            //Calcular o tempo de preparação da tarefa j
            setup_time = Instance::m_setup_time[i][previous_job][(*j)];

            if(update_h){
                //Armazenar o instante de início da tarefa j
                H1[*j] = job_start_time1[*j] + setup_time;
            }

            //Tempo de processamento da tarefa j
            l = job_mode_op[*j];
            processing_time = ceil(double(Instance::m_processing_time[i][*j])/
                                double(Instance::v_speed_factor[l]));


            //Armazenar o instante de término da tarefa
            job_end_time1[*j] = H1[*j] + processing_time;

            //Atualizar a tarefa anterior
            previous_job = *j;

            //Atualizar o instante de início da próxima tarefa
            start = job_end_time1[*j];

        }

        //Calcular o instante de conclusão de cada máquina
        if(scheduling[i].size() > 0){

            last_job = scheduling[i].back();

            machine_completion_time[i] = job_end_time1[last_job];
        }
        else{
            machine_completion_time[i] = 0;
        }

    }
}


/*
 * Método para calcular o instante inicial para cada tarefa
 * considerando um tempo mínimo entre as tarefas
 */
void Solution::CalculateShorterTimeHorizonMachine(unsigned machine)
{
    unsigned start;
    unsigned last_job, l;
    unsigned processing_time, setup_time, previous_job;

    previous_job = 0;
    start = 0;

    //Percorrer as tarefas de cada máquina
    for(auto j = scheduling[machine].begin(); j != scheduling[machine].end(); ++j){

        job_start_time1[*j] = start;

        //Tempo de preparação da tarefa j
        setup_time = Instance::m_setup_time[machine][previous_job][(*j)];

        //Armazenar o tempo de início da tarefa j
        H1[*j] = job_start_time1[*j] + setup_time;

        //Tempo de processamento da tarefa j
        l = job_mode_op[*j];
        processing_time = ceil(double(Instance::m_processing_time[machine][*j])/
                double(Instance::v_speed_factor[l]));

        //Identificar a tarefa anterior
        previous_job = *j;

        job_end_time1[*j] = H1[*j] + processing_time;

        //A próxima tarefa começa após a tarefa atual
        start = job_end_time1[*j];

    }

    //Calcular o instante de conclusão de cada máquina
    if(scheduling[machine].size() > 0){

        last_job = scheduling[machine].back();

        machine_completion_time[machine] = job_end_time1[last_job];
    }
    else{
        machine_completion_time[machine] = 0;
    }


}

/*
 * Método para calcular o instante inicial para cada tarefa
 * considerando a alocação atual e evitando o horário de pico
 */
void Solution::CalculateHorizonAvoidingPeak()
{

    unsigned previous_job, setup_time, processing_time, l, total_time_job;

    for(size_t i = 1; i <= Instance::num_machine; ++i){

        machine_completion_time[i] = 0;
        previous_job = 0;

        //Percorrer as tarefas de cada máquina
        for(auto j = scheduling[i].begin(); j != scheduling[i].end(); ++j){

            //Tempo de preparação da tarefa j
            setup_time = Instance::m_setup_time[i][previous_job][(*j)];

            //Tempo de processamento da tarefa j
            l = job_mode_op[*j];
            processing_time = ceil(double(Instance::m_processing_time[i][*j])/
                    double(Instance::v_speed_factor[l]));

            total_time_job = setup_time + processing_time;

            unsigned end_day = floor(job_end_time1[*j]/(double)Instance::num_planning_horizon);

            //Condição para evitar o horário de pico
            if(job_end_time1[previous_job] < Instance::v_peak_start[end_day]
                    && job_end_time1[previous_job] + total_time_job > Instance::v_peak_start[end_day]){
                //Atualizar o instante de início
                job_start_time1[*j] = max(Instance::v_peak_end[end_day] - setup_time, job_end_time1[previous_job]);
                H1[*j] = job_start_time1[*j] + setup_time;
                job_end_time1[*j] = H1[*j] + processing_time;
            }
            else{
                //Atualizar o instante de início
                job_start_time1[*j] = job_end_time1[previous_job];
                H1[*j] = job_start_time1[*j] + setup_time;
                job_end_time1[*j] = H1[*j] + processing_time;
            }

            //A tarefa j passa a ser a tarefa anterior
            previous_job = *j;

        }

        //Atualizar o tempo de término da máquina
        machine_completion_time[i] = job_end_time1[previous_job];

    }
}


/*
 * Método para calcular o instante inicial para cada tarefa
 * considerando a alocação atual e evitando o horário de pico
 */
void Solution::CalculateHorizonAvoidingPeakMachine(unsigned machine)
{

    unsigned previous_job, setup_time, processing_time, l, total_time_job;

    //for(size_t i = 1; i <= Instance::num_machine; ++i){

        machine_completion_time[machine] = 0;
        previous_job = 0;

        //Percorrer as tarefas de cada máquina
        for(auto j = scheduling[machine].begin(); j != scheduling[machine].end(); ++j){

            //Tempo de preparação da tarefa j
            setup_time = Instance::m_setup_time[machine][previous_job][(*j)];

            //Tempo de processamento da tarefa j
            l = job_mode_op[*j];
            processing_time = ceil(double(Instance::m_processing_time[machine][*j])/
                    double(Instance::v_speed_factor[l]));

            total_time_job = setup_time + processing_time;

            unsigned end_day = floor(job_end_time1[*j]/(double)Instance::num_planning_horizon);

            //Condição para evitar o horário de pico
            if(job_end_time1[previous_job] < Instance::v_peak_start[end_day]
                    && job_end_time1[previous_job] + total_time_job > Instance::v_peak_start[end_day]){
                //Atualizar o instante de início
                job_start_time1[*j] = max(Instance::v_peak_end[end_day] - setup_time, job_end_time1[previous_job]);
                H1[*j] = job_start_time1[*j] + setup_time;
                job_end_time1[*j] = H1[*j] + processing_time;
            }
            else{
                //Atualizar o instante de início
                job_start_time1[*j] = job_end_time1[previous_job];
                H1[*j] = job_start_time1[*j] + setup_time;
                job_end_time1[*j] = H1[*j] + processing_time;
            }

            //A tarefa j passa a ser a tarefa anterior
            previous_job = *j;

        }

        //Atualizar o tempo de término da máquina
        machine_completion_time[machine] = job_end_time1[previous_job];

    //}
}

void Solution::CalculateObjective()
{
    //Calcular o makespan
    makeSpan = 0;
    unsigned size;
    for (unsigned i = 1; i <= Instance::num_machine; ++i) {
        machine_completion_time[i] = 0;
        size = scheduling[i].size();
        if(size){
            unsigned last_job = scheduling[i][size-1];
            machine_completion_time[i] = job_end_time1[last_job];
            if(machine_completion_time[i] > makeSpan){
                makeSpan = machine_completion_time[i];
            }
        }
    }

    //Calcular o custo total de energia

    this->TEC = 0;

    for(unsigned i = 1; i <= Instance::num_machine; ++i){

        this->machine_TEC[i] = 0;

        for(auto j = scheduling[i].begin(); j != scheduling[i].end(); ++j){

            this->machine_TEC[i] += CalcPECToJob(i, *j, H1[*j]);

        }

        this->TEC += this->machine_TEC[i];
    }
}

void Solution::CalculateObjectiveDiscrete()
{
    //Calcular o makespan
    makeSpan = 0;
    unsigned size;
    for (unsigned i = 1; i <= Instance::num_machine; ++i) {
        machine_completion_time[i] = 0;
        size = scheduling[i].size();
        if(size){
            unsigned last_job = scheduling[i][size-1];
            machine_completion_time[i] = job_end_time1[last_job];
            if(machine_completion_time[i] > makeSpan){
                makeSpan = machine_completion_time[i];
            }
        }
    }

    //Calcular o custo total de energia

    this->TEC = 0;

    for(unsigned i = 1; i <= Instance::num_machine; ++i){

        this->machine_TEC[i] = 0;

        for(auto j = scheduling[i].begin(); j != scheduling[i].end(); ++j){

            this->machine_TEC[i] += CalcPECToJob(i, *j, H1[*j], true);

        }

        this->TEC += this->machine_TEC[i];
    }
}

void Solution::CalculateObjectiveMachine(unsigned machine)
{

    //Calcular o makespan
    makeSpan = 0;
    unsigned size;
    for (unsigned i = 1; i <= Instance::num_machine; ++i) {
        machine_completion_time[i] = 0;
        size = scheduling[i].size();
        if(size){
            unsigned last_job = scheduling[i][size-1];
            machine_completion_time[i] = job_end_time1[last_job];
            if(machine_completion_time[i] > makeSpan){
                makeSpan = machine_completion_time[i];
            }
        }
    }


    //Calcular o custo total de energia

    this->TEC -= this->machine_TEC[machine];

    this->machine_TEC[machine] = 0;

    for(auto j = scheduling[machine].begin(); j != scheduling[machine].end(); ++j){

        this->machine_TEC[machine] += CalcPECToJob(machine, *j, H1[*j]);

    }

    this->TEC += this->machine_TEC[machine];
}

/*
 * Calcular o custo de energia para a tarefa que está em position
 * na máquina machine, começando a ser preparado em start e termina
 * em end, após a execução
 */
double Solution::CalcPECToJob(unsigned machine, unsigned job, unsigned h, bool discrete_time)
{
    double pt, end, time_job_on, time_job_off;

    if(Instance::discretization_factor != 1 || discrete_time)
        pt = ceil(double(Instance::m_processing_time[machine][job])
                /double(Instance::v_speed_factor[job_mode_op[job]]));
    else
        pt = double(Instance::m_processing_time[machine][job])
                /double(Instance::v_speed_factor[job_mode_op[job]]);

    end = h + pt;

    unsigned h_day = floor(double(h)/double(Instance::num_planning_horizon));

    //Se a tarefa começa antes do pico
    if(h < Instance::v_peak_start[h_day]){

        //Se a tarefa termina antes do pico
        if(end < Instance::v_peak_start[h_day]){

            time_job_off = pt;

            time_job_on = 0;
        }
        //Se a tarefa termina dentro do pico
        else if(end < Instance::v_peak_end[h_day]){

            time_job_off = Instance::v_peak_start[h_day] - h;

            time_job_on = end - Instance::v_peak_start[h_day];

        }
        else{

            time_job_off = Instance::v_peak_start[h_day] - h;
            time_job_off += end - Instance::v_peak_end[h_day];

            time_job_on = Instance::v_peak_end[h_day] - Instance::v_peak_start[h_day];

        }
    }
    //Se a tarefa começa dentro do horário de pico
    else if(h < Instance::v_peak_end[h_day]){
        //Se a tarefa termina dentro do pico
        if(end < Instance::v_peak_end[h_day]){
            time_job_off = 0;

            time_job_on = pt;
        }
        else{
            time_job_on = Instance::v_peak_end[h_day] - h;

            time_job_off = end - Instance::v_peak_end[h_day];
        }
    }
    else{
        time_job_off = pt;

        time_job_on = 0;
    }

    double PEC, PEC_on, PEC_off;

    PEC_off = (double(time_job_off*Instance::rate_off_peak
                      *Instance::v_consumption_factor[job_mode_op[job]]
                        *Instance::v_machine_potency[machine])/double(60));
    PEC_on = (double(time_job_on*Instance::rate_on_peak*Instance::v_consumption_factor[job_mode_op[job]]
                *Instance::v_machine_potency[machine])/double(60));

    PEC = PEC_off + PEC_on;

    return PEC;

}

void Solution::Check()
{
    unsigned job, previous, end_job, start_job;
    unsigned ctm, time_job, makespan_calc;
    double tec_calc;

    makespan_calc = 0;
    tec_calc = 0;

    for (unsigned i = 1; i <= Instance::num_machine; i++) {
        previous = 0;
        start_job = 0;
        ctm = 0;
        for (unsigned j = 0; j < scheduling[i].size(); j++) {

            //Selecionar a tarefa atual
            job = scheduling[i][j];

            //Se o tempo calculado for diferente do valor armazenado, existe um erro
            if(job_start_time1[job] < start_job){
                cout << "=========Erro==============" << endl;
                cout << "Inicio preparação da tarefa " << job << ", na máquina " << i << ", valor esperado: "
                            << start_job << " armazenado: " << job_start_time1[job] << endl;

            }

            //Calcular o tempo de término da tarefa atual
            time_job = ceil(double(Instance::m_processing_time[i][job])
                            / double(Instance::v_speed_factor[job_mode_op[job]]));

            //Se o tempo calculado for diferente do valor armazenado, existe um erro
            if(H1[job] != job_start_time1[job] + Instance::m_setup_time[i][previous][job]){
                cout << "=========Erro==============" << endl;
                cout << "Inicio execução da tarefa " << job << ", na máquina " << i << ", valor esperado: "
                            << job_start_time1[job] + Instance::m_setup_time[i][previous][job]
                               << " armazenado: " << H1[job] << endl;

            }

            end_job = job_start_time1[job] + time_job + Instance::m_setup_time[i][previous][job];

            //Se o tempo calculado for diferente do valor armazenado, existe um erro
            if(job_end_time1[job] != end_job){
                cout << "=========Erro==============" << endl;
                cout << "Fim Execução  da tarefa " << job << ", na máquina " << i << ", valor esperado: "
                            << end_job << " armazenado: " << job_end_time1[job] << endl;

            }

            ctm = end_job;

            tec_calc += CalcPECToJob(i, job, H1[job]);

            //Atualizar a informação da tarefa anterior e do instante de início
            previous = job;
            start_job = end_job;

        }

        if(ctm > makespan_calc){
            makespan_calc = ctm;
        }

        if(ctm != machine_completion_time[i]){
            cout << "=========Erro==============" << endl;
            cout << "Tempo de termino da máquina " << i << endl;
        }


    }

    if(makespan_calc != makeSpan){
        cout << "=========Erro==============" << endl;
        cout << "Makespan, valor esperado: " << makespan_calc << " armazenado: " << makeSpan << endl;
    }

    if((tec_calc - TEC > 0.0001)){
        cout << "=========Erro==============" << endl;
        cout << "Custo de energia, valor esperado: " << tec_calc << " armazenado: " << TEC << endl;
    }

    for (unsigned i = 1; i <= Instance::num_jobs; i++) {
        //Se o tempo calculado for diferente do valor armazenado, existe um erro
        if(job_mode_op[i] < 1 || job_mode_op[i] > Instance::num_mode_op){
            cout << "=========Erro==============" << endl;
            cout << "Tarefa " << i << " com modo de operação " << job_mode_op[i] << " errado" << endl;

        }
    }
}

unsigned Solution::FindJobBestPosMacMakespan(unsigned new_job, unsigned machine)
{

    unsigned previos_job, best_position, decreased_time, increased_time;
    unsigned completion_time, smaller_completion_time;

    smaller_completion_time = INT_MAX;

    //Retornar se não definiu o modo de operação de new_job
    if(job_mode_op[new_job] == 0){
        return scheduling[machine].size();
    }

    //A tarefa fictícia zero é anterior à primeira tarefa da máquina i
    previos_job = 0;

    best_position = 0;

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

        //Se o novo tempo de término da máquina i é menor
        //que o melhor tempo de termíno encontra até agora,
        //então encontrei uma posição melhor para fazer a inserção
        if(completion_time < smaller_completion_time){
            smaller_completion_time = completion_time;
            best_position = j - scheduling[machine].begin();
        }

        //A tarefe atual passa a ser a tarefa anterior
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

    //Se o novo tempo de término da máquina i é menor
    //que o melhor tempo de termíno encontra até agora,
    //então encontrei uma posição melhor para fazer a inserção
    if(completion_time < smaller_completion_time){
        smaller_completion_time = completion_time;
        best_position = scheduling[machine].size();
    }

    return best_position;
}


/*
 * Método para encontrar a melhor posição para new_job em machine
 * considerando a minimização do custo de energia
 */
unsigned Solution::FindJobBestPosMacTEC3(unsigned new_job, unsigned machine)
{

    unsigned best_position, h, setup, previos_job, smaller_add_job_tec;
    double machine_tec;

    //A tarefa fictícia zero é anterior à primeira tarefa da máquina i
    previos_job = best_position = 0;

    smaller_add_job_tec = INT_MAX;

    //Tentar inserir antes de cada tarefa presente na máquina
    for(auto j = scheduling[machine].begin(); j != scheduling[machine].end(); ++j){

        setup = Instance::m_setup_time[machine][previos_job][new_job];

        h = job_end_time1[previos_job] + setup;

        machine_tec = CalcPECToJob(machine, *j, h);

        if(machine_tec < smaller_add_job_tec){
            smaller_add_job_tec = machine_tec;
            best_position = j - scheduling[machine].begin();
        }

        //A tarefa atual passa a ser a tarefa anterior
        previos_job = *j;

    }

    return best_position;

}


void Solution::SelectBestModeOpJob(unsigned machine, unsigned position, unsigned new_mode_op)
{
    unsigned current_mode, job;
    unsigned pt_old, pt_new;
    double diff_t;

    job = scheduling[machine][position];

    current_mode = job_mode_op[job];

    pt_new = ceil(double(Instance::m_processing_time[machine][job])/
            double(Instance::v_speed_factor[new_mode_op]));
    pt_old = ceil(double(Instance::m_processing_time[machine][job])/
                        double(Instance::v_speed_factor[current_mode]));

    diff_t = (pt_new - pt_old);


    if(machine_completion_time[machine] + diff_t <= Instance::num_planning_horizon){
        unsigned start = H1[job];
        unsigned previos;
        if(position > 0){
            previos = position-1;
        }
        else{
            previos = 0;
        }
        for(unsigned j = position; j < scheduling.size(); j++){
            job_start_time1[j] = start;
            H1[j] = Instance::m_setup_time[machine][previos][j];
            job_end_time1[j] = H1[j] + Instance::m_processing_time[machine][j];

            previos = j;
        }

        job_mode_op[job] = new_mode_op;

        machine_completion_time[machine] += diff_t;
    }

}

/*
 * Inserir uma nova tarefa em uma máquina específica da solução atual,
 * escolher a posição que implique em menor tempo de término nessa máquina
 * Escolher a posição que gere menor diferença de tempo de preparação,
 * uma vez que o tempo de processamento é constante
 */
void Solution::AddJobGreedyMakespanMachine(unsigned machine, unsigned new_job, unsigned mode_op)
{

    unsigned previos_job, best_position, decreased_time, increased_time;
    int difference, smaller_difference;
    smaller_difference = INT_MAX;

    //A tarefa fictícia zero é anterior à primeira tarefa da máquina
    previos_job = 0;

    //Definir o modo de operação para a nova tarefa
    job_mode_op[new_job] = mode_op;

    //Tentar inserir antes de cada tarefa presente na máquina
    for(auto j = scheduling[machine].begin(); j != scheduling[machine].end(); ++j){

        //Tempo de preparação que será reduzido
        decreased_time = Instance::m_setup_time[machine][previos_job][*j];

        //Tempo de preparação que será incrementado
        increased_time = Instance::m_setup_time[machine][previos_job][new_job];
        increased_time += Instance::m_setup_time[machine][new_job][*j];

        //Diferença de tempo para inserção na posição j
        difference = increased_time - decreased_time;

        //Se a diferença é menor que a menor encontrada
        if(difference < smaller_difference){
            smaller_difference = difference;
            best_position = j - scheduling[machine].begin();
        }

        //Atualizar a tarefa anterior
        previos_job = *j;

    }

    //Tempo de preparação que será incrementado
    increased_time = Instance::m_setup_time[machine][previos_job][new_job];

    difference = increased_time;

    if(difference < smaller_difference){
        smaller_difference = difference;
        best_position = scheduling[machine].size();
    }

    //Inserir a nova tarefa na última posição
    scheduling[machine].push_back(new_job);

    //Deslocar as tarefas que estão após da melhor posição
    for (unsigned j = scheduling[machine].size()-1; j > best_position; j--) {
        scheduling[machine][j] = scheduling[machine][j-1];
    }

    //Alocar a nova tarefa na melhor posição
    scheduling[machine][best_position] = new_job;

}


/*
 * Inserir uma nova tarefa em uma máquina da solução atual,
 * escolher a posição que implique em menor custo total de energia nessa máquina
 */
void Solution::AddJobGreedyTECMachine3(unsigned machine, unsigned new_job, unsigned mode_op)
{

    unsigned best_position, completion_time, difference;
    unsigned previos_job, next_job;


    //Definir o modo de operação da nova tarefa
    job_mode_op[new_job] = mode_op;

    //Percorrer cada máquina
    //for (unsigned i = 1; i <= Instance::num_machine; i++) {

        //Encontrar a melhor posição na máquina i
        best_position = FindJobBestPosMacTEC3(new_job, machine);

        //Identificar a tarefa anterior
        if(best_position == 0){
            previos_job = 0;
        }
        else{
            previos_job = scheduling[machine][best_position-1];
        }

        //Identificar a tarefa seguinte
        if(best_position < scheduling[machine].size()){
            next_job = scheduling[machine][best_position];
        }
        else{
            next_job = 0;
        }

        //Tempo de processamento de new_job que será incrementado
        difference = ceil(double(Instance::m_processing_time[machine][new_job]) /
                               double(Instance::v_speed_factor[job_mode_op[new_job]]));

        //Tempo de preparação antes de new_job que será incrementado
        difference += Instance::m_setup_time[machine][previos_job][new_job];

        if(next_job > 0){
            //Tempo de preparação após new_job que será incrementado
            difference += Instance::m_setup_time[machine][new_job][next_job];

            //Tempo de preparação entre previos_job e next_job que será reduzido
            difference -= Instance::m_setup_time[machine][previos_job][next_job];
        }

        //Tempo de termino da máquina i será igual ao valor atual mais
        //o custo da inserção da tarefa na posição j
        completion_time = this->machine_completion_time[machine] + difference;

    //}

    //Inserir a nova tarefa na última posição
    scheduling[machine].push_back(new_job);

    //Deslocar as tarefas que estão após da melhor posição
    for (unsigned j = scheduling[machine].size()-1; j > best_position; j--) {
        scheduling[machine][j] = scheduling[machine][j-1];
    }

    //Alocar a nova tarefa na melhor posição identificada
    scheduling[machine][best_position] = new_job;

    this->machine_completion_time[machine] = completion_time;

    auto start = 0;
    previos_job = 0;
    for(auto it = scheduling[machine].begin(); it != scheduling[machine].end(); ++it){

        job_start_time1[*it] = start;
        H1[*it] = job_start_time1[*it] + Instance::m_setup_time[machine][previos_job][*it];
        job_end_time1[*it] = H1[*it] + ceil(double(Instance::m_processing_time[machine][*it]) /
                                            double(Instance::v_speed_factor[job_mode_op[*it]]));

        start = job_end_time1[*it];
        previos_job = *it;
    }


}

/*
 * Inserir uma nova tarefa em todas as posições entre todas as máquinas
 * Escolher a posição e a máquina que gere menor makespan
 * e como critério de desempate, o menor tempo de término na máquina
 */
void Solution::AddJobGreedyMakespan(unsigned new_job, unsigned mode_op)
{
    unsigned best_position_machine, best_position, completion_time, difference;
    unsigned previos_job, next_job, best_machine;
    unsigned smaller_completion_time = INT_MAX;

    //Definir o modo de operação da nova tarefa
    job_mode_op[new_job] = mode_op;

    //Percorrer cada máquina
    for (unsigned i = 1; i <= Instance::num_machine; i++) {

        //Encontrar a melhor posição na máquina i
        best_position_machine = FindJobBestPosMacMakespan(new_job, i);

        //Identificar a tarefa anterior
        if(best_position_machine == 0){
            previos_job = 0;
        }
        else{
            previos_job = scheduling[i][best_position_machine-1];
        }

        //Identificar a tarefa seguinte
        if(best_position_machine < scheduling[i].size()){
            next_job = scheduling[i][best_position_machine];
        }
        else{
            next_job = 0;
        }

        //Tempo de processamento de new_job que será incrementado
        difference = ceil(double(Instance::m_processing_time[i][new_job]) /
                               double(Instance::v_speed_factor[job_mode_op[new_job]]));

        //Tempo de preparação antes de new_job que será incrementado
        difference += Instance::m_setup_time[i][previos_job][new_job];

        if(next_job > 0){
            //Tempo de preparação após new_job que será incrementado
            difference += Instance::m_setup_time[i][new_job][next_job];

            //Tempo de preparação entre previos_job e next_job que será reduzido
            difference -= Instance::m_setup_time[i][previos_job][next_job];
        }

        //Tempo de termino da máquina i será igual ao valor atual mais
        //o custo da inserção da tarefa na posição j
        completion_time = this->machine_completion_time[i] + difference;

        if(completion_time < smaller_completion_time){
            smaller_completion_time = completion_time;
            best_machine = i;
            best_position = best_position_machine;
        }

    }

    //Inserir a nova tarefa na última posição
    scheduling[best_machine].push_back(new_job);

    //Deslocar as tarefas que estão após da melhor posição
    for (unsigned j = scheduling[best_machine].size()-1; j > best_position; j--) {
        scheduling[best_machine][j] = scheduling[best_machine][j-1];
    }

    //Alocar a nova tarefa na melhor posição identificada
    scheduling[best_machine][best_position] = new_job;

    this->machine_completion_time[best_machine] = smaller_completion_time;
}


/*
 * Escolher a posição e a máquina que gere menor makespan
 * e como critério de desempate, o menor tempo de término na máquina
 */
void Solution::GreedyChoiceMakespan(unsigned new_job, unsigned mode_op, unsigned &best_machine,
                                    unsigned &best_position, unsigned &diff_time, unsigned &new_makespan)
{
    unsigned best_position_machine, current_completion_time, difference;
    unsigned previos_job, next_job;
    unsigned smaller_completion_time = INT_MAX;
    unsigned smaller_makespan = INT_MAX;
    unsigned current_makespan =0;

    //Definir o modo de operação da nova tarefa
    job_mode_op[new_job] = mode_op;

    //Percorrer cada máquina
    for (unsigned i = 1; i <= Instance::num_machine; i++) {

        //Encontrar a melhor posição na máquina i
        best_position_machine = FindJobBestPosMacMakespan(new_job, i);

        //Identificar a tarefa anterior
        if(best_position_machine == 0){
            previos_job = 0;
        }
        else{
            previos_job = scheduling[i][best_position_machine-1];
        }

        //Identificar a tarefa seguinte
        if(best_position_machine < scheduling[i].size()){
            next_job = scheduling[i][best_position_machine];
        }
        else{
            next_job = 0;
        }

        //Tempo de processamento de new_job que será incrementado
        difference = ceil(double(Instance::m_processing_time[i][new_job]) /
                               double(Instance::v_speed_factor[job_mode_op[new_job]]));

        //Tempo de preparação antes de new_job que será incrementado
        difference += Instance::m_setup_time[i][previos_job][new_job];

        if(next_job > 0){
            //Tempo de preparação após new_job que será incrementado
            difference += Instance::m_setup_time[i][new_job][next_job];

            //Tempo de preparação entre previos_job e next_job que será reduzido
            difference -= Instance::m_setup_time[i][previos_job][next_job];
        }

        //Tempo de termino da máquina i será igual ao valor atual mais
        //o custo da inserção da tarefa na posição j
        current_completion_time = this->machine_completion_time[i] + difference;

        if(current_completion_time > this->makeSpan){
            current_makespan = current_completion_time;
        }

        if(current_makespan < smaller_makespan){
            smaller_makespan = current_makespan;
            smaller_completion_time = current_completion_time;
            diff_time = difference;
            best_machine = i;
            best_position = best_position_machine;
        }
        else if(current_makespan == smaller_makespan){
            if(current_completion_time < smaller_completion_time){
                smaller_completion_time = current_completion_time;
                diff_time = difference;
                best_machine = i;
                best_position = best_position_machine;

            }
        }

    }

    new_makespan = smaller_makespan;
}


/*
 * Escolher aleatoriamente a máquina e a posição
 */
void Solution::RandomChoiceMakespan(unsigned new_job, unsigned mode_op, unsigned &best_machine, unsigned &best_position, unsigned &diff_time)
{

    unsigned best_position_machine, completion_time, difference;
    unsigned previos_job, next_job;
    unsigned smaller_completion_time = INT_MAX;

    //Definir o modo de operação da nova tarefa
    job_mode_op[new_job] = mode_op;

    int i = 1 + rand()%Instance::num_machine;
    if(scheduling[i].size() > 0)
        best_position_machine = rand()%scheduling[i].size();
    else
        best_position_machine = 0;

    //Identificar a tarefa anterior
    if(best_position_machine == 0){
        previos_job = 0;
    }
    else{
        previos_job = scheduling[i][best_position_machine-1];
    }

    //Identificar a tarefa seguinte
    if(best_position_machine < scheduling[i].size()){
        next_job = scheduling[i][best_position_machine];
    }
    else{
        next_job = 0;
    }

    //Tempo de processamento de new_job que será incrementado
    difference = ceil(double(Instance::m_processing_time[i][new_job]) /
                           double(Instance::v_speed_factor[job_mode_op[new_job]]));

    //Tempo de preparação antes de new_job que será incrementado
    difference += Instance::m_setup_time[i][previos_job][new_job];

    if(next_job > 0){
        //Tempo de preparação após new_job que será incrementado
        difference += Instance::m_setup_time[i][new_job][next_job];

        //Tempo de preparação entre previos_job e next_job que será reduzido
        difference -= Instance::m_setup_time[i][previos_job][next_job];
    }

    //Tempo de termino da máquina i será igual ao valor atual mais
    //o custo da inserção da tarefa na posição j
    completion_time = this->machine_completion_time[i] + difference;

    if(completion_time < smaller_completion_time){
        smaller_completion_time = completion_time;
        diff_time = difference;
        best_machine = i;
        best_position = best_position_machine;

    }

}


/*
 * Escolher a posição e a máquina que gere menor TEC
 * e como critério de desempate, o menor tempo de término na máquina
 */
void Solution::GreedyChoiceTEC(unsigned new_job, unsigned mode_op, unsigned &best_machine,
                               unsigned &best_position, unsigned &diff_time, double &pec_job)
{

    unsigned best_position_machine, difference;
    unsigned previos_job, next_job, h, smaller_completion_time;
    double new_job_tec, smaller_new_job_tec;

    smaller_new_job_tec = smaller_completion_time = INT_MAX;

    //Definir o modo de operação da nova tarefa
    job_mode_op[new_job] = mode_op;

    best_machine = Instance::num_machine+1;

    //Percorrer cada máquina
    for (unsigned i = 1; i <= Instance::num_machine; i++) {

        //Encontrar a melhor posição na máquina i
        best_position_machine = FindJobBestPosMacTEC3(new_job, i);

        //Identificar a tarefa anterior
        if(best_position_machine == 0){
            previos_job = 0;
        }
        else{
            previos_job = scheduling[i][best_position_machine-1];
        }

        //Identificar a tarefa seguinte
        if(best_position_machine < scheduling[i].size()){
            next_job = scheduling[i][best_position_machine];
        }
        else{
            next_job = 0;
        }

        //Tempo de processamento de new_job que será incrementado
        difference = ceil(double(Instance::m_processing_time[i][new_job]) /
                               double(Instance::v_speed_factor[job_mode_op[new_job]]));

        //Tempo de preparação antes de new_job que será incrementado
        difference += Instance::m_setup_time[i][previos_job][new_job];

        if(next_job > 0){
            //Tempo de preparação após new_job que será incrementado
            difference += Instance::m_setup_time[i][new_job][next_job];

            //Tempo de preparação entre previos_job e next_job que será reduzido
            difference -= Instance::m_setup_time[i][previos_job][next_job];
        }

        //Tempo de termino da máquina i será igual ao valor atual mais
        //o custo da inserção da tarefa na posição j
        //completion_time = this->machine_completion_time[i] + difference;

        h = job_end_time1[previos_job];
        new_job_tec = CalcPECToJob(i, new_job, h);

        //Se encontrou uma posição com menor custo
        //if(new_job_tec < smaller_new_job_tec && completion_time <= Instance::num_planning_horizon){
        if(new_job_tec < smaller_new_job_tec){
            smaller_new_job_tec = new_job_tec;
            best_machine = i;
            best_position = best_position_machine;
            pec_job = new_job_tec;
            diff_time = difference;
            //smaller_completion_time = completion_time;
        }

    }

}


/*
 * Escolher aleatoriamente a posição e a máquina que gere menor TEC
 */
void Solution::RandomChoiceTEC(unsigned new_job, unsigned mode_op, unsigned &best_machine, unsigned &best_position, unsigned &diff_time, double &pec_job)
{

    unsigned best_position_machine, difference;
    unsigned previos_job, next_job, h, smaller_completion_time;
    double new_job_tec, smaller_new_job_tec;

    smaller_new_job_tec = smaller_completion_time = INT_MAX;

    //Definir o modo de operação da nova tarefa
    job_mode_op[new_job] = mode_op;

    best_machine = Instance::num_machine+1;

    //Percorrer cada máquina
    //for (unsigned i = 1; i <= Instance::num_machine; i++) {

        int i = 1 + rand()%Instance::num_machine;
        if(scheduling[i].size() > 0)
            best_position_machine = rand()%scheduling[i].size();
        else
            best_position_machine = 0;

        //Encontrar a melhor posição na máquina i
        //best_position_machine = FindJobBestPosMacTEC3(new_job, i);

        //Identificar a tarefa anterior
        if(best_position_machine == 0){
            previos_job = 0;
        }
        else{
            previos_job = scheduling[i][best_position_machine-1];
        }

        //Identificar a tarefa seguinte
        if(best_position_machine < scheduling[i].size()){
            next_job = scheduling[i][best_position_machine];
        }
        else{
            next_job = 0;
        }

        //Tempo de processamento de new_job que será incrementado
        difference = ceil(double(Instance::m_processing_time[i][new_job]) /
                               double(Instance::v_speed_factor[job_mode_op[new_job]]));

        //Tempo de preparação antes de new_job que será incrementado
        difference += Instance::m_setup_time[i][previos_job][new_job];

        if(next_job > 0){
            //Tempo de preparação após new_job que será incrementado
            difference += Instance::m_setup_time[i][new_job][next_job];

            //Tempo de preparação entre previos_job e next_job que será reduzido
            difference -= Instance::m_setup_time[i][previos_job][next_job];
        }

        //Tempo de termino da máquina i será igual ao valor atual mais
        //o custo da inserção da tarefa na posição j
        //completion_time = this->machine_completion_time[i] + difference;

        h = job_end_time1[previos_job];
        new_job_tec = CalcPECToJob(i, new_job, h);

        //Se encontrou uma posição com menor custo
        //if(new_job_tec < smaller_new_job_tec && completion_time <= Instance::num_planning_horizon){
        if(new_job_tec < smaller_new_job_tec){
            smaller_new_job_tec = new_job_tec;
            best_machine = i;
            best_position = best_position_machine;
            pec_job = new_job_tec;
            diff_time = difference;
            //smaller_completion_time = completion_time;
        }

    //}

}

/*
 * Inserir uma nova tarefa em todas as posições entre todas as máquinas
 * Escolher a posição e a máquina que gere menor TEC
 */
void Solution::AddJobGreedyTEC2(unsigned new_job, unsigned mode_op)
{

    unsigned best_position_machine, best_position, completion_time, difference;
    unsigned previos_job, next_job, best_machine, h, new_job_tec, smaller_new_job_tec, smaller_completion_time;

    smaller_new_job_tec = smaller_completion_time = INT_MAX;

    //Definir o modo de operação da nova tarefa
    job_mode_op[new_job] = mode_op;

    //Percorrer cada máquina
    for (unsigned i = 1; i <= Instance::num_machine; i++) {

        //Encontrar a melhor posição na máquina i
        best_position_machine = FindJobBestPosMacMakespan(new_job, i);

        //Identificar a tarefa anterior
        if(best_position_machine == 0){
            previos_job = 0;
        }
        else{
            previos_job = scheduling[i][best_position_machine-1];
        }

        //Identificar a tarefa seguinte
        if(best_position_machine < scheduling[i].size()){
            next_job = scheduling[i][best_position_machine];
        }
        else{
            next_job = 0;
        }

        //Tempo de processamento de new_job que será incrementado
        difference = ceil(double(Instance::m_processing_time[i][new_job]) /
                               double(Instance::v_speed_factor[job_mode_op[new_job]]));

        //Tempo de preparação antes de new_job que será incrementado
        difference += Instance::m_setup_time[i][previos_job][new_job];

        if(next_job > 0){
            //Tempo de preparação após new_job que será incrementado
            difference += Instance::m_setup_time[i][new_job][next_job];

            //Tempo de preparação entre previos_job e next_job que será reduzido
            difference -= Instance::m_setup_time[i][previos_job][next_job];
        }

        //Tempo de termino da máquina i será igual ao valor atual mais
        //o custo da inserção da tarefa na posição j
        completion_time = this->machine_completion_time[i] + difference;

        h = job_end_time1[previos_job];
        new_job_tec = CalcPECToJob(i, new_job, h);

        if(new_job_tec < smaller_new_job_tec && completion_time <= Instance::num_planning_horizon){
            smaller_new_job_tec = new_job_tec;
            best_machine = i;
            best_position = best_position_machine;
            smaller_completion_time = completion_time;
        }

    }

    //Inserir a nova tarefa na última posição
    scheduling[best_machine].push_back(new_job);

    //Deslocar as tarefas que estão após da melhor posição
    for (unsigned j = scheduling[best_machine].size()-1; j > best_position; j--) {
        scheduling[best_machine][j] = scheduling[best_machine][j-1];
    }

    //Alocar a nova tarefa na melhor posição identificada
    scheduling[best_machine][best_position] = new_job;

    this->machine_completion_time[best_machine] = smaller_completion_time;

    auto start = 0;
    previos_job = 0;
    for(auto it = scheduling[best_machine].begin(); it != scheduling[best_machine].end(); ++it){

        job_start_time1[*it] = start;
        H1[*it] = job_start_time1[*it] + Instance::m_setup_time[best_machine][previos_job][*it];
        job_end_time1[*it] = H1[*it] + ceil(double(Instance::m_processing_time[best_machine][*it]) /
                                            double(Instance::v_speed_factor[job_mode_op[*it]]));

        start = job_end_time1[*it];
        previos_job = *it;
    }
}

/*
 * Inserir uma nova tarefa em todas as posições entre todas as máquinas
 * Escolher a posição e a máquina que gere menor TEC
 */
void Solution::AddJobGreedyTEC3(unsigned new_job, unsigned mode_op)
{

    unsigned best_position_machine, best_position, completion_time, difference;
    unsigned previos_job, next_job, best_machine, h, new_job_tec, smaller_new_job_tec, smaller_completion_time;

    smaller_new_job_tec = smaller_completion_time = INT_MAX;

    //Definir o modo de operação da nova tarefa
    job_mode_op[new_job] = mode_op;

    best_machine = Instance::num_machine+1;
    best_position = 0;

    //Percorrer cada máquina
    for (unsigned i = 1; i <= Instance::num_machine; i++) {

        //Encontrar a melhor posição na máquina i
        best_position_machine = FindJobBestPosMacTEC3(new_job, i);

        //Identificar a tarefa anterior
        if(best_position_machine == 0){
            previos_job = 0;
        }
        else{
            previos_job = scheduling[i][best_position_machine-1];
        }

        //Identificar a tarefa seguinte
        if(best_position_machine < scheduling[i].size()){
            next_job = scheduling[i][best_position_machine];
        }
        else{
            next_job = 0;
        }

        //Tempo de processamento de new_job que será incrementado
        difference = ceil(double(Instance::m_processing_time[i][new_job]) /
                               double(Instance::v_speed_factor[job_mode_op[new_job]]));

        //Tempo de preparação antes de new_job que será incrementado
        difference += Instance::m_setup_time[i][previos_job][new_job];

        if(next_job > 0){
            //Tempo de preparação após new_job que será incrementado
            difference += Instance::m_setup_time[i][new_job][next_job];

            //Tempo de preparação entre previos_job e next_job que será reduzido
            difference -= Instance::m_setup_time[i][previos_job][next_job];
        }

        //Tempo de termino da máquina i será igual ao valor atual mais
        //o custo da inserção da tarefa na posição j
        completion_time = this->machine_completion_time[i] + difference;

        h = job_end_time1[previos_job];
        new_job_tec = CalcPECToJob(i, new_job, h);

        //Se encontrou uma posição com menor custo
        //if(new_job_tec < smaller_new_job_tec && completion_time <= Instance::num_planning_horizon){
        if(new_job_tec < smaller_new_job_tec){
            smaller_new_job_tec = new_job_tec;
            best_machine = i;
            best_position = best_position_machine;
            smaller_completion_time = completion_time;
        }

    }

    //Inserir a nova tarefa na última posição
    scheduling[best_machine].push_back(new_job);

    //Deslocar as tarefas que estão após da melhor posição
    for (unsigned j = scheduling[best_machine].size()-1; j > best_position; j--) {
        scheduling[best_machine][j] = scheduling[best_machine][j-1];
    }

    //Alocar a nova tarefa na melhor posição identificada
    scheduling[best_machine][best_position] = new_job;

    this->machine_completion_time[best_machine] = smaller_completion_time;

    auto start = 0;
    previos_job = 0;
    for(auto it = scheduling[best_machine].begin(); it != scheduling[best_machine].end(); ++it){

        job_start_time1[*it] = start;
        H1[*it] = job_start_time1[*it] + Instance::m_setup_time[best_machine][previos_job][*it];
        job_end_time1[*it] = H1[*it] + ceil(double(Instance::m_processing_time[best_machine][*it]) /
                                            double(Instance::v_speed_factor[job_mode_op[*it]]));

        start = job_end_time1[*it];
        previos_job = *it;
    }


}

void Solution::InsertRandomPosition(unsigned new_job)
{
    unsigned machine, job_position;

    //Escolher aleatoriamente uma máquina
    machine = 1 + rand()%Instance::num_machine;

    if(scheduling[machine].size() > 0){
        //Escolher aleatoriamente uma posição nesse máquina
        job_position = rand()%scheduling[machine].size();
    }
    else{
        job_position = 0;
    }


    //Inserir a nova tarefa na posição e na máquina escolhida
    scheduling[machine].insert(scheduling[machine].begin()+job_position, new_job);
}

/*
 * Troca com uma máquina
 */
void Solution::SwapInside(unsigned machine, unsigned pos_job1, unsigned pos_job2)
{
    unsigned aux;
    aux = this->scheduling[machine][pos_job1];
    this->scheduling[machine][pos_job1] = this->scheduling[machine][pos_job2];
    this->scheduling[machine][pos_job2] = aux;
}

/*
 * Troca com uma máquina com delta
 * Muda os tempos de preparação, e pode alterar o makespan
 * Não muda o TEC
 */
void Solution::SwapInsideDelta(unsigned machine, unsigned pos_job1, unsigned pos_job2)
{

    //Realizar a troca
    this->SwapInside(machine, pos_job1, pos_job2);

    //Calcula os objetivos considerando apenas a máquina modificada
    this->CalculateShorterTimeHorizonMachine(machine);
    this->CalculateObjectiveMachine(machine);
}

/*
 * Troca com duas máquinas
 */
void Solution::SwapOutside(unsigned machine1, unsigned pos_job1, unsigned machine2, unsigned pos_job2)
{
    unsigned aux;
    aux = this->scheduling[machine1][pos_job1];
    this->scheduling[machine1][pos_job1] = this->scheduling[machine2][pos_job2];
    this->scheduling[machine2][pos_job2] = aux;
}

/*
 * Troca com duas máquinas com delta
 * Muda os tempos de preparação e de processamento, e pode alterar o makespan
 * Muda o TEC
 */
void Solution::SwapOutsideDelta(unsigned machine1, unsigned pos_job1, unsigned machine2, unsigned pos_job2)
{
    //Realizar a troca
    this->SwapOutside(machine1, pos_job1, machine2, pos_job2);

    //Calcula os objetivos considerando apenas a máquina modificada
    this->CalculateShorterTimeHorizonMachine(machine1);
    this->CalculateObjectiveMachine(machine1);

    this->CalculateShorterTimeHorizonMachine(machine2);
    this->CalculateObjectiveMachine(machine2);
}

/*
 * Inserção com uma máquina
 * A tarefa que está em pos1 será inserida em pos2
 */
void Solution::InsertInside(unsigned machine, unsigned pos1, unsigned pos2)
{

    unsigned aux;
    aux = this->scheduling[machine][pos1];
    if(pos1 < pos2){
        unsigned i;
        for(i = pos1; i < pos2; i++){
            this->scheduling[machine][i] = this->scheduling[machine][i+1];
        }
        this->scheduling[machine][i] = aux;
    }
    else{
        unsigned i;
        for(i = pos1; i > pos2; i--){
            this->scheduling[machine][i] = this->scheduling[machine][i-1];
        }
        this->scheduling[machine][i] = aux;
    }
}

/*
 * Inserção com uma máquina com delta
 * A tarefa que está em pos1 será inserida em pos2
 */
void Solution::InsertInsideDelta(unsigned machine, unsigned pos1, unsigned pos2)
{
    InsertInside(machine, pos1, pos2);

    //Calcula os objetivos considerando apenas a máquina modificada
    this->CalculateShorterTimeHorizonMachine(machine);
    this->CalculateObjectiveMachine(machine);
}

/*
 * Inserção com duas máquinas
 * A tarefa que está em pos1 da máquina 1 será inserida em pos2 da máquina 2
 */
void Solution::InsertOutside(unsigned machine1, unsigned pos1, unsigned machine2, unsigned pos2)
{
    //Criar um iterator para a primeira tarefa
    auto it_job1 = this->scheduling[machine1].begin() + pos1;
    auto it_job2 = this->scheduling[machine2].begin() + pos2;

    //Inserir a primeira tarefa na outra máquina
    this->scheduling[machine2].insert(it_job2, *it_job1);

    //Remover a primeira tarefa da sua antiga posição
    this->scheduling[machine1].erase(it_job1);
}



/*
 * Inserção com duas máquinas
 * A tarefa que está em pos1 da máquina 1 será inserida em pos2 da máquina 2
 */
void Solution::InsertOutsideDelta(unsigned machine1, unsigned pos1, unsigned machine2, unsigned pos2)
{
    InsertOutside(machine1, pos1, machine2, pos2);

    //Calcula os objetivos considerando apenas a máquina modificada
    this->CalculateShorterTimeHorizonMachine(machine1);
    this->CalculateObjectiveMachine(machine1);

    this->CalculateShorterTimeHorizonMachine(machine2);
    this->CalculateObjectiveMachine(machine2);
}

void Solution::ChangeModeOpJob(unsigned machine, unsigned position, unsigned new_mode_op)
{

    unsigned job;

    job = scheduling[machine][position];

    job_mode_op[job] = new_mode_op;

}

void Solution::ChangeModeOpJobDelta(unsigned machine, unsigned position, unsigned new_mode_op)
{
    ChangeModeOpJob(machine, position, new_mode_op);

    //Calcula os objetivos considerando apenas a máquina modificada
    this->CalculateShorterTimeHorizonMachine(machine);
    this->CalculateObjectiveMachine(machine);
}

/*
 * Trocar o instante de início de uma tarefa
 */
void Solution::ChangeHJob(unsigned machine, unsigned position, unsigned add_h)
{
    unsigned job;

    //Identificar a tarefa
    job = scheduling[machine][position];

    //Definir o instance de início
    unsigned start = H1[job] + add_h;
    //Identificar a tarefa anterior
    unsigned previos;
    if(position > 0){
        previos = position-1;
    }
    else{
        previos = 0;
    }
    //Alterar os instantes de início da tarefa e de suas sucessoras
    for(unsigned j = position; j < scheduling.size(); j++){
        //Atualizar os instantes de início e de término
        job_start_time1[j] = start;
        H1[j] = Instance::m_setup_time[machine][previos][j];
        job_end_time1[j] = H1[j] + Instance::m_processing_time[machine][j];

        //j passa a ser a tarefa anterior
        previos = j;
        //A próxima tarefa começa após a tarefa atual
        start = job_end_time1[j] = H1[j];
    }
}

unsigned Solution::GetFastestOpMode()
{
    //Selecionar o modo de operação, mais rápido
    //Como ele é um fator, deve ser escolhido aquele de maior valor
    unsigned best_op_mode = 1;
    for(unsigned i = 2; i <= Instance::num_mode_op; i++){
        if(Instance::v_speed_factor[i] > Instance::v_speed_factor[best_op_mode]){
            best_op_mode = i;
        }
    }

    return best_op_mode;
}

unsigned Solution::GetCheapestOpMode()
{

    unsigned best_op_mode = 1;

    for(unsigned i = 2; i <= Instance::num_mode_op; i++){
        if(Instance::v_consumption_factor[i]/Instance::v_speed_factor[i]
                < Instance::v_consumption_factor[best_op_mode]/Instance::v_speed_factor[best_op_mode]){
            best_op_mode = i;
        }
    }

    return best_op_mode;
}

unsigned Solution::GetMakespanMachine()
{
    unsigned makespan_machine = 1;

    for(unsigned i = 2; i <= Instance::num_machine; i++){
        if(machine_completion_time[i] > machine_completion_time[makespan_machine]){
            makespan_machine = i;
        }
    }

    return makespan_machine;
}

void Solution::AddJob(unsigned new_job, unsigned machine, unsigned positon, unsigned diff_time)
{
    //Inserir j na solução s, na máquina i, na posição p

    //Inicialmente, inserimos a nova tarefa na última posição
    scheduling[machine].push_back(new_job);

    //Deslocar as tarefas que estão após da melhor posição
    for (unsigned j = scheduling[machine].size()-1; j > positon; j--) {
        scheduling[machine][j] = scheduling[machine][j-1];
    }

    //Alocar a nova tarefa na melhor posição identificada
    scheduling[machine][positon] = new_job;
    machine_completion_time[machine] += diff_time;

    if(machine_completion_time[machine] > makeSpan){
        makeSpan = machine_completion_time[machine];
    }
}



bool CompareMakespan(Solution * l, Solution * r) //(2)
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

bool CompareTEC(Solution * l, Solution * r) //(2)
{
    if(l->TEC < r->TEC){
        return true;
    }
    else if (l->TEC > r->TEC){
        return false;
    }
    else{
        if(l->makeSpan < r->makeSpan){
            return true;
        }
        else{
            return false;
        }
    }

}


double Z_STAR::makespan = 0;
double Z_STAR::TEC = 0;
