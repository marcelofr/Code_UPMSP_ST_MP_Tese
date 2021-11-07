#ifndef NSGAI_H
#define NSGAI_H


#include "nsgaii.h"
#include "moga.h"

#define SHARE 0.5

void nsga_i(algorithm_data alg_data, vector<GASolution*> &P, Timer *t1);
void ComputeFitness(vector<vector<GASolution*>> &front, unsigned population_size);
void NonDominatedSort(vector<vector<GASolution*> > &front, vector<GASolution*> &pop);
void Selection(vector<vector<GASolution*>> &front, vector<GASolution*> &pop, unsigned population_size);

#endif // NSGAI_H
