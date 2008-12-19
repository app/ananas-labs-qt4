/****************************************************************************
** $Id: mainform.cpp,v 1.2 2008/12/06 22:56:39 leader Exp $
**
** Code file of the Main form of Ananas Engine application
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
**
** This file is part of the Engine application of the Ananas
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

/****************************************************************************
****************************************************************************/
#include <qmenubar.h>
#include <qmessagebox.h>
#include <q3toolbar.h>
#include <qstatusbar.h>
#include <qworkspace.h>
#include <qapplication.h>
#include <qaction.h>
#include <qdir.h>
//Added by qt3to4:
#include <Q3Frame>
#include <QPixmap>
#include <Q3PopupMenu>

#include "mainform.h"

#include "ananas.h"

MainForm *mainform=NULL;
QWorkspace *mainformws=NULL;
aWindowsList *mainformwl=NULL;
//extern void messageproc(int n, const char *msg);

/*
 *  Constructs a AnanasMainWindow as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
MainForm::MainForm( QWidget* parent, const char* name, Qt::WFlags fl )
    : Q3MainWindow( parent, name, fl )
{
//    QApopupmenu *popup;
    Q3VBox	*vb = new Q3VBox(this);

    setIcon( rcIcon("a-system.png"));
    vb->setFrameStyle( Q3Frame::StyledPanel | Q3Frame::Sunken );
    ws = new QWorkspace( vb );
    wl = new aWindowsList();
    ws->setScrollBarsEnabled( TRUE );
    setCentralWidget( vb );
    statusBar()->setName("statusbar");
    if ( !name ) setName( "mainwindow" );
    //--engine_settings.insertSearchPath( QSettings::Unix, QString(QDir::homeDirPath())+QString("/.ananas"));
    //--engine_settings.insertSearchPath( QSettings::Windows, "/ananasgroup/ananas" );

//    QStringList lst = settings.entryList("/engine");
    engine_settings.beginGroup("/engine");
    bool maximize = engine_settings.readBoolEntry( "/maximize", 0 );
    int width = engine_settings.readNumEntry( "/geometry/width", 600 );
    int height = engine_settings.readNumEntry( "/geometry/height", 480 );
    int offset_x = engine_settings.readNumEntry( "/geometry/x", 0 );
    int offset_y = engine_settings.readNumEntry( "/geometry/y", 0 );
    engine_settings.endGroup();
    resize( QSize(width, height) );//  .expandedTo(minimumSizeHint()) );
    move(offset_x,offset_y);
    if(maximize)
    {
//	   setWindowState(windowState() ^ WindowMaximized);
    }
    rcfile="";
}

bool
MainForm::init()
{
    MessagesWindow *msgWindow = new MessagesWindow( this );// , WDestructiveClose );
    moveDockWindow( msgWindow, Qt::DockBottom );
    setMessageHandler( true );
    msgWindow->hide();
    if ( !initEngine() ) return false;
    connect( menubar, SIGNAL(activated(int)), &engine, SLOT(on_MenuBar(int)) );
    languageChange();
    engine.project.addObject(menubar);
    engine.project.addObject(this);
    engine.project.addObject(statusBar());
    languageChange();
    engine.on_systemstart();
    return true;
}

bool
MainForm::initEngine()
{
    if (rcfile.isEmpty()) return false;
    if ( !engine.init( rcfile ) ) return false;
    engine.ws = ws;
    engine.wl = wl;
    connect( &engine, SIGNAL( statusMessage( const QString & ) ), this, SLOT( statusMessage( const QString & ) ) );
    md = engine.md;
    if ( !md ) return false;
    initMenuBar();
    initStatusBar();
    initActionBar();
    return true;
}

void
MainForm::initMenuBar()
{
	Q3PopupMenu *m;
	m = new Q3PopupMenu();
	windowsMenu = new Q3PopupMenu();
    	connect( windowsMenu, SIGNAL( aboutToShow() ),
	     this, SLOT( windowsMenuAboutToShow() ) );
	m->insertItem(rcIcon("a-system.png"), tr( "About" ), this, SLOT( helpAbout() ));
	//windowsMenu->insertItem(rcIcon("a-system.png"), tr( "Windows" ), this, SLOT( windowsMenuAboutToShow() ));
        menubar = new AMenuBar( md, this, "menubar");
	InsertMainMenu( tr("&Help"), m );
    	InsertMainMenu( tr("&Windows"), windowsMenu );
	menuBar()->show();
}

void
MainForm::initStatusBar()
{
}

void
MainForm::initActionBar()
{
    aCfgItem obj, item;

    obj = md->find( md->find( mdc_interface ), md_toolbars );	// look for md_toolbars
    item = md->firstChild( obj );
    while ( !item.isNull() ) {	// foreach md_toolbar
	new aToolBar( md, item, &engine, this, md->attr( item, mda_name ) );		// create toolbar
	item = md->nextSibling( item );	// next md_toolbar
    }
}

void
MainForm::helpAbout()
{
    QMessageBox::about( this, tr("About Ananas.Engine program"),
			tr("<h4>Ananas.Engin %1</h4> is a programm "
			   "for executing application of accounting automation system<br><br>"
			   "Copyright 2003-2008 Leader Infotech, Valery Grazdankin, "
			   "Copyright 2003-2008 Project Ananas, Andrey Paskal, Grigory Panov, Andrey Strelnikov<br>"
			   "License: GPL<br><br>"
			   "technical support:<br>"
			   "lider-infotech@narod.ru<br>"
			   "app@linux.ru.net<br>"
			   "Web sites:<br>"
			   "<a href=\"http://ananas.lrn.ru\">http://ananas.lrn.ru</a><br>"
			   "<a href=\"http://www.leaderit.ru\">http://www.leaderit.ru</a>"
			   "<br>Avalable extensions:<h4>%2</h4><br>"
			   ).arg(ananas_libversion()).arg( AExtensionFactory::keys().join(", ") ) );
}

void MainForm::InsertMainMenu(QString text, QObject *pop){
    menubar->insertItem(text, (Q3PopupMenu *) pop);

}


void
MainForm::Exit(int code)
{

	qApp->exit(code);
}
void
MainForm::close()
{
        engine_settings.beginGroup("/engine");
//	engine_settings.writeEntry( "/maximize", windowState()&WindowMaximized ? true: false);
	engine_settings.writeEntry( "/geometry/width", width() );
	engine_settings.writeEntry( "/geometry/height", height() );
	engine_settings.writeEntry( "/geometry/x", pos().x() );
	engine_settings.writeEntry( "/geometry/y", pos().y() );
	engine_settings.endGroup();
	printf("exit\n" );
//MainWindow::close();
}

void
MainForm::statusMessage( const QString &msg )
{
 	statusBar()->message( msg );
}

void
MainForm::setBackground( const QPixmap &pix ){
	ws->setBackgroundPixmap( pix );
}

/*
 *  Destroys the object and frees any allocated resources
 */
MainForm::~MainForm()
{
	engine.on_systemstop();
	close();
//	printf("Mainform destructor\n");
	if(ws) delete ws;
	ws = 0;
	if(wl) delete wl;
	wl=0;
	if(menubar) delete menubar;
	menubar=0;
//	printf("delete mainform\n");
//    cfg->on_systemstop();
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void MainForm::languageChange()
{
    setCaption(QString( tr("Ananas")+" "+ananas_libversion() )+": "+md->info( md_info_name ) );
//    setCaption( tr( "Ananas VERSION" ) );
}

void MainForm::windowsMenuAboutToShow()
{
    windowsMenu->clear();
    int cascadeId = windowsMenu->insertItem(tr("&Cascade"), ws, SLOT(cascade() ) );
    int tileId = windowsMenu->insertItem(tr("&Tile"), ws, SLOT(tile() ) );
    int horTileId = windowsMenu->insertItem(tr("Tile &horizontal"), this, SLOT(tileHorizontal() ) );
    if ( ws->windowList().isEmpty() ) {
	windowsMenu->setItemEnabled( cascadeId, FALSE );
	windowsMenu->setItemEnabled( tileId, FALSE );
	windowsMenu->setItemEnabled( horTileId, FALSE );
    }
    windowsMenu->insertSeparator();
    QWidgetList windows = ws->windowList();
    if(windows.count()==0) return;
    int i=0, count = windows.count();

    do
    {
	int id=0;
	if(windows.at(i) && windows.at(i)->isHidden())
	{
		++i;
		continue;
	}
	id = windowsMenu->insertItem(windows.at(i)->caption(), this, SLOT( windowsMenuActivated( int ) ) );
	windowsMenu->setItemParameter( id, i );
	windowsMenu->setItemChecked( id, ws->activeWindow() == windows.at(i) );
	++i;
    }while( i < count );
}

void MainForm::tileHorizontal()
{
    // primitive horizontal tiling
    QWidgetList windows = ws->windowList();
    int count =0;
    for ( int i = 0; i < int(windows.count()); ++i )
    {
	QWidget *window = windows.at(i);
	if( !window ) continue;
	if(!window->isHidden()) count++;
    }
    if ( !count ) return;

    int heightForEach = ws->height() / count;
    int y = 0;
    for ( int i = 0; i < windows.count(); ++i )
    {
	QWidget *window = windows.at(i);
	if ( !window ) continue;
	if ( window->isHidden() ) continue;
	if ( window->windowState() == Qt::WindowMaximized )
	{
	    // prevent flicker
	    window->hide();
	    window->showNormal();
	}
	int preferredHeight = window->minimumHeight()+window->parentWidget()->baseSize().height();
	int actHeight = QMAX(heightForEach, preferredHeight);

	window->parentWidget()->setGeometry( 0, y, ws->width(), actHeight );
	y += actHeight;
    }
}

void MainForm::windowsMenuActivated( int id )
{
    QWidget* w = ws->windowList().at( id );
    if ( w ) w->showNormal();
    w->setFocus();
}
