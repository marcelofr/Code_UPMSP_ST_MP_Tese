#include "math_model.h"

Model::Model()
{

}

Model::~Model()
{
    delete model;
}

void Model::Create(GRBModel *model, double MaxTime)
{
    this->model = model;


    model->set(GRB_IntParam_OutputFlag, 0);

#ifdef DEBUG
    model->set(GRB_IntParam_OutputFlag, 1);
#endif

    model->set(GRB_IntParam_Threads, 1);
    //model->set(GRB_IntParam_LazyConstraints, 1);
    model->set(GRB_IntParam_Presolve, 0);
    model->set(GRB_DoubleParam_TimeLimit, MaxTime);
    //model->set(GRB_DoubleParam_TimeLimit, 300.0);
    model->set(GRB_DoubleParam_MIPGap, EPS);

}

void Model::AddVar()
{
    string str;

    //X_ijhl = 1 Variável binária que assume valor 1 se a tarefa j for alocada à máquina i
    //no instante de tempo h e no modo de operação l, e valor 0, caso contrário
    this->X = vector<vector<vector<vector<GRBVar>>>>
                (Instance::num_machine+1, vector<vector<vector<GRBVar>>>
                    (Instance::num_jobs+1, vector<vector<GRBVar>>
                        (Instance::num_planning_horizon+1, vector<GRBVar>
                            (Instance::num_mode_op+1))));

    for (size_t i = 1; i <= Instance::num_machine; i++) {
        for (size_t j = 1; j <= Instance::num_jobs; j++) {
            for (size_t h = 0; h <= Instance::num_planning_horizon; h++) {
                for (size_t l = 1; l <= Instance::num_mode_op; l++) {
                    str = "X[" + itos(i) + "][" + itos(j) + "][" + itos(h) + "][" + itos(l) + "]";
                    X[i][j][h][l] = model->addVar(0, 1, 0, GRB_BINARY, str);
                }
            }
        }
    }

    //Variável para o makespan
    CMax = model->addVar(0, GRB_INFINITY, 0, GRB_INTEGER, "CMax");

    //Variável para o PEC_on
    PecOn = model->addVar(0, GRB_INFINITY, 0, GRB_CONTINUOUS, "PecOn");

    //Variável para o PEC_off
    PecOff = model->addVar(0, GRB_INFINITY, 0, GRB_CONTINUOUS, "PecOff");

    // Integrar novas variáveis ao modelo
    model->update();
}

void Model::SetInitialSolutionToMathModel(Solution *MySolutionLS)
{
    string str;
    //unsigned size;
    for (unsigned i = 1; i <= Instance::num_machine; i++) {
        //size = MySolutionLS->scheduling[i].size();
        //Demais tarefas
        for (auto j = MySolutionLS->scheduling[i].begin(); j != MySolutionLS->scheduling[i].end(); ++j) {
            //str = "x[" + itos(i) + "][" + itos(MySolutionLS.scheduling[i][j-1]) + "][" + itos(MySolutionLS.scheduling[i][j]) + "]";
            //model->getVarByName(str).set(GRB_DoubleAttr_Start, 1);
            this->X[i][*j][MySolutionLS->H1[*j]][MySolutionLS->job_mode_op[*j]].set(GRB_DoubleAttr_Start, 1);
        }
    }
}

void Model::SetObjective(double alpha)
{
    // Set objective
    //(2)
    GRBLinExpr aux = (CMax/double(Instance::num_planning_horizon))*alpha +((PecOn+PecOff)/double(Instance::max_energy_cost))*(1-alpha);
    model->setObjective(aux, GRB_MINIMIZE);
}

/*
 * Definir TEP como o objetivo para o problema epsilon restrito
 */
void Model::SetObjectiveTEP()
{

    GRBLinExpr aux = PecOn+PecOff;
    model->setObjective(aux, GRB_MINIMIZE);

}

/*
 * Definir Makespan como o objetivo para o problema epsilon restrito
 */
void Model::SetObjectiveMakespan()
{

    GRBLinExpr aux = CMax;
    model->setObjective(aux, GRB_MINIMIZE);

}

void Model::SetConstraint()
{
    GRBLinExpr aux1, aux2, aux3, aux4, resultado;
    double aux0;
    string str;

    //(2)
    //Toda tarefa j deve ser processada apenas uma vez
    unsigned limit, pt_round;
    for (unsigned j = 1; j <= Instance::num_jobs; j++) {
        aux1 = 0;
        for (unsigned i = 1; i <= Instance::num_machine; i++) {
            for (unsigned l = 1; l <= Instance::num_mode_op; l++) {
                pt_round = ceil(
                            double(Instance::m_processing_time[i][j])/
                            double(Instance::v_speed_factor[l])
                           );
                limit = Instance::num_planning_horizon - pt_round;
                for (unsigned h = 0; h <= limit; h++) {
                    aux1 +=X[i][j][h][l];
                }
            }
        }
        str = "Sum(X[i, " + itos(j) + ", l, h]) = 1 | i(1:" + itos(Instance::num_machine) +
                "), l(1:" + itos(Instance::num_mode_op) + "), h(0:" + itos(Instance::num_planning_horizon) + ")";
        model->addConstr(aux1, GRB_EQUAL, 1, str);
    }

    //(3)
    //
    unsigned limit2;
    int a, b;
    for (unsigned i = 1; i <= Instance::num_machine; i++) {
        for (unsigned j = 1; j <= Instance::num_jobs; j++) {
            for (unsigned k = 1; k <= Instance::num_jobs; k++) {
                if(j != k ){
                    for (unsigned l = 1; l <= Instance::num_mode_op; l++) {
                        for (unsigned h = 0; h <= Instance::num_planning_horizon; h++) {
                            aux1 = 0;
                            pt_round = ceil(
                                        double(Instance::m_processing_time[i][j])/
                                        double(Instance::v_speed_factor[l])
                                       );
                            a = h + pt_round + Instance::m_setup_time[i][j][k] - 1;
                            b = Instance::num_planning_horizon;
                            limit2 = unsigned(min(a, b));
                            for (unsigned u = h; u <= limit2; u++) {
                                for (unsigned l1 = 1; l1 <= Instance::num_mode_op; l1++) {
                                    aux1 +=X[i][k][u][l1];
                                }
                            }
                            //str = "Sum(X[i, " + itos(j) + ", l, h]) = 1 | i(1:" + itos(Instance::numMachine) + "), l(1:" + itos(Instance::numModeOp) + "), h(0:" + itos(Instance::numPlanningHorizon) + ")";
                            model->addConstr(X[i][j][h][l] + aux1, GRB_LESS_EQUAL, 1, str);
                        }
                    }
                }
            }
        }
    }

    //(4)
    for (unsigned i = 1; i <= Instance::num_machine; i++) {
        for (unsigned j = 1; j <= Instance::num_jobs; j++) {
            for (unsigned l = 1; l <= Instance::num_mode_op; l++) {
                for (unsigned h = 0; h <= Instance::num_planning_horizon; h++) {
                    //str = "Sum(X[i, " + itos(j) + ", l, h]) = 1 |
                    //i(1:" + itos(Instance::numMachine) + "), l(1:" + itos(Instance::numModeOp) + "),
                    //h(0:" + itos(Instance::numPlanningHorizon) + ")";
                    pt_round = ceil(
                                double(Instance::m_processing_time[i][j])/
                                double(Instance::v_speed_factor[l])
                               );
                    model->addConstr(CMax, GRB_GREATER_EQUAL, X[i][j][h][l]*(h+pt_round), str);
                }
            }
        }
    }

    aux1 = 0;
    int c;


    //(5)
    resultado = 0;
    aux0 = 0;
    for (unsigned i = 1; i <= Instance::num_machine; i++) {
        for (unsigned j = 1; j <= Instance::num_jobs; j++) {
            for (unsigned l = 1; l <= Instance::num_mode_op; l++) {

                aux0 = double(Instance::v_consumption_factor[l]*
                        Instance::v_machine_potency[i]*
                        Instance::rate_on_peak)/double(60);

                aux2 = 0;
                for (unsigned h = 0; h < Instance::v_peak_start[0]; h++) {
                    pt_round = ceil(
                                double(Instance::m_processing_time[i][j])/
                                double(Instance::v_speed_factor[l])
                               );
                    a = h + pt_round - 1;
                    b = Instance::v_peak_end[0];
                    c = min(a, b);
                    a = 0;
                    b = c - (Instance::v_peak_start[0] - 1);
                    c = max(a, b);
                    aux2 += X[i][j][h][l]*c;

                    //cout << aux2 << endl;
                }

                aux3 = 0;
                for (unsigned h = Instance::v_peak_start[0]; h <= Instance::v_peak_end[0]; h++) {
                    pt_round = ceil(
                                    double(Instance::m_processing_time[i][j])/
                                    double(Instance::v_speed_factor[l])
                                );
                    a = h + pt_round;
                    b = Instance::v_peak_end[0] + 1;
                    c = min(a, b);
                    c -= h;
                    aux3 += X[i][j][h][l]*c;
                }

                resultado += (aux0)*(aux2+aux3);
            }
        }
    }

    //str = "Sum(X[i, " + itos(j) + ", l, h]) = 1 | i(1:" + itos(Instance::numMachine) + "),
    //l(1:" + itos(Instance::numModeOp) + "), h(0:" + itos(Instance::numPlanningHorizon) + ")";
    model->addConstr(PecOn, GRB_GREATER_EQUAL, resultado, str);


    //(6) PEC_off
    aux1 = 0;
    aux2 = 0;
    aux3 = 0;
    aux4 = 0;
    aux0 = 0;
    resultado = 0;
    int d;
    for (unsigned i = 1; i <= Instance::num_machine; i++) {
        for (unsigned j = 1; j <= Instance::num_jobs; j++) {
            for (unsigned l = 1; l <= Instance::num_mode_op; l++) {

                aux0 = double(Instance::v_consumption_factor[l]*
                        Instance::v_machine_potency[i]*
                        Instance::rate_off_peak)/double(60);

                aux2 = 0;
                for (unsigned h = 0; h < Instance::v_peak_start[0]; h++) {
                    a = h + ceil(
                                    double(Instance::m_processing_time[i][j])/
                                    double(Instance::v_speed_factor[l])
                                );
                    b = Instance::v_peak_start[0];
                    c = min(a, b);
                    d = c - h;
                    a = 0;
                    b = h + ceil(
                                    double(Instance::m_processing_time[i][j])/
                                    double(Instance::v_speed_factor[l])
                                ) - Instance::v_peak_end[0] - 1;
                    c = max(a, b);
                    d = d + c;
                    aux2 += X[i][j][h][l]*d;
                }

                aux3 = 0;
                for (unsigned h = Instance::v_peak_start[0]; h <= Instance::v_peak_end[0]; h++) {
                    a = 0;
                    b = h + ceil(
                                    double(Instance::m_processing_time[i][j])/
                                    double(Instance::v_speed_factor[l])
                                ) - Instance::v_peak_end[0] - 1;
                    c = max(a, b);
                    aux3 += X[i][j][h][l]*c;
                }

                aux4 = 0;
                for (unsigned h = Instance::v_peak_end[0]+1; h <= Instance::num_planning_horizon; h++) {
                    c = ceil(
                                double(Instance::m_processing_time[i][j])/
                                double(Instance::v_speed_factor[l])
                            );
                    aux4 += X[i][j][h][l]*c;
                }

                resultado += aux0*(aux2+aux3+aux4);

            }
        }
    }
    //str = "Sum(X[i, " + itos(j) + ", l, h]) = 1 | i(1:" + itos(Instance::numMachine) + "),
    //l(1:" + itos(Instance::numModeOp) + "), h(0:" + itos(Instance::numPlanningHorizon) + ")";
    model->addConstr(PecOff, GRB_GREATER_EQUAL, resultado, str);

}

void Model::SetConstraintWithMakespan(unsigned makespan)
{
    this->SetConstraint();
    model->addConstr(CMax, GRB_LESS_EQUAL, makespan);
}

void Model::SetConstraintWithTEP(double TEP)
{

    GRBLinExpr aux = PecOn+PecOff;

    this->SetConstraint();
    model->addConstr(aux, GRB_LESS_EQUAL, TEP);
}

void Model::Optimize()
{
    model->optimize();

}

void Model::PrintVars()
{

    double valor;
    string str;

    cout << "=======  X  ==========" << endl << endl;

    for (unsigned i = 1; i <= Instance::num_machine; i++) {
        for (unsigned j = 1; j <= Instance::num_jobs; j++) {
            for (unsigned h = 0; h <= Instance::num_planning_horizon; h++) {
                for (unsigned l = 1; l <= Instance::num_mode_op; l++) {

                    str = "X[" + itos(i) + "][" + itos(j) + "][" + itos(h) + "][" + itos(l) + "]";
                    valor = model->getVarByName(str).get(GRB_DoubleAttr_X);

                    if(valor > 0)
                        cout << str << endl;

                }
            }
        }
    }
}

struct row{
    unsigned h;
    unsigned job;
    unsigned machine;
    unsigned op_mode;
};

bool compare_struct(const row &a, const row &b){
    return a.h < b.h;
}

void Model::GetSolutionFromModel(Solution *MySolution)
{
    double valor;
    string str;

    vector<row> v_r;
    row r;

    try{

        for (unsigned i = 1; i <= Instance::num_machine; i++) {
            v_r.clear();
            MySolution->scheduling[i].clear();
            for (unsigned j = 1; j <= Instance::num_jobs; j++) {
                for (unsigned h = 0; h <= Instance::num_planning_horizon; h++) {
                    for (unsigned l = 1; l <= Instance::num_mode_op; l++) {

                        str = "X[" + itos(i) + "][" + itos(j) + "][" + itos(h) + "][" + itos(l) + "]";
                        valor = model->getVarByName(str).get(GRB_DoubleAttr_X);

                        if(valor > 0){
                            r.h = h;
                            r.job = j;
                            r.machine = i;
                            r.op_mode = l;

                            v_r.push_back(r);

                        }

                    }
                }
            }
            sort(v_r.begin(), v_r.end(), compare_struct);
            for(auto it:v_r){
                MySolution->scheduling[it.machine].push_back(it.job);
                MySolution->H1[it.job] = it.h;
                MySolution->job_mode_op[it.job] = it.op_mode;
            }
            v_r.clear();
        }

        str = "CMax";
        valor = model->getVarByName(str).get(GRB_DoubleAttr_X);

        str = "PecOff";
        valor = model->getVarByName(str).get(GRB_DoubleAttr_X);

        valor = model->getObjective().getValue();

        MySolution->CalculateShorterTimeHorizon(false);

        MySolution->CalculateObjectiveDiscrete();

    }
    catch(GRBException e){
        cout << e.getErrorCode();
    }
}

void RunWeightedMathModel(double max_time, double alpha, Solution * my_solution)
{

    //Instance::ReadJulioInstance(instance_file);
    //Instance::PrintInstance1();

    double d_num;

    try {
        // Criar ambiente
        GRBEnv env = GRBEnv();

        // Criar modelo
        GRBModel model = GRBModel(env);

        Model *Mymodel = new Model();
        Mymodel->Create(&model, max_time);
        Mymodel->AddVar();
        Mymodel->SetObjective(alpha);
        Mymodel->SetConstraint();
        //Mymodel->SetInitialSolutionToMathModel(my_solution);
        Mymodel->Optimize();

        Mymodel->GetSolutionFromModel(my_solution);

        if(model.get(GRB_IntAttr_Status) == GRB_OPTIMAL){
            my_solution->is_optimal = true;
        }
        else{
            my_solution->is_optimal = false;
        }

        string var;

        var = "CMax";
        d_num = model.getVarByName(var).get(GRB_DoubleAttr_X);
        my_solution->makeSpan = d_num;
        #ifdef DEBUG
        cout << endl << var << ": " << d_num << endl;
        #endif

        var = "PecOn";
        d_num = model.getVarByName(var).get(GRB_DoubleAttr_X);
        my_solution->TEC = d_num;
        #ifdef DEBUG
        cout << var << ": " << model.getVarByName(var).get(GRB_DoubleAttr_X) << endl;
        #endif

        var = "PecOff";
        d_num = model.getVarByName(var).get(GRB_DoubleAttr_X);
        my_solution->TEC += d_num;
        #ifdef DEBUG
        cout << var << ": " << model.getVarByName(var).get(GRB_DoubleAttr_X) << endl;
        //Mymodel->get

        Mymodel->PrintVars();

        my_solution->Print();
        #endif


    } catch(GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch(...) {
        cout << "Exception during optimization" << endl;
    }

}
