#include "generate_intances.h"

void GenerateSmallInstances()
{

    InstanceParam ip;

    srand (time(NULL));

    //TODO mudar aqui
    vector<unsigned> num_jobs;
    for(unsigned i=1; i<=5; i++){
        num_jobs.push_back(5 + (1*i));
    }

    //TODO mudar aqui
    vector<unsigned> num_machines;
    for(unsigned i=1; i<=1; i++){
        num_machines.push_back(1 + i);
    }

    ip.num_op_mode = 3;

    vector<pair<unsigned, unsigned>> v_range_setup(1);

    v_range_setup[0].first = 1;
    v_range_setup[0].second = 9;

    pair<unsigned, unsigned>range_processing;
    range_processing.first = 1;
    range_processing.second = 99;
    ip.range_processing = range_processing;


    pair<unsigned, unsigned>range_potency;
    range_potency.first = 40;
    range_potency.second = 200;
    ip.range_potency = range_potency;

    ip.destination_folder = DESTINATION_FOLDER_SMALL;

    ip.speed_factor.push_back(1.2);
    ip.speed_factor.push_back(1);
    ip.speed_factor.push_back(0.8);

    ip.consumption_factor.push_back(1.5);
    ip.consumption_factor.push_back(1);
    ip.consumption_factor.push_back(0.6);

    for(auto it_jobs : num_jobs){
        for(auto it_machines : num_machines){
            for (auto it_setup: v_range_setup) {

                ip.num_jobs = it_jobs;
                ip.num_machines = it_machines;

                ip.num_days = 1;

                ip.range_setup = it_setup;

                CreateNewIntance(ip);

            }
        }
    }
}

void CreateNewIntance(InstanceParam ip)
{

    unsigned new_num_days;

    //Gerar dados da instância
    GenerateInstanceData(ip);

    Solution * my_solution = new Solution();

    //Gerar uma solução inicial gulosa considerando o objetivo do TEC
    my_solution->GenerateGreedySolutionTEC3();
    new_num_days = ceil(double(my_solution->makeSpan/(double)Instance::num_planning_horizon));

    for(unsigned i=Instance::num_days; i<new_num_days; i++){

        Instance::v_peak_start.push_back(PEAK_START + (PLAN_HORIZON_SIZE+1)*i);

        Instance::v_peak_end.push_back(PEAK_END + (PLAN_HORIZON_SIZE+1)*i);
    }

    Instance::num_days = new_num_days;

    //Gerar uma solução inicial gulosa considerando o objetivo do makespan
    //my_solution->GenerateGreedySolutionMakespan();

    /*
     * --Calcular o maior valor possível para o makespan--
     * maior tempo de término para todas as tarefas
     * maior tempo de preparação para todas as tarefas
     * todas na mesma máquina
     * Não pode exceder o horizonte de planejamento
    */

    //Encontra o fator de velocidade mais lento
    double slower_speed_factor = Instance::v_speed_factor[1];
    for(unsigned op=2; op <= Instance::num_mode_op; op++){
        if(Instance::v_speed_factor[op] < slower_speed_factor){
            slower_speed_factor = Instance::v_speed_factor[op];
        }
    }

    //Encontrar o maior custo para cada tarefa
    unsigned max_proc_time_job = 0;
    unsigned sum_proc_time_job = 0;
    for (unsigned j=1; j <= Instance::num_jobs; j++) {
        max_proc_time_job = 0;
        for (unsigned i=1; i <= Instance::num_machine; i++) {
            if(Instance::m_processing_time[i][j] > max_proc_time_job)
                max_proc_time_job = Instance::m_processing_time[i][j];
        }
        sum_proc_time_job += max_proc_time_job;
    }

    unsigned max_setup_time_job = 0;
    unsigned sum_setup_time_job = 0;
    for (unsigned j=1; j <= Instance::num_jobs; j++) {
        max_setup_time_job = 0;
        for (unsigned i=1; i <= Instance::num_machine; i++) {
            for (unsigned k=1; k <= Instance::num_jobs; k++) {
                if(Instance::m_setup_time[i][j][k] > max_setup_time_job)
                    max_setup_time_job = Instance::m_setup_time[i][j][k];
            }
        }
        sum_setup_time_job += max_setup_time_job;
    }

    unsigned max_makespan = ceil(double(sum_proc_time_job)/double(slower_speed_factor))
            + sum_setup_time_job;
    //Instance::max_makespan = min(max_makespan, Instance::num_planning_horizon*Instance::num_days);
    Instance::max_makespan = max_makespan;

    //Encontrar a máquina com a maior potência (e maior consumo)
    unsigned higher_potency_op = 1;
    for(unsigned op=2; op <= Instance::num_machine; op++){
        if(Instance::v_machine_potency[op] > Instance::v_machine_potency[higher_potency_op]){
            higher_potency_op = op;
        }
    }

    //Encontrar o modo de operação com maior fator de consumo consumo
    unsigned higher_consumption_factor = 1;
    for(unsigned op=2; op <= Instance::num_mode_op; op++){
        if(Instance::v_consumption_factor[op] > Instance::v_consumption_factor[higher_consumption_factor]){
            higher_consumption_factor = op;
        }
    }

    double max_tec;
    max_tec = sum_proc_time_job*Instance::v_machine_potency[higher_potency_op]
            *Instance::rate_on_peak*Instance::v_consumption_factor[higher_consumption_factor];

    max_tec = double(max_tec)/double(60);
    Instance::max_energy_cost = max_tec;

    //Salvar dados da instância em arquivo
    SaveInstanceDataToFile(ip);

}

void GenerateLargeInstances()
{

    InstanceParam ip;

    vector<unsigned> num_jobs;
    /*for(unsigned i=1; i<=5; i++){
        num_jobs.push_back(i*50);
    }*/
    num_jobs.push_back(50);
    num_jobs.push_back(250);
    num_jobs.push_back(750);

    vector<unsigned> num_machines;
    for(unsigned i=1; i<=2; i++){
        num_machines.push_back(i*10);
    }

    ip.num_op_mode = 5;

    vector<pair<unsigned, unsigned>> v_range_setup(2);

    v_range_setup[0].first = 1;
    v_range_setup[0].second = 9;

    v_range_setup[1].first = 1;
    v_range_setup[1].second = 124;

    pair<unsigned, unsigned>range_processing;
    range_processing.first = 1;
    range_processing.second = 99;
    ip.range_processing = range_processing;


    pair<unsigned, unsigned>range_potency;
    range_potency.first = 40;
    range_potency.second = 200;
    ip.range_potency = range_potency;

    ip.destination_folder = DESTINATION_FOLDER_LARGER;

    ip.speed_factor.push_back(1.2);
    ip.speed_factor.push_back(1.1);
    ip.speed_factor.push_back(1);
    ip.speed_factor.push_back(0.9);
    ip.speed_factor.push_back(0.8);

    ip.consumption_factor.push_back(1.5);
    ip.consumption_factor.push_back(1.25);
    ip.consumption_factor.push_back(1);
    ip.consumption_factor.push_back(0.8);
    ip.consumption_factor.push_back(0.6);

    for(auto it_jobs : num_jobs){
        for(auto it_machines : num_machines){
            for (auto it_setup: v_range_setup) {

                ip.num_jobs = it_jobs;
                ip.num_machines = it_machines;

                ip.num_days = 1;

                ip.range_setup = it_setup;

                CreateNewIntance(ip);
            }
        }
    }
}

void GenerateInstanceData(InstanceParam ip){

    Instance::num_jobs = ip.num_jobs;
    Instance::num_machine = ip.num_machines;
    Instance::num_days = ip.num_days;
    Instance::num_planning_horizon = PLAN_HORIZON_SIZE;
    Instance::num_mode_op = ip.num_op_mode;
    Instance::rate_on_peak = RATE_IN_PEAK;
    Instance::rate_off_peak = RATE_OFF_PEAK;
    Instance::max_energy_cost = MAX_COST;
    Instance::max_makespan = MAX_COST;

    Instance::Init();

    //Gerar o início do horário de pico para cada dia i
    Instance::v_peak_start.clear();
    for(unsigned i=0; i<ip.num_days; i++){
        Instance::v_peak_start.push_back(PEAK_START + PLAN_HORIZON_SIZE*i);
    }

    Instance::v_peak_end.clear();
    for(unsigned i=0; i<ip.num_days; i++){
        Instance::v_peak_end.push_back(PEAK_END + PLAN_HORIZON_SIZE*i);
    }

    for(unsigned i=1; i<=ip.num_op_mode; i++){
        Instance::v_speed_factor[i] = ip.speed_factor[i-1];
    }

    for(unsigned i=1; i<=ip.num_op_mode; i++){
        Instance::v_consumption_factor[i] = ip.consumption_factor[i-1];
    }

    unsigned max_potency = 0;
    for(unsigned i=1; i<=ip.num_machines; i++){
        Instance::v_machine_potency[i] = ip.range_potency.first + rand()%(ip.range_potency.second - ip.range_potency.first);
        if(Instance::v_machine_potency[i] > max_potency){
            max_potency = Instance::v_machine_potency[i];
        }
    }

    unsigned max_processing_time_job = 0;
    for(unsigned j=1; j<=ip.num_jobs; j++){
        for(unsigned i=1; i<=ip.num_machines; i++){
            Instance::m_processing_time[i][j] = ip.range_processing.first + rand()%(ip.range_processing.second-ip.range_processing.first);
            if(Instance::m_processing_time[i][j] > max_processing_time_job){
                max_processing_time_job = Instance::m_processing_time[i][j];
            }
        }
    }

    unsigned max_setup_time_job = 0;
    unsigned min, max;
    for(unsigned i=1; i<=ip.num_machines; i++){
        for(unsigned j=1; j<=ip.num_jobs; j++){
            for(unsigned k=1; k<=ip.num_jobs; k++){

                if(j!=k){

                    min = ip.range_setup.first;
                    max = ip.range_setup.second;

                    //Percorrer todas as outras tarefas
                    for(unsigned l=1; l<=ip.num_jobs; l++){

                        //Limitar apenas quando os tempos de preparação já foram definidos
                        if(l!=j && l!=k && Instance::m_setup_time[i][j][l]>0 && Instance::m_setup_time[i][l][k]>0){

                            //O valor gerado não pode ser inferior a diferença dos outros dois
                            min = abs(int(Instance::m_setup_time[i][j][l]-Instance::m_setup_time[i][l][k]))+1;

                            if(min > ip.range_setup.second){
                                max = min + 1;
                            }
                            else{
                                //O valor gerado não pode ser superior a soma dos outros dois
                                max = Instance::m_setup_time[i][j][l]+Instance::m_setup_time[i][l][k] - 1;

                                if(max > ip.range_setup.second){
                                    max = ip.range_setup.second;
                                }
                            }
                        }
                    }

                }
                else{

                    //O tempo de preparação de uma tarefa para ela mesma é zero
                    min = 0;
                    max = 0;

                }

                if(max != min){
                    //Gerar um número entre min e max
                    Instance::m_setup_time[i][j][k] = min + rand()%(max - min);
                }
                else{
                    Instance::m_setup_time[i][j][k] = min;
                }
                if(Instance::m_setup_time[i][j][k] > max_setup_time_job){
                    max_setup_time_job = Instance::m_setup_time[i][j][k];
                }
            }
        }
    }

    //Calcular o makespan máximo

    Instance::max_makespan = Instance::num_jobs*max_processing_time_job/Instance::v_consumption_factor[1] +
            Instance::num_jobs*(max_setup_time_job-1);

    Instance::max_energy_cost =
            Instance::num_jobs*
            max_processing_time_job/
            Instance::rate_on_peak*
            Instance::v_consumption_factor[1]*
            max_potency/
            60;

}

void SaveInstanceDataToFile(InstanceParam ip){

    string file_name;

    //Definir o nome do arquivo
    file_name = itos(Instance::num_jobs);
    file_name += "_";
    file_name += itos(Instance::num_machine);
    file_name += "_";
    file_name += itos(PLAN_HORIZON_SIZE);
    file_name += "_";
    file_name += itos(Instance::num_mode_op);
    file_name += "_S_";
    file_name += itos(ip.range_setup.first) + "-" + itos(ip.range_setup.second);
    file_name += ".dat";

    string full_name_file;

    full_name_file = ip.destination_folder+file_name;

    filesystem::path dir(ip.destination_folder);

    if(!(filesystem::exists(dir))){
        cout<< "A pasta " << ip.destination_folder << " NÃO existe!" << endl;

        if (filesystem::create_directory(dir))
            cout<< "A pasta " << ip.destination_folder << " foi CRIADA com sucesso!" << endl;
    }

    ofstream MyFile;
    MyFile.open(full_name_file);

    MyFile << "n " << Instance::num_jobs << endl;
    MyFile << "m " << Instance::num_machine << endl;
    MyFile << "n_day " << Instance::num_days << endl;
    MyFile << "hl " << PLAN_HORIZON_SIZE << endl;
    MyFile << "o " << Instance::num_mode_op << endl;
    MyFile << "rate_in_peak " << RATE_IN_PEAK << endl;
    MyFile << "rate_off_peak " << RATE_OFF_PEAK << endl;
    MyFile << "max_energy_cost " << Instance::max_energy_cost << endl;
    MyFile << "max_makespan " << Instance::max_makespan << endl;

    MyFile << endl;

    MyFile << "peak_start" << endl;
    for(unsigned i=0; i<Instance::num_days; i++){

        MyFile << Instance::v_peak_start[i] << endl;

    }
    MyFile << endl;

    MyFile << "peak_end" << endl;
    for(unsigned i=0; i<Instance::num_days; i++){

        MyFile << Instance::v_peak_end[i] << endl;

    }
    MyFile << endl;


    MyFile << "v" << endl;
    for(unsigned i=1; i<=Instance::num_mode_op; i++){

        MyFile << Instance::v_speed_factor[i] << endl;

    }
    MyFile << endl;

    MyFile << "lambda" << endl;
    for(unsigned i=1; i<=Instance::num_mode_op; i++){

        MyFile << Instance::v_consumption_factor[i] << endl;

    }

    MyFile << endl;

    MyFile << "pi" << endl;
    for(unsigned i=1; i<=Instance::num_machine; i++){
        MyFile << Instance::v_machine_potency[i] << endl;
    }
    MyFile << endl;

    MyFile << "processing" << endl;
    for(unsigned j=1; j<=Instance::num_jobs; j++){
        for(unsigned i=1; i<=Instance::num_machine; i++){
            MyFile << Instance::m_processing_time[i][j] << "\t";
        }
        MyFile << endl;
    }
    MyFile << endl;

    MyFile << "setup" << endl;

    for(unsigned i=1; i<=Instance::num_machine; i++){
        for(unsigned j=1; j<=Instance::num_jobs; j++){
            for(unsigned k=1; k<=Instance::num_jobs; k++){

                //Salvar o número no arquivo
                MyFile << Instance::m_setup_time[i][j][k] << "\t";
                MyFile.flush();

            }
            MyFile << endl;
        }
        MyFile << endl;
    }

    MyFile.close();
}
