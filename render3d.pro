#-------------------------------------------------
#
# Project created by QtCreator 2015-12-05T11:09:55
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = render3d
TEMPLATE = app


SOURCES += main.cpp\
        main_window.cpp

HEADERS  += \
    vec.hpp \
    main_window.hpp

FORMS    += main_window.ui
