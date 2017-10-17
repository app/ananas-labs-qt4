#project for Administrator application from Ananas automation accounting system
TARGET	= ananas-administrator
include ( ../lib/lib.pri )
include ( ../plugins/plugins.pri )
include ( ../ananas.pri )
QT += script
QT += scripttools


TEMPLATE	= app
LANGUAGE	= C++
DESTDIR = ../../bin

LIBS	+= -L../../lib -lananas 
LIBS    += -L../../lib/designer -lananasplugin -lqdataschema -L$(QTDIR)/lib

INCLUDEPATH	+= ../lib ../lib/tmp/ui ../lib/.ui

HEADERS	+= atreeitems.h \
	mdtree.h \
	mainform.h \
	usersform.h \
	rolesform.h \
	deditpermissions.h \
	dselectuser.h \
	deditrole.h \
	dedituser.h \
	dselectrole.h

SOURCES	+= main.cpp \
	atreeitems.cpp \
	mdtree.cpp \
	mainform.cpp \
	usersform.cpp \
	rolesform.cpp \
	deditpermissions.cpp \
	dselectuser.cpp \
	deditrole.cpp \
	dedituser.cpp \
	dselectrole.cpp
	

FORMS	= \ 
	mainform.ui \
	usersform.ui \
	rolesform.ui \
	deditpermissions.ui \
	dselectuser.ui \
	deditrole.ui \
	dedituser.ui \
	dselectrole.ui

RESOURCES +=admin.qrc



	
TRANSLATIONS = \
    ../../translations/ananas-administrator-en.ts \
    ../../translations/ananas-administrator-ru.ts 


unix {
        target.path = /$(BINDIR)
#        aadmin.files = ananas-administrator
#        aadmin.extra = cp -df ananas-administrator $(INSTALL_ROOT)$(BINDIR)
     }

INSTALLS += target



	
