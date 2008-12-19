#include "dselectdb.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

#include "atests.h"
#include "dhelpcfgform.h"
/*
 *  Constructs a dSelectDB as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
dSelectDB::dSelectDB(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
    ,settings( QSettings::IniFormat, QSettings::UserScope, "Ananas company Ltd", "Ananas4")
{
    setupUi(this);

    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
dSelectDB::~dSelectDB()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void dSelectDB::languageChange()
{
    retranslateUi(this);
}


//#include "acfgobj.h"
#include <qdir.h>
#include <qstring.h>
#include <q3filedialog.h>
#include <qlineedit.h>
#include <qstring.h>
#include <qdom.h>
//Added by qt3to4:
#include <Q3PopupMenu>

#include "deditrc.h"
#include "dimportdb.h"
#include "acfgrc.h"
#include "atests.h"
#include "adataexchange.h"
#include "alog.h"

void dSelectDB::init()
{
	aLog::init("ananas.log", aLog::Debug);
	createMenu();
	progressBar1->hide();
	QString fname;
	withgroups = 1;
	//--settings.insertSearchPath( QSettings::Unix, QString(QDir::homeDirPath())+QString("/.ananas"));
	//--settings.insertSearchPath( QSettings::Windows, "/ananasgroup/ananas" );

	local = settings.entryList("/groups").count();
	changes = false;
	/*--if(!local)
	{

		aLog::print(aLog::Debug, tr("dSelectDB local settings not found"));
		settings.insertSearchPath( QSettings::Windows, "/ananasgroup/ananas/globalsettings");
#ifdef Q_OS_WIN32
		QStringList subkeys = settings.subkeyList("/ananasgroup/ananas/globalsettings");
		settings.insertSearchPath(QSettings::Windows,"/ananasgroup/ananas/globalsettings");
		QStringList entryGroup;
		for(uint i=0; i<subkeys.count();i++)
		{
			QString str =subkeys[i];
			//str = str.left(str.length()-2); // length("rc")=2
			entryGroup =settings.entryList("/"+str);
			settings.beginGroup("/"+str);
			readSettings(entryGroup);
			settings.endGroup();
		}

#else
		QString suff = "*_grouprc";
		QDir dir("/etc/ananas",suff,QDir::Name | QDir::IgnoreCase, QDir::Files | QDir::Readable);
		QStringList subkeys = dir.entryList(suff);
		settings.insertSearchPath(QSettings::Unix,"/etc/ananas/");
		QStringList entryGroup;
		for(uint i=0; i<subkeys.count();i++)
		{
			QString str =subkeys[i];
			str = str.left(str.length()-2); // length("rc")=2
			entryGroup =settings.entryList("/"+str);
			settings.beginGroup("/"+str);
			readSettings(entryGroup);
			settings.endGroup();
		}
#endif
	}
	else*/
	{
		QStringList lst = settings.entryList("/groups");
		settings.beginGroup("/groups");
		readSettings(lst);
		settings.endGroup();
	}
	setIcon( rcIcon("a-system.png"));
	listDBRC->hideColumn( 1 );
	listDBRC->setSorting( -1 );
	listDBRC->header()->hide();
	listDBRC->setRootIsDecorated(1);
	buttonOk->setEnabled( FALSE );
	return;
}

void dSelectDB::readSettings(QStringList entryGroup)
{
	rcfile="";
	ro_mode=0;
	QFile f, fc;
	QString s, sp, sn, rc;
	rcListViewItem *item, *gitem=NULL; //, *subitem;
	for(uint j=0; j<entryGroup.count();j++)
	{
        	QStringList eitems;
		eitems = settings.entryList(entryGroup[j]);
		QString groupName = "unknown group";
		groupName = settings.readEntry(entryGroup[j]);
		aLog::print(aLog::Debug, tr("dSelectDB read settings for group with name %1").arg(groupName));
		rcListViewItem * lastIt = (rcListViewItem *) listDBRC->lastItem();
		if(lastIt!=NULL)
		{
			while(lastIt->depth()>0)
			{
				lastIt =(rcListViewItem *) lastIt->parent();
			}
			gitem= new rcListViewItem(listDBRC, lastIt, groupName, "", true );
			gitem->setOpen(true);
		}
		else
		{
			gitem= new rcListViewItem(listDBRC, groupName, "", true );
			gitem->setOpen(true);
		}
//		withgroups=1;
		QMap<QString,QString> cfg;
		for(int k = eitems.count()-1; k>=0; k--)
		{
			if(k<0) break;
			rc = settings.readEntry(entryGroup[j]+"/"+eitems[k]);
			cfg = aTests::readConfig(QDir::convertSeparators(rc));
			sn=cfg["dbtitle"];
			if (gitem) item= new rcListViewItem( gitem, sn, rc );
			else item= new rcListViewItem( listDBRC, sn, rc );
		}
	}
}


void dSelectDB::itemSelect( Q3ListViewItem *item )
{
	if (! item ) return;
	rcListViewItem *i = ( rcListViewItem *) item ;
	buttonOk->setEnabled( !i->group );
}


void dSelectDB::newGroup()
{
	Q3ListViewItem *gitem;
	gitem = new rcListViewItem(listDBRC, tr("New group"), "", true );
	listDBRC->setRootIsDecorated(1);
	gitem->setOpen(true);
	listDBRC->setSelected(gitem, true);
	editItem();
	withgroups=1;
	changes = true;
	//saveRC();
}


void dSelectDB::newItem()
{
	rcListViewItem *item, *gitem;
	QString rc;
	item= ( rcListViewItem *) listDBRC->selectedItem();
	if (!item) return;
	if (withgroups) {
		if (item->parent()) gitem = ( rcListViewItem *) item->parent();
		else gitem=item;
		item = new rcListViewItem(gitem, tr("New shema"), "myrc");
		gitem->setOpen(true);

	} else {
		item = new rcListViewItem(listDBRC, tr("New shema"), "myrc");
	}
	listDBRC->setSelected(item, true);
	editItem();
	changes = true;
}


void dSelectDB::editItem()
{
	rcListViewItem *item;
	dEditRC *d = new dEditRC(this);

	item = ( rcListViewItem *) listDBRC->selectedItem();
	if (!item) return;
	if ( item->group )
	{
	// Group
		item->setRenameEnabled(0, true);
		item->startRename(0);
		changes = true;
	}
	else
	{
	// Resource
		d->setdata( QDir::convertSeparators(item->rcfile), item );
		if ( d->exec() == QDialog::Accepted )
		{
			changes = true;
		}
	}
}



void dSelectDB::deleteItem()
{
	Q3ListViewItem *item;
	item=listDBRC->selectedItem();
	if (item)
	{
		QString msg = tr("Delete item?");
		if(QMessageBox::question(this,tr("confirm delete"),msg,QMessageBox::Ok,QMessageBox::Cancel)!=QMessageBox::Ok)
		{
			return;
		}

		delete item;
		if (listDBRC->childCount()==0) withgroups=0;
		changes = true;
	}
}


void dSelectDB::saveRC()
{
	rcListViewItem *item, *gitem;
	QFile f( rcfile );
	aLog::print(aLog::Info, tr("dSelectDB save settings"));
	if (!local)
	{
		if(!changes) return;
		QString home = QDir::homeDirPath();
#ifndef Q_OS_WIN32
		QString msg = QString("Все внесенные изменения будут сохранены локально в вашем домашнем каталоге.\nПри очередном запуске программы будут использованы локальные настройки из каталога\n%1/.ananas.\nЕсли Вы хотите использовать глобальные настройки, вам следует удалить каталог\n%2/.ananas").arg(home).arg(home);
#else
		QString msg = tr("save settings local?");
#endif
		if(QMessageBox::question(this,tr("save"),QString::fromUtf8(msg),QMessageBox::Ok,QMessageBox::Cancel)!=QMessageBox::Ok)
		{
			return;
		}
	}
		//--settings.removeSearchPath( QSettings::Unix, "/etc/ananas" );
		//--settings.removeSearchPath( QSettings::Windows, "/ananasgroup/ananas/globalsettings");


		gitem= ( rcListViewItem *) listDBRC->firstChild();
		uint gcount=0;
		uint ecount=0;
		clearSettings();

		settings.beginGroup("/groups");

		while (gitem)
		{
			if (withgroups)
			{
                                settings.writeEntry(QString::number(gcount),gitem->text(0));
				if(gitem->childCount())
				{
					item = ( rcListViewItem *) gitem->firstChild();
					while (item)
					{
						settings.writeEntry(QString::number(gcount)+"/"+QString::number(ecount),item->rcfile);
						item =  ( rcListViewItem *) item->nextSibling();
						++ecount;
					}
				}
			}
			++gcount;
			gitem =  ( rcListViewItem *) gitem->nextSibling();
		}
		settings.endGroup();
}


void dSelectDB::onOK()
{
	rcListViewItem *item;

	saveRC();
	item =  ( rcListViewItem *) listDBRC->selectedItem();
	if (item) {
	             if ( !item->rcfile.isEmpty() ) {
			 rcfile = item->rcfile;
			 accept();
		}
	}
}


void dSelectDB::onCancel()
{
	//saveRC();
	reject();
}


void dSelectDB::ItemRenamed(Q3ListViewItem *item, int col)
{
	if (!item) return;
	if (col>1) return;
}


void dSelectDB::clearSettings()
{
        QStringList eitems;
	QStringList entryGroup =settings.entryList("/groups");
	settings.beginGroup("/groups");
	for(uint j=0; j<entryGroup.count();j++)
	{
		eitems = settings.entryList(entryGroup[j]);
		settings.removeEntry(entryGroup[j]);
		for(int k = eitems.count()-1; k>=0; k--)
		{
			if(k<0) break;
			settings.removeEntry(entryGroup[j]+"/"+eitems[k]);
		}
	}
	settings.endGroup();
}


void dSelectDB::onHelp()
{
	HelpCfgForm f;
	f.exec();
}

void dSelectDB::createMenu()
{

	menuBar = new QMenuBar(this);
	Q3PopupMenu *menuFile = new Q3PopupMenu(this);
	menuFile->insertItem(tr("E&xit"), this, SLOT(onCancel()));

	Q3PopupMenu *menuEdit = new Q3PopupMenu(this);
	menuEdit->insertItem(tr("New &group"), this, SLOT(newGroup()));
	menuEdit->insertItem(tr("&New shema"), this, SLOT(newItem()));
	menuEdit->insertSeparator();
	menuEdit->insertItem(tr("Edi&t"),  this, SLOT(editItem()));
	menuEdit->insertSeparator();
	menuEdit->insertItem(tr("&Delete"),  this, SLOT(deleteItem()));

	Q3PopupMenu *menuAction = new Q3PopupMenu(this);
	menuAction->insertItem(tr("&Import"),  this, SLOT(importItem()));
	menuAction->insertItem(tr("&Export"),  this, SLOT(exportItem()));

	menuBar->insertItem(tr("Program"), menuFile);
	menuBar->insertItem(tr("Actions"), menuEdit);
	menuBar->insertItem(tr("Service"), menuAction);
	layout()->setMenuBar(menuBar);
	menuBar->show();
}


void dSelectDB::importItem()
{
	rcListViewItem *item,*gitem;
	dImportDB *d = new dImportDB(this);
	item = (rcListViewItem *)listDBRC->selectedItem();
	if (!item) return;
	if (withgroups)
	{
		if (item->parent()) gitem = ( rcListViewItem *) item->parent();
		else gitem=item;
		item = new rcListViewItem(gitem, tr("New shema"), "myrc");
		gitem->setOpen(true);

	}
	else
	{
		item = new rcListViewItem(listDBRC, tr("New shema"), "myrc");
	}
	listDBRC->setSelected(item, true);
	d->setdata("", item );
	if ( d->exec() == QDialog::Accepted )
	{
		bool res;
		QString rcfile = item->rcfile;
		QString filename = d->eCfgName->text();
		progressBar1->show();
		progressBar1->reset();
		aBackup newBase;
		connect (&newBase, SIGNAL(progress(int,int)), progressBar1, SLOT( setProgress(int,int)));
		if(newBase.importData(rcfile, filename, true)==false)
		{
			QMessageBox::information(this,tr("Backup"),newBase.lastError(),Qt::NoButton);
			progressBar1->hide();
			aLog::print(aLog::Info, tr("dSelectDB business schema import"));
		}
		else
		{
			aLog::print(aLog::Error, tr("dSelectDB restore: %1").arg(newBase.lastError()));
			QMessageBox::critical(this,tr("Backup"),newBase.lastError(),0, Qt::NoButton);
			progressBar1->hide();
			delete item;
			item =0;
			return;
		}



		changes = true;
	}
	else
	{
		delete item;
		item = 0;
		return;
	}
}


void dSelectDB::exportItem()
{
//	progressBar1->setTotalSteps(10);
	QString dir,rcfile,filename;
#ifndef Q_OS_WIN32
	dir = "/home";
#endif

	rcListViewItem *item =  ( rcListViewItem *) listDBRC->selectedItem();
	if (item)
	{
		if ( !item->rcfile.isEmpty() && !item->group )
		{
			rcfile = item->rcfile;
		}
		else
		{
			QMessageBox::information(this,tr("Select item"),tr("Please, select item for export"),Qt::NoButton);
			return;
		}

		Q3FileDialog *fdlg = new Q3FileDialog(this, "fileDialog",true); // create modal dialog
		fdlg->setMode ( Q3FileDialog::AnyFile );
		if(fdlg->exec()==QDialog::Accepted)
		{
			filename = fdlg->selectedFile();
		}
		else
		{
			return;
		}
		delete fdlg;
		fdlg = 0;

		aBackup backupBase;
		progressBar1->show();
		progressBar1->reset();
		connect (&backupBase, SIGNAL(progress(int,int)), progressBar1, SLOT( setProgress(int,int)));
		if(backupBase.exportData(rcfile, filename, true )==0)
		{
			aLog::print(aLog::Info, tr("dSelectDB backup ok"));
			QMessageBox::information(this,tr("Backup"),backupBase.lastError(),Qt::NoButton);
			progressBar1->hide();

		}
		else
		{
			aLog::print(aLog::Debug, tr("dSelectDB backup: %1").arg(backupBase.lastError()));
			QMessageBox::critical(this,tr("Backup"),backupBase.lastError(),0, Qt::NoButton);
			progressBar1->hide();
			return;
		}
	}
	else
	{
		QMessageBox::information(this,tr("Select item"),tr("Please, select item for export"),Qt::NoButton);
		return;
	}

}


void dSelectDB::onDblClick( Q3ListViewItem *item)
{

	if (! (( rcListViewItem *)item)->group )
	{
		onOK();

	}
}

