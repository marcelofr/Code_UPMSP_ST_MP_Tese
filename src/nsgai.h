#ifndef NSGAI_H
#define NSGAI_H


#include "nsgaii.h"

void nsga_i(algorithm_data alg_data, vector<GASolution*> &P, Timer *t1);
void ComputeFitness(vector<GASolution *> &p, vector<vector<GASolution*>> &F);

#endif // NSGAI_H
