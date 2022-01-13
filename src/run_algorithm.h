#ifndef RUN_ALGORITHM_H
#define RUN_ALGORITHM_H


#include <map>
//#include "../common/instance.h"
#include "common_basic.h"
//#include "set_solution.h"
#include "nsgaii.h"
#include "monosolution.h"
#include "math_model.h"
#include "spea2.h"
#include "moga.h"
#include "metric.h"
#include "nsgai.h"
//#include "localsearch_basic.h"
#include "localsearch.h"
#include "decomposition.h"

using namespace std;

void RunAlgorithm(algorithm_data alg_data);

void SalveFileSolution(algorithm_data alg_data);
//void SalveReferenceSolution(vector<pair<unsigned, double>> non_dominated_set, string folder_solution, string instance_name, string algorithm_name);
void SelectOnlyValidSolutions(vector<Solution*> non_dominated_set);

//void GenerateReferenceSet(string folder_solution, map<string, map<string, vector<pair<unsigned, double>>>> sets);
void CalculateMetric(string folder_solution);
void HypervolumeMetric(map<string,map<string, map<string, vector<pair<unsigned, double>>>>> sets,
                                map<string,map<string, map<string, double>>> &hypervolume,
                                map<string, pair<unsigned, double>> reference_points);
void DiversityMetric(map<string,map<string, map<string, vector<pair<unsigned, double>>>>> sets,
                                map<string,map<string, map<string, double>>> &diversity);

void Discretize(unsigned factor);

void RunAlgorithmNSGAII(algorithm_data alg_data, vector<Solution*> &non_dominated_set, Timer *t1);
void RunAlgorithmExact(algorithm_data alg_data, vector<Solution*> &non_dominated_set);
void RunAlgorithmSPEA2(algorithm_data alg_data, vector<Solution*> &non_dominated_set, Timer *t1);
void RunAlgorithmMOGA(algorithm_data alg_data, vector<Solution*> &non_dominated_set, Timer *t1);
void RunAlgorithmNSGAI(algorithm_data alg_data, vector<Solution*> &non_dominated_set, Timer *t1);

void RunAlgorithmMOVNS(algorithm_data alg_data, vector<Solution*> &non_dominated_set, Timer *t1);
void RunAlgorithmMOVNSArroyo(algorithm_data alg_data, vector<Solution*> &nd_set_solution, Timer *t1);
//void RunAlgorithmMOVNSEduardo(algorithm_data alg_data, vector<Solution*> &non_dominated_set, Timer *t1);
void RunAlgorithmMOVNSD(algorithm_data alg_data, vector<Solution*> &non_dominated_set, Timer *t1);

#endif // RUN_ALGORITHM_H
