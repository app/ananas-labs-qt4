/****************************************************************************
** $Id: wgrouptree.h,v 1.1 2008/11/05 21:16:30 leader Exp $
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

#ifndef WGROUPTREE_H
#define WGROUPTREE_H

#include <q3listview.h>
#include <qwidgetplugin.h>
#include <qwidget.h>
//Added by qt3to4:
#include <QKeyEvent>
#include "awidget.h"

class wGroupTreeItem;

class QT_WIDGET_PLUGIN_EXPORT wGroupTree : public aWidget
{
    Q_OBJECT
public:
	Q3ListView *tree;
	wGroupTreeItem *root;

	wGroupTree( QWidget *parent = 0, Qt::WFlags fl = 0 );
	virtual ~wGroupTree();

	virtual void initObject( aDatabase *adb );
	virtual QDialog* createEditor( QWidget *parent );
	virtual Q3ToolBar*	createToolBar( Q3MainWindow *parent );
	virtual aObject *createDBObject(  aCfgItem obj, aDatabase *adb );
	void buildGroupTree( aCfgItem obj, aCatGroup * cg1, wGroupTreeItem * wG );
	void findGroupTree();
	wGroupTreeItem *findItem( ANANAS_UID id );
public slots:
	void NewGroup();
	void DeleteGroup();
	void EditGroup();
//	void UpdateItem( aCatGroup *g );
private slots:
	void on_selected( ANANAS_UID element );
//	void updateItem( ANANAS_UID element );
	void updateItem( ANANAS_UID element );
	void on_selectionChanged( Q3ListViewItem *);
signals:
    virtual void selected( ANANAS_UID group );
    virtual void selectionChanged( const qulonglong );
protected:
	virtual void keyPressEvent ( QKeyEvent *e );
private:
};



class wGroupTreeItem : public Q3ListViewItem
{
public:
	ANANAS_UID id;
	int level;
    wGroupTreeItem( Q3ListView *parent, const QString &name = QString::null );
    wGroupTreeItem( wGroupTreeItem *parent, wGroupTreeItem *after, const QString &name = QString::null, int newlevel = 0, ANANAS_UID newid = 0 );
    wGroupTreeItem( wGroupTreeItem *parent, wGroupTreeItem *after, aCatGroup *g = 0 );
    virtual ~wGroupTreeItem();
    wGroupTreeItem *parentItem();
private:
};



#endif // WGROUPTREE_H

