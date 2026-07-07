QT += widgets

QT += multimedia multimediawidgets

CONFIG += c++17

TARGET = videoplayer

TEMPLATE = app

INCLUDEPATH += \
    $$PWD/src/ui

SOURCES += \
    src/main.cpp \
    src/ui/videoplayermainwindow.cpp

HEADERS += \
    src/ui/videoplayermainwindow.h

FORMS += \
    forms/videoplayermainwindow.ui

TRANSLATIONS += \
    mui/videoplayer_zh_CN.ts
CONFIG += lrelease embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
