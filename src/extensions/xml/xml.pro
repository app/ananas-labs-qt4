include(../../lib/lib.pri)
TARGET    = aextxml
	
SOURCES	+= aextxml.cpp 
HEADERS	+= aextxml.h 

include ( ../../ananas.pri )

shared {
win32:DEFINES+= ANANAS_DLL
} else {
win32:DEFINES   += ANANAS_NO_DLL
}

TEMPLATE	=lib
CONFIG	+= plugin

INCLUDEPATH	+= ../../lib 
LIBS	+= -L../../../lib -lananas4

LANGUAGE	= C++
win32:DESTDIR = ..

unix{
        target.path = /$(LIBDIR)/ananas4
#        libext.files = libaextxml.so
        INSTALLS += target
}

