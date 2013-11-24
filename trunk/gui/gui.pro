QT *= network

TARGET = jiraworksheetgui
TEMPLATE = lib

DEFINES += GUI_LIBRARY

DESTDIR 	=	../bin
OBJECTS_DIR	=	$$DESTDIR/objects
UI_DIR		=	$$DESTDIR/ui
MOC_DIR		=	$$DESTDIR/moc
RCC_DIR		=	$$DESTDIR/rcc

LIBS += -ljiraworksheetcore -L../bin

INCLUDEPATH += UI_DIR

SOURCES += \
    worksheetwidget.cpp \
	worksheetmodel.cpp \

HEADERS +=\
	gui_global.h \
    worksheetwidget.h \
	worksheetmodel.h \

FORMS += \
	worksheetwidget.ui \
