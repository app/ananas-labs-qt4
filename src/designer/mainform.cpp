#include "mainform.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

#include "qtoolbutton.h"

#include <qapplication.h>
#include <qsettings.h>
#include "q3filedialog.h"
#include "qstatusbar.h"
#include "qmessagebox.h"
#include <qapplication.h>
//Added by qt3to4:
#include <QCloseEvent>
#include <Q3Frame>
#include <Q3PopupMenu>

#include "ananas.h"
//#include "adatabase.h"
//#include "acfgrc.h"
#include "cfgform.h"
//#include "messageswindow.h"
#include "dselectdb.h"
#include "alog.h"

//#include "qananastable.h"

extern CfgForm *configform;
//extern QApplication *application;
extern void messageproc(int n, const char *msg);

/*
 *  Constructs a MainForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
MainForm::MainForm(QWidget* parent, const char* name, Qt::WindowFlags fl)
    : Q3MainWindow(parent, name, fl)
{
    setupUi(this);

    (void)statusBar();
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
MainForm::~MainForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void MainForm::languageChange()
{
    retranslateUi(this);
}

void MainForm::filePrint()
{

}

void MainForm::fileExit()
{
//    cfg_done();
}

void MainForm::editUndo()
{

}

void MainForm::editRedo()
{

}

void MainForm::editCut()
{

}

void MainForm::editCopy()
{

}

void MainForm::editPaste()
{

}

void MainForm::editFind()
{

}

void MainForm::helpIndex()
{

}

void MainForm::helpContents()
{

}

void MainForm::helpAbout()
{
    QMessageBox::about( this, tr("About Ananas.Designer"),
			tr("<h4>Ananas.Designer %1</h4> is a programm<br>"
			   "for adjusting accounting automation system<br><br>"
			   "Copyright 2003-2006 Leader Infotech, Valery Grazdankin <br>"
			   "Copyright 2003-2006 Project Ananas, Andrey Paskal, Grigory Panov, Andrey Strelnikov<br>"
			   "License: GPL<br><br>"
			   "technical support:<br>"
			   "lider-infotech@narod.ru<br>"
			   "app@linux.ru.net<br>"
			   "Web sites:<br>"
			   "<a href=\"http://ananas.lrn.ru\">http://ananas.lrn.ru</a><br>"
			   "<a href=\"http://www.leaderit.ru\">http://www.leaderit.ru</a>"
			   "<br><br>Avalable extensions:<br>%2").arg(ananas_libversion()).arg( ""/*--AExtensionFactory::keys().join(", ")*/) );
}


void MainForm::init()
{


    setName("ananas-designer_mainwindow");
    rcfile="";
    windowsMenu = new Q3PopupMenu( this );
    windowsMenu->setCheckable( TRUE );
    connect( windowsMenu, SIGNAL( aboutToShow() ),
	     this, SLOT( windowsMenuAboutToShow() ) );
    menuBar()->insertItem( tr("&Windows"), windowsMenu );

    menuBar()->insertSeparator();
    Q3PopupMenu * help = new Q3PopupMenu( this );
    menuBar()->insertItem( tr("&Help"), help );

    help->insertItem( tr("&About"), this, SLOT(helpAbout()), Qt::Key_F1);
  //  help->insertItem( tr("&Test"), this, SLOT(helpTest()));
    //    help->insertItem( "About &Qt", this, SLOT(aboutQt()));
    help->insertSeparator();
    //    help->insertItem( trUtf8("Что &это"), this, SLOT(whatsThis()), SHIFT+Key_F1);


    //--designer_settings.insertSearchPath( QSettings::Unix, QString(QDir::homeDirPath())+QString("/.ananas"));
    //--designer_settings.insertSearchPath( QSettings::Windows, "/ananasgroup/ananas" );

//    QStringList lst = settings.entryList("/engine");
    QSettings designer_settings;
    designer_settings.beginGroup("/designer");
    bool maximize = designer_settings.readBoolEntry( "/maximize", 0 );
    int width = designer_settings.readNumEntry( "/geometry/width", 600 );
    int height = designer_settings.readNumEntry( "/geometry/height", 480 );
    int offset_x = designer_settings.readNumEntry( "/geometry/x", 0 );
    int offset_y = designer_settings.readNumEntry( "/geometry/y", 0 );
    designer_settings.endGroup();
    resize( QSize(width, height) );//  .expandedTo(minimumSizeHint()) );
    move(offset_x,offset_y);
    if(maximize)
    {
//	   setWindowState(windowState() ^ WindowMaximized);
    }

    setIcon( rcIcon("a-system.png"));
    setCaption(tr("Ananas: Designer"));
    Q3VBox* vb = new Q3VBox( this );
    vb->setFrameStyle( Q3Frame::StyledPanel | Q3Frame::Sunken );
    ws = new QWorkspace( vb );
    wl = new aWindowsList();
    ws->setScrollBarsEnabled( TRUE );
    setCentralWidget( vb );
    cfgform=NULL;

    MessagesWindow *msgWindow = new MessagesWindow( this ); // , WDestructiveClose );
    moveDockWindow( msgWindow, Qt::DockBottom );
    setMessageHandler( TRUE );
    msgWindow->hide();
    tbMetadata->setShown( FALSE );
    tbActions->setShown( FALSE );
    tbInterface->setShown( FALSE );
    tbRights->setShown( FALSE );
    tbLanguages->setShown( FALSE );
    tbImageCollection->setShown( FALSE );
    TBToolbar->setShown( FALSE );
    tbTabs->setShown( FALSE );
    lastTabId = 0;
	lastFormId = 0;
    configSaveAction->setVisible( FALSE );
}

void MainForm::configNew()
{
	CfgForm *cf = cfgForm();
	if (cf)
	{
		if ( !cf->close() ) return; // close configuration form if opened
    	}
    	cf = new CfgForm(ws, 0, Qt::WDestructiveClose); // create new cfgform
   	if ( cf )
	{
		cf->init( rcfile, TRUE ); // initialization NEW configuration
		connectSignals( cf );
		cf->ws=ws;
		cf->show();
 		((QWidget*)cf->parent())->move(0,0);
		//--addTab(++lastTabId,cf->name());
		addTab(cf);
	}
}

void MainForm::configOpen()
{
	CfgForm *cf = cfgForm();
	if ( cf )
	{
		if ( !cf->close() ) return;
   	}
	if ( rcfile.isEmpty()) return;
	cf = new CfgForm( ws, 0, Qt::WDestructiveClose);
	if ( cf )
	{
		cf->init( rcfile, FALSE );	//	initialization configuration
		connectSignals( cf );
		cf->ws=ws;
		cf->show();
 		((QWidget*)cf->parent())->move(0,0);
		//--addTab(++lastTabId,cf->name());
		addTab(cf);
    }
}

void MainForm::configSave()
{
    emit tosave();
    CfgForm *cf = cfgForm();
    if ( cf ) cf->save();	//	get configuration form and save its data
}

void MainForm::configSaveAs()
{
}



void MainForm::fileOpen()
{

}

void MainForm::fileSave()
{

}

void MainForm::fileSaveAs()
{

}

void MainForm::fileNew()
{

}

void MainForm::windowsMenuActivated( int id )
{
    QWidget* w = ws->windowList().at( id );
    if ( w ) w->showNormal();
    w->setFocus();
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
    for ( int i = 0; i < int( windows.count() ); ++i ) {
	int id = windowsMenu->insertItem(windows.at(i)->caption(), this, SLOT( windowsMenuActivated( int ) ) );
	windowsMenu->setItemParameter( id, i );
	windowsMenu->setItemChecked( id, ws->activeWindow() == windows.at(i) );
    }
}

void MainForm::tileHorizontal()
{
    // primitive horizontal tiling
    QWidgetList windows = ws->windowList();
    if ( !windows.count() )
	return;

    int heightForEach = ws->height() / windows.count();
    int y = 0;
    for ( int i = 0; i < int(windows.count()); ++i ) {
	QWidget *window = windows.at(i);
	if ( window->windowState() == Qt::WindowMaximized ) {
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


void MainForm::closeEvent( QCloseEvent *e )
{
    if (configform)
    {
	if (!configform->close())
	{
	    e->ignore();
	    return;
	}
    }

    QSettings designer_settings;
    designer_settings.beginGroup("/designer");
//	designer_settings.writeEntry( "/maximize", windowState()&WindowMaximized ? true: false);
	designer_settings.writeEntry( "/geometry/width", width() );
	designer_settings.writeEntry( "/geometry/height", height() );
	designer_settings.writeEntry( "/geometry/x", pos().x() );
	designer_settings.writeEntry( "/geometry/y", pos().y() );
	designer_settings.endGroup();
	//aLog::print(aLog::Debug,"exit");
    Q3MainWindow::closeEvent( e );
}


void MainForm::connectSignals( CfgForm *cf )
{
    connect(objNewFormAction, SIGNAL(activated()), cf, SLOT(newForm()));
    connect(objNewTableAction, SIGNAL(activated()), cf, SLOT(newTable()));
    connect(objNewFieldAction, SIGNAL(activated()), cf, SLOT(newField()));
    connect(objRenameAction, SIGNAL(activated()), cf->mdtree, SLOT(itemRename()));
    connect(objNewObjAction, SIGNAL(activated()), cf->mdtree, SLOT(itemNew()));
    connect(objEditAction, SIGNAL(activated()), cf->mdtree, SLOT(itemEdit()));
    connect(objDeleteAction, SIGNAL(activated()), cf->mdtree, SLOT(itemDelete()));
}


void MainForm::getMd( aCfg **md )
{
	CfgForm *cf = cfgForm();
	if ( !cf ) return;
	*md = &cf->cfg;
}

aCfg* MainForm::getMetadata()
{
	return md;
}

/*!
Get current configuration form
*/
CfgForm *
MainForm::cfgForm()
{
	CfgForm *res = 0;
	QWidgetList windows = ws->windowList();
	for ( int i = 0; i < int( windows.count() ); ++i ) {
		if ( strcmp( windows.at(i)->className(),"CfgForm") == 0 ){
			res = ( CfgForm *) windows.at(i);
			break;
		}
	}
	return res;
}


QWidget *
MainForm::activeWindow()
{
	return ws->activeWindow();
}

void MainForm::setId( qulonglong* id )
{
	lastFormId= *id;
}

void
MainForm::getId(qulonglong* objId)
{
	*objId = lastFormId;
}


void MainForm::addTab(int uid, const QString& winName )
{
    QWidgetList windows = ws->windowList();
    QToolButton* bt;

    QString S = winName;
    for ( int i = 0; i < int( windows.count() ); i++ ) {
		if( !strcmp(windows.at(i)->name(), S.ascii()))
		{
			windows.at(i)->setName(QString(windows.at(i)->name()+QString("_%1").arg(uid)));
			//ixmap pixmap(*windows.at(i)->icon());
			bt = new QToolButton(QIcon(),
					     windows.at(i)->caption(),
					     "",
					     windows.at(i),
					     SLOT(setFocus( )),
					     tbTabs,
					     windows.at(i)->name() );

			bt->setUsesTextLabel ( true );
//		 	bt->setAutoRaise ( true );
		//	aLog::print(aLog::Debug, QString("button name = %1").arg(bt->name()));
			break;

		}
/*		if( windows.at(i)->isShown() and !strcmp(windows.at(i)->name(),"designer_mainwindow"))
		{

			bt = new QToolButton(QIconSet(),
					     windows.at(i)->caption(),
					     "",
					     windows.at(i),
					     SLOT(setFocus( )),
					     tbTabs,
					     windows.at(i)->name() );

			bt->setUsesTextLabel ( true );
//		 	bt->setAutoRaise ( true );
			printf("button name = %s\n",bt->name());
			break;
		}
		*/
	}
   // }
//    if(windows.count()>1)
    if(tbTabs->queryList ("QToolButton").size() > 0)
    {
	//    tbTabs->show();

    	tbTabs->setShown( true );
    }
    else
    {
	//   tbTabs->hide();
    	tbTabs->setShown( false );
    }
}


void MainForm::removeTab(const  QString &winName )
{
//	int ind = winName.findRev("_");
//	printf("winName = %s\n",winName.ascii());
//	QString str = winName;
//	str = str.remove(ind,winName.length()-ind);
	QObject *button = tbTabs->child( winName );
	if(button)
	{

//		printf("deletes tab\n");
		delete button;
		button = NULL;
	}

}


void MainForm::closeChildWindows()
{

    QWidgetList windows = ws->windowList();
    for ( int i = 0; i < int( windows.count() ); i++ )
    {
 if(    windows.at(i)->className() != "CfgForm")
 {
  windows.at(i)->close();
 }
    }
}


void MainForm::fileNewAction_activated()
{

}
