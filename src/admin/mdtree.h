/****************************************************************************
** $Id: mdtree.h,v 1.3 2008/12/05 21:11:54 leader Exp $
**
** Header file of the Metadata Tree of Ananas Administrator applications
**
** Created : 20050609
**
** Copyright (C) 2003-2005 Leader InfoTech.  All rights reserved.
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

#ifndef MDTREE_H
#define MDTREE_H
#include <Qt3Support>
//#include <qlistview.h>
//#include <qintdict.h>
#include "acfg.h"
#include "atreeitems.h"
#include "auser.h"
#include "arole.h"

#define QIntDict Q3IntDict
#define QValueList Q3ValueList

//#include "usersform.h"
//#include "rolesform.h"

//extern aFormDesigner *formdesigner;
//#include "cfg.h"
class QWidget;
class UsersForm;
class RolesForm;

class  aListViewItem : public ananasListViewItem
{

public:
/*	int id;
	aCfgItem obj;
	QWidget *editor;
	aCfg *md;
*/
	aDatabase *adb;
	
	aListViewItem(	ananasListViewItem *parent,
			ananasListViewItem *after,
			aCfg * cfgmd, 
			aCfgItem cfgobj, 
			const QString &name = QString::null,
			aUser *usr = NULL,
			aRole *rl = NULL);
//	aListViewItem(QListViewItem *parent, aCfg * cfgmd, aCfgItem cfgobj, const QString &name = QString::null );
	aListViewItem(	QListView *parent, 
			aCfg * cfgmd,
			aCfgItem obj,
			const QString &name = QString::null);
	
	~aListViewItem();
	QString text( int column ) const;
	
	aUser *user;
	aRole *role;
	
	void loadTree();
	void setup();
	void update();
	void deleteUser();
	void deleteRole();
	void editUser(bool isNew);
	void editRole(bool isNew);
	void addUser();
	void addRole();
	void remove();

	void editPermission();
//	void newDocument();
//	void newCatalogue();
//	void newJournal();
//	void newIRegister();
//	void newARegister();
//	void newReport();
//	void newForm();
//	void newWebForm();
//	void newTable();
	void newColumn();

protected:
//	virtual void okRename( int col );
	virtual int compare( Q3ListViewItem *i, int col, bool accending ) const;
private:
//	void loadDocument ();
//	void loadJournal ();
//	void loadIRegister ();
//	void loadARegister ();
//	void loadCatalogue ();
//	void loadReport ();
	void loadRoles (aListViewItem *a);
	void loadUsers ( aListViewItem *a );
	void loadRlUsers ( aListViewItem *a );
	void loadUsrRoles (aListViewItem *a);
//	void loadForms (aListViewItem *parent);
//	void loadWebForms (aListViewItem *parent);
//	void loadColumns ( aListViewItem *parent );
};


class aMetadataTreeView : public ananasTreeView
{
	Q_OBJECT
public:
//	aCfg *md;

	aMetadataTreeView( QWidget *parent, aCfg *cfgmd );
	aMetadataTreeView( UsersForm *parent, aCfg *cfgmd );
	aMetadataTreeView( RolesForm *parent, aCfg *cfgmd );

signals: 
	void updateUsers();
	void updateRoles();
	
public slots:
	void ContextMenu();
	void newUser();
	void editUser();
	void delUser();
	void addUser();
	void newRole();
	void editRole();
	void delRole();
	void addRole();
	
	void itemDelete();
	void itemSetPermission();
private slots:
	void on_collapsed( Q3ListViewItem *item );
private:
	QIntDict<QWidget> editors;
};

#endif
