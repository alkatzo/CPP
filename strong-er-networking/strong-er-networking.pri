QT += core
QT += network

HEADERS += \
    $$PWD/connection.h \
    $$PWD/networklistener.h \
    $$PWD/networkmanager.h \
    $$PWD/networkmanagerclient.h \
    $$PWD/networkmanagerserver.h \
    $$PWD/peermanager.h \
    $$PWD/peermanagerclient.h \
    $$PWD/peermanagerserver.h \
    $$PWD/server.h

SOURCES += \
    $$PWD/connection.cpp \
    $$PWD/networklistener.cpp \
    $$PWD/networkmanager.cpp \
    $$PWD/networkmanagerclient.cpp \
    $$PWD/networkmanagerserver.cpp \
    $$PWD/peermanager.cpp \
    $$PWD/peermanagerclient.cpp \
    $$PWD/peermanagerserver.cpp \
    $$PWD/server.cpp
