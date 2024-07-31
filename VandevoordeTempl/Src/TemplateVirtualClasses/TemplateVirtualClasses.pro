TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        database.cpp \
        main.cpp \
        rdbms.cpp

HEADERS += \
    database.h \
    executionpolicy.h \
    rdbms.h
