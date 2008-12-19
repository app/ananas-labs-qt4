/****************************************************************************
** $Id: mdtree.h,v 1.1 2008/11/05 21:16:27 leader Exp $
**
** Header file of the Metadata Tree of Ananas Designer applications
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

#ifndef MDTREE_H
#define MDTREE_H
#include <q3listview.h>
#include <q3intdict.h>
#include "acfg.h"
#include "formdesigner.h"
#include "atreeitems.h"

extern aFormDesigner *formdesigner;
//#include "cfg.h"
class QWidget;

class aListViewItem : public ananasListViewItem
{
public:
/*	int id;
	aCfgItem obj;
	QWidget *editor;
	aCfg *md;
*/
	aListViewItem(ananasListViewItem *parent, ananasListViewItem *after, aCfg * cfgmd, aCfgItem cfgobj, const QString &name = QString::null );
//	aListViewItem(QListViewItem *parent, aCfg * cfgmd, aCfgItem cfgobj, const QString &name = QString::null );
	aListViewItem(Q3ListView *parent, aCfg * cfgmd, aCfgItem obj, const QString &name = QString::null );
	~aListViewItem();
	QString text( int column ) const;
	void loadTree();
	void setup();
	void update();
	void edit();
	void newObject();
	void newField();
	void newDocument();
	void newCatalogue();
	void newJournal();
	void newIRegister();
	void newARegister();
	void newReport();
	void newForm();
	void newWebForm();
	void newTable();
	void newColumn();
	void saveItem();
	void loadItem();


protected:
//	virtual void okRename( int col );
	virtual int compare( Q3ListViewItem *i, int col, bool accending ) const;
private:
	void loadDocument ();
	void loadJournal ();
	void loadIRegister ();
	void loadARegister ();
	void loadCatalogue ();
	void loadReport ();
	void loadFields (aListViewItem *parent);
	void loadForms (aListViewItem *parent);
	void loadWebForms (aListViewItem *parent);
	void loadColumns ( aListViewItem *parent );
};


class aMetadataTreeView : public ananasTreeView
{
	Q_OBJECT
public:
//	aCfg *md;

	aMetadataTreeView( QWidget *parent, aCfg *cfgmd );

public slots:
	void ContextMenu();
	void itemNew();
	void itemEdit();
	void itemMoveUp();//не работает, проверить поиск элементов  в визуальном дереве
	void itemMoveDown();//не работает, проверить поиск элементов  в визуальном дереве
	void itemDelete();
	void itemRename();
	void itemSave();
	void itemLoad();
private slots:
	void on_collapsed( Q3ListViewItem *item );
private:
	Q3IntDict<QWidget> editors;
};

#endif
