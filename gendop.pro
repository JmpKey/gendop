QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    databasemanager.cpp \
    dialogchosegroup.cpp \
    dialogchoseteacher.cpp \
    dialoggroup.cpp \
    dialogstudent.cpp \
    dialogteacher.cpp \
    dialogyear.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    adaptation.h \
    databasemanager.h \
    dialogchosegroup.h \
    dialogchoseteacher.h \
    dialoggroup.h \
    dialogstudent.h \
    dialogteacher.h \
    dialogyear.h \
    mainwindow.h

FORMS += \
    dialogchosegroup.ui \
    dialogchoseteacher.ui \
    dialoggroup.ui \
    dialogstudent.ui \
    dialogteacher.ui \
    dialogyear.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
