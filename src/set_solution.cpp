#include "set_solution.h"

void SortByMakespan(vector<Solution*> &set_solution)
{
    sort(set_solution.begin(), set_solution.end(), CompareMakespan);
}

void SortByTEC(vector<Solution*> &set_solution)
{
    sort(set_solution.begin(), set_solution.end(), CompareTEC);
}


void SortByMakespan(vector<GASolution *> &set_solution)
{
    sort(set_solution.begin(), set_solution.end(), CompareMakespan);
}

void SortByTEC(vector<GASolution*> &set_solution)
{
    sort(set_solution.begin(), set_solution.end(), CompareTEC);
}
