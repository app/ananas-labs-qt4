TARGET    = aextsql
QT += qt3support
	
SOURCES	+= aextsql.cpp 
HEADERS	+= aextsql.h 

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
#win32:DLLDESTDIR	= ..

unix{
	target.path = $(LIBDIR)/ananas 
#	libext.files = libaextsql.so
	INSTALLS += target
}
