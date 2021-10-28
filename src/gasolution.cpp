#include "gasolution.h"

GASolution::GASolution() : Solution(){

}

GASolution::GASolution (const GASolution &s):Solution(s){

    this->crowding_distance = s.crowding_distance;
    this->rank = s.rank;
    this->counter_solution_non_dominated = s.counter_solution_non_dominated;

    this->fitness = s.fitness;
    this->strength_value =s.strength_value ;
    this->raw_fitness = s.raw_fitness ;
    this->density = s.density;

    copy(s.set_solution_dominated.begin(), s.set_solution_dominated.end(), back_inserter(this->set_solution_dominated));
}

GASolution::~GASolution(){
    set_solution_dominated.clear();
}

GASolution* GASolution::Create(){
    return new GASolution();
}

GASolution& GASolution::operator=(const GASolution &s){

    Solution::operator=(s);

    this->crowding_distance = s.crowding_distance;
    this->rank = s.rank;
    this->counter_solution_non_dominated = s.counter_solution_non_dominated;

    //this->set_solution_dominated.clear();
    copy(s.set_solution_dominated.begin(), s.set_solution_dominated.end(), back_inserter(this->set_solution_dominated));

    return *this;
}

bool GASolution::operator <(const Solution& s) {

    return Solution::operator <(s);
}

bool GASolution::operator ==(const Solution& s) {

    return Solution::operator ==(s);
}

