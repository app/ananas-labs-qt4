TARGET    = aextte
SOURCES	+= aextte.cpp 
HEADERS	+= aextte.h 
include ( ../../ananas.pri )
shared {
    win32:DEFINES+= ANANAS_DLL
} else {
    win32:DEFINES   += ANANAS_NO_DLL
}
TEMPLATE	=lib
CONFIG	+= plugin
INCLUDEPATH	+= ../../lib 
LIBS	+= -L../../lib -lananas
LANGUAGE	= C++

unix {
    INCLUDEPATH	+= /usr/include/tradeequip
#    LIBS += -L/usr/lib -ltradeequip
}

win32 {
    INCLUDEPATH	+= d:\src\tradeequip\src\lib
#    LIBS += -L/src/tradeequip/lib -ltradeequip
}

win32:DESTDIR = ..

unix{
        target.path = $$(LIBDIR)/ananas4
#        libext.files = libaextte.so
        INSTALLS += target
}

