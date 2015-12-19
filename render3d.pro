QT       += core gui
#<<<<<<< HEAD
#
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS += -std=c++1z -stdlib=libc++
#=======
#CONFIG += c++14

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
#>>>>>>> 67e05be32ac0b70e3b93ae0a4805948a6534caa5

TARGET = render3d
TEMPLATE = app


#<<<<<<< HEAD
SOURCES += \
    main.cpp \
    main_window.cpp

HEADERS  += \
    main_window.hpp \
    mat.hpp \
    trafo_mats.hpp
    vec3d.h
    vec.hpp \

FORMS += \
    main_window.ui

