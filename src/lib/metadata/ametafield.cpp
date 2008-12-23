/****************************************************************************
** $Id: ametafield.cpp,v 1.2 2008/12/20 21:17:49 leader Exp $
**
** Code file of the Ananas configuration objects of Ananas
** Designer and Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) Grigory Panov <gr1313 at mail.ru>, Yoshkar-Ola.
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
#include "ametafield.h"


/*!
 * \class AMetaField
 *
 */
AMetaField::AMetaField( AMetaObject *parent )
:AMetaObject("MetaField","", parent )
{
    setId( lastId() );
    setName( QString("%1_%2").arg( tr("Field") ).arg( id() ) );
    setFieldType( Unknown );
    setFieldSubType( 0 );
    setWidth( 0 );
    setDecimals( 0 );
    setNotNull( false );
    setNotNegative( false );
    setCalcSumm( false );
}


/*
AMetaField::AMetaField( FieldTypes t, int st, int w, int d, bool notnul, bool notneg, bool calcsum,  )
:AMetaObject("MetaField", "", parent )
{
    setFieldType( t );
    setFieldSubType( st );
    setWidth( w );
    setDecimals( d );
    setNotNull( notnul );
    setNotNegative( notneg );
    setCalcSumm( calcsum );
}
*/

int 
AMetaField::fieldType()
{
    return attr("type").toInt();
}



void 
AMetaField::setFieldType( FieldTypes t )
{
    setAttr( "type", t );
}



int 
AMetaField::fieldSubType()
{
    return attr("subtype").toInt();
}


void 
AMetaField::setFieldSubType( int st )
{
    setAttr( "subtype", st );
}


int 
AMetaField::width()
{
    return attr("width").toInt();
}


void 
AMetaField::setWidth( int w )
{
    setAttr( "width", w );
}



int 
AMetaField::decimals()
{
    return attr("decimals").toInt();
}


void 
AMetaField::setDecimals( int d )
{
    setAttr( "decimals", d );
}



bool 
AMetaField::notNull()
{
    return attr("notnull").toBool();
}


void 
AMetaField::setNotNull( bool f )
{
    setAttr( "notnull", f );
}


bool 
AMetaField::notNegative()
{
    return attr("notnegative").toBool();
}


void 
AMetaField::setNotNegative( bool f )
{
    setAttr( "notnegative", f );
}



bool 
AMetaField::calcSumm()
{
    return attr("calcsumm").toBool();
}


void 
AMetaField::setCalcSumm( bool f )
{
    setAttr( "calcsumm", f );
}



/*!
 * \class AMetaFields
 *
 */
AMetaFields::AMetaFields()
:AMetaGroup("MetaFields")
{

}

