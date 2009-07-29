#!define TARGET first for correct temporray paths definitions
TARGET = ananasplugin4

include( plugins.pri )
include( ../lib/lib.pri )
include ( ../ananas.pri )
load(qsa)

TEMPLATE = lib
shared:CONFIG += dll plugin

DESTDIR = ../../lib/designer
DLLDESTDIR = ../../bin

INCLUDEPATH += ../plugins ../lib 
LIBS += -L../../lib -lananas4 

shared {
    win32:DEFINES+= QT_PLUGIN # ANANAS_DLL
} else {
    win32:DEFINES   += ANANAS_NO_DLL
}

unix {
    LIBS += -L/usr/X11R6/lib/
}

HEADERS = \
    aform.h \
    atoolbar.h \
    awidgets_plugin.h \
    eaddobj.h \
    engine.h \
    qwidgetplugin.h 

SOURCES = \
    aform.cpp \
    atoolbar.cpp \
    awidgets_plugin.cpp \
    engine.cpp \
    eaddobj.cpp 
        
FORMS = \
    eaddobj.ui 

RESOURCES += \
    plugins.qrc
    
TRANSLATIONS = \
    ../../translations/ananas-plugins-en.ts \
    ../../translations/ananas-plugins-ru.ts 

#SLASH = /
unix {
    target.path = $(QTDIR)/plugins/designer
#    lplugin.files = libananasplugin4.so
#    lplugin.extra = cp -f $(DESTDIR)/libananasplugin4.so $(INSTALL_ROOT)$(LIBDIR) || true
    lpluginheader.path = $(INCLUDEDIR)
    lpluginheader.files = $$HEADERS
}  

win32 {
#   target.path = $(QTDIR)\plugins\designer
    lplugin.path = $(QTDIR)\plugins\designer
    lplugin.files = ananasplugin4.*
    lplugin.extra = copy ananasplugin4.* $(QTDIR)\lib
    INSTALLS += lplugin 
}  

unix{
    INSTALLS +=target  
    INSTALLS += lpluginheader
}

include ( actionbutton/actionbutton.pri )
include ( catalogue/catalogue.pri )
include ( combobox/combobox.pri )
include ( dbfield/dbfield.pri )
include ( dbtable/dbtable.pri )
include ( document/document.pri )
include ( field/field.pri )
include ( grouptree/grouptree.pri )
include ( journal/journal.pri )
include ( report/report.pri )
include ( table/table.pri )
