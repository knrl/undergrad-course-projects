QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    databasemanagement.cpp \
    intertransactionwindow.cpp \
    listaccountwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    mngloginwindow.cpp \
    mngwindow.cpp \
    moneyprocesswindow.cpp \
    newaccountwindow.cpp \
    transactionwindow.cpp \
    userwindow.cpp

HEADERS += \
    databasemanagement.h \
    intertransactionwindow.h \
    listaccountwindow.h \
    mainwindow.h \
    mngloginwindow.h \
    mngwindow.h \
    moneyprocesswindow.h \
    newaccountwindow.h \
    transactionwindow.h \
    userwindow.h

FORMS += \
    intertransactionwindow.ui \
    listaccountwindow.ui \
    mainwindow.ui \
    mngloginwindow.ui \
    mngwindow.ui \
    moneyprocesswindow.ui \
    newaccountwindow.ui \
    transactionwindow.ui \
    userwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
