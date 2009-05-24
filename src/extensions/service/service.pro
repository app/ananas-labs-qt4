TARGET    = aextservice
	
SOURCES	+= aextservice.cpp 
HEADERS	+= aextservice.h 

include ( ../../ananas.pri )
include ( ../../lib/lib.pri )

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
	libext.path = $(LIBDIR)/ananas
	libext.files = libaextservice.so
        INSTALLS += libext
}

