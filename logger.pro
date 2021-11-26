TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        jsoncpp.cpp \
        main.cpp

HEADERS += \
    json/json-forwards.h \
    json/json.h \
    logger.h
