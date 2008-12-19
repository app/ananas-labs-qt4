/****************************************************************************
** $Id: amenubar.cpp,v 1.1 2008/11/05 21:16:28 leader Exp $
**
** Code file of the Ananas Menu bar of Ananas
** Designer and Engine applications
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

/******************************************************************
 ******************************************************************/

#include "amenubar.h"
//Added by qt3to4:
#include <Q3PopupMenu>
#include <QPixmap>


AMenuBar::AMenuBar( QWidget* parent , const char* name  )
	:QMenuBar( parent, name ) {

}

AMenuBar::AMenuBar( aCfg *cfg, QWidget* parent , const char* name  )
:QMenuBar( parent, name )
{
	md = cfg;
	ReadMenu( md->find( md->find( mdc_interface ), md_mainmenu) );
}

void
AMenuBar::ReadMenu( aCfgItem obj )
{
	aCfgItem	cobj;
	Q3PopupMenu	*parent;
	QString		text, aKey ;
	long 		id;

	if ( !md )
		return;
	cobj = md->firstChild ( obj );
	while ( !cobj.isNull() )
	{
		id = md->id( cobj );
		if ( md->objClass ( cobj ) == md_submenu )
		{
			parent = new Q3PopupMenu ();
			insertItem( md->attr( cobj, mda_name ), parent );
			ReadMenu( parent, cobj );
		}
		if ( md->objClass ( cobj ) == md_command )
		{
			text = md->sText ( cobj, md_menutext );
			if ( text == "" ) text = md->attr( cobj, mda_name );
			aKey = md->sText ( cobj, md_key );
			QMenuBar::insertItem( text, this, SLOT(on_Item()), QKeySequence( aKey ), id );
		}
		if ( md->objClass ( cobj ) == md_separator )
		{
			insertSeparator();
		}
		cobj = md->nextSibling ( cobj );
	}

}

void
AMenuBar::ReadMenu( Q3PopupMenu *parent, aCfgItem obj )
{
	aCfgItem	cobj, apix;
	Q3PopupMenu	*mparent;
	QString		text, aKey;
	long id, pid;
    QPixmap		pix;

	if ( !md )
		return;

	cobj = md->firstChild ( obj );
	while ( !cobj.isNull() )
	{
		id = md->id( cobj );
		if ( md->objClass ( cobj ) == md_submenu )
		{
			mparent = new Q3PopupMenu ();
			parent->insertItem( md->attr ( cobj, mda_name ), mparent );
			ReadMenu( mparent, cobj );
		}
		if ( md->objClass ( cobj ) == md_command )
		{
			text = md->sText ( cobj, md_menutext );
			if ( text == "" )
				text = md->attr( cobj, mda_name );
			aKey = md->sText ( cobj, md_key );
			pid = md->text( md->findChild( cobj, md_comaction, 0 ) ).toLong();
			apix = md->findChild( md->find( pid ), md_active_picture, 0 );
			pix.loadFromData( md->binary( apix ) );
			parent->insertItem( pix, text, this, SLOT(on_Item()), QKeySequence( aKey ), id );
			pix = 0;
		}
		if ( md->objClass ( cobj ) == md_separator )
		{
			parent->insertSeparator();
		}
		cobj = md->nextSibling ( cobj );
	}

}


AMenuBar::~AMenuBar(){
}

int AMenuBar::insertItem ( const QString & text, Q3PopupMenu * popup, int id, int index ) {
	return QMenuBar::insertItem ( text, popup, id, index);
};

/*
int
AMenuBar::insertItem ( const QString & text, const QObject * receiver, const char * member, const QKeySequence & accel, int id , int index ) {
	return QMenuBar::insertItem( text, receiver, member, accel, id, index);
}
int
AMenuBar::insertItem ( const QPixmap & pixmap, const QObject * receiver, const char * member, const QKeySequence & accel , int id , int index ) {
	return QMenuBar::insertItem ( pixmap, receiver, member, accel,  id, index);
}
int AMenuBar::insertItem ( const QIconSet & icon, const QPixmap & pixmap, const QObject * receiver, const char * member, const QKeySequence & accel , int id , int index  ) {
	return QMenuBar::insertItem ( icon, pixmap, receiver, member, accel,  id, index);
};
int AMenuBar::insertItem ( const QString & text, int id , int index  ) {
	return QMenuBar::insertItem( text, id, index );
};
int AMenuBar::insertItem ( const QIconSet & icon, const QString & text, int id , int index  ) {
	return QMenuBar::insertItem ( icon, text,  id, index);
};
int AMenuBar::insertItem ( const QIconSet & icon, const QString & text, QPopupMenu * popup, int id, int index ) {
	return QMenuBar::insertItem ( icon, text, popup,  id, index);
};
int AMenuBar::insertItem ( const QPixmap & pixmap, int id , int index ) {
	return QMenuBar::insertItem ( pixmap, id, index);
};
int AMenuBar::insertItem ( const QIconSet & icon, const QPixmap & pixmap, int id , int index ) {
	return QMenuBar::insertItem ( icon, pixmap, id, index);
};
int AMenuBar::insertItem ( const QPixmap & pixmap, QPopupMenu * popup, int id, int index ) {
	return QMenuBar::insertItem ( pixmap, popup,  id, index);
};
int AMenuBar::insertItem ( const QIconSet & icon, const QPixmap & pixmap, QPopupMenu * popup, int id , int index ) {
	return QMenuBar::insertItem ( icon, pixmap, popup,  id, index);
};
int AMenuBar::insertItem ( QWidget * widget, int id, int index ) {
	return QMenuBar::insertItem ( widget,  id, index);
};
int AMenuBar::insertItem ( const QIconSet & icon, QCustomMenuItem * custom, int id , int index ) {
	return QMenuBar::insertItem ( icon, custom, id, index);
};
int AMenuBar::insertItem ( QCustomMenuItem * custom, int id , int index ) {
	return QMenuBar::insertItem ( custom,  id, index);
};
int AMenuBar::insertSeparator ( int index ) {
	return QMenuBar::insertSeparator ( index);
};
*/
