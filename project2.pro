QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialog.cpp \
    experimentswindow.cpp \
    main.cpp \
    mainwindow.cpp \
    pathwindow.cpp \
    source.cpp

HEADERS += \
    dialog.h \
    experimentswindow.h \
    graph.h \
    mainwindow.h \
    path.h \
    pathwindow.h \
    point.h \
    router.h \
    topology_control.h \
    udg_generation.h

FORMS += \
    dialog.ui \
    experimentswindow.ui \
    mainwindow.ui \
    pathwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
