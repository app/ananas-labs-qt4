include (../plugins/plugins.pri)
include (../lib/lib.pri)
include (../ananas.pri)
#load(qsa)
QT += script
QT += scripttools


TARGET = ananas4
TEMPLATE = app

DESTDIR = ../../bin

INCLUDEPATH += ../lib ../../tmp/ui/ananas ../plugins
LIBS += -L../../lib -lananas4 -L../../lib/designer -lananasplugin4 
#LIBS += -L../../lib -lananas4 -L../../lib/designer -lananasplugin4 -lqt4-qdataschema 

HEADERS = \
    mainform.h

SOURCES = \
    main.cpp \
    mainform.cpp

#FORMS = qadocjournal.ui

RESOURCE = \
    ananas.qrc
    
TRANSLATIONS = \
    ../../translations/ananas-engine-en.ts \
    ../../translations/ananas-engine-ru.ts 
    
unix {
    target.path = /$(BINDIR)
#    ananas.files = ananas4
}
win32{
#    ananas.path =.
#    ananas.extra = CALL create_base.bat
}    

INSTALLS += target

