/****************************************************************************
** $Id: aliaseditor.cpp,v 1.1 2008/11/05 21:16:27 leader Exp $
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
#include "aliaseditor.h"
#include "acfg.h"


aAliasEditor::aAliasEditor( aCfg *c, aCfgItem o, Q3Table *t )
{
    ac = c;
    obj = o;
    tAliases = t;
    tAliases->setNumRows( 0 );
    tAliases->setNumCols( 1 );
    tAliases->horizontalHeader()->setLabel( 0, tr("Name") );
}

aAliasEditor::~aAliasEditor()
{
}

void aAliasEditor::setData()
{
    int i, j, n;
    aCfgItem alias, langs, lang;
    QString langtag;

    langs = ac->find( ac->find( mdc_root ), md_languages, 0 );
    langCount = ac->count( langs, md_language );
    n = ac->countChild( obj, md_alias );
    tAliases->setNumRows( langCount );
	for ( i = 0; i < langCount; i++ ) {
	    lang = ac->findChild( langs, md_language, i );
	    langtag = ac->attr( lang, mda_tag );
	    tAliases->verticalHeader()->setLabel( i, langtag );
	    for ( j = 0; j < n; j++) {
		alias = ac->findChild( obj, md_alias, j );
		if ( langtag == ac->attr( alias, mda_tag ) ) {
		    tAliases->setText( i, 0, ac->attr( alias, mda_name ) );
		}
	    }
	}
}

void aAliasEditor::updateMD()
{
    int i;
    aCfgItem alias;

    do {
	alias = ac->findChild( obj, md_alias, 0 ) ;
	if ( !alias.isNull() ) ac->remove( alias );
    } while ( !alias.isNull() );
    for (i = 0; i < tAliases->numRows(); i++ ) {
	if ( tAliases->text( i, 0 ) != "" ) {
	    alias = ac->insert( obj, md_alias, tAliases->text( i, 0 ), -1 );
	   ac->setAttr( alias, mda_tag, tAliases->verticalHeader()->label( i ));
	}
    }
}

