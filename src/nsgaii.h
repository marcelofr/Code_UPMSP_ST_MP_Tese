#ifndef NSGAII_H
#define NSGAII_H


#include <limits>
#include "set_solution.h"

void nsga_ii(algorithm_data alg_data, vector<GASolution*> &P, Timer *t1);
void FastNonDominatedSort(vector<vector<GASolution*>> &F, vector<GASolution*> &P);
void ComputeCrowdingDistance(vector<GASolution*> &F_i);
void Sort(vector<GASolution*> &F_i);

#endif // NSGAII_H
