TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    puzzle/puzzle.cpp \
    grid/grid.cpp \

HEADERS += \
    puzzle/puzzle.h \
    grid/grid.h \

LIBS += -lncurses
