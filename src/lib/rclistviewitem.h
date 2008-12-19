/****************************************************************************
** $Id: rclistviewitem.h,v 1.1 2008/11/05 21:16:28 leader Exp $
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
#ifndef RCLISTVIEWITEM_H
#define RCLISTVIEWITEM_H

//#include <qdir.h>
//#include <qstring.h>
#include <q3listview.h>
//#include <qlineedit.h>
//#include <qstring.h>

//#include "deditrc.h"
//#include "acfgrc.h"
//#include "atests.h"

class Q3ListView;
class Q3ListViewItem;

class rcListViewItem : public Q3ListViewItem
{
public:
	QString rcfile;
	bool group;

	rcListViewItem( Q3ListView *parent,
			const QString &name = QString::null,
			const QString &rc = QString::null,
			bool fgroup = false );

	rcListViewItem( Q3ListView *parent,
			rcListViewItem* after,
			const QString &name = QString::null,
			const QString &rc = QString::null,
			bool fgroup = false );

	rcListViewItem( rcListViewItem *parent,
			const QString &name = QString::null,
			const QString &rc = QString::null,
			bool fgroup = false );

	virtual ~rcListViewItem();
	void init( const QString &rc = QString::null, bool fgroup = false );
};
#endif

