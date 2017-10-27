/****************************************************************************
** $Id: main.cpp,v 1.7 2009/05/22 08:03:55 app Exp $
**
** Main file of Ananas Designer application
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
**
** This file is part of the Designer application of the Ananas
** automation accounting system.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.leaderit.ru/page=ananas or email sales@leaderit.ru
** See http://www.leaderit.ru/gpl/ for GPL licensing information.
**
** Contact org@leaderit.ru if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#include <aapplication.h>
#include <qtranslator.h>
#include <qsplashscreen.h>
#include <qtextcodec.h>
//Added by qt3to4:
#include <QPixmap>
#include <q3mimefactory.h>
#include "alog.h"
#include "ananas.h"
#include "mainform.h"
#include "dselectdb.h"
#include "formdesigner.h"

MainForm *mainform = 0;
// aWindowsList *mainformwl = 0;
// QWorkspace *mainformws = 0;
//QApplication *application = 0;
QTranslator *translator = 0, tr_app(0), tr_lib(0), tr_plugins(0);
aFormDesigner *formdesigner = 0;
QString lang="en",
	rcfile="",
	username="",
	userpassword="";
aDatabase* database= NULL;
aCfgRc rc;

int setTranslator(QString langdir, QString lang)
{
//        QString langdir;
//#ifdef Q_OS_WIN32
//	langdir = qApp->applicationDirPath()+"/translations/";
//#else
//	langdir = "/usr/share/ananas/translations/";
//#endif
	tr_app.load( langdir+"ananas-designer-"+lang+".qm",".");
	tr_lib.load( langdir+"ananas-lib-"+lang+".qm",".");
	tr_plugins.load( langdir+"ananas-plugins-"+lang+".qm",".");
	return 0;
}



int
parseCommandLine( AApplication *a )
{
	QString param, name, value;
	int i, argc;
	char **argv;
	
	argc = a->argc();
	argv = a->argv();
//	char *s, locale[50]="en";

//	strncpy( locale, QTextCodec::locale(), sizeof( locale ) );
//	s = strchr( locale, '_' );
//	if ( s ) {
//	    *s = 0;
//	}
//        lang = locale;
        setTranslator( a->langDir(), a->lang() );
//	printf("locale=%s\n", locale );
	QString str_ru=QString::null, str_en=QString::null;
	bool lang_setted = false;
	bool help_setted = false;
	for ( i=1; i<argc; i++)
	{
	    param = argv[i];
	    name = param.section("=",0,0).lower();
	    value = param.section("=",1);
	    if (param == "--help")
	    {
		    str_ru = "Использование: ananas-designer [--help] [--lang=<LANG>] [--rc=<RC_PATH>]\n";
		    str_ru+= "LANG=ru|en\n";
		    str_ru+= "RC_PATH=путь к *.rc файлу описания бизнес схемы\n";

	    	    str_en = "Usage: ananas-designer [--help] [--lang=<LANG>] [--rc=<RC_PATH>]\n";
		    str_en+= "LANG=ru|en\n";
		    str_en+= "RC_PATH=path to *.rc file of paticular business scheme\n";
		    help_setted = true;

	    }
	    if (name == "--lang") {
		lang = value;
		lang_setted = true;
	        setTranslator( a->langDir(), lang );
	    }
	    if (name == "--rc") rcfile = value;
	}
	if(help_setted)
	{
		if(lang == "ru")
		{
			printf("%s",(const char*)str_ru.local8Bit());
		}
		else
		{
			printf("%s",str_en.ascii());
		}
		return 1;
	}
	return 0;
}

int main( int argc, char ** argv )
{
	AApplication app ( argc, argv, AApplication::Designer );
	MainForm *appWindow = new MainForm();
//#ifndef _Windows
	QTextCodec::setCodecForCStrings( QTextCodec::codecForName("UTF8") );
//#endif

    // Для QSettings
	app.setOrganizationName("ananasgroup");
	app.setApplicationName("ananas");

	mainform = appWindow;
	// mainformws = mainform->ws;
	// mainformwl = mainform->wl;
//	application = &app;
//	dSelectDB dselectdb;
	QPixmap pixmap;
	QString configWorkDir = QDir::homeDirPath()+"./ananas";
	QString logLevel =  "0";

	if ( parseCommandLine( &app ) ) return 1;
	if ( !rcfile.isEmpty() )
	{
		rc.read( rcfile );
		configWorkDir = rc.value("workdir", configWorkDir);
		logLevel = rc.value("debug", "0");
		
	}
	if (logLevel == "0" || logLevel == "1" || logLevel == "2")
		aLog::init( configWorkDir+"/ananas.log", logLevel.toInt() );
	else
		aLog::init( configWorkDir+"/ananas.log", 0 );

	qApp->installTranslator( &tr_lib );
	qApp->installTranslator( &tr_plugins );
	qApp->installTranslator( &tr_app );
	pixmap = QPixmap( ":/images/designer-splash-"+lang+".png" );
	if ( pixmap.isNull() )
#ifdef Q_OS_WIN32
	pixmap = QPixmap( qApp->applicationDirPath()+"/designer-splash-"+lang+".png" );
	qApp->addLibraryPath( qApp->applicationDirPath() );
#else
	pixmap = QPixmap( "/usr/share/ananas/designer/locale/designer-splash-"+lang+".png" );
	qApp->addLibraryPath( "/usr/lib/ananas/" );
	qApp->addLibraryPath( "/usr/lib/ananas/designer/" );
//	qApp->setLibraryPaths( QStringList() <<"/usr/lib/ananas/" );
#endif
	foreach (QString path, app.libraryPaths())
		aLog::debug(QString("libraryPaths list item " ) + path);
     
	if ( pixmap.isNull() )
	pixmap = QPixmap( ":/images/designer-splash-en.png" );
   	QSplashScreen *splash = new QSplashScreen( pixmap );


	dSelectDB dialogDBSelect;

	if ( rcfile.isEmpty() )
	{
		if (dialogDBSelect.exec()==QDialog::Accepted) rcfile = dialogDBSelect.rcfile;
	}
	if ( !rcfile.isEmpty() )
	{
		if ( !database ) database = aDatabase::database();
		if ( !database->init( rcfile ) )
		{
			aLog::error("Unable to use rcfile " + rcfile);
			return 1;
		}
	}
	else
	{
		aLog::debug("No rcfile selected or available?");
		return 0;
	}

	splash->show();
	splash->message( QObject::tr("Init application"), Qt::AlignBottom, Qt::white );
	appWindow->rcfile = rcfile;
	appWindow->md = &database->cfg;
	qApp->setMainWidget( appWindow );
	splash->message( QObject::tr("Init forms designer"), Qt::AlignBottom, Qt::white  );
	formdesigner = new aFormDesigner();

	//--formdesigner->reparent( mainformws, 0, QPoint( 5, 5 ), false );
	//		mainformws->addWindow(formdesigner);
	formdesigner->setCaption( QObject::tr("Dialog forms designer") );
	formdesigner->hide();
	appWindow->show();
	splash->clear();
	splash->finish( appWindow );
	delete splash;
	qApp->connect( qApp, SIGNAL( lastWindowClosed() ), qApp, SLOT( quit() ) );
	// mainform->configOpen();
	appWindow->configOpen();
	//		QTextCodec::setCodecForCStrings( QTextCodec::codecForName("UTF8") );

	int res = app.exec();
	// if( appWindow ) delete appWindow;
	// appWindow=0;
	// aLog::close();
	// ananas_logout();
	// return rc;

	aLog::close();
	return res;
}
