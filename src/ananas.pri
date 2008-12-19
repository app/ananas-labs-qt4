#QMAKE_CLEAN += $(TARGET) Makefile
CONFIG	+= qt warn_on release thread
#LANGUAGE = C++

QT += qt3support sql xml
CONFIG += warn_on designer

unix {
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
    INCLUDEPATH +=.ui ../lib ../../lib ../../plugins ../plugins ../lib/.ui ../../lib/.ui
}
win32 {
    UI_DIR = tmp\ui
    MOC_DIR = tmp\moc
    OBJECTS_DIR = obj
    debug:CONFIG += console
    INCLUDEPATH +=tmp\ui ..\lib ..\..\lib ..\..\plugins ..\lib\tmp\ui ..\..\lib\tmp\ui ..\plugins
    INCLUDEPATH	+= ..\..\plugins 
#    LIBS	+= -L..\..\plugins -lananasplugin
}

INCLUDEPATH +=$(QTDIR)/include/qdataschema
