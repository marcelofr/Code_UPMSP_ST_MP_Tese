#ifndef METRIC_H
#define METRIC_H


#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iostream>
#include "common_basic.h"
#include <filesystem>
//#include "common/file_solution.h"
//#include "hypervolume.h"
//#include "diversity.h"

using namespace std;

void CalculateHypervolumeSet(map<string,map<string, map<string, vector<pair<unsigned, double>>>>> sets,
                          map<string,map<string, map<string, double>>> &hypervolume,
                          map<string, pair<unsigned, double>> reference_points);
void CalculateDiversitySet(map<string,map<string, map<string, vector<pair<unsigned, double>>>>> sets,
                          map<string,map<string, map<string, double>>> &diversity);
void GenerateReferenceSet(string folder_solution,
                          map<string,map<string, map<string, vector<pair<unsigned, double>>>>> &sets,
                          map<string,map<string, map<string, double>>> &hypervolume_ref,
                          map<string, pair<unsigned, double>> &reference_points);
void SalveReferenceSolution(vector<pair<unsigned, double>> non_dominated_set, string folder_solution,
                            string instance_name, pair<unsigned, double> reference_point);
void ReadFiles(vector<string> files,
               map<string,map<string, map<string, vector<pair<unsigned, double>>>>> &sets);

double CalculateHypervolume(vector<pair<unsigned, double>> set_solution, pair<unsigned, double> reference);
double CalculateDiversity(vector<pair<unsigned, double>> set_solution,
                          pair<unsigned, double> first_extreme_point,
                          pair<unsigned, double> second_extreme_point);
void FindFilesInFolder(string folder_solution, vector<string> &files);
void ReadFile(algorithm_data &alg_data);

#endif // METRIC_H
