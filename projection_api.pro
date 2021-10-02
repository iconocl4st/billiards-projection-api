QT += core gui widgets

CONFIG += c++1z debug
TARGET = qt-projection-api
TEMPLATE = app

INCLUDEPATH += $$(REPOS)/billiards-common/src src /usr/include

SOURCES += \
    src/project_api.cpp \
    src/front_end/qt/qt_front_end.cpp \
    src/front_end/qt/ProjectorWidget.cpp

HEADERS += src/front_end/qt/ProjectorWidget.h

LIBS += -lboost_system

binaries.path = $$(DESTDIR)/app
binaries.files += $$OUT_PWD/qt-projection-api
binaries.CONFIG = no_check_exist
INSTALLS += binaries
