include(../../lib/lib.pri)
TARGET    = aext_meta
SOURCES	+= aext_meta.cpp 
HEADERS	+= aext_meta.h 
include ( ../../ananas.pri )
shared {
win32:DEFINES+= ANANAS_DLL
} else {
win32:DEFINES   += ANANAS_NO_DLL
}
TEMPLATE	=lib
LIBS	+= -L../../../lib -lananas4
CONFIG		+= plugin
INCLUDEPATH	+= ../../lib 
LANGUAGE	= C++
DESTDIR     		= ..
