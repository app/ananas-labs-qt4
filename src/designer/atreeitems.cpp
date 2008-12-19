/****************************************************************************
** $Id: atreeitems.cpp,v 1.2 2008/11/08 20:16:35 leader Exp $
**
** Header file of the Ananas visual tree object
** of Ananas Designer applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
**
** This file is part of the Designer application  of the Ananas
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

#include <q3listview.h>
#include <q3header.h>
#include <q3popupmenu.h>
#include <qlabel.h>
#include <qcursor.h>

#include "atreeitems.h"
#include "alog.h"

ananasListViewItem::ananasListViewItem( Q3ListView *parent, aCfg * cfgmd, aCfgItem cfgobj,
										 const QString &name )
: Q3ListViewItem( parent )
{
	obj = cfgobj;
	md = cfgmd;
	if ( name.isNull() ) setText( 0, md->attr( obj, mda_name ) );
	else setText( 0, name );
	id = md->id(obj);
}

ananasListViewItem::ananasListViewItem( ananasListViewItem *parent, ananasListViewItem *after,
										 aCfg * cfgmd, aCfgItem cfgobj, const QString &name )
: Q3ListViewItem( parent, after )
{
	obj = cfgobj;
	md = cfgmd;
	if ( name.isNull() ) setText( 0, md->attr( obj, mda_name ) );
	else setText( 0, name );
	id = md->id(obj);
}

ananasListViewItem::ananasListViewItem( Q3ListView *parent, Q3ListViewItem *after, aCfg * cfgmd, aCfgItem cfgobj, const QString &name )
: Q3ListViewItem( parent, after )
{
	obj = cfgobj;
	md = cfgmd;
	if ( name.isNull() ) setText( 0, md->attr( obj, mda_name ) );
	else setText( 0, name );
	id = md->id(obj);
}

void
ananasListViewItem::clearTree()
{
	Q3ListViewItem	*item, *nextitem;

	// clear tree
	item = firstChild();
	while( item )
	{
		nextitem = item->nextSibling();
		delete item;
		item = nextitem;
	}
}

/*
void
ananasListViewItem::moveItem ( QListViewItem * after )
{
CHECK_POINT
	aCfgItem temp = obj, temp2 = ((ananasListViewItem*)after)->obj;
	QListViewItem::moveItem( after );
	obj = temp;
	((ananasListViewItem*)after)->obj = temp2;
}
*/
void
ananasListViewItem::moveUp ()
{
	if(!previousSibling()) return; // not previous item! - no changes
	aCfgItem item = previousSibling()->obj;

	if ( obj.isNull() )
	{
		aLog::print(aLog::Error, QObject::tr(" Ananas List View Item %1 is null").arg(md->attr(obj,mda_name)));
		return;
	}
	if( md->swap( obj, item ) )
	{
		previousSibling()->moveItem( this );
		aLog::print(aLog::Debug, QObject::tr("Ananas List View Item swaping"));
	}
	else
	{
		aLog::print(aLog::Error, QObject::tr("Ananas List View Item swaping"));
	}
}

void
ananasListViewItem::moveDown ()
{
	if(!nextSibling()) return; // not next item! - no changes
	aCfgItem item = nextSibling()->obj;
	if ( item.isNull() )
	{
		aLog::print(aLog::Error, QObject::tr(" Ananas List View Item %1 is null").arg(md->attr(obj,mda_name)));
		return;
	}
	if( md->swap( obj, item ) )
	{
		moveItem( nextSibling() );
		aLog::print(aLog::Debug, QObject::tr("Ananas List View Item swaping"));
	}
	else
	{
		aLog::print(aLog::Error, QObject::tr("Ananas List View Item swaping"));
	}
}

ananasListViewItem *
ananasListViewItem::previousSibling() // becose QListViewItem not have function previousSibling();
{
	Q3ListViewItem *parent, *item;
	parent = this->parent();
	item = parent->firstChild();
	while ( item )
		if ( (ananasListViewItem *)item->nextSibling() == this )
			return (ananasListViewItem *) item;
		else item = item->nextSibling();
	return 0;
}

ananasListViewItem*
ananasListViewItem::nextSibling()
{
	return (ananasListViewItem *)Q3ListViewItem::nextSibling();
}


void
ananasListViewItem::okRename( int col )
{
	Q3ListViewItem::okRename( col );
	if ( id && !obj.isNull() && col == 0 ) {
		setText( 0, text( 0 ).stripWhiteSpace() );
		md->setAttr( obj, mda_name, text( 0 ) );
	}
}

/*
ananasListViewItem*
ananasListViewItem::getLastChild( QListViewItem * parent )
{
	QListViewItem *item, *nextitem;
	item = parent->firstChild();
	while( item )
	{
		nextitem = item->nextSibling();
		if ( nextitem )
			item = nextitem;
		else
			return (ananasListViewItem*) item;
	}
	return 0;
};
*/

ananasListViewItem*
ananasListViewItem::getLastChild()
{
	Q3ListViewItem *item, *nextitem;
	item = firstChild();
	while( item )
	{
		nextitem = item->nextSibling();
		if ( nextitem )
			item = nextitem;
		else
			return (ananasListViewItem*) item;
	}
	return 0;
};


ananasTreeView::ananasTreeView ( QWidget *parent, aCfg *cfgmd )
:Q3ListView ( parent )
{
	md = cfgmd;
	addColumn( "" );
	header()->hide();
	setSorting ( -1 );
	setSelectionMode( Single );
};


void
ananasTreeView::ContextMenuAdd( Q3PopupMenu * m )
{
	//--QLabel *caption = new QLabel( tr("<font color=darkblue><u><b>" "Context Menu</b></u></font>"), this );
	//--caption->setAlignment( Qt::AlignCenter );
	//--m->insertItem( caption );
	m->insertItem( tr("&Rename"), this, SLOT( itemRename() ), Qt::CTRL+Qt::Key_R);
	m->insertItem( tr("&Edit"),  this, SLOT( itemEdit() ), Qt::CTRL+Qt::Key_E );
	m->insertItem( tr("&Delete"), this, SLOT( itemDelete() ), Qt::CTRL+Qt::Key_D );
	m->insertItem( tr("&MoveUp"), this, SLOT( itemMoveUp() ), Qt::CTRL+Qt::Key_U );
	m->insertItem( tr("&MoveDown"), this, SLOT( itemMoveDown() ), Qt::CTRL+Qt::Key_M );
	m->insertItem( tr("&SaveItem"), this, SLOT( itemSave() ) );
	m->insertItem( tr("&LoadItem"), this, SLOT( itemLoad() ) );
	m->insertSeparator();
};

void
ananasTreeView::deleteItem()
{
	ananasListViewItem *i = (ananasListViewItem *) selectedItem();
	if ( i )
	{
		if ( i->id ) {
			md->remove( i->obj );
			delete i;
		}
	}
};

void
ananasTreeView::moveUpItem()
{
	ananasListViewItem *i = (ananasListViewItem *) selectedItem();
	if ( i )
	{
		if ( i->id ) {
			i->moveUp();
		}
	}
}

void
ananasTreeView::moveDownItem()
{
	ananasListViewItem *i = (ananasListViewItem *) selectedItem();
	if ( i )
	{
		if ( i->id ) {
			i->moveDown();
		}
	}
}

void
ananasTreeView::renameItem()
{
	if ( selectedItem() ) selectedItem()->startRename( 0 );
}

