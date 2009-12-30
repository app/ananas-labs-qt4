include(../plugins/plugins.pri)
include(../lib/lib.pri)
TARGET = ananas4-designer
TEMPLATE = app

CONFIG += designer

DESTDIR = ../../bin

INCLUDEPATH += . ./formdesigner ./formdesigner/private  ../lib ../../tmp/ui/ananas ../plugins ../editor
LIBS += -L../../lib -lananas4 -lqt4designer -L../../lib/designer -lananasplugin4 -lqt4-qdataschema -L../editor -lqtscriptedit
unix {
    #LIBS += -L$(QTINC)/QtDesigner
    LIBS += -lQtDesignerComponents
#    LIBS += -lqt4designer
}
win32{
    LIBS += -lQtDesignerComponents4
}

MOC_DIR = ../../tmp/moc/$$TARGET
OBJECTS_DIR = ../../tmp/obj/$$TARGET
UI_DIR = ../../tmp/ui/$$TARGET

#LIBS	+= -L../lib -L../plugins -lananas -lananasplugin -lqsa -ldesignercore -lqassistantclient
#LIBS	+= -L. -L../lib -L../plugins -lananas -lananasplugin -lqsa -lformdesigner -lqassistantclient
#INCLUDEPATH	+= ../lib ../lib/tmp/ui ../lib/.ui $(QTDIR)/tools/designer/designer ../../qt-headers/tools/designer/designer /usr/lib
#INCLUDEPATH	+= ../lib ../lib/tmp/ui ../lib/.ui ./formdesigner /usr/lib

include ( ../ananas.pri )
#load(qsa)
QT += script
QT += scripttools

HEADERS	= \
    actiontree.h \
    aliaseditor.h \
    atreeitems.h \
    cfgform.h \
#    cfgform.ui.h \
    deditaction.h \
#    deditaction.ui.h \
    deditareg.h \
#    deditareg.ui.h \
    deditcat.h \
#    deditcat.ui.h \
    deditcfg.h \
#    deditcfg.ui.h \
    deditcolumns.h \
#    deditcolumns.ui.h \
    deditcommand.h \
#    deditcommand.ui.h \
    deditdialog.h \
#    deditdialog.ui.h \
    deditdoc.h \
#    deditdoc.ui.h \
    deditfield.h \
#    deditfield.ui.h \
    deditimagecollection.h \
#    deditimagecollection.ui.h \
    deditireg.h \
#    deditireg.ui.h \
    deditjournal.h \
#    deditjournal.ui.h \
    deditlang.h \
#    deditlang.ui.h \
    deditreport.h \
#    deditreport.ui.h \
    deditrole.h \
#    deditrole.ui.h \
    dedittoolbar.h \
#    dedittoolbar.ui.h \
    deditwebform.h \
#    deditwebform.ui.h \
    formdesigner.h \
    interfacetree.h \
    mainform.h \
#    mainform.ui.h \
    mdtree.h \
    pixmappreview.h \
    roleeditor.h     \
    formdesigner/formwindowsettings.h \
    formdesigner/newform.h \
    formdesigner/plugindialog.h \
    formdesigner/qdesigner_actioneditor.h \
    formdesigner/qdesigner_actions.h \
    formdesigner/qdesigner_formwindow.h \
    formdesigner/qdesigner_objectinspector.h \
    formdesigner/qdesigner_pch.h \
    formdesigner/qdesigner_propertyeditor.h \
    formdesigner/qdesigner_resourceeditor.h \
    formdesigner/qdesigner_settings.h \
    formdesigner/qdesigner_signalsloteditor.h \
    formdesigner/qdesigner_toolwindow.h \
    formdesigner/qdesigner_widgetbox.h \
    formdesigner/qdesigner_workbench.h \
    formdesigner/saveformastemplate.h

SOURCES	= \
    actiontree.cpp \
    aliaseditor.cpp \
    atreeitems.cpp \
    cfgform.cpp \
    deditaction.cpp \
    deditareg.cpp \
    deditcat.cpp \
    deditcfg.cpp \
    deditcolumns.cpp \
    deditcommand.cpp \
    deditdialog.cpp \
    deditdoc.cpp \
    deditfield.cpp \
    deditimagecollection.cpp \
    deditireg.cpp \
    deditjournal.cpp \
    deditlang.cpp \
    deditreport.cpp \
    deditrole.cpp \
    dedittoolbar.cpp \
    deditwebform.cpp \
    formdesigner.cpp \
    interfacetree.cpp \
    main.cpp \
    mainform.cpp \
    mdtree.cpp \
    roleeditor.cpp     \
    formdesigner/formwindowsettings.cpp \
    formdesigner/newform.cpp \
    formdesigner/plugindialog.cpp \
    formdesigner/qdesigner_actioneditor.cpp \
    formdesigner/qdesigner_actions.cpp \
    formdesigner/qdesigner_formwindow.cpp \
    formdesigner/qdesigner_objectinspector.cpp \
    formdesigner/qdesigner_propertyeditor.cpp \
    formdesigner/qdesigner_resourceeditor.cpp \
    formdesigner/qdesigner_settings.cpp \
    formdesigner/qdesigner_signalsloteditor.cpp \
    formdesigner/qdesigner_toolwindow.cpp \
    formdesigner/qdesigner_widgetbox.cpp \
    formdesigner/qdesigner_workbench.cpp \
    formdesigner/saveformastemplate.cpp

FORMS = \
    cfgform.ui \
    deditaction.ui \
    deditareg.ui \
    deditcat.ui \
    deditcfg.ui \
    deditcolumns.ui \
    deditcommand.ui \
    deditdialog.ui \
    deditdoc.ui \
    deditfield.ui \
    deditimagecollection.ui \
    deditireg.ui \
    deditjournal.ui \
    deditlang.ui \
    deditreport.ui \
    deditrole.ui \
    dedittoolbar.ui \
    deditwebform.ui \
    mainform.ui     \
    formdesigner/formwindowsettings.ui \
    formdesigner/newform.ui \
    formdesigner/plugindialog.ui \
    formdesigner/saveformastemplate.ui

RESOURCES = \
    designer.qrc 
#    formdesigner/designer.qrc

TRANSLATIONS = \
    ../../translations/ananas-designer-en.ts \
    ../../translations/ananas-designer-ru.ts 

unix {
	target.path = /$(BINDIR)
#	adesigner.files = ananas-designer
#	acfg.path = /etc/ananas
#	acfg.files= testwrite.cfg
     }	

INSTALLS += target


