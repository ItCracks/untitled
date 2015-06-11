TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    objloader.cpp \
    mesh.cpp \
    texture.cpp \
    stb_image.c \
    shaderloader.cpp
LIBS += -lglfw3 -lglew32 -lopengl32

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    objloader.h \
    mesh.h \
    texture.h \
    stb_image.h \
    shaderloader.hpp \
    transform.h

DISTFILES += \
    Readme.txt


