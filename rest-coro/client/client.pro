QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    er_apidefault.cpp \
    er_base.cpp \
    er_dispatcher.cpp \
    er_integrationmanager.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    er_apidefault.h \
    er_base.h \
    er_dispatcher.h \
    er_future.h \
    er_integrationmanager.h \
    helper.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# ZLIB
win32:INCLUDEPATH *= $$[QT_INSTALL_HEADERS]/QtZlib

include($$PWD/qcoro/qcoro.pri)
include($$PWD/openapi/ER_client.pri)
