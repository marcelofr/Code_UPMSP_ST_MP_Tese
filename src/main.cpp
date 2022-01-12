#include <iostream>
#include <run_algorithm.h>
#include "generate_intances.h"

using namespace std;

int main(int argc, char** argv)
{

    algorithm_data alg_data;

    if(argc > 1){

        alg_data.param.instance_folder = argv[1];
        alg_data.param.instance_name = argv[2];
        alg_data.param.instance_extension = argv[3];
        alg_data.param.instance_file = alg_data.param.instance_folder + alg_data.param.instance_name + alg_data.param.instance_extension;
        alg_data.param.s_seed = argv[4];
        alg_data.param.algorithm_name = argv[5];
        alg_data.param.folder_solution = argv[6];
        alg_data.param.s_max_time_factor = argv[7];
        if(alg_data.param.algorithm_name == "NSGAII" ||
                alg_data.param.algorithm_name == "SPEA2" ||
                alg_data.param.algorithm_name == "MOGA" ||
                alg_data.param.algorithm_name == "NSGAI"){
            alg_data.param.s_population_size = argv[8];
            alg_data.param.s_prob_mutation = argv[9];
        }
        else if(alg_data.param.algorithm_name == "MOVNS_Arroyo"){
            alg_data.param.s_destruction_factor = argv[8];
        }
        else if(alg_data.param.algorithm_name == "MOVNS_D"){
            alg_data.param.s_decomposition_size = argv[8];
            alg_data.param.s_decomposition_neighboor_size = argv[9];
        }
        else if(alg_data.param.algorithm_name == "EXACT"){
            alg_data.param.s_alpha = argv[8];
        }


        //Param.file_solution = Param.folder_solution + Param.algorithm + "_" + Param.instance_name + ".sol";
        alg_data.param.file_solution = alg_data.param.folder_solution + alg_data.param.algorithm_name + "_" + alg_data.param.instance_name + "_" + alg_data.param.s_seed + ".sol";

    }
    else{

        Instance::discretization_factor = 10;

        alg_data.param.instance_folder = "../../Instances/SMALL/"; alg_data.param.instance_name = "6_2_1439_3_S_1-9";
        //alg_data.param.instance_folder = "../../Instances/SMALL_2022_01_12/"; alg_data.param.instance_name = "6_2_1439_3_S_1-9";
        //alg_data.param.instance_folder = "../../Instances/SMALL/"; alg_data.param.instance_name = "9_2_1439_3_S_1-9";
        //alg_data.param.instance_folder = "../../Instances/LARGE/"; alg_data.param.instance_name = "50_20_1439_5_S_1-9";
        //alg_data.param.instance_folder = "../../Instances/LARGE/"; alg_data.param.instance_name = "250_10_1439_5_S_1-124";
        //alg_data.param.instance_folder = "../../Instances/LARGE/"; alg_data.param.instance_name = "750_10_1439_5_S_1-124";
        //alg_data.param.instance_folder = "../../Instances/LARGE/"; alg_data.param.instance_name = "750_10_1439_5_S_1-9";

        alg_data.param.instance_extension = ".dat";
        alg_data.param.instance_file = alg_data.param.instance_folder + alg_data.param.instance_name + alg_data.param.instance_extension;

        //alg_data.param.s_seed = "50396";
        //alg_data.param.s_seed = "6241";
        alg_data.param.s_seed = "45433";

        alg_data.param.algorithm_name = "MOVNS_D";
        //alg_data.param.algorithm_name = "NSGAII"; //nsga-ii
        //alg_data.param.algorithm_name = "EXACT";
        //alg_data.param.algorithm_name = "SPEA2";
        //alg_data.param.algorithm_name = "MOGA";
        //alg_data.param.algorithm_name = "NSGAI";

        //alg_data.param.algorithm_name = "CalculateMetric";
        //alg_data.param.algorithm_name = "GENERATE_SMALL_INSTANCES";
        //alg_data.param.algorithm_name = "GENERATE_LARGE_INSTANCES";

        alg_data.param.s_max_time_factor = "2000";
        alg_data.param.folder_solution = "../../Solutions/SMALL_11_01_2022_21_08/";
        //alg_data.param.folder_solution = "../../Solutions/LARGE_8/";
        //alg_data.param.folder_solution = "../../Solutions/Test/";
        alg_data.param.file_solution = alg_data.param.folder_solution + alg_data.param.algorithm_name + "_"
               + alg_data.param.instance_name + "_" + alg_data.param.s_seed +  ".sol";
        alg_data.param.s_alpha = "0.0001";
        alg_data.param.s_population_size = "110";
        alg_data.param.s_prob_mutation = "5";

        alg_data.param.s_decomposition_size = "40";
        alg_data.param.s_decomposition_neighboor_size = "8";

        alg_data.param.s_destruction_factor = "10";

    }

    //Fazer as conversões de dados
    alg_data.param.u_seed = unsigned(atoi(alg_data.param.s_seed.c_str()));
    alg_data.param.u_max_time_factor = unsigned(atoi(alg_data.param.s_max_time_factor.c_str()));
    //GA
    alg_data.param.u_population_size = unsigned(atoi(alg_data.param.s_population_size.c_str()));
    alg_data.param.u_prob_mutation = unsigned(atoi(alg_data.param.s_prob_mutation.c_str()));
    //EXACT
    alg_data.param.d_alpha = atof(alg_data.param.s_alpha.c_str());
    //MOVNS_Arroyo
    alg_data.param.u_destruction_factor = unsigned(atoi(alg_data.param.s_destruction_factor.c_str()));
    //MOVNS_D
    alg_data.param.u_decomposition_size = unsigned(atoi(alg_data.param.s_decomposition_size.c_str()));
    alg_data.param.u_decomposition_neighboor_size = unsigned(atoi(alg_data.param.s_decomposition_neighboor_size.c_str()));


    if(alg_data.param.algorithm_name == "CalculateMetric"){
        CalculateMetric(alg_data.param.folder_solution);
    }
    else if(alg_data.param.algorithm_name == "GENERATE_SMALL_INSTANCES"){
        GenerateSmallInstances();

    }
    else if(alg_data.param.algorithm_name == "GENERATE_LARGE_INSTANCES"){
        GenerateLargeInstances();
    }
    else{
        RunAlgorithm(alg_data);
    }


    return 0;
}
