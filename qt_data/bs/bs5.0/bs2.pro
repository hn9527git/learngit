#-------------------------------------------------
#
# Project created by QtCreator 2019-03-04T11:44:21
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
QT       += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bs2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        chuankou.cpp \
    mymainwindow.cpp \
    conn_mysql_dialog.cpp \
    conn_dialog.cpp \
    set.cpp \
    cangku.cpp \
    cangku_add.cpp \
    cangku_in.cpp \
    cangku_out.cpp \
    ck_set.cpp

HEADERS += \
        chuankou.h \
    mymainwindow.h \
    conn_dialog.h \
    conn_mysql_dialog.h \
    set.h \
    cangku.h \
    cangku_add.h \
    cangku_in.h \
    cangku_out.h \
    ck_set.h

FORMS += \
        chuankou.ui \
    mymainwindow.ui \
    conn_mysql_dialog.ui \
    conn_dialog.ui \
    set.ui \
    cangku.ui \
    cangku_add.ui \
    cangku_in.ui \
    cangku_out.ui \
    ck_set.ui

RESOURCES += \
    images.qrc

RC_ICONS = myico.ico
