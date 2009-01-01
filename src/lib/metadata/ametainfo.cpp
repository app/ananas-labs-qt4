/****************************************************************************
** $Id: ametainfo.cpp,v 1.1 2008/12/24 16:19:38 leader Exp $
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
#include "ametainfo.h"

/*!
 * \class AMetaInfo
 *
 */
AMetaInfo::AMetaInfo()
:AMetaObject("Info")
{
    setAppName("new");
    setAuthor("unknown");
    setDate( QDate::currentDate() );
    setLastId( 100 );

}


AMetaInfo::AMetaInfo( const AMetaInfo &info )
:AMetaObject("Info")
{
}


AMetaInfo::~AMetaInfo()
{
}


AMetaInfo& 
AMetaInfo::operator=(const AMetaInfo&)
{
    return *this;
}


QString 
AMetaInfo::appName()
{
    return attr("name").toString();
}


void 
AMetaInfo::setAppName( const QString &name )
{
    setAttr( "name", name );
}


int
AMetaInfo::lastId()
{
    return attr( "lastid" ).toInt();
}


void 
AMetaInfo::setLastId( int id )
{
    setAttr( "lastid", id );
}


QString 
AMetaInfo::author()
{
    return attr("author").toString();
}


void 
AMetaInfo::setAuthor( const QString &name )
{
    setAttr( "author", name );
}


QDate 
AMetaInfo::date()
{
    return attr("date").toDate();
}


void 
AMetaInfo::setDate( QDate d )
{
    setAttr( "date", d );
}
