#ifndef MONOSOLUTION_H
#define MONOSOLUTION_H

#include "localsearch_basic.h"

class MonoSolution : public LSSolution{
public:
    double objective_funtion;
    pair<double, double> weights;

    MonoSolution();

    MonoSolution (const MonoSolution &s);

    ~MonoSolution();

    MonoSolution* Create();

    MonoSolution& operator=(const MonoSolution &s);

    bool operator <(const MonoSolution& s);

    bool operator ==(const MonoSolution& s);

    void CalculeMonoObjective();

    void CalculeMonoObjectiveTchebycheff();

    void GenerateGreedySolutionWeigth();
    void GenerateGreedySolutionWeigth_Caro();

    unsigned FindJobBestPosMacWeigth(unsigned new_job, unsigned machine,
                                     double &smaller_weigth_obj_machine,
                                     unsigned &diff_time_job)
    ;

    void GreedyChoiceWeigth(unsigned new_job, unsigned mode_op, unsigned &best_machine, unsigned &best_position, unsigned &diff_time, double &obj_job)
    ;

};

#endif // MONOSOLUTION_H
