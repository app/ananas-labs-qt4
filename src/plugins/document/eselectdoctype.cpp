#include "eselectdoctype.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

/*
 *  Constructs a eSelectDocType as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
eSelectDocType::eSelectDocType(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
eSelectDocType::~eSelectDocType()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void eSelectDocType::languageChange()
{
    retranslateUi(this);
}

void eSelectDocType::init()
{
    listDocs->header()->hide();
    listDocs->setColumnWidth( 0, 1000 );
    listDocs->setFocus();
}


void eSelectDocType::setJournal( aCfg *md, int id )
{
    aCfgItem obj, dobj;
    int i, cnt, t, did;
    QString name;

    listDocs->clear();
    dlist.clear();
    obj = md->find( id );
    if ( !obj.isNull() ) {
	dlist = md->getJournalDocuments( obj );
//	printf("docs=%s\n", ( const char *) dlist.join("\n") );
	for (i=0;i< dlist.count();i++) {
	    did = dlist[i].toInt();
	    dobj = md->find( did );
	    if ( !dobj.isNull() ) {
		name = md->attr( dobj, mda_name );
		new Q3ListViewItem( listDocs, name, dlist[i] );
	    }
	}
	listDocs->setSelected( listDocs->firstChild(), true );
    }
}


void eSelectDocType::accepted()
{
    docId = listDocs->selectedItem()->text(1).toInt();
  //  printf("docId=%i\n", docId );
    accept();
}
