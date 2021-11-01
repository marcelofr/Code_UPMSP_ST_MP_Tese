#include "metric.h"

void CalculateHypervolumeSet(map<string,map<string, map<string, vector<pair<unsigned, double>>>>> sets,
                          map<string,map<string, map<string, double>>> &hypervolume,
                          map<string, pair<unsigned, double>> reference_points)
{

    algorithm_data alg_data;
    double hv;

    //Para cada instÃ¢ncia
    for(auto &it_instance: sets){

        for(auto &it_algorithm : it_instance.second){

            //Para cada semente
            for(auto &it_seed : it_algorithm.second){
                //Calcular o hipervolume para cada soluÃ§Ã£o e armazenar
                //SortByMakespan(sd.non_dominated_set);
                hv = CalculateHypervolume(it_seed.second, reference_points[it_instance.first]);
                hypervolume[it_instance.first][it_algorithm.first].insert({it_seed.first, hv});
            }

        }

    }

}

void CalculateDiversitySet(map<string,map<string, map<string, vector<pair<unsigned, double>>>>> sets,
                          map<string,map<string, map<string, double>>> &diversity)
{

    algorithm_data alg_data;
    double dvst;
    pair<unsigned, double> first_extreme_point, second_extreme_point;

    //Para cada instÃ¢ncia
    for(auto &it_instance: sets){

        //first_extreme_point = pair<unsigned, double>(5,2);
        //first_extreme_point = it_instance.second.end()->second.end()->second.front();
        first_extreme_point = it_instance.second["ref_set"]["all"].front();
        second_extreme_point = it_instance.second["ref_set"]["all"].back();

        for(auto &it_algorithm : it_instance.second){

            //Para cada semente
            for(auto &it_seed : it_algorithm.second){
                //Calcular a diversidade para cada soluÃ§Ã£o e armazenar
                dvst = CalculateDiversity(it_seed.second, first_extreme_point, second_extreme_point);
                diversity[it_instance.first][it_algorithm.first].insert({it_seed.first, dvst});
            }

        }

    }

}

void ReadFiles(vector<string> files,
               map<string,map<string, map<string, vector<pair<unsigned, double>>>>> &sets){

    algorithm_data alg_data;

    //Ordenar o vetor de arquivos
    sort(files.begin(), files.end());

    //Ler cada arquivo de soluÃ§Ã£o
    for(auto &it : files){
        alg_data.param.file_solution = it;
        alg_data.non_dominated_set.clear();
        ReadFile(alg_data);

        //Montar a estrutura com todas as instÃ¢ncias e seus conjunto de soluÃ§Ãµes
        //sets[alg_data.param.instance_name].insert({alg_data.param.s_seed, alg_data.non_dominated_set});
        sets[alg_data.param.instance_name][alg_data.param.algorithm_name][alg_data.param.s_seed] = alg_data.non_dominated_set;

        //sets[alg_data.param.instance_name][alg_data.param.s_seed].insert(sets[alg_data.param.instance_name][alg_data.param.s_seed].begin(), alg_data.non_dominated_set.begin(), alg_data.non_dominated_set.end());

    }
}

void GenerateReferenceSet(string folder_solution,
                          map<string,map<string, map<string, vector<pair<unsigned, double>>>>> &sets,
                          map<string,map<string, map<string, double>>> &hypervolume_ref,
                          map<string, pair<unsigned, double>> &reference_points){

    double hv;
    vector<pair<unsigned, double>> non_dominated_set;

    pair<unsigned, double> reference_point;

    for(auto &it_instance: sets){

        //cout << instance.first << "\t";

        //Iniciar o ponto de referÃªncia com zero, mas ele precisa ser atualizado
        reference_point.first = reference_point.second = 0;
        non_dominated_set.clear();

        for(auto &it_algorithm : it_instance.second){
        for(auto &it_seed: it_algorithm.second){
            for(auto &point : it_seed.second){
                //cout << point.first << "\t" << point.second << endl;
                AddPoint(point, non_dominated_set);

                if(point.first > reference_point.first){
                    reference_point.first = point.first;
                }
                if(point.second > reference_point.second){
                    reference_point.second = point.second;
                }
            }
        }
        }

        reference_points[it_instance.first] = reference_point;

        SortByMakespan(non_dominated_set);
        SalveReferenceSolution(non_dominated_set, folder_solution, it_instance.first, reference_point);

        //Inserir o conjunto de referÃªncia em sets
        sets[it_instance.first]["ref_set"].insert({"all", non_dominated_set});

        //Inserir o ponto de referÃªncia em reference_points
        //reference_points.insert({instance.first, reference_point});

        hv = CalculateHypervolume(non_dominated_set, reference_point);
        //cout << "Hipervolume reference: " << hv << endl;

        hypervolume_ref[it_instance.first]["ref_set"].insert({"all", hv});

    }

}

void SalveReferenceSolution(vector<pair<unsigned, double>> non_dominated_set,
                            string folder_solution, string instance_name,
                            pair<unsigned, double> reference_point){

    ofstream MyFile;

    string file_name;

    file_name = folder_solution + "Ref_" + instance_name + ".ref";

    //Abrir o arquivo
    MyFile.open(file_name);

    MyFile << "Instance: " << instance_name << endl;
    //MyFile << "Algorithm: " << algorithm_name << endl;
    MyFile << "Max_makespan: " << reference_point.first << endl;
    MyFile << "Max_PEP: " << reference_point.second << endl;

    MyFile << endl;

    MyFile << "Makespan" << "\t" << "TEC";

    for (auto it=non_dominated_set.begin(); it != non_dominated_set.end();++it) {
        MyFile << endl << it->first << "\t" << it->second;
    }
    MyFile << "\t" << "END";

    MyFile.close();
}

/*
 * Método para calcular o hipervolume para problemas de minimização com dois objetivos
 * Considera que o conjunto está ordenado pelo makespan, crescente
 */
double CalculateHypervolume(vector<pair<unsigned, double>> set_solution, pair<unsigned, double> reference)
{

    double hypervolume, b, h, h_last;

    auto it = set_solution.begin();

    b = reference.first - it->first;
    h = reference.second - it->second;

    hypervolume = b * h;

    h_last = it->second;

    for (++it; it != set_solution.end();++it) {

        b = reference.first - it->first;
        h = h_last - it->second;

        hypervolume += b * h;

        h_last = it->second;
    }

    return hypervolume;
}

/*
 * Método para calcular a diversidade para problemas de minimização com dois objetivos
 * Considera que o conjunto está ordenado pelo makespan, crescente
 */
double CalculateDiversity(vector<pair<unsigned, double>> set_solution,
                          pair<unsigned, double> first_extreme_point,
                          pair<unsigned, double> second_extreme_point)
{
    double sum1, sum2, avg, result, aux;

    aux = CalcEuclideanDistance(first_extreme_point, set_solution.front());
    sum1 = aux;
    sum2 = aux;

    avg = 0;
    for(unsigned index=0; index < set_solution.size()-1; index++){
        avg += CalcEuclideanDistance(set_solution[index], set_solution[index+1]);
    }
    avg = double(avg)/double(set_solution.size()-1);

    for(unsigned index=0; index < set_solution.size()-1; index++){
        aux = CalcEuclideanDistance(set_solution[index], set_solution[index+1]);
        sum1 += abs(aux - avg);
    }

    aux = CalcEuclideanDistance(set_solution.back(), second_extreme_point);
    sum1 += aux;
    sum2 += aux;

    sum2 += (set_solution.size()-1)*avg;

    result = double(sum1)/double(sum2);

    return result;

}

/*
 * Método para identificar os arquivos presentes em uma pasta e
 * salvar seus nomes em um vetor, considera apenas os arquivos com a extensão .sol
 */
void FindFilesInFolder(string folder_solution, vector<string> &files)
{
    int a;
    //Criar um vetor com o nome de todos os arquivos do diretório
    for (auto entry : filesystem::directory_iterator(folder_solution)){
        a = entry.path().string().find(".sol");
        if(a >= 0){
            files.push_back(entry.path());
        }
    }
}

/*
 * Método para ler um arquivo com um conjunto de soluções não-dominadas e
 * salvar os dados na estrutura ir
 */
void ReadFile(algorithm_data &alg_data)
{
    fstream file;
    string str;
    unsigned num;
    double d_num;
    pair<unsigned, double> p;

    file.open(alg_data.param.file_solution);

    //Instância
    file >> str;
    file >> str;
    alg_data.param.instance_name = str;

    //Nome do algoritmo
    file >> str;
    file >> str;
    alg_data.param.algorithm_name = str;

    //Tempo
    file >> str;
    file >> d_num;
    alg_data.time_limit = d_num;

    //Semente
    file >> str;
    file >> num;
    alg_data.param.u_seed = num;
    alg_data.param.s_seed = itos(num);

    //Tempo passado
    file >> str;
    file >> d_num;
    alg_data.elapsed_time_sec = d_num;

    if(alg_data.param.algorithm_name == "GA"){

        //Tamanho da população
        file >> str;
        file >> str;
        alg_data.param.u_population_size = num;

        //Probabilidade de mutação
        file >> str;
        file >> str;
        alg_data.param.u_prob_mutation = num;

        //param3
        file >> str;
        file >> str;
    }
    else if(alg_data.param.algorithm_name == "MOVNS_Arroyo"){

        //Tamanho da população
        file >> str;
        file >> num;
        alg_data.param.s_destruction_factor = num;

        //param2
        file >> str;
        file >> str;

        //param3
        file >> str;
        file >> str;
    }
    else if(alg_data.param.algorithm_name == "MOVNS_D"){
        //Tamanho da população
        file >> str;
        file >> num;
        alg_data.param.s_decomposition_size = num;

        //Probabilidade de mutação
        file >> str;
        file >> num;
        alg_data.param.s_decomposition_neighboor_size = num;

        //param3
        file >> str;
        file >> str;
    }
    else{
        //param1
        file >> str;
        file >> str;

        //param2
        file >> str;
        file >> str;

        //param3
        file >> str;
        file >> str;
    }



    //Pular o nome dos objetivos
    file >> str;
    file >> str;

    alg_data.non_dominated_set.clear();
    //Ler o conjunto não-dominado
    while (file >> p.first >> p.second)
    {
        alg_data.non_dominated_set.push_back(p);
        //Se tem espaço
        if(file.peek() == '\t'){
            //Ler a string END
            file >> str;

        }
        //Se final do arquivo
        if(file.peek() == EOF){
            break;
        }

    }

    file.close();
}
