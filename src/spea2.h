#ifndef SPEA2_H
#define SPEA2_H

#include <limits>
#include "set_solution.h"
#include <algorithm>

void FitnessAssignment(vector<GASolution *> &p, vector<GASolution *> &a);
void spea2(algorithm_data alg_data, vector<GASolution*> &p, Timer *t1);
void EnvironmentalSelection(vector<GASolution *> &new_a, vector<GASolution *> &p,
                            vector<GASolution *> &a, unsigned population_size);
void BinaryTournamentSelection(vector<GASolution *> a_new, vector<GASolution *> &m, int population_size);

#endif // SPEA2_H
