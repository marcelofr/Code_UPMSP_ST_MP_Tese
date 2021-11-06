#include "run_algorithm.h"

void RunAlgorithm(algorithm_data alg_data){

    Timer *t1 = new Timer();

    //Make trace log in file (.trace.log)
    MakeTrace();

    //Ler a instância
    Instance::ReadMarceloInstance(alg_data.param.instance_file);
    //Instance::PrintInstance1();exit(0);

    Instance::seed = alg_data.param.u_seed;

    Discretize(1);

    vector<Solution*> nd_set_solution;

    alg_data.is_optimal = false;

    t1->start();

    //Definir o tempo limite de execução
    alg_data.time_limit = alg_data.param.u_max_time_factor * Instance::num_jobs * log(Instance::num_machine);

    #ifdef DEBUG
        cout << "Tempo limite: " << alg_data.time_limit << endl;
    #endif

    if(alg_data.param.algorithm_name == "NSGAII"){
        RunAlgorithmNSGAII(alg_data, nd_set_solution, t1);
    }
    else if(alg_data.param.algorithm_name == "EXACT"){
        RunAlgorithmExact(alg_data, nd_set_solution);
    }
    else if(alg_data.param.algorithm_name == "SPEA2"){
        RunAlgorithmSPEA2(alg_data, nd_set_solution, t1);
    }
    else if(alg_data.param.algorithm_name == "MOGA"){
        RunAlgorithmMOGA(alg_data, nd_set_solution, t1);
    }
    else if(alg_data.param.algorithm_name == "NSGAI"){
        RunAlgorithmNSGAI(alg_data, nd_set_solution, t1);
    }

    t1->stop();

    //Eliminar soluções que estão fora do horizonte de planejamento
    SelectOnlyValidSolutions(nd_set_solution);

    alg_data.elapsed_time_sec = t1->getElapsedTimeInMilliSec();
    alg_data.non_dominated_set.clear();
    pair<unsigned, double> aux;
    for(auto it:nd_set_solution){
        #ifdef DEBUG
            it->Check();
        #endif
        aux.first = it->makeSpan*Instance::discretization_factor;
        aux.second = it->TEC*Instance::discretization_factor;
        alg_data.non_dominated_set.push_back(aux);
    }

    //Ordenar os pontos
    SortByMakespan(alg_data.non_dominated_set);

    #ifndef IRACE
        //Salvar o conjunto não-dominado em arquivo
        SalveFileSolution(alg_data);
    #endif

    #ifdef IRACE
        pair<unsigned, double> reference_point;
        double hv;
        /*reference_point.first = alg_data.non_dominated_set.back().first;
        reference_point.second = alg_data.non_dominated_set.front().second;*/
        reference_point.first = Instance::max_makespan;
        reference_point.second = Instance::max_energy_cost;
        hv = CalculateHypervolume(alg_data.non_dominated_set, reference_point);
        //cout << hv << " " << ir.elapsed_time_sec << endl;
        cout << hv << endl;
    #endif

    delete t1;

}

void RunAlgorithmNSGAII(algorithm_data alg_data, vector<Solution*> &non_dominated_set, Timer *t1){


    NDSetSolution<GASolution *> *non_dominated_set_ga = new NDSetSolution<GASolution *>();

    non_dominated_set_ga->ConstrutiveGreedyAndRandom(alg_data.param.u_population_size);

    #ifdef DEBUG
        cout << "===========Inicio População Inicial===========" << endl;
        PrintPopulation(non_dominated_set_ga->set_solution);
        //non_dominated_set_ga->PrintSetSolution();
        cout << "===========Fim População Inicial===========" << endl << endl;
        //exit(0);
    #endif

    //alg_data.time_limit=0;
    nsgaii(alg_data, non_dominated_set_ga->set_solution, t1);

    SortByMakespan(non_dominated_set_ga->set_solution);

    #ifdef DEBUG
        cout << "===========Inicio NSGA===========" << endl;
        PrintPopulation(non_dominated_set_ga->set_solution);
        //non_dominated_set_ga->PrintSetSolution();
        t1->printElapsedTimeInMilliSec();
        cout << "===========Fim NSGA===========" << endl;
    #endif

    non_dominated_set.clear();
    for(auto it:non_dominated_set_ga->set_solution){
        non_dominated_set.push_back(it);
    }
}

void RunAlgorithmSPEA2(algorithm_data alg_data, vector<Solution*> &non_dominated_set, Timer *t1){


    NDSetSolution<GASolution *> *non_dominated_set_ga = new NDSetSolution<GASolution *>();
    NDSetSolution<GASolution *> *nds = new NDSetSolution<GASolution *>();

    non_dominated_set_ga->ConstrutiveGreedyAndRandom(alg_data.param.u_population_size);

    #ifdef DEBUG
        cout << "===========Inicio População Inicial SPEA2===========" << endl;
        PrintPopulation(non_dominated_set_ga->set_solution);
        //non_dominated_set_ga->PrintSetSolution();
        cout << "===========Fim População Inicial SPEA2===========" << endl << endl;
        //exit(0);
    #endif

    //alg_data.time_limit=0;
    spea2(alg_data, non_dominated_set_ga->set_solution, t1);

    for(auto it:non_dominated_set_ga->set_solution){
        nds->AddSolution(it);
    }

    SortByMakespan(nds->set_solution);

    #ifdef DEBUG
        cout << "===========Inicio SPEA2===========" << endl;
        PrintPopulation(nds->set_solution);
        //non_dominated_set_ga->PrintSetSolution();
        t1->printElapsedTimeInMilliSec();
        cout << "===========Fim SPEA2===========" << endl;
    #endif

    non_dominated_set.clear();
    for(auto it:nds->set_solution){
        non_dominated_set.push_back(it);
    }

    delete non_dominated_set_ga;
    delete nds;
}

void RunAlgorithmMOGA(algorithm_data alg_data, vector<Solution*> &non_dominated_set, Timer *t1){


    NDSetSolution<GASolution *> *non_dominated_set_ga = new NDSetSolution<GASolution *>();
    NDSetSolution<GASolution *> *nds = new NDSetSolution<GASolution *>();

    non_dominated_set_ga->ConstrutiveGreedyAndRandom(alg_data.param.u_population_size);

    #ifdef DEBUG
        cout << "===========Inicio População Inicial MOGA===========" << endl;
        PrintPopulation(non_dominated_set_ga->set_solution);
        //non_dominated_set_ga->PrintSetSolution();
        cout << "===========Fim População Inicial MOGA===========" << endl << endl;
        //exit(0);
    #endif

    //alg_data.time_limit=0;
    moga(alg_data, non_dominated_set_ga->set_solution, t1);

    for(auto it:non_dominated_set_ga->set_solution){
        nds->AddSolution(it);
    }

    SortByMakespan(nds->set_solution);

    #ifdef DEBUG
        cout << "===========Inicio MOGA===========" << endl;
        PrintPopulation(nds->set_solution);
        //non_dominated_set_ga->PrintSetSolution();
        t1->printElapsedTimeInMilliSec();
        cout << "===========Fim MOGA===========" << endl;
    #endif

    non_dominated_set.clear();
    for(auto it:nds->set_solution){
        non_dominated_set.push_back(it);
    }

    delete non_dominated_set_ga;
    delete nds;
}

void RunAlgorithmNSGAI(algorithm_data alg_data, vector<Solution*> &non_dominated_set, Timer *t1){


    NDSetSolution<GASolution *> *non_dominated_set_ga = new NDSetSolution<GASolution *>();
    NDSetSolution<GASolution *> *nds = new NDSetSolution<GASolution *>();

    non_dominated_set_ga->ConstrutiveGreedyAndRandom(alg_data.param.u_population_size);

    #ifdef DEBUG
        cout << "===========Inicio População Inicial===========" << endl;
        PrintPopulation(non_dominated_set_ga->set_solution);
        //non_dominated_set_ga->PrintSetSolution();
        cout << "===========Fim População Inicial===========" << endl << endl;
        //exit(0);
    #endif

    //alg_data.time_limit=0;
    nsga_i(alg_data, non_dominated_set_ga->set_solution, t1);

    for(auto it:non_dominated_set_ga->set_solution){
        nds->AddSolution(it);
    }

    SortByMakespan(nds->set_solution);

    #ifdef DEBUG
        cout << "===========Inicio SPEAII===========" << endl;
        PrintPopulation(nds->set_solution);
        //non_dominated_set_ga->PrintSetSolution();
        t1->printElapsedTimeInMilliSec();
        cout << "===========Fim SPEAII===========" << endl;
    #endif

    non_dominated_set.clear();
    for(auto it:nds->set_solution){
        non_dominated_set.push_back(it);
    }

    delete non_dominated_set_ga;
    delete nds;
}


void RunAlgorithmExact(algorithm_data alg_data, vector<Solution*> &non_dominated_set){

    MonoSolution * my_solution;

    //Gerar uma solução inicial gulosa considerando o objetivo do makespan
    my_solution = new MonoSolution();

    my_solution->weights.first = alg_data.param.d_alpha;
    my_solution->weights.second = 1-alg_data.param.d_alpha;

    //my_solution->GenerateGreedySolutionWeigth();

    //Modelo ponderado
    RunWeightedMathModel(alg_data.time_limit, alg_data.param.d_alpha, my_solution);

    non_dominated_set.push_back(my_solution);

    if(my_solution->is_optimal){
        alg_data.is_optimal = true;
    }

    alg_data.param.file_solution = alg_data.param.folder_solution + alg_data.param.algorithm_name + "_" + alg_data.param.instance_name + "_" + alg_data.param.s_alpha + ".sol";
}


void Discretize(unsigned factor){

    /*Fazer a discretização do tempo*/
    Instance::discretization_factor = factor;
    Instance::num_planning_horizon = ceil(double(Instance::num_planning_horizon+1)
                                      /double(Instance::discretization_factor))-1;
    for(unsigned i=0; i<Instance::num_days; i++){
        Instance::v_peak_start[i] = ceil(double(Instance::v_peak_start[i])
                                        /double(Instance::discretization_factor));
        Instance::v_peak_end[i] = ceil(double(Instance::v_peak_end[i]+1)
                                   /double(Instance::discretization_factor))-1;
    }
    for(unsigned i=1; i<=Instance::num_machine; i++){
        for(unsigned j=1; j<=Instance::num_jobs; j++){
            Instance::m_processing_time[i][j] = ceil(double(Instance::m_processing_time[i][j])
                                                     /double(Instance::discretization_factor));
            for(unsigned k=1; k<=Instance::num_jobs; k++){
                Instance::m_setup_time[i][j][k] = ceil(double(Instance::m_setup_time[i][j][k])
                                                       /double(Instance::discretization_factor));
            }
        }
    }
    Instance::max_energy_cost = ceil(double(Instance::max_energy_cost)
                                     /double(Instance::discretization_factor));
    Instance::max_makespan = ceil(double(Instance::max_makespan)
                                  /double(Instance::discretization_factor));

}

/*
 * Método para salvar em arquivo a solução encontrada por um algoritmo
 */
void SalveFileSolution(algorithm_data alg_data){

    ofstream MyFile;

    //Tentar abrir um arquivo existente
    MyFile.open(alg_data.param.file_solution, ios_base::out | ios_base::in | ios_base::ate);  // will not create file

    //Se o arquivo não existe então criar um novo
    if (!MyFile.is_open())
    {
        MyFile.clear();
        MyFile.open(alg_data.param.file_solution, ios_base::out);  // will create if necessary

    }
    else{
        MyFile << endl << endl;
    }

    MyFile << "Instance: " << alg_data.param.instance_name << endl;
    MyFile << "Algorithm: " << alg_data.param.algorithm_name << endl;
    MyFile << "Time_limit: "<< alg_data.time_limit << endl;
    MyFile << "Seed: "<< alg_data.param.s_seed << endl;
    MyFile << "Elapsed_time: " << alg_data.elapsed_time_sec << endl;
    if(alg_data.param.algorithm_name == "NSGAII" || alg_data.param.algorithm_name == "SPEA2" || alg_data.param.algorithm_name == "MOGA"){
        MyFile << "param1: " << alg_data.param.s_population_size << endl;
        MyFile << "param2: " << alg_data.param.s_prob_mutation << endl;
        MyFile << "param3: " << "nan" << endl;
    }
    else{
        MyFile << "param1: " << "nan" << endl;
        MyFile << "param2: " << "nan" << endl;
        MyFile << "param3: " << "nan" << endl;
    }


    MyFile << endl;

    MyFile << "Makespan" << "\t" << "TEC";

    for (auto it=alg_data.non_dominated_set.begin(); it != alg_data.non_dominated_set.end();++it) {
        MyFile << endl << it->first << "\t" << it->second;
    }

    //Imprimir demarcação de final de arquivo
    if(alg_data.param.algorithm_name == "EXACT"){

        if(alg_data.is_optimal){
            MyFile << "\t" << "*";
        }
        else{
            MyFile << "\t" << "-";
        }
    }

    MyFile << "\t" << "END";

    MyFile.close();
}

/*
 * Função para selecionar apenas solução válidas que estão no conjunto não-dominado
 */
void SelectOnlyValidSolutions(vector<Solution*> non_dominated_set){
    for(auto it_sol = non_dominated_set.begin(); it_sol != non_dominated_set.end();){
        //Se o makespan é maior que o horizonte de planejamento
        if((*it_sol)->makeSpan > (Instance::num_planning_horizon)*Instance::num_days){
            //Remover essa solução do conjunto
            it_sol = non_dominated_set.erase(it_sol);
        }
        else{
            ++it_sol;
        }
    }
}


void CalculateMetric(string folder_solution)
{

    vector<string> files;
    map<string,map<string, map<string, vector<pair<unsigned, double>>>>> sets;
    map<string,map<string, map<string, double>>> hypervolume;
    map<string,map<string, map<string, double>>> diversity;
    map<string, pair<unsigned, double>> reference_points;

    //Encontrar todos os arquivos que estão na pasta de soluções
    FindFilesInFolder(folder_solution, files);

    //Ler o conteúdo de cada arquivo de solução e guardar em sets
    ReadFiles(files, sets);

    //Gerar o conjunto de referência
    //Calcular seu hipervolume do conjunto de referência
    //Salvá-lo em um arquivo
    GenerateReferenceSet(folder_solution, sets, hypervolume, reference_points);

    //Calcular hipervolume para todas as soluções encontradas
    HypervolumeMetric(sets, hypervolume, reference_points);

    //Calcular diversidade para todas as soluções encontradas
    DiversityMetric(sets, diversity);

}


void HypervolumeMetric(map<string,map<string, map<string, vector<pair<unsigned, double>>>>> sets,
                                map<string,map<string, map<string, double>>> &hypervolume,
                                map<string, pair<unsigned, double>> reference_points)
{

    //Calcular o hipervolume para todas as instâncias em sets
    CalculateHypervolumeSet(sets, hypervolume, reference_points);

    cout << "----------------Hipervolume-----------------" << endl;

    cout << setprecision(10);

    //Imprimir os resultados sumarizados
    cout << "Resultados sumarizados" << endl << endl;
    //Cabeçalho
    cout << "Instance ";
    for(auto &it_algorithm : sets.begin()->second){
        cout << " " << it_algorithm.first << " ";
    }
    cout << " Reference point";
    cout << endl;

    //Corpo
    for(auto &instance : hypervolume){

        cout << instance.first << " " ;


        for(auto &it_algorithm : instance.second){
        long HV = 0;
        for(auto &seed : it_algorithm.second){

            HV += seed.second;
            //cout << seed.second << " ";
        }
        cout << HV/it_algorithm.second.size() << " ";
        }

        cout << reference_points[instance.first].first << ";" << reference_points[instance.first].second;

        cout << endl;
    }

    cout << endl;
    cout << endl;

    //Imprimir os resultados sumarizados
    cout << "Resultados completos" << endl << endl;

    //Cabeçalho
    cout << "Algoritmo ";
    for(auto &it_algorithm : sets.begin()->second){

        for(auto &it_seed : it_algorithm.second){
            cout << " " << it_algorithm.first << " ";
        }
    }

    cout << endl;

    cout << "Instance\\Seed ";
    for(auto &it_algorithm : sets.begin()->second){

        for(auto &it_seed : it_algorithm.second){
            cout << " " << it_seed.first << " ";
        }

    }
    cout << endl;

    //Corpo
    for(auto &instance : hypervolume){

        cout << instance.first << " " ;


        for(auto &it_algorithm : instance.second){
            long HV = 0;
            for(auto &seed : it_algorithm.second){

                HV = seed.second;
                //cout << seed.second << " ";
                cout << HV << " ";
            }

        }

        cout << endl;
    }
}

void DiversityMetric(map<string,map<string, map<string, vector<pair<unsigned, double>>>>> sets,
                                map<string,map<string, map<string, double>>> &diversity)
{

    cout << endl << endl << "----------------Diversidade-----------------" << endl;
    //Calcular a diversidade para todas as instâncias em sets
    CalculateDiversitySet(sets, diversity);

    cout << setprecision(10);

    //Imprimir os resultados sumarizados
    cout << "Resultados sumarizados" << endl << endl;
    //Cabeçalho
    cout << "Instance ";
    for(auto &it_algorithm : sets.begin()->second){
        cout << " " << it_algorithm.first << " ";
    }
    cout << endl;

    //Corpo
    for(auto &instance : diversity){

        cout << instance.first << " " ;


        for(auto &it_algorithm : instance.second){
        double HV = 0;
        for(auto &seed : it_algorithm.second){

            HV += seed.second;
            //cout << seed.second << " ";
        }
        cout << HV/it_algorithm.second.size() << " ";
        }

        cout << endl;
    }

    cout << endl;
    cout << endl;

    //Imprimir os resultados sumarizados
    cout << "Resultados completos" << endl << endl;

    //Cabeçalho
    cout << "Algoritmo ";
    for(auto &it_algorithm : sets.begin()->second){

        for(auto &it_seed : it_algorithm.second){
            cout << " " << it_algorithm.first << " ";
        }
    }

    cout << endl;

    cout << "Instance\\Seed ";
    for(auto &it_algorithm : sets.begin()->second){

        for(auto &it_seed : it_algorithm.second){
            cout << " " << it_seed.first << " ";
        }

    }
    cout << endl;

    //Corpo
    for(auto &instance : diversity){

        cout << instance.first << " " ;


        for(auto &it_algorithm : instance.second){
            double HV = 0;
            for(auto &seed : it_algorithm.second){

                HV = seed.second;
                //cout << seed.second << " ";
                cout << HV << " ";
            }

        }

        cout << endl;
    }

}
