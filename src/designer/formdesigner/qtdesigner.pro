include ( qtdesigner.pri )
include ( ../../ananas.pri )

TEMPLATE = lib
TARGET   = qt4designer

INCLUDEPATH += ./private

DEFINES  += 

LIBS += -lQtDesignerComponents


DESTDIR = ../../../lib
DLLDESTDIR = ../../../bin
