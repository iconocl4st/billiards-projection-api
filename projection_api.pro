QT += core gui widgets

CONFIG += c++17 debug
TARGET = qt-projection-api
TEMPLATE = app

INCLUDEPATH += "/mnt/1f0ab4b3-c472-49e1-92d8-c0b5664f7fdb/ProjectsForFun/Pool/repos/billiards-common/src"

SOURCES += \
    src/project_api.cpp \
    src/front_end/qt/qt_front_end.cpp \
    src/front_end/qt/QtFrontEnd.cpp \
    src/front_end/qt/ProjectorWidget.cpp

HEADERS  += src/front_end/qt/ProjectorWidget.h

binaries.path = $$(DESTDIR)/app
binaries.files += qt-projection-api
binaries.CONFIG = no_check_exist
INSTALLS += binaries
