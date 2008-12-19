/****************************************************************************
** $Id: atreeitems.cpp,v 1.2 2008/12/05 21:11:54 leader Exp $
**
** Header file of the Ananas visual tree object
** of Ananas Designer applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) 2003-2005 Grigory Panov <gr1313 at mail dot ru>, Yoshkar-Ola.
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

//#include <qlistview.h>
//#include <qheader.h>
//#include <qpopupmenu.h>
#include <qlabel.h>
#include <qcursor.h>

#include "atreeitems.h"

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
	QListViewItem	*item, *nextitem;

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

ananasListViewItem *
ananasListViewItem::previousSibling()
{
	if ( !this ) return 0;
	QListViewItem *parent, *item;
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
	return (ananasListViewItem *)QListViewItem::nextSibling();
}




ananasListViewItem*
ananasListViewItem::getLastChild()
{
	QListViewItem *item, *nextitem;
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
ananasTreeView::ContextMenuAdd( QPopupMenu * m )
{
	//m->insertSeparator();
};




