include ( /usr/local/qwt-6.2.0/features/qwt.prf )

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++2a

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cartesiantree.cpp \
    chooseaction.cpp \
    choosenodexy.cpp \
    graphiccomponent.cpp \
    main.cpp \
    mainwindow.cpp \
    observerpattern.cpp \
    random.cpp

HEADERS += \
    actionbuttons.h \
    cartesiantree.h \
    chooseaction.h \
    choosenodexy.h \
    graphiccomponent.h \
    mainwindow.h \
    observerpattern.h \
    random.h

FORMS += \
    chooseaction.ui \
    choosenodexy.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
