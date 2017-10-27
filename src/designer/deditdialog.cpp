#include "deditdialog.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

#include "qtextcodec.h"

#include <qstring.h>
#include <qfile.h>
#include <qstatusbar.h>
#include "acfg.h"
//#include "qsproject.h"
//#include "qsscript.h"
//#include <qsinterpreter.h>
#include <qdialog.h>
//--#include <qwidgetfactory.h>
#include <QFormBuilder>
#include <qobject.h>
#include <q3textstream.h>
#include <qbuffer.h>
//#include <qsinterpreter.h>
//#include <qseditor.h>
#include <q3process.h>
#include <qapplication.h>

#include "formdesigner.h"
#include "mainform.h"
#include "alog.h"

//#include "qananastable.h"
//#include <unistd.h>

//extern QApplication *application;

/*
 *  Constructs a dEditDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
dEditDialog::dEditDialog(QWidget* parent, const char* name, Qt::WindowFlags fl)
    : Q3MainWindow(parent, name, fl)
{
    setupUi(this);

    (void)statusBar();
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
dEditDialog::~dEditDialog()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void dEditDialog::languageChange()
{
    retranslateUi(this);
}

void dEditDialog::init()
{
	delete statusBar();
	docId =  0;
//	eModule->setInterpreter(new QSInterpreter());
//	fd = new aFormDesigner();
}


void dEditDialog::destroy()
{
    updateMD();
    ( (MainForm*)this->topLevelWidget() )->wl->remove( this );
    ( (MainForm*)this->topLevelWidget() )->removeTab(name());
}


void dEditDialog::setData( aListViewItem *o )
{
	item=NULL;
	QString parentClass;
	if (o) {
		item = o;
		aCfg *md = o->md;
		aCfgItem obj = o->obj;
		aAliasEditor *a = new aAliasEditor( md, obj, tAliases );
		al = a;
		al->setData();
		eName->setText( md->attr( obj, mda_name ) );
		eDescription->setText( md->sText( obj, md_description ) );
                eModule->setPlainText( md->sText( obj, md_sourcecode ) );
		eFormFile->setText( QString("inputform_")+QString::number(item->id)+QString(".ui"));
		setCaption( tr("Form:") + eName->text() );
		parentClass = md->objClass( md->parent ( md->parent( obj ) ) );
		docId = md->id(md->parent(md->parent( obj )));
		if ( parentClass == md_document ) {
//			cbFormMode->insertItem(QObject::tr("Document"));
		}
		if ( parentClass == md_catalogue ) {
//			cbDefault->insertItem(QObject::tr("selection"));
			cbFormMode->clear();
			cbFormMode->insertItem(QObject::tr("Elements list dialog"));
			cbFormMode->insertItem(QObject::tr("Element dialog"));
			cbFormMode->insertItem(QObject::tr("Group dialog"));
		}
		else cbSelect->setHidden( true );
		int i = md->sText(obj, md_defaultmod).toInt();
		cbNew->setChecked( (i>>md_form_new)%2 );
		cbView->setChecked( (i>>md_form_view)%2 );
		cbEdit->setChecked( (i>>md_form_edit)%2 );
		cbSelect->setChecked( (i>>md_form_select)%2 );
		i = md->attr( obj, mda_type ).toInt();
		cbFormMode->setCurrentItem( i );
		i = md->attr( obj, mda_readonly ).toInt();
		cbReadOnly->setChecked( i );
		QString ui= md->sText( obj, md_formdesign );
//		bPreview->setDisabled(ui.isEmpty());
	}
}

//#include "mainform.h"
//#include <formwindow.h>
//extern MainForm *mainform;

void dEditDialog::EditForm()
{
	foreach (QWidget *widget, QApplication::topLevelWidgets()) {
		if (widget->name() == QString("ananas-designer_mainwindow") )
		{
			connect( this, SIGNAL( setId( qulonglong * ) ), widget, SLOT( setId( qulonglong * ) ));
			emit ( setId( &docId ));
		}
    }

	aCfg *md = item->md;
	aCfgItem obj = item->obj, o;
	QString ui, s, tpl_name = "inputform.ui.tpl", tpldir;
	QFile f( eFormFile->text() );
//	char* arg;
//	int rc;
	//MainForm *mw = (MainForm*) topLevelWidget();
	//printf("end getting pointer to Main form\n");
	QStringList env;
//	QProcess proc( this );

#ifdef Q_OS_WIN32
	tpldir = qApp->applicationDirPath()+"/templates/";
#else
	tpldir = "/usr/share/ananas/templates/";
#endif
	tpldir = ":/designer/templates/";

/*
	int rc;
	char* arg;
	QFile f;
	QString s;
	const char *ui;
	int fc=0;

    if (cfg_rcfile()) {
	setenv("ANANASRCFILE",cfg_rcfile(),1);
    } else {
	cfg_message(1, "Ошибка! Не удалось получить значение для записи в ANANASRCFILE\n");
    }
*/

   if (!f.exists())
   {
	aLog::print(aLog::Debug,tr("dEditDialog form file does not exists"));
	ui= md->sText( obj, md_formdesign );
	if (!ui.isEmpty())
	{

		aLog::print(aLog::Debug, tr("dEditDialog create form file from metadata"));
		QFile f1(eFormFile->text());
		f1.open(QIODevice::WriteOnly);
		f1.writeBlock(ui, strlen(ui));
		f1.close();

	}
	else
	{
		aLog::print(aLog::Debug, tr("dEditDialog create new form file from template"));
		o = md->parent( md->parent( obj ) );
		if ( md->objClass( o ) == md_document ) tpl_name = "inputformdoc.tpl";
		if ( md->objClass( o ) == md_catalogue ) tpl_name = "inputformcat.tpl";
		if ( md->objClass( o ) == md_journal ) tpl_name = "inputformjourn.tpl";
		if ( md->objClass( o ) == md_report ) tpl_name = "inputformrep.tpl";
		QFile fi( tpldir+tpl_name );
		if ( fi.open( QIODevice::ReadOnly ))
		{
			if(f.open( QIODevice::WriteOnly ))
			{
				Q3TextStream tso( &f );
				tso.setEncoding( Q3TextStream::UnicodeUTF8 );
				QString form = fi.readAll();
//			printf("obj id = %i\n", md->id( o ) );
				form.replace( QString("$$$id$$$"), QString("%1").arg( md->id( o ) ) );
				form.replace( QString("$$$name$$$"), QString("") );
				form.replace( QString("$$$caption$$$"), md->attr( o, mda_name ) );
				tso << form; //tsi.read();
				fi.close();
				f.close();
			}
			else
			{
				aLog::print(aLog::Error, tr("dEditDialog open file %1 for write").arg(f.name()));
			}
		}
		else
		{
			aLog::print(aLog::Error,tr("dEditDialog open file %1 for read from templates directory %2").arg(tpl_name).arg(tpldir));
		}
//		s=QString("cp inputform.ui.tpl ")+  eFormFile->text();
// 		system((const char *)s);
	}
   }

   	if(!QFile(eFormFile->text()).exists())
   	{

		aLog::print(aLog::Error, tr("dEditDialog file %1 not exists").arg(eFormFile->text()));
   	}
	formdesigner->fileOpen( eFormFile->text() );
   	aLog::print(aLog::Debug, tr("dEditDialog show formdesigner"));
	formdesigner->show();

//	}


//
//	if (!proc.start()) {
//		printf("Error running qt designer\n");
//	}
//CHECK_POINT
/*
    if ((rc=fork())==0) {
	arg = strdup( (const char *) eFormFile->text() );
	execlp("designer","designer","-client",arg,0);
	printf("Error running qt designer: errno=%d\terror=%s\n",
		      errno,strerror(errno));
    } else if (rc==-1) {
	printf("Error while fork: errno=%d\terror=%s\n",
		      errno,strerror(errno));
    }
*/
}


void dEditDialog::EditModule()
{
//	cfgobj_editcode(objectptr);
}


void dEditDialog::updateMD()
{

    formdesigner->fileSaveAll();
	aCfg *md = item->md;
	aCfgItem obj = item->obj;
	QString ui;
	QFile f(eFormFile->text());
//if (cancelupdate) return;
	al->updateMD();
	item->setText( 0, eName->text().stripWhiteSpace() );
	md->setAttr( obj, mda_name, eName->text().stripWhiteSpace() );
	md->setAttr( obj, mda_type, cbFormMode->currentItem() );
	md->setAttr( obj, mda_readonly, cbReadOnly->isChecked() );
	md->setSText( obj, md_description, eDescription->text() );
        md->setSText( obj, md_sourcecode, eModule->toPlainText() );
//	ui.arg(cbDefault->currentItem());
//	md->setSText( obj, md_defaultmod, QString( "%1" ).arg( cbDefault->currentItem() ) );
	int i = (1<<md_form_new) * cbNew->isChecked() + \
		(1<<md_form_view) * cbView->isChecked() + \
		(1<<md_form_edit) * cbEdit->isChecked() +\
		(1<<md_form_select) * cbSelect->isChecked();
	md->setSText( obj, md_defaultmod, QString( "%1" ).arg( i ) );
	if (f.open(QIODevice::ReadOnly)){
		Q3TextStream ts(&f);
		ts.setEncoding(Q3TextStream::UnicodeUTF8);
		ui=ts.read();
		md->setSText( obj, md_formdesign, ui );
		f.close();
		f.remove();
	}

}



void dEditDialog::formPreview()
{
//    if ( !QFile::exists(eFormFile->text())) return;

	aCfg *md = item->md;
	aCfgItem obj = item->obj;
	QString ui, s;
	QFile f(eFormFile->text());

	if (!f.exists()){
		ui= md->sText( obj, md_formdesign );
		if (!ui.isEmpty()) {
			f.open(QIODevice::WriteOnly);
			f.writeBlock(ui, strlen(ui));
			f.close();
		} else {
		    return;
			QFile fi("inputform.ui.tpl");
			if ( fi.open( QIODevice::ReadOnly ) && f.open( QIODevice::WriteOnly ) ){
				Q3TextStream tsi( &fi ), tso( &f );
				tso << tsi.read();
				fi.close();
				f.close();
			}
		}
	}

	QIODevice *d;
	d= &f;
	d->open(QIODevice::ReadOnly);
	//--QWidget *form = QWidgetFactory::create(d);
    QFormBuilder fb;
    QWidget *form = fb.load(d);
	d->close();
	if (form) form->show();
	else printf("Error create form\n");

}




qulonglong dEditDialog::getDocId()
{
    return docId;
}
