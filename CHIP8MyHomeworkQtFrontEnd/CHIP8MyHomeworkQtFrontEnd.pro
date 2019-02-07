TEMPLATE = app
TARGET = CHIP8MyHomework

QT = core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    main.cpp \
    window.cpp \
    helper.cpp \
    widget.cpp \
    glwidget.cpp

HEADERS += \
    window.h \
    helper.h \
    widget.h \
    glwidget.h
