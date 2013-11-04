QT       += network
QT       -= gui

TARGET = jiraworksheetcore
TEMPLATE = lib

DESTDIR 	=	../bin
OBJECTS_DIR	=	$$DESTDIR/objects
UI_DIR		=	$$DESTDIR/ui
MOC_DIR		=	$$DESTDIR/moc
RCC_DIR		=	$$DESTDIR/rcc

DEFINES += CORE_LIBRARY

LIBS += -lqjson -L../bin

INCLUDEPATH += ../qjson-master/include

SOURCES += \
    datafetcher.cpp

HEADERS +=\
        core_global.h \
    datafetcher.h
