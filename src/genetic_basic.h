#ifndef GENETIC_BASIC_H
#define GENETIC_BASIC_H


#include "gasolution.h"
#include <algorithm>    // std::sort

//#define POPULATION_SIZE 100
//#define NEW_POPULATION_SIZE 100
//#define PROBABILITY_CROSSOVER 80
//#define TAM_CROSSOVER 98
//#define PROBABILITY_MUTATION 5

void GenerateInitialPopulation(vector<GASolution*> &population, unsigned tam_population);
bool PopulationAddIndividual(vector<GASolution*> &population, GASolution *individual);
void PrintPopulation(vector<GASolution*> &population);
void Crossover(vector<GASolution*> &population, vector<GASolution*> &new_population, unsigned population_size);
void Mutation(vector<GASolution*> &population, vector<GASolution*> &new_population, unsigned prob_mutation);

void GenerateOffspring3(GASolution *parent1, GASolution *parent2,
                       GASolution *offspring1, GASolution *offspring2);
void MutationSwapInside(GASolution &individual);
void MutationSwapOutside(GASolution &individual);
void MutationInsertInside(GASolution &individual);
void MutationInsertOutside(GASolution &individual);
void MutationChangeOpMode(GASolution &individual);
void MutationChangeH(GASolution &individual);
void SolutionListToVector(GASolution s, vector<unsigned> &v_solution);

/*void SortByMakespan(vector<GASolution*> &population);
void SortByTEC(vector<GASolution*> &population);*/

void UnionPopulation(vector<GASolution*> &R, vector<GASolution*> &P, vector<GASolution*> &Q);

bool CompareFitness(GASolution * l, GASolution * r);
void SortByFitness(vector<GASolution *> &set_solution);

#endif // GENETIC_BASIC_H
