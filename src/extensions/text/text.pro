include(../../lib/lib.pri)
TARGET    = aexttext
SOURCES	 += aexttext.cpp
HEADERS	 += aexttext.h 
include ( ../../ananas.pri )
shared {
win32:DEFINES+= ANANAS_DLL
} else {
win32:DEFINES   += ANANAS_NO_DLL
}
TEMPLATE	=lib
CONFIG	+= plugin
LIBS	+= -L../../../lib -lananas4
INCLUDEPATH	+= ../../lib 
LANGUAGE	= C++
DESTDIR     		= ..
#win32:DLLDESTDIR	= ..
