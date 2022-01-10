#ifndef DECOMPOSITION_H
#define DECOMPOSITION_H

#include "monosolution.h"
#include "set_solution.h"
#include <algorithm>
#include "common_basic.h"

bool SwapInsideLSMonoFI(MonoSolution *neighbor_solution, const MonoSolution *best_solution);
bool SwapInsideLSMonoBI(MonoSolution *neighbor_solution);

bool SwapOutsideLSMonoFI(MonoSolution *neighbor_solution, const MonoSolution *best_solution);
bool SwapOutsideLSMonoBI(MonoSolution *neighbor_solution);

bool InsertInsideLSMonoFI(MonoSolution *neighbor_solution, const MonoSolution *best_solution);
bool InsertInsideLSMonoBI(MonoSolution *neighbor_solution);

bool InsertOutsideLSMonoFI(MonoSolution *neighbor_solution, const MonoSolution* best_solution);
bool InsertOutsideLSMonoBI(MonoSolution *neighbor_solution);

bool ChangeOpModeLSMonoFI(MonoSolution *neighbor_solution, const MonoSolution *best_solution);
bool ChangeOpModeLSMonoBI(MonoSolution *neighbor_solution);

bool CompareMakespanMonoSolution(MonoSolution *& l, MonoSolution *& r);
void SortByMakespanMonoSolution(vector<MonoSolution*> &set_solution);

//void SetWeights(NDSetSolution<MonoSolution *> non_dominated_set);
void SetWeights(vector<MonoSolution *> &set_solution);
void GenerateWeightVector(vector<pair<double, double>> &W, unsigned num_weights);

MonoSolution * Destruction(MonoSolution * solution, unsigned level);
void Construction(MonoSolution * partial_solution, vector<unsigned> removed_jobs);
bool IntesificationArroyo(MonoSolution *current_solution, unsigned level);

MonoSolution * Shaking(MonoSolution *cur_solution, unsigned op_neighbor, unsigned level);
MonoSolution *GenNeighborSol(MonoSolution *my_solution, unsigned op_neighbor);

void MOVNS_D(NDSetSolution<MonoSolution *> &non_dominated_set, algorithm_data alg_data, Timer *t1);
bool LS_Mono_BI(MonoSolution *my_solution, unsigned op_neighbor);
void MOVNS_D_Vivian(NDSetSolution<MonoSolution *> &non_dominated_set, algorithm_data alg_data, Timer *t1);
void UpdateZ_STAR(vector<MonoSolution *> &set_solution);

//--------------
void Crossover(vector<MonoSolution*> &population, vector<MonoSolution*> &new_population, unsigned population_size);
void GenerateOffspring3(MonoSolution *parent1, MonoSolution *parent2,
                        MonoSolution *offspring1, MonoSolution *offspring2);

#endif // DECOMPOSITION_H
