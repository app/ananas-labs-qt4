/****************************************************************************
** $Id: interfacetree.cpp,v 1.1 2008/11/05 21:16:27 leader Exp $
**
** Code file of the Interface Tree of Ananas Designer applications
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
//Added by qt3to4:
#include <QPixmap>

#include "acfg.h"
#include "interfacetree.h"
#include "mainform.h"
#include "deditcommand.h"
#include "dedittoolbar.h"
#include "awindowslist.h"

extern MainForm *mainform;
extern QPixmap rcIcon(const char *name);
extern void set_Icon(Q3ListViewItem *item, const char *name);

InterfaceListViewItem::InterfaceListViewItem( Q3ListView *parent, aCfg * cfgmd, aCfgItem cfgobj, const QString &name )
: ananasListViewItem( parent, cfgmd, cfgobj, name )
{
	id = md->id(obj);
	if ( ( id ) && ( md->objClass(obj) != md_separator ) ) setRenameEnabled(0, true);
	aCfgItem comaction, active;
	QPixmap pix;
	int idd;

	comaction = md->findChild( obj, md_comaction, 0 );
	idd = md->text( comaction ).toInt();
	active = md->findChild( md->find( idd ), md_active_picture, 0 );
	pix.loadFromData( md->binary( active ) );
	setPixmap( 0, pix );
	pix = 0;
};

InterfaceListViewItem::InterfaceListViewItem( ananasListViewItem *parent, ananasListViewItem
						*after, aCfg * cfgmd, aCfgItem cfgobj, const QString &name )
: ananasListViewItem( parent, after, cfgmd, cfgobj, name )
{
	id = md->id( obj );
	if ( ( id ) && ( md->objClass(obj) != md_separator ) ) setRenameEnabled(0, true);

	aCfgItem comaction, active;
	int idd;
	QPixmap pix;

	comaction = md->findChild( obj, md_comaction, 0 );
	idd = md->text( comaction ).toInt();
	active = md->findChild( md->find( idd ), md_active_picture, 0 );
	pix.loadFromData( md->binary( active ) );
	setPixmap( 0, pix );
	pix = 0;
};

void
InterfaceListViewItem::loadTree()
{
	QString				oclass;
	aCfgItem			cobj;

	clearTree();

	if ( !md ) return;
	cobj = md->firstChild ( obj );
	while ( !cobj.isNull() )
	{
		oclass = md->objClass ( cobj );
		if ( oclass == md_submenu || oclass == md_popupmenu || oclass == md_toolbar )
		{
			loadSubmenu ( this, getLastChild(), cobj );
		}
		oclass = md->objClass ( cobj );
		if ( oclass == md_command )
		{
			loadCommand ( this, getLastChild(), cobj );
		}
		oclass = md->objClass ( cobj );
		if ( oclass == md_separator )
		{
			loadSeparator ( this, getLastChild(), cobj );
		}
		cobj = md->nextSibling ( cobj );
	}
};

void
InterfaceListViewItem::loadSubmenu ( ananasListViewItem * parent, ananasListViewItem *after, aCfgItem child )
{
	InterfaceListViewItem * mparent;
	QString				oclass;
	aCfgItem			cobj;

	mparent = new InterfaceListViewItem( parent, after, md, child, QString::null );
	mparent->setPixmap(0, rcIcon("submenu.png"));
	cobj = md->firstChild ( mparent->obj );
	while ( !cobj.isNull() )
	{
		oclass = md->objClass ( cobj );
		if ( oclass == md_submenu  || oclass == md_toolbar )
		{
			loadSubmenu ( mparent, mparent->getLastChild(), cobj );
		}
		oclass = md->objClass ( cobj );
		if ( oclass == md_command )
		{
			loadCommand ( mparent, mparent->getLastChild(), cobj );
		}
		oclass = md->objClass ( cobj );
		if ( oclass == md_separator )
		{
			loadSeparator ( mparent, mparent->getLastChild(), cobj );
		}
		cobj = md->nextSibling ( cobj );
	}
};

void
InterfaceListViewItem::loadCommand ( ananasListViewItem * parent, ananasListViewItem *after, aCfgItem child )
{
	new InterfaceListViewItem(  parent, after, md, child, QString::null );
};

void
InterfaceListViewItem::loadSeparator ( ananasListViewItem * parent, ananasListViewItem *after, aCfgItem child )
{
	InterfaceListViewItem 	*newitem;
	newitem = new InterfaceListViewItem( parent, after, md,child,QObject::tr("-------------------") );
	newitem->setPixmap(0, rcIcon("separator.png"));

};

void
InterfaceListViewItem::newCommand ()
{
	aCfgItem			newobj;
	InterfaceListViewItem 	*newitem;//, *par;
	QString 			oclass = md->objClass( obj );

/*	par = (InterfaceListViewItem *)this->parent();
	if ( par->text( 0 ) != QObject::tr("Interface") ) obj = md->findChild( obj, md_toolbars );
*/	if ( oclass == md_popupmenu || oclass == md_submenu ||
		oclass == md_mainmenu || oclass == md_toolbar )
	{
		setSelected( FALSE );
		setOpen( TRUE );
		newobj = md->insert( obj, md_command, QObject::tr("New Command") );
		newitem = new InterfaceListViewItem( this, getLastChild(), md, newobj );
//		newitem->setSelected( TRUE );
		newitem->edit();
	};
}

void
InterfaceListViewItem::newSubmenu ()
{
	aCfgItem			newobj;
	InterfaceListViewItem 	*newitem;
	QString 			oclass = md->objClass( obj );

	if ( oclass == md_submenu || oclass ==  md_mainmenu ||
		oclass == md_toolbars || oclass == md_popupmenus || oclass == md_popupmenu )
	{
		setSelected( FALSE );
		setOpen( TRUE );
		if ( oclass == md_popupmenus ) newobj = md->insert( obj, md_popupmenu, QObject::tr("New Popup menu") );
		else
		if ( oclass == md_toolbars ) newobj = md->insert( obj, md_toolbar, QObject::tr("New tool bar") );
		else
		newobj = md->insert( obj, md_submenu, QObject::tr("New Submenu") );
		newitem = new InterfaceListViewItem( this, getLastChild(), md, newobj );
		newitem->setPixmap(0, rcIcon("submenu.png"));
//		newitem->setSelected( TRUE );
	};

}

void
InterfaceListViewItem::newSeparator ()
{
	aCfgItem			newobj;
	InterfaceListViewItem 	*newitem;
	QString 			oclass = md->objClass( obj );

	if ( oclass == md_submenu || oclass == md_mainmenu || oclass == md_popupmenu )
	{
		setSelected( FALSE );
		setOpen( TRUE );
		newobj = md->insert( obj, md_separator );
		newitem = new InterfaceListViewItem( this, getLastChild(), md, newobj, QObject::tr("-------------------") );
		newitem->setPixmap(0, rcIcon("separator.png"));
//		newitem->setSelected( TRUE );

	};
}

void
InterfaceListViewItem::edit ()
{
    QWorkspace *ws = mainform->ws;
    aWindowsList *wl = mainform->wl;
    QString oclass = md->objClass( obj );
    int objid = md->id( obj );
    if ( wl->find( objid ) ) {
	wl->get( objid )->setFocus();
	return;
    }

    if ( oclass == md_command )
    {
	dEditCommand * e = new dEditCommand ( ws, 0, Qt::WDestructiveClose );
	wl->insert( objid, e );
	editor = e;
	QObject::connect( mainform, SIGNAL( tosave() ), editor, SLOT( updateMD() ) );
	e->setData( this );
	e->show();
	//--mainform->addTab(++mainform->lastTabId,e->name());
	mainform->addTab(e);
	return;
    }
}


InterfaceTreeView::InterfaceTreeView ( QWidget *parent, aCfg *cfgmd )
: ananasTreeView ( parent, cfgmd )
{
	InterfaceListViewItem *mainmenu, *toolbars, *popups;
	aCfgItem iface, item;
	iface = md->find ( mdc_interface );
	if ( iface.isNull() ) iface = md->insert( md->find ( mdc_root ), md_interface, QString::null, -1 );
	item = md->find ( iface, md_toolbars );
	if ( item.isNull() ) item = md->insert( iface, md_toolbars, QString::null, -1 );
	toolbars = new InterfaceListViewItem ( this, md, item, QObject::tr ( "Toolbars" ) );
	toolbars->setPixmap(0, rcIcon("toolbar.png"));
	toolbars->loadTree();
	toolbars->setOpen ( TRUE );
	item = md->find ( iface, md_mainmenu );
	if ( item.isNull() ) item = md->insert( iface, md_mainmenu, QString::null, -1 );
	mainmenu = new InterfaceListViewItem ( this, md, item, QObject::tr ( "Main menu" ) );
	mainmenu->setPixmap(0, rcIcon("m_menu.png"));
	mainmenu->loadTree();
	mainmenu->setOpen ( TRUE );
	item = md->find ( iface, md_popupmenus );
	if ( item.isNull() ) item = md->insert( iface, md_popupmenus, QString::null, -1 );
	popups = new InterfaceListViewItem ( this, md, item, QObject::tr ( "Popup menus" ) );
	popups->setPixmap(0, rcIcon("p_menus.png"));
	popups->loadTree();
	popups->setOpen ( TRUE );
	connect( this, SIGNAL( contextMenuRequested( Q3ListViewItem*, const QPoint&, int) ), this, SLOT(ContextMenu() ) );
	connect( this, SIGNAL( returnPressed( Q3ListViewItem*) ), this, SLOT( itemEdit() ) );
	connect( this, SIGNAL( doubleClicked( Q3ListViewItem*) ), this, SLOT( itemEdit() ) );
};


void
InterfaceTreeView::ContextMenu()
{
	Q3PopupMenu *m=new Q3PopupMenu( this, "PopupMenu" );
	Q_CHECK_PTR(m);

/*	QLabel *caption = new QLabel( "<font color=darkblue><u><b>" "Context Menu</b></u></font>", this );
	caption->setAlignment( Qt::AlignCenter );
	m->insertItem( caption );
	m->insertItem( tr("&Rename"), this, SLOT( itemRename() ), CTRL+Key_R);
	m->insertItem( tr("&Edit"),  this, SLOT( itemEdit() ), CTRL+Key_E );
	m->insertItem( tr("&Delete"), this, SLOT( itemDelete() ), CTRL+Key_D );
	m->insertItem( tr("&MoveUp"), this, SLOT( itemMoveUp() ), CTRL+Key_U );
	m->insertItem( tr("&MoveDown"), this, SLOT( itemMoveDown() ), CTRL+Key_M );
*/
	ananasTreeView::ContextMenuAdd( m );
	m->insertItem( tr("&New Submenu"),  this, SLOT( itemNewSubmenu() ), Qt::CTRL+Qt::Key_N );
	m->insertItem( tr("New &Command"),  this, SLOT( itemNewCommand() ), Qt::CTRL+Qt::Key_C );
	m->insertItem( tr("New &Separator"),  this, SLOT( itemNewSeparator() ), Qt::CTRL+Qt::Key_S );
	m->exec( QCursor::pos() );
	delete m;
};

void
InterfaceTreeView::itemNewSubmenu()
{
	InterfaceListViewItem *i = (InterfaceListViewItem *) selectedItem();
	if ( i )
		i->newSubmenu();
};

void
InterfaceTreeView::itemNewCommand()
{
	InterfaceListViewItem *i = (InterfaceListViewItem *) selectedItem();
	if ( i )
		i->newCommand();
};

void
InterfaceTreeView::itemNewSeparator()
{
	InterfaceListViewItem *i = (InterfaceListViewItem *) selectedItem();
	if ( i )
		i->newSeparator();
};

void
InterfaceTreeView::itemDelete()
{
	deleteItem();
};

void
InterfaceTreeView::itemMoveUp()
{
	moveUpItem();
};

void
InterfaceTreeView::itemMoveDown()
{
	moveDownItem();
};

void
InterfaceTreeView::itemEdit()
{
	InterfaceListViewItem *i = (InterfaceListViewItem *) selectedItem();
	if ( i ) i->edit();
};
void
InterfaceTreeView::itemRename()
{
	renameItem();
}

