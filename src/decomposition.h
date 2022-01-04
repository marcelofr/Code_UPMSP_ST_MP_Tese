#ifndef DECOMPOSITION_H
#define DECOMPOSITION_H

#include "monosolution.h"
#include "set_solution.h"
#include <algorithm>
#include "common_basic.h"



bool SwapInsideLSMono_FI(MonoSolution *my_solution);
bool SwapInsideLSMono_BI(MonoSolution *my_solution);
bool SwapOutsideLSMono_FI(MonoSolution *my_solution);
bool SwapOutsideLSMono_BI(MonoSolution *my_solution);
bool InsertInsideLSMono_FI(MonoSolution *my_solution);
bool InsertInsideLSMono_BI(MonoSolution *my_solution);
bool InsertOutsideLSMono_FI(MonoSolution *my_solution);
bool InsertOutsideLSMono_BI(MonoSolution *my_solution);
bool ChangeOpModeLSMono_FI(MonoSolution *my_solution);
bool ChangeOpModeLSMono_BI(MonoSolution *my_solution);

bool CompareMakespanMonoSolution(MonoSolution *& l, MonoSolution *& r);
void SortByMakespanMonoSolution(vector<MonoSolution*> &set_solution);

void SetWeights(NDSetSolution<MonoSolution *> non_dominated_set);
void GenerateWeightVector(vector<pair<double, double>> &W, unsigned num_weights);

MonoSolution * Destruction(MonoSolution * solution, unsigned level);
void Construction(MonoSolution * partial_solution, vector<unsigned> removed_jobs);
bool IntesificationArroyo(MonoSolution *current_solution, unsigned level);

MonoSolution * Shaking(MonoSolution *cur_solution, unsigned op_neighbor, unsigned level);
MonoSolution *GenNeighborSol(MonoSolution *my_solution, unsigned op_neighbor);

void MOVNS_D(NDSetSolution<MonoSolution *> &non_dominated_set, algorithm_data alg_data, Timer *t1);
bool LS_Mono_BI(MonoSolution *my_solution, unsigned op_neighbor);


#endif // DECOMPOSITION_H
