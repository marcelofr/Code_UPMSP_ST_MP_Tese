TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        common_basic.cpp \
        gasolution.cpp \
        genetic_basic.cpp \
        instance.cpp \
        main.cpp \
        math_model.cpp \
        moga.cpp \
        mono_solution.cpp \
        nsgaii.cpp \
        run_algorithm.cpp \
        set_solution.cpp \
        solution.cpp \
        speaii.cpp

HEADERS += \
    common_basic.h \
    gasolution.h \
    genetic_basic.h \
    instance.h \
    math_model.h \
    moga.h \
    mono_solution.h \
    nsgaii.h \
    run_algorithm.h \
    set_solution.h \
    solution.h \
    speaii.h

# INCLUDEPATH += $$PWD/../gurobi702/linux64/include
# INCLUDEPATH += /opt/gurobi752/linux64/include
#INCLUDEPATH += /home/marcelo/gurobi702/linux64/include
INCLUDEPATH += /home/marcelo/gurobi912/linux64/include

# LIBS += -L /opt/gurobi752/linux64/lib/ -lgurobi_g++5.2 -lgurobi75
# LIBS += -L$$PWD/../gurobi702/linux64/lib/ -lgurobi_g++5.2 -lgurobi70
#LIBS += -L /home/marcelo/gurobi702/linux64/lib/ -lgurobi_g++5.2 -lgurobi70
LIBS += -L /home/marcelo/gurobi912/linux64/lib/ -lgurobi_g++5.2 -lgurobi91
