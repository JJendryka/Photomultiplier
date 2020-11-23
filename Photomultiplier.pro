QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets charts serialport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    controldock.cpp \
    experiment.cpp \
    main.cpp \
    mainwindow.cpp \
    measurementdock.cpp \
    packet.cpp \
    serialport.cpp \
    terminaldock.cpp

HEADERS += \
    controldock.h \
    experiment.h \
    mainwindow.h \
    measurementdock.h \
    packet.h \
    serialport.h \
    terminaldock.h

FORMS += \
    controldock.ui \
    mainwindow.ui \
    measurementdock.ui \
    terminaldock.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.md
