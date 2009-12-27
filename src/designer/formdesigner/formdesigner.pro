include ( qtdesigner.pri )
include ( ../../ananas.pri )

TEMPLATE = lib
TARGET   = qt4designer

INCLUDEPATH += ./private

DEFINES  += 

LIBS += -lQtDesignerComponents


DESTDIR = ../../../lib
DLLDESTDIR = ../../../bin
unix {
	VERSION 	= 1.0.0
	target.path = /$(LIBDIR)
	INSTALLS += target
}
