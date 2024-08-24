include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

ROOT = $$PWD/../

INCLUDEPATH += $$ROOT

SOURCES += \
        main.cpp \
        tst_func_return_type.cpp \
        tst_mytestcase.cpp

message("INCLUDEPATH: $$INCLUDEPATH")
