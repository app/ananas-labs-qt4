/****************************************************************************
** $Id: rclistviewitem.cpp,v 1.1 2008/11/05 21:16:28 leader Exp $
**
** Code file of the Ananas select database window
** of Ananas Designer and Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
**
** This file is part of the Library of the Ananas
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
#include "rclistviewitem.h"
#include "qpixmap.h"

rcListViewItem::rcListViewItem( Q3ListView *parent,
			const QString &name,
			const QString &rc,
			bool fgroup): Q3ListViewItem( parent, name )
	{
		init( rc, fgroup );
	};

rcListViewItem::rcListViewItem( Q3ListView *parent,
			rcListViewItem* after,
			const QString &name,
			const QString &rc,
			bool fgroup ): Q3ListViewItem( parent, after, name )
	{
		init( rc, fgroup );
	};

rcListViewItem::rcListViewItem( rcListViewItem *parent,
			const QString &name,
			const QString &rc,
			bool fgroup): Q3ListViewItem( parent, name )
	{
		init( rc, fgroup );
	};

rcListViewItem::~rcListViewItem()
	{
//		rcfile = "";
	};

void
rcListViewItem::init( const QString &rc, bool fgroup)
	{
		group = fgroup;
		rcfile = rc;
		if ( group ) setPixmap(0, QPixmap(":/images/lib_dbgroup.png") );
		else setPixmap(0, QPixmap(":/images/lib_database.png") );
	};
