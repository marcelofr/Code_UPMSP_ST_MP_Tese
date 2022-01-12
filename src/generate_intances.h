#ifndef GENERATE_INTANCES_H
#define GENERATE_INTANCES_H

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <filesystem>

#include "common_basic.h"
#include "instance.h"
#include "solution.h"


#define PEAK_START 1080
#define PEAK_END 1259
#define RATE_IN_PEAK 0.47753
#define RATE_OFF_PEAK 0.32282
#define PLAN_HORIZON_SIZE 1439
#define MAX_COST 999
//#define DESTINATION_FOLDER "../../Instances/SMALL_2020_07_24/"
#define DESTINATION_FOLDER_SMALL "../../Instances/SMALL_2022_01_12/"
#define DESTINATION_FOLDER_LARGER "../../Instances/LARGER_2022_01_12/"



using namespace std;

struct InstanceParam{
    unsigned num_jobs, num_machines, num_days, num_op_mode;
    vector<double> speed_factor, consumption_factor;
    vector<unsigned> machine_potency;
    string destination_folder;
    pair<unsigned, unsigned>range_setup, range_processing, range_potency;
};

void GenerateSmallInstances();
void GenerateLargeInstances();

void CreateNewIntance(InstanceParam ip);
void GenerateInstanceData(InstanceParam ip);
void SaveInstanceDataToFile(InstanceParam ip);

#endif // GENERATE_INTANCES_H
