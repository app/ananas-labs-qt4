#include "deditwebform.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

#include <qstatusbar.h>
#include "acfg.h"
//#include <qsinterpreter.h>
//#include <qseditor.h>

/*
 *  Constructs a dEditWebForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
dEditWebForm::dEditWebForm(QWidget* parent, const char* name, Qt::WindowFlags fl)
    : Q3MainWindow(parent, name, fl)
{
    setupUi(this);

    (void)statusBar();
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
dEditWebForm::~dEditWebForm()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void dEditWebForm::languageChange()
{
    retranslateUi(this);
}

void dEditWebForm::init()
{
	delete statusBar();
//	eServerModule->setInterpreter(new QSInterpreter());
//	eClientModule->setInterpreter(new QSInterpreter());
}

void dEditWebForm::destroy()
{
    updateMD();
    ( (MainForm*)this->topLevelWidget() )->wl->remove( this );
    ( (MainForm*)this->topLevelWidget() )->removeTab(name());
    item->editor = 0;
}


void dEditWebForm::setData( aListViewItem *o )
{
	item = o;
	aCfg *md = o->md;
	aCfgItem obj = o->obj;

	aAliasEditor *a = new aAliasEditor( md, obj, tAliases );
	al = a;
	al->setData();
	setCaption( tr("Web form:") + md->attr( obj, mda_name ) );
	eName->setText( md->attr( obj, mda_name ) );
	eDescription->setText( md->sText( obj, md_description ) );
        eServerModule->setPlainText( md->sText( obj, md_servermodule ) );
        eClientModule->setPlainText( md->sText( obj, md_clientmodule ) );
	eFormSource->setText( md->sText( obj, md_formsource ) );
}

void dEditWebForm::updateMD()
{
	aCfg *md = item->md;
	aCfgItem obj = item->obj;

	al->updateMD( );
	item->setText( 0, eName->text().stripWhiteSpace() );
	md->setAttr( obj, mda_name, eName->text().stripWhiteSpace() );
	md->setSText( obj, md_description, eDescription->text() );
        md->setSText( obj, md_servermodule, eServerModule->toPlainText() );
        md->setSText( obj, md_clientmodule, eClientModule->toPlainText() );
	md->setSText( obj, md_formsource, eFormSource->text() );
}
