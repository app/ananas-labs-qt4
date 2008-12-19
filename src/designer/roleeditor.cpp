/****************************************************************************
** $Id: roleeditor.cpp,v 1.1 2008/11/05 21:16:27 leader Exp $
**
** Code file of the Alias editor of Ananas
** Designer applications
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

#include <q3header.h>
#include "roleeditor.h"
#include "acfg.h"


aRoleEditor::aRoleEditor( aCfg *c, aCfgItem o, Q3Table *t, const char *p )
{
    ac = c;
    obj = o;
    tRoles = t;
    tRoles->setNumRows( 0 );
    tRoles->setNumCols( 1 );
    tRoles->horizontalHeader()->setLabel( 0, tr("Read") );
    parent = p;
    if ( !strcmp( parent, md_document ) ) {
	tRoles->setNumCols( 5 );
	tRoles->horizontalHeader()->setLabel( 1, tr("Write") );
	tRoles->horizontalHeader()->setLabel( 2, tr("Delete") );
	tRoles->horizontalHeader()->setLabel( 3, tr("Turn On") );
	tRoles->horizontalHeader()->setLabel( 4, tr("Turn Off") );
    }
    if ( !strcmp( parent, md_catalogue ) ) {
	tRoles->setNumCols( 3 );
	tRoles->horizontalHeader()->setLabel( 1, tr("Write") );
	tRoles->horizontalHeader()->setLabel( 2, tr("Delete") );
    }

}

aRoleEditor::~aRoleEditor()
{
}

void
aRoleEditor::setData()
{
    int i, j, n;
    aCfgItem orole, roles, role;
    QString rolename, right;

    roles = ac->find( ac->find( mdc_root ), md_roles, 0 );
    roleCount = ac->count( roles, md_role );
    n = ac->countChild( obj, md_role );
    tRoles->setNumRows( roleCount );
    for ( i = 0; i < roleCount; i++ ) {
	role = ac->findChild( roles, md_role, i );
	rolename = ac->attr( role, mda_name );
	tRoles->verticalHeader()->setLabel( i, rolename );
	Q3CheckTableItem *r = new Q3CheckTableItem( tRoles, QString::null );
	tRoles->setItem( i, 0, r );
	Q3CheckTableItem *w = new Q3CheckTableItem( tRoles, QString::null );
    	Q3CheckTableItem *d = new Q3CheckTableItem( tRoles, QString::null );
    	Q3CheckTableItem *on = new Q3CheckTableItem( tRoles, QString::null );
    	Q3CheckTableItem *off = new Q3CheckTableItem( tRoles, QString::null );
	if ( !strcmp( parent, md_catalogue ) ) {
		tRoles->setItem( i, 1, w );
		tRoles->setItem( i, 2, d );
	    }
	if ( !strcmp( parent, md_document ) ) {
		tRoles->setItem( i, 1, w );
		tRoles->setItem( i, 2, d );
		tRoles->setItem( i, 3, on );
		tRoles->setItem( i, 4, off );
	    }
	for ( j = 0; j < n; j++) {
	    orole = ac->findChild( obj, md_role, j );
	    if ( rolename == ac->attr( orole, mda_name ) ) {
		right = ac->attr( orole, mda_rights );
		if ( right.find( "-r" ) > -1 ) r->setChecked( TRUE );
		if ( right.find( "-w" ) > -1 ) w->setChecked( TRUE );
		if ( right.find( "-d" ) > -1 ) d->setChecked( TRUE );
		if ( right.find( "-on" ) > -1 ) on->setChecked( TRUE );
		if ( right.find( "-off" ) > -1 ) off->setChecked( TRUE );
	    }
	}
    }
}

void aRoleEditor::updateMD()
{
    int i;
    aCfgItem role;

    do {
	role = ac->findChild( obj, md_role, 0 ) ;
	if ( !role.isNull() ) ac->remove( role );
    } while ( !role.isNull() );
    for ( i = 0; i < tRoles->numRows(); i++ ) {
	QString right;
	Q3CheckTableItem *q;
	role = ac->insert( obj, md_role, tRoles->text( i, 0 ), -1 );
	ac->setAttr( role, mda_name, tRoles->verticalHeader()->label( i ));
	q = (Q3CheckTableItem *)tRoles->item( i, 4 );
	if ( q ) if ( q->isChecked() ) right.insert( 0, "-off" );
	q = (Q3CheckTableItem *)tRoles->item( i, 3 );
	if ( q ) if ( q->isChecked() ) right.insert( 0, "-on" );
	q = (Q3CheckTableItem *)tRoles->item( i, 2 );
	if ( q ) if ( q->isChecked() ) right.insert( 0, "-d" );
	q = (Q3CheckTableItem *)tRoles->item( i, 1 );
	if ( q ) if ( q->isChecked() ) right.insert( 0, "-w" );
	q = (Q3CheckTableItem *)tRoles->item( i, 0 );
	if ( q ) if ( q->isChecked() ) right.insert( 0, "-r" );
	ac->setAttr( role, mda_rights, right );
    }
}

