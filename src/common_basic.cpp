#include "common_basic.h"

void MakeTrace(){

    ofstream MyFile(".trace.log", ofstream::app);

    auto t = time(nullptr);
    auto tm = *localtime(&t);

    MyFile << "Inicio: " << put_time(&tm, "%d-%m-%Y %H-%M-%S") << endl;
}

string itos(int i) {
   stringstream s;
   s << i;
   return s.str();
}

string itos(size_t i) {
    stringstream s;
    s << i;
    return s.str();
}

string itos(unsigned i) {
    stringstream s;
    s << i;
    return s.str();
}

string dtos(double i) {
    stringstream s;
    s << i;
    return s.str();
}

void init(int uf[], int N ) {
    for(int i = 0; i < N; i++)
        uf[i] = -1;
}

void init(vector<int> uf)
{
    for(auto i = uf.begin(); i < uf.end(); i++)
        *i = -1;
}

/* Union of sets containing x and y */
void Union( int uf[],int x, int y ) {
    x = Find(uf, x);
    y = Find(uf, y);

    if(x != y) {
        if(x < y) {
            uf[y] = x;
        }
        else {
            uf[x] = y;
        }
    }
}

/* Find root of set containing x */
int Find( int uf[], int x ) {
    int r = x;

    /* Find root */
    while(uf[r] >= 0)
        r = uf[r];

    return r;
}


/*
 * Método para adicionar um novo ponto ao conjunto não-dominado, caso possíveis:
 * 1 - O novo ponto é dominado então ela não será adicionada
 * 2 - A nova solução é não-dominada então ela será adicionada
 * 3 - A solução domina então ela será adicionada e as outras serão removidas
 * Se conseguiu adicionar my_solution, então retorna verdadeiro
 */
bool AddPoint(pair<unsigned, double> point, vector<pair<unsigned, double>> &non_dominated_set)
{

    //caso 1
    for(auto it_sol = non_dominated_set.begin(); it_sol != non_dominated_set.end(); ++it_sol){
        //Se point é dominada por alguma solução do conjunto
        if(it_sol->first <= point.first && it_sol->second <= point.second){
            return false;
        }
    }

    //Caso 3
    for(auto it_sol = non_dominated_set.begin(); it_sol != non_dominated_set.end();){
        //Se point domina alguma solução do conjunto
        if(point.first <= it_sol->first && point.second <= it_sol->second){
            //Remover essa solução do conjunto
            it_sol = non_dominated_set.erase(it_sol);
        }
        else{
            ++it_sol;
        }
    }
    //Caso 2 e 3
    non_dominated_set.push_back(point);
    return true;
}

bool CompareMakespanPoint(pair<unsigned, double> & l, pair<unsigned, double> & r) //(2)
{
    if(l.first < r.first){
        return true;
    }
    else if (l.first > r.first){
        return false;
    }
    else{
        if(l.second < r.second){
            return true;
        }
        else{
            return false;
        }
    }

}

void SortByMakespan(vector<pair<unsigned, double>> &set_solution)
{
    sort(set_solution.begin(), set_solution.end(), CompareMakespanPoint);
}

double CalcEuclideanDistance(pair<double, double> cof_weighted, pair<double, double> point_solution)
{

    double distance;

    distance = double(sqrt(pow(cof_weighted.first-point_solution.first,2)+
                      pow(cof_weighted.second-point_solution.second,2)));

    return distance;
}
