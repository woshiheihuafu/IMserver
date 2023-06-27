#-------------------------------------------------
#
# Project created by QtCreator 2022-04-18T23:37:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IMServer
TEMPLATE = app

INCLUDEPATH += ./MySQL
INCLUDEPATH += ./net
INCLUDEPATH += ./mediator

LIBS += -lWs2_32

SOURCES += main.cpp\
        imserverdialog.cpp \
    mediator/TcpClientMediator.cpp \
    mediator/TcpServerMediator.cpp \
    net/TcpClient.cpp \
    net/TcpServer.cpp \
    mediator/INetMediator.cpp \
    ckernel.cpp \
    logindialog.cpp \
    useritem.cpp \
    chatdialog.cpp



FORMS    += imserverdialog.ui \
    logindialog.ui \
    useritem.ui \
    chatdialog.ui

HEADERS += \
    net/INet.h \
    net/packet.h \
    net/TcpClient.h \
    net/TcpServer.h \
    mediator/INetMediator.h \
    mediator/TcpClientMediator.h \
    mediator/TcpServerMediator.h \
    imserverdialog.h \
    ckernel.h \
    logindialog.h \
    useritem.h \
    chatdialog.h

RESOURCES += \
    resource.qrc

DISTFILES += \
    image/png.png
