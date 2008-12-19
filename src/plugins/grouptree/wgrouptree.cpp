/****************************************************************************
** $Id: wgrouptree.cpp,v 1.1 2008/11/05 21:16:30 leader Exp $
**
** Header file of the document plugin of Ananas
** Designer and Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
**
** This file is part of the Ananas Plugins of the Ananas
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

#include <q3header.h>
#include <qlayout.h>
#include <qaction.h>
//Added by qt3to4:
#include <q3mimefactory.h>
#include <Q3GridLayout>
#include <QKeyEvent>
#include "wgrouptree.h"
#include "awidget.h"

wGroupTreeItem::wGroupTreeItem( Q3ListView *parent, const QString &name )
    : Q3ListViewItem( parent, name )
{
	level = -1;
	parent = 0;
	id = 0;
}


wGroupTreeItem::wGroupTreeItem( wGroupTreeItem *parent, wGroupTreeItem *after, const QString &name, int newlevel, ANANAS_UID newid )
    : Q3ListViewItem( parent, after, name )
{
	level = newlevel;
	id = newid;
}


wGroupTreeItem::wGroupTreeItem( wGroupTreeItem *parent, wGroupTreeItem *after, aCatGroup *g )
    : Q3ListViewItem( parent, after )
{
	level = g->Value( "Level" ).toInt();
	id = g->getUid();
//	setText( 0, g->Value("Name").toString()+" "+QString::number( level )+" ("+QString::number( id )+")" );
	setText( 0, g->displayString() );
}


wGroupTreeItem::~wGroupTreeItem()
{
}



wGroupTreeItem *
wGroupTreeItem::parentItem()
{
	return ( wGroupTreeItem *) parent();
}


wGroupTree::wGroupTree( QWidget *parent, Qt::WFlags fl )
    : aWidget( parent, "wGroupTree", fl )
{
	tree = new Q3ListView( this );
	tree->addColumn( "" );
	tree->header()->hide();
	tree->setSorting( 0 );
	tree->setSelectionMode( Q3ListView::Single );
	root = new wGroupTreeItem( tree, "ROOT" );
	root->setOpen( true );
	root->setPixmap(0, rcIcon("wcatalogue.png"));

	Q3GridLayout *l = new Q3GridLayout( this );
	l->addWidget( tree, 0, 0 );
	connect(tree, SIGNAL(selectionChanged(Q3ListViewItem*)),
		this, SLOT( on_selectionChanged(Q3ListViewItem*)));
}


wGroupTree::~wGroupTree()
{
}


void
wGroupTree::initObject( aDatabase *adb )
{
	aWidget::initObject( adb );
	findGroupTree();
	//connectSlots();
}


QDialog*
wGroupTree::createEditor( QWidget * )//parent )
{
    return 0; //new eCatalogue( parent );
}



/*!
 * Create aDocument database object.
 */
aObject *
wGroupTree::createDBObject(  aCfgItem obj, aDatabase *adb )
{
	return new aCatGroup( obj, adb );
}


void
wGroupTree::buildGroupTree( aCfgItem obj, aCatGroup * cg1, wGroupTreeItem * wG )
{
	aCatGroup cg2 (obj, db);
	QString t;
	int err = 0;
	wGroupTreeItem *item;
	CHECK_POINT

	err = cg2.SelectChild( cg1 );
	if ( !err )
	{
//		CHECK_POINT
//		QListViewItem * parent1 = new QListViewItem ( wG );
//		wG = new wGroupTreeItem ( tree, parent1 );
    	do
    	{
//			CHECK_POINT
			t = cg2.Value("Name").toString();
//			CHECK_POINT
			item = new wGroupTreeItem ( wG, 0, &cg2 ); //cg2.Value("Level").toInt(), cg2.getUid() );
			item->setPixmap( 0, rcIcon( "t_cat_g.png" ));
//			buildGroupTree( obj, &cg2, wG );
//			CHECK_POINT
			buildGroupTree( obj, &cg2, item );
//			CHECK_POINT
    	}
    	while ( cg2.Next() );
	} else {
//		CHECK_POINT
		printf("table %s err = %d\n", ( const char *)cg2.table()->name(), err);
	}
}



void
wGroupTree::findGroupTree()
{
	int oid = 0;
	aCfgItem tab, cat;
	if ( !db || !md ) return;
	CHECK_POINT
	aWidget *pc = parentContainer( this );
	if ( !pc ) return;
	CHECK_POINT
	printf("inserted in %s\n", pc->className());
	if ( pc->className() == QString("wCatalogue") ) {
		cat = md->find( pc->getId() ); // md->find(mdc_metadata), md_catalogues, 0 ), md_catalogue, 0 );
		root->setText( 0, md->attr( cat, mda_name ) );
//		CHECK_POINT
//		wGroupTreeItem *wG = new wGroupTreeItem ( tree, root, "root" );
//		aCatGroup cg ( cat, db );

//		CHECK_POINT
		buildGroupTree( cat, 0, root );
		CHECK_POINT
	}
}


/*!
 *\~english
 *
 *\~russian
 * ������ ������������� �����.
 *\~
 */
void
wGroupTree::keyPressEvent ( QKeyEvent *e )
{
	ANANAS_UID id = 0;
	wGroupTreeItem * item = ( wGroupTreeItem * ) tree->currentItem();
	printf("key=%04x\n",e->key());
	switch ( e->key() ){
	case Qt::Key_Return:
		id = item->id;
		if ( e->state() == Qt::ShiftModifier ) {
			printf("Shift+Return pressed %Li\n", id);
			if ( id ) EditGroup();
		} else {
			printf("Return pressed %Li\n", id );
			emit( selected( id ) );
		}
		e->accept();
		break;
	case Qt::Key_Escape:
		break;
	case Qt::Key_Delete:
		id = item->id;
		if ( id ) DeleteGroup();
		break;
	case Qt::Key_Insert:
		NewGroup();
		item = ( wGroupTreeItem * ) tree->currentItem();
		id = item->id;
		if ( id ) EditGroup();
		break;

	default:
		e->ignore();
		break;
	}
	aWidget::keyPressEvent( e );
}



void
wGroupTree::NewGroup()
{
	wGroupTreeItem *item, *cur;
	QString t = "Element";
	int level;

	cur = ( wGroupTreeItem *) tree->currentItem();
	if ( !cur ) cur = root;
	level = cur->level + 1;
	( ( aCatGroup *) dbobj )->New();
	dbobj->SetValue("Name", t);
	dbobj->setSysValue("level", level );
	dbobj->setSysValue("idp", cur->id );
	dbobj->Update();
	item = new wGroupTreeItem ( cur, 0, ( aCatGroup *) dbobj ); //level, dbobj->getUid() );
	item->id = dbobj->getUid();
	cur->setOpen( true );
	tree->setCurrentItem( item );

}


void
wGroupTree::DeleteGroup()
{
CHECK_POINT
	aCatGroup g( *getMDObject(), db);
	wGroupTreeItem * item = ( wGroupTreeItem * ) tree->currentItem();
	if(item!=root)
	{
		g.select( item->id );
		g.Delete();
		item->id=0;
		item->setText(0,"X");
	}
}


void
wGroupTree::EditGroup()
{
	ANANAS_UID id = 0;
	wGroupTreeItem * item = ( wGroupTreeItem * ) tree->currentItem();
	if(item!=root)
	{
		aForm * f = 0;
		if ( item ) id = item->id;
CHECK_POINT
		if ( id ) {
			if ( engine ) {
				f = engine->openForm( parentContainer( this )->getId(), 0, md_action_edit, md_form_group, id, this );
				if ( f ) {
					f->SelectGroup(id);
					connect(f, SIGNAL( update( ANANAS_UID )), this, SLOT(updateItem( ANANAS_UID )));
//				connect(f, SIGNAL(selected( Q_ULLONG )), this, SLOT(on_selected( Q_ULLONG )));
//				f->closeAfterSelect = true;
				}
			} else printf("No Engine\n");

		}
	}
}


//void
//wGroupTree::UpdateItem( aCatGroup *g )
//{
//}


/*!
 * Show appropriated group after catalogue item selected.
 */
void
wGroupTree::on_selected( ANANAS_UID element )
{

}


/*!
 * Update item after edit.
 */
void
wGroupTree::updateItem( ANANAS_UID element )
{
	wGroupTreeItem *i = 0;
	aCatGroup g( *getMDObject(), db);

	printf("Need update text %Li\n", element);
	i = findItem( element );
	if ( i ) {
		g.select( element );
		printf("founded text %Li %s\n", element, (const char *) i->text(0));
		i->setText( 0, g.displayString() );

	}
}


wGroupTreeItem *
wGroupTree::findItem( ANANAS_UID id )
{
	wGroupTreeItem *i = 0;

	Q3ListViewItemIterator it( tree );
	while ( i = ( wGroupTreeItem *) it.current() ) {
		if ( i->id == id ) break;
		++it;
	}
	return i;
}


/*!
 * Create toolbar for Journal.
 */
Q3ToolBar*
wGroupTree::createToolBar( Q3MainWindow * owner )
{
	QAction *a;
	Q3ToolBar *t = new Q3ToolBar( owner, "GroupTreeTools" );

	a = new QAction(
	rcIcon("doc_new.png"),
	tr("New"),
	QKeySequence(""),//Insert"),
	t,
	tr("New group")
	);
	a->setToolTip(tr("New group (Ins)"));
	a->addTo( t );
	connect( a, SIGNAL( activated() ), this, SLOT( NewGroup() ) );

	a = new QAction(
	rcIcon("doc_edit.png"),
	tr("Edit"),
	QKeySequence(""),//Return"),
	t,
	tr("Edit group")
	);
	a->setToolTip(tr("Edit group (Enter)"));
	a->addTo( t );
	connect( a, SIGNAL( activated() ), this, SLOT( EditGroup() ) );

/*	a = new QAction(
	QPixmap::fromMimeSource("doc_view.png"),
	tr("View"),
	QKeySequence("Shifh+Return"),
	t,
	tr("View element")
	);
	a->setToolTip(tr("View element (Shift+Enter)"));
	a->addTo( t );
	connect( a, SIGNAL( activated() ), this, SLOT( view() ) );
*/
	a = new QAction(
	rcIcon("doc_delete.png"),
	tr("Delete group"),
	QKeySequence(""),//Delete"),
	t,
	tr("Delete group")
	);
	a->setToolTip(tr("Delete group (Delete)"));
	a->addTo( t );
	connect( a, SIGNAL( activated() ), this, SLOT( DeleteGroup() ) );


	return 0;
}

void
wGroupTree::on_selectionChanged( Q3ListViewItem * item)
{
	printf("wGroupTree id =%llu\n",((wGroupTreeItem*) item)->id);
	emit(selectionChanged( ((wGroupTreeItem*) item)->id) );
}
