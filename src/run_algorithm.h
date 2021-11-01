#ifndef RUN_ALGORITHM_H
#define RUN_ALGORITHM_H


#include <map>
//#include "../common/instance.h"
#include "common_basic.h"
#include "set_solution.h"
#include "nsgaii.h"
#include "mono_solution.h"
#include "math_model.h"
#include "speaii.h"
#include "moga.h"
#include "metric.h"

using namespace std;

void RunAlgorithm(algorithm_data alg_data);

void SalveFileSolution(algorithm_data alg_data);
//void SalveReferenceSolution(vector<pair<unsigned, double>> non_dominated_set, string folder_solution, string instance_name, string algorithm_name);
void SelectOnlyValidSolutions(vector<Solution*> non_dominated_set);

//void GenerateReferenceSet(string folder_solution, map<string, map<string, vector<pair<unsigned, double>>>> sets);
void CalculateMetric(string folder_solution, string folder_instance);
void HypervolumeMetric(map<string,map<string, map<string, vector<pair<unsigned, double>>>>> sets,
                                map<string,map<string, map<string, double>>> &hypervolume,
                                map<string, pair<unsigned, double>> reference_points);
void DiversityMetric(map<string,map<string, map<string, vector<pair<unsigned, double>>>>> sets,
                                map<string,map<string, map<string, double>>> &diversity);

void Discretize(unsigned factor);

void RunAlgorithmNSGAII(algorithm_data alg_data, vector<Solution*> &non_dominated_set, Timer *t1);
void RunAlgorithmExact(algorithm_data alg_data, vector<Solution*> &non_dominated_set);
void RunAlgorithmSPEAII(algorithm_data alg_data, vector<Solution*> &non_dominated_set, Timer *t1);
void RunAlgorithmMOGA(algorithm_data alg_data, vector<Solution*> &non_dominated_set, Timer *t1);

#endif // RUN_ALGORITHM_H
