TEMPLATE = app
CONFIG += console c++20
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=gnu++1z

SOURCES += \
        common_basic.cpp \
        decomposition.cpp \
        gasolution.cpp \
        generate_intances.cpp \
        genetic_basic.cpp \
        instance.cpp \
        localsearch.cpp \
        localsearch_basic.cpp \
        main.cpp \
        math_model.cpp \
        metric.cpp \
        moga.cpp \
        monosolution.cpp \
        nsgai.cpp \
        nsgaii.cpp \
        run_algorithm.cpp \
        set_solution.cpp \
        solution.cpp \
        spea2.cpp

HEADERS += \
    common_basic.h \
    decomposition.h \
    gasolution.h \
    generate_intances.h \
    genetic_basic.h \
    instance.h \
    localsearch.h \
    localsearch_basic.h \
    math_model.h \
    metric.h \
    moga.h \
    monosolution.h \
    nsgai.h \
    nsgaii.h \
    run_algorithm.h \
    set_solution.h \
    solution.h \
    spea2.h

# INCLUDEPATH += $$PWD/../gurobi702/linux64/include
# INCLUDEPATH += /opt/gurobi752/linux64/include
#INCLUDEPATH += /home/marcelo/gurobi702/linux64/include
INCLUDEPATH += /home/marcelo/gurobi912/linux64/include

# LIBS += -L /opt/gurobi752/linux64/lib/ -lgurobi_g++5.2 -lgurobi75
# LIBS += -L$$PWD/../gurobi702/linux64/lib/ -lgurobi_g++5.2 -lgurobi70
#LIBS += -L /home/marcelo/gurobi702/linux64/lib/ -lgurobi_g++5.2 -lgurobi70
LIBS += -L /home/marcelo/gurobi912/linux64/lib/ -lgurobi_g++5.2 -lgurobi91
