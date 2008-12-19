/****************************************************************************
** $Id: atreeitems.h,v 1.2 2008/12/05 21:11:54 leader Exp $
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

#ifndef ATREEITEMS_H
#define ATREEITEMS_H
#include <Qt3Support>
//#include <qpopupmenu.h>
#include <qlistview.h>
//#include <qintdict.h>
#include "acfg.h"

#define QListView Q3ListView
#define QListViewItem Q3ListViewItem
#define QPopupMenu Q3PopupMenu
#define QHeader Q3Header

class QWidget;

class ananasListViewItem : public Q3ListViewItem
{
public:
	int id;
	aCfgItem obj;
	QWidget *editor;
	aCfg *md;

	ananasListViewItem( Q3ListView *parent, aCfg * cfgmd, aCfgItem cfgobj, const QString &name = QString::null );
	ananasListViewItem( Q3ListView *parent, Q3ListViewItem *after, aCfg * cfgmd, aCfgItem cfgobj, const QString &name = QString::null );
	ananasListViewItem( ananasListViewItem *parent, ananasListViewItem *after, aCfg * cfgmd,
						aCfgItem cfgobj, const QString &name = QString::null );

	void clearTree();
//	void moveUp ();
//	void moveDown ();

	ananasListViewItem *previousSibling();
	ananasListViewItem *nextSibling();
//	ananasListViewItem *getLastChild( QListViewItem * parent );
	ananasListViewItem *getLastChild();
protected:
//	virtual void okRename( int col );
};

class ananasTreeView
: public Q3ListView
{
    Q_OBJECT
public:
    aCfg *md;

	ananasTreeView( QWidget *parent, aCfg *cfgmd );

	void ContextMenuAdd(  QPopupMenu * m );
//	void deleteItem();
//	void moveUpItem ();
//	void moveDownItem ();
//	void renameItem();
};


#endif
