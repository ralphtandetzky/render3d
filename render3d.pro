#-------------------------------------------------
#
# Project created by QtCreator 2015-11-30T20:35:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS += -std=c++1z -stdlib=libc++

TARGET = render3d
TEMPLATE = app


SOURCES += main.cpp \
    main_window.cpp

HEADERS  += \
    main_window.h \
    vec3d.h

FORMS += \
    main_window.ui
