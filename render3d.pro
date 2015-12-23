QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS += -std=c++1z -stdlib=libc++

TARGET = render3d
TEMPLATE = app

SOURCES += \
    main.cpp \
    main_window.cpp

HEADERS  += \
    main_window.hpp \
    mat.hpp \
    trafo_mats.hpp \
    vec.hpp \

FORMS += \
    main_window.ui

