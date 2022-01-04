#ifndef SOLUTION_H
#define SOLUTION_H


#include "instance.h"
#include <vector>
#include <limits.h>
#include "common_basic.h"
#include <math.h>       // ceil
#include <algorithm>    // std::random_shuffle

using namespace std;

class Solution
{
public:
    //Objetivos
    unsigned makeSpan;
    double TEC;
    bool is_optimal;

    //Sequenciamento
    vector<vector<unsigned>> scheduling;

    //Instante de início de processamento de cada tarefa
    vector<unsigned> H1;

    //Instante de término de processamento de cada tarefa
    vector<unsigned> job_end_time1;

    //Instante de início de preparação de cada tarefa
    vector<unsigned> job_start_time1;

    //Modo de operação de cada tarefa
    vector<unsigned> job_mode_op;

    //Instante de conclusão de cada máquina
    vector<unsigned> machine_completion_time;

    //Custo de energia de cada máquina
    vector<double> machine_TEC;

    //double PEC_on, PEC_off;

    Solution();
    Solution (const Solution &s);
    ~Solution();
    Solution& operator=(const Solution &s);
    bool operator <(const Solution& s);
    bool operator ==(const Solution& s);
    bool operator <=(const Solution& s);


    void Print();
    void DummyInitialSolution();
    void RandomInitialSolution();
    void GenerateGreedySolutionMakespan();
    //void GreedyInitialSolutionTEC();
    //void GreedyInitialSolutionTEC2();
    void GenerateGreedySolutionTEC3();

    void CalculateShorterTimeHorizon(bool update_h=true);
    void CalculateShorterTimeHorizonMachine(unsigned machine);
    void CalculateHorizonAvoidingPeak();
    void CalculateHorizonAvoidingPeakMachine(unsigned machine);
    void CalculateObjective();
    void CalculateObjectiveDiscrete();
    void CalculateObjectiveMachine(unsigned machine);

    double CalcPECToJob(unsigned machine, unsigned job, unsigned h, bool discrete_time=false);

    void Check();

    unsigned FindJobBestPosMacMakespan(unsigned new_job, unsigned machine);
    unsigned FindJobBestPosMacTEC(unsigned new_job, unsigned machine, unsigned &best_op);
    unsigned FindJobBestPosMacTEC3(unsigned new_job, unsigned machine);


    void SelectBestModeOpJob(unsigned machine, unsigned position, unsigned new_mode_op);

    //Solução inicial
    void AddJobGreedyMakespan(unsigned new_job, unsigned mode_op);
    void GreedyChoiceMakespan(unsigned new_job, unsigned mode_op, unsigned &best_machine,
                              unsigned &best_position, unsigned &diff_time, unsigned &new_makespan);
    void RandomChoiceMakespan(unsigned new_job, unsigned mode_op, unsigned &best_machine, unsigned &best_position, unsigned &diff_time);
    void GreedyChoiceTEC(unsigned new_job, unsigned mode_op, unsigned &best_machine, unsigned &best_position, unsigned &diff_time, double &pec_job);
    void RandomChoiceTEC(unsigned new_job, unsigned mode_op, unsigned &best_machine, unsigned &best_position, unsigned &diff_time, double &pec_job);
    //void AddJobGreedyTEC(unsigned new_job, unsigned mode_op);
    void AddJobGreedyTEC2(unsigned new_job, unsigned mode_op);
    void AddJobGreedyTEC3(unsigned new_job, unsigned mode_op);
    void AddJobGreedyTECMachine3(unsigned machine, unsigned new_job, unsigned mode_op);

    //Cruzamento
    void AddJobGreedyMakespanMachine(unsigned machine, unsigned new_job, unsigned mode_op);
    //void AddJobGreedyTECMachine(unsigned machine, unsigned new_job, unsigned mode_op);

    //Mutação
    void InsertRandomPosition(unsigned new_job);

    void SwapInside(unsigned machine, unsigned job1, unsigned job2);
    void SwapInsideDelta(unsigned machine, unsigned job1, unsigned job2);
    void SwapOutside(unsigned machine1, unsigned pos_job1, unsigned machine2, unsigned pos_job2);
    void SwapOutsideDelta(unsigned machine1, unsigned pos_job1, unsigned machine2, unsigned pos_job2);
    void InsertInside(unsigned machine, unsigned pos1, unsigned pos2);
    void InsertInsideDelta(unsigned machine, unsigned pos1, unsigned pos2);
    void InsertOutside(unsigned machine1, unsigned pos1, unsigned machine2, unsigned pos2);
    void InsertOutsideDelta(unsigned machine1, unsigned pos1, unsigned machine2, unsigned pos2);
    void ChangeModeOpJob(unsigned machine, unsigned position, unsigned new_mode_op);
    void ChangeModeOpJobDelta(unsigned machine, unsigned position, unsigned new_mode_op);
    void ChangeHJob(unsigned machine, unsigned position, unsigned add_h);

    unsigned GetFastestOpMode();
    unsigned GetCheapestOpMode();
    unsigned GetMakespanMachine();
    void AddJob(unsigned new_job, unsigned machine, unsigned positon, unsigned diff_time);


    double CalcEuclideanDistance(Solution *s)
    {

        double distance;

        int a;
        double b;

        a = int(this->makeSpan-s->makeSpan);
        b = double(this->TEC-s->TEC);

        distance = double(sqrt(pow(a,2)+pow(b,2)));

        return distance;
    }
};

bool CompareMakespan(Solution * l, Solution * r);
bool CompareTEC(Solution * l, Solution * r);
//void SortByMakespanSolution(vector<Solution*> &set_solution);

class Z_STAR
{
public:
    static double makespan;
    static double TEC;
};

#endif // SOLUTION_H
