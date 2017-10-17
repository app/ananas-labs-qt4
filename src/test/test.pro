TARGET	= ananas-test 
include(../lib/lib.pri)
include(../plugins/plugins.pri)
include(../ananas.pri )

SOURCES	+= \
	    main.cpp \ 
	    testametadata.cpp

HEADERS	+=  \
	    test.h \
	    testametadata.h
TRANSLATES = 

CONFIG +=qtestlib
FORMS	= 
TEMPLATE	=app
INCLUDEPATH	+= ../lib ../plugins
LIBS	+= -L$(QTDIR)/lib -lqsa -L../../lib -lananas
LIBS	+= -L../../lib/designer -lananasplugin -lqdataschema
