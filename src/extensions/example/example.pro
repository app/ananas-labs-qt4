include(../../lib/lib.pri)
TARGET    = aextexample
SOURCES	+= aextexample.cpp 
HEADERS	+= aextexample.h 
include ( ../../ananas.pri )
shared {
win32:DEFINES+= ANANAS_DLL
} else {
win32:DEFINES   += ANANAS_NO_DLL
}
TEMPLATE	=lib
LIBS	+= -L../../../lib -lananas
CONFIG		+= plugin
INCLUDEPATH	+= ../../lib 
LANGUAGE	= C++
DESTDIR     		= ..
#win32:DLLDESTDIR	= ..
