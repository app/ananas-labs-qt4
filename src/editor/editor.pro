TEMPLATE = lib
#DESTDIR = ../lib
TARGET = qtscriptedit
#DEFINES += Q_SCRIPTDEBUG_BUILD_LIB
include(editor.pri)
unix:VERSION 	= 1.0.0
unix:target.path = /$(LIBDIR)
unix:INSTALLS += target
