#include "deditjournal.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

#include <qstatusbar.h>
#include "acfg.h"

/*
 *  Constructs a dEditJournal as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
dEditJournal::dEditJournal(QWidget* parent, const char* name, Qt::WindowFlags fl)
    : Q3MainWindow(parent, name, fl)
{
    setupUi(this);

    (void)statusBar();
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
dEditJournal::~dEditJournal()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void dEditJournal::languageChange()
{
    retranslateUi(this);
}

void dEditJournal::setData( aListViewItem *o )
{
    item = o;
    md = o->md;
    obj = o->obj;
    aCfgItem cobj, doc, docs;
    int i, j, n;
    aAliasEditor *a = new aAliasEditor( md, obj, tAliases );
    al = a;
    al->setData();
    aRoleEditor *r = new aRoleEditor( md, obj, tRoles, md_journal );
    re = r;
    re->setData();
    setCaption( tr("Journal:") + md->attr( obj, mda_name ) );
    eName->setText( md->attr( obj, mda_name ) );
    eDescription->setText( md->sText( obj, md_description ) );

    i = md->attr ( obj, mda_type ).toInt();
    cbType-> setCurrentItem(i);

    journalDocs = new ananasTreeView( tabWidget18->page(2),  md  );
    journalDocs->setSorting( -1 );
    // TODO Fix me!!!
    //--layout28->addWidget( journalDocs, 0, 0 );
    n = md->count( obj, md_used_doc );
    for ( j = 0; j < n; j++ )
    {
	cobj = md->find( obj, md_used_doc, j );
	if ( cobj.isNull() )
	    continue;
//		did = md->text(cobj).toLong();
	doc = md->find(md->text(cobj).toLong());
	if ( doc.isNull() )
	    md->remove(cobj);
	else
	    new ananasListViewItem( journalDocs, journalDocs->lastItem(), md, doc );
    }

    allDocs = new ananasTreeView( tabWidget18->page(2),  md  );
    allDocs->setSorting( -1 );
    // TODO Fix me!!!
    //--layout29->add( allDocs );
    docs = md->find(md->find(mdc_metadata),md_documents,0);
    n = md->count( docs, md_document );
    for ( j = 0; j < n; j++ )
    {
	cobj = md->find( docs, md_document, j );
	if ( cobj.isNull() )
	    return;
	//		did = md->id(cobj);
	doc = md->find(md->id(cobj));
	new ananasListViewItem( allDocs, allDocs->lastItem(), md, doc );
    }
    docPage = tabWidget18->page(2);
    if ( cbType->currentItem() != 1 )
	typeChange();
}


void dEditJournal::init()
{
    statusBar()->hide();
}


void dEditJournal::updateMD()
{
    aCfgItem doc, docs;
    int i;

    al->updateMD();
    re->updateMD();
    item->setText( 0, eName->text().stripWhiteSpace() );
    md->setAttr( obj, mda_name, eName->text().stripWhiteSpace() );
    md->setSText( obj, md_description, eDescription->text() );
    md->setAttr( obj, mda_type, cbType->currentItem() );
    docs = md->find(obj,md_columns,0);
    do
    {
	doc = md->findChild( docs, md_used_doc, 0 );
	if ( !doc.isNull() )
	    md->remove( doc );
    } while ( !doc.isNull() );
    if ( cbType->currentItem() != 1 )
	return;
    ananasListViewItem *aitem = (ananasListViewItem *)journalDocs->firstChild();
    for ( i = 0; i < journalDocs->childCount(); i++ )
    {
	doc = md->insert( docs, md_used_doc, QString::null, -1 );
	md->setText( doc, QString( "%1" ).arg( aitem->id ) );
	aitem = (ananasListViewItem *)aitem->nextSibling();
    }

}

void dEditJournal::destroy()
{
    updateMD();
    ( (MainForm*)this->topLevelWidget() )->wl->remove( this );
    ( (MainForm*)this->topLevelWidget() )->removeTab(name());
}




void dEditJournal::typeChange()
{
    if ( cbType->currentItem() != 1 )
	tabWidget18->removePage(tabWidget18->page(2));
    else
	tabWidget18->insertTab(docPage,QObject::tr("Documents"));
}


void dEditJournal::addDoc()
{
    ananasListViewItem *cur = (ananasListViewItem *)allDocs->selectedItem();
    if (!cur)
	return;
    if ( md->objClass( cur->obj ) != md_document )
	return;
    new ananasListViewItem(journalDocs, journalDocs->lastItem(), md, cur->obj );
}


void dEditJournal::removeDoc()
{
    journalDocs->removeItem( journalDocs->selectedItem() );
}


void dEditJournal::moveUp()
{
    ananasListViewItem *aitem, *after;

    aitem = (ananasListViewItem *) journalDocs->selectedItem();
    if ( aitem )
    {
	after = (ananasListViewItem *)aitem->itemAbove();
	if ( after )
	    after->moveItem( aitem );
    }
}


void dEditJournal::moveDown()
{
    ananasListViewItem *aitem, *after;
    aitem = (ananasListViewItem *)journalDocs->selectedItem();
    if ( aitem )
    {
	after = (ananasListViewItem *)aitem->itemBelow();
	if ( after )
	    aitem->moveItem( after );
    }
}
