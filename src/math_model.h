#ifndef MATH_MODEL_H
#define MATH_MODEL_H


#include <math.h>       /* ceil */
#include "gurobi_c++.h"
#include "solution.h"


class Model
{
protected:
    GRBModel *model;
    vector<vector<vector<vector<GRBVar>>>> X;
    //vector<vector<GRBVar>> Y;
    //vector<GRBVar> C;
    GRBVar CMax, PecOn, PecOff;

    vector<vector<vector<double>>> FinalSolution;
public:
    Model();
    ~Model();

    void Create(GRBModel *model, double MaxTime);
    void AddVar();
    void SetInitialSolutionToMathModel(Solution *MySolutionLS);
    void SetObjective(double alpha);
    void SetObjectiveTEP();
    void SetObjectiveMakespan();
    void SetConstraint();
    void SetConstraintWithMakespan(unsigned makespan);
    void SetConstraintWithTEP(double TEP);
    void Optimize();
    void PrintVars();

    void GetSolutionFromModel(Solution *MySolution);
};

void RunWeightedMathModel(double max_time, double alpha, Solution * my_solution);


#endif // MATH_MODEL_H
