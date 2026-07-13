QT += widgets

QT += multimedia multimediawidgets

CONFIG += c++17

TARGET = videoplayer

TEMPLATE = app

INCLUDEPATH += \
    $$PWD/src \
    $$PWD/src/ui \
    $$PWD/src/controller \
    $$PWD/src/util

SOURCES += \
    src/main.cpp \
    src/controller/videoplayercontroller.cpp \
    src/ui/videoplayermainwindow.cpp

HEADERS += \
    src/controller/videoplayercontroller.h \
    src/ui/videoplayermainwindow.h \
    src/util/fileutil.h

TRANSLATIONS += \
    mui/zh_CN/videoplayer.ts
CONFIG += lrelease embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
