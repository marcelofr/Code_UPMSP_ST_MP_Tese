#ifndef LOCALSEARCH_BASIC_H
#define LOCALSEARCH_BASIC_H

#include "solution.h"
#include "set_solution.h"

#define QTD_NEIGHBOR 5

class LSSolution : public Solution{
public:
    bool was_visited;
    vector<unsigned> removed_jobs;

    LSSolution():Solution(){
        was_visited = false;
    }

    LSSolution* Create(){
        return new LSSolution();
    }

    ~LSSolution(){
        removed_jobs.clear();
    }

    LSSolution& operator=(const LSSolution &s){
        Solution::operator=(s);

        this->was_visited = s.was_visited;
        this->removed_jobs = s.removed_jobs;
        return *this;
    }
    //bool operator <(const Solution& s) ;
    //bool operator ==(const Solution& s) ;

    void RemovingJob(unsigned machine, unsigned job_position){
        auto it = this->scheduling[machine].begin()+job_position;

        this->removed_jobs.push_back(*it);

        this->H1[*it] = 0;
        this->job_start_time1[*it] = 0;
        this->job_end_time1[*it] = 0;
        this->job_mode_op[*it] = 0;

        this->scheduling[machine].erase(it);

    }
};


bool SwapInsideLS_FI(LSSolution *my_solution, NDSetSolution<LSSolution*> &non_dominated_set);
bool SwapInsideLS_BI(LSSolution *my_solution, NDSetSolution<LSSolution*> &non_dominated_set_out);
bool SwapOutsideLS_FI(LSSolution *my_solution, NDSetSolution<LSSolution*> &non_dominated_set);
bool SwapOutsideLS_BI(LSSolution* my_solution, NDSetSolution<LSSolution*> &non_dominated_set);

bool InsertInsideLS_FI(LSSolution *my_solution, NDSetSolution<LSSolution*> &non_dominated_set);
bool InsertInsideLS_BI(LSSolution* my_solution, NDSetSolution<LSSolution *> &non_dominated_set);
bool InsertOutsideLS_FI(LSSolution *my_solution, NDSetSolution<LSSolution*> &non_dominated_set);
bool InsertOutsideLS_BI(LSSolution *my_solution, NDSetSolution<LSSolution *> &non_dominated_set);

bool ChangeOpModeLS_FI(LSSolution *my_solution, NDSetSolution<LSSolution*> &non_dominated_set);
bool ChangeOpModeLS_BI(LSSolution *my_solution, NDSetSolution<LSSolution *> &non_dominated_set);
bool ChangeHLS(LSSolution *my_solution, vector<LSSolution*> &non_dominated_set);

//bool AddSolution(LSSolution *my_solution, vector<LSSolution*> &non_dominated_set);

LSSolution *SelectNonDomintatedSolution(NDSetSolution<LSSolution*> &non_dominated_set);
unsigned SelectNonVisitedSolution(NDSetSolution<LSSolution *> non_dominated_set);
unsigned ExistNonDomintatedSolution(NDSetSolution<LSSolution *> non_dominated_set);
LSSolution* GenRandomNeighborSol(LSSolution *my_solution);
LSSolution *GenNeighborSol(LSSolution *my_solution, unsigned op_neighbor);

//void Shaking(NDSetSolution<LSSolution *> *non_dominated_set_in, NDSetSolution<LSSolution *> *non_dominated_set_out, unsigned op_neighbor, unsigned level);
void Shaking(LSSolution *solution, unsigned op_neighbor, unsigned level);
//void Shaking_Eduardo(NDSetSolution<LSSolution *> *non_dominated_set_in, NDSetSolution<LSSolution *> *non_dominated_set_out, unsigned op_neighbor, unsigned level);
void Shaking(LSSolution *cur_solution, unsigned level);

LSSolution * Destruction(LSSolution * solution, unsigned level);
bool Construction(LSSolution * partial_solution,NDSetSolution<LSSolution *> *non_dominated_set_partial_a, vector<unsigned> removed_jobs);
bool IntesificationArroyo(LSSolution *current_solution, NDSetSolution<LSSolution *> &non_dominated_set, unsigned level);

void SortByMakespanLSSolution(vector<LSSolution*> &set_solution);


#endif // LOCALSEARCH_BASIC_H
