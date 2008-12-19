#include "deditdoc.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

#include <qstatusbar.h>
#include "acfg.h"
#include <qsinterpreter.h>
#include <qseditor.h>

/*
 *  Constructs a dEditDoc as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
dEditDoc::dEditDoc(QWidget* parent, const char* name, Qt::WindowFlags fl)
    : Q3MainWindow(parent, name, fl)
{
    setupUi(this);

    (void)statusBar();
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
dEditDoc::~dEditDoc()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void dEditDoc::languageChange()
{
    retranslateUi(this);
}

void dEditDoc::init()
{
	delete statusBar();
	eModule->setInterpreter( new QSInterpreter() );
	eStrViewF->setInterpreter( new QSInterpreter() );
	eStrViewF->setEnabled( FALSE );
}


void dEditDoc::destroy()
{
    updateMD();
    item->editor = 0;
    ( (MainForm*)this->topLevelWidget() )->wl->remove( this );
    ( (MainForm*)this->topLevelWidget() )->removeTab(name());
}


void dEditDoc::setData( aListViewItem *o )
{
    long int i;
    int fid, fcount, fieldid, stdf;
    item = o;
    aCfg *md = o->md;
    aCfgItem obj = o->obj, sv, field;

    aAliasEditor *a = new aAliasEditor( md, obj, tAliases );
    al = a;
    al->setData();
    aRoleEditor *r = new aRoleEditor( md, obj, tRoles, md_document );
    re = r;
    re->setData();
    setCaption( tr("Document:") + md->attr( obj, mda_name ) );
    eName->setText( md->attr( obj, mda_name ) );
    eDescription->setText( md->sText( obj, md_description ) );
    eModule->setText( md->sText( obj, md_sourcecode ) );
    sv = md->find( obj, md_string_view );
    eStrViewF->setText( md->sText( sv, md_svfunction ) );
    eSv->insertItem( "[ standart function ]" );
    eSv->insertItem( "[ user function ]" );
    fid = md->sText( sv, md_fieldid ).toInt();
    stdf = md->attr( sv, mda_stdf ).toInt();
    eSv->setCurrentItem( 0 );
    for ( i = 0; i < md->count( obj, md_field ); i++ ) {
	field = md->find( obj, md_field, i );
	eSv->insertItem( QString("%1").arg( md->attr( field, mda_name ) ) );
	fieldid = md->id( field );
	fields.insert( i, new int( fieldid ) );
	if ( fid == fieldid && !stdf ) {
	    eSv->setCurrentItem( i + 2 );
	    eStrViewF->setEnabled( FALSE );
	}
    }
    if ( !fid & !stdf ) {
	eStrViewF->setEnabled( TRUE );
	eSv->setCurrentItem( 1 );
    }
}

void dEditDoc::updateMD()
{
	aCfg *md = item->md;
	aCfgItem obj = item->obj, sv;

	al->updateMD();
	re->updateMD();
	item->setText( 0, eName->text().stripWhiteSpace() );
	md->setAttr( obj, mda_name, eName->text().stripWhiteSpace() );
	md->setSText( obj, md_description, eDescription->text() );
	md->setSText( obj, md_sourcecode, eModule->text() );
	sv = md->find( obj, md_string_view );
	if ( sv.isNull() ) sv = md->insert( obj, md_string_view );
	md->setSText( sv, md_svfunction, eStrViewF->text() );
	if ( eSv->currentItem() == 0 ) md->setAttr( sv, mda_stdf, "1" );
	else {
	    md->setAttr( sv, mda_stdf, "0" );
	    if ( eSv->currentItem() == 1 ) md->setSText( sv, md_fieldid, "0" );
	    else md->setSText( sv, md_fieldid, QString("%1").arg( *(fields.find( eSv->currentItem() - 2 ) ) ) );
	}
}



void dEditDoc::eSv_activated( int index )
{
    if ( index == 1 ) eStrViewF->setEnabled( TRUE );
    else eStrViewF->setEnabled( FALSE );
}
