#ifndef MOGA_H
#define MOGA_H

#include <limits>
#include "set_solution.h"
#include <algorithm>
#include "genetic_basic.h"
#include "speaii.h"

void moga(algorithm_data alg_data, vector<GASolution*> &p, Timer *t1);
void SortByFitness(vector<GASolution *> &set_solution);
void Selection(vector<GASolution *> &p, unsigned population_size);
void Union(vector<GASolution *> &a, vector<GASolution *> &b);
void ComputeFitness(vector<GASolution *> &p, pair<double,double> w);

#endif // MOGA_H
