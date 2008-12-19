/****************************************************************************
** $Id: ametaobject.cpp,v 1.2 2008/12/15 22:22:48 leader Exp $
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

#include <qdatetime.h>
//#include <qobject.h>
#include <qfile.h>
#include <qstringlist.h>
#include <QTextStream>
#include <QDomDocument>

#include "ametaobject.h"
#include "ametaobjectgroup.h"

/*!
 * \class AMetaObject
 *
 */
/*!
 *\en
 *	Metadata object for metadata script access.
 *\_en
 *\ru
 *	\brief Объект метаданных для обеспечения доступа из скриптов.
 *	Наследует QObject.
 *\_ru
 */

AMetaObject::AMetaObject(const QString &objectclass, 
                     const QString &objectname, 
                     AMetaObjectGroup *parent)
:QObject( parent )
{
    setObjectName( objectname );
    setClassName( objectclass );
    setDescription("");
}


Q_ULONG 
AMetaObject::id()
{
    return v_id;
}


void 
AMetaObject::setId( Q_ULONG new_id )
{
    v_id = new_id;
}


void 
AMetaObject::setClassName( const QString &name )
{
    v_className = name;
}


QString
AMetaObject::className()
{
    return v_className;
}


QString
AMetaObject::name()
{
    return objectName();
}


QString 
AMetaObject::description()
{
    return text("description");
}

void 
AMetaObject::setDescription( const QString &descr )
{
    setText( "description", descr );
}


void 
AMetaObject::setAttr( const QString &name, const QVariant &value )
{
    if ( value.isNull() && attrExists( name ) ) v_attr.remove( name );
    else v_attr[ name ] = value;
}



void 
AMetaObject::setText( const QString &name, const QString &text )
{
    if ( text.isNull() && textExists( name ) ) v_text.remove( name );
    else v_text[ name ] = text;
}


void 
AMetaObject::setRawdata( const QString &name, const QByteArray &data )
{
    if ( data.isNull() && rawdataExists( name ) ) v_rawdata.remove( name );
    else v_rawdata[ name ] = data;
}



QVariant
AMetaObject::attr( const QString &name ) const
{
    return v_attr.value( name, QVariant() );
}



QString 
AMetaObject::text( const QString &name ) const
{
    return v_text.value( name, QString::null );
}



QByteArray 
AMetaObject::rawdata( const QString &name ) const
{
    return v_rawdata.value( name, QByteArray() );
}



QVariant 
AMetaObject::attr( int idx ) const
{
    return attr( attrName( idx ) );
}



QString 
AMetaObject::text( int idx ) const
{
    return text( textName( idx ) );
}



QByteArray 
AMetaObject::rawdata( int idx ) const
{
    return rawdata( rawdataName( idx ) );
}



QString 
AMetaObject::attrName( int idx ) const
{
    if ( idx < 0 || idx >= attrCount() ) return QString::null;
    return v_attr.keys().at( idx );
}



QString 
AMetaObject::textName( int idx ) const
{
    if ( idx < 0 || idx >= textCount() ) return QString::null;
    return v_text.keys().at( idx );
}



QString 
AMetaObject::rawdataName( int idx ) const
{
    if ( idx < 0 || idx >= rawdataCount() ) return QString::null;
    return v_rawdata.keys().at( idx );
}



bool 
AMetaObject::attrExists( const QString &name )
{
    return v_attr.contains( name );
}



bool 
AMetaObject::textExists( const QString &name )
{
    return v_text.contains( name );
}



bool 
AMetaObject::rawdataExists( const QString &name )
{
    return v_rawdata.contains( name );
}



int 
AMetaObject::attrCount() const
{
    return v_attr.size();
}



int 
AMetaObject::textCount() const
{
    return v_text.size();
}


int 
AMetaObject::rawdataCount() const
{
    return v_rawdata.size();
}



/*!
 * \class AMetaCatalogue
 *
 */
AMetaCatalogue::AMetaCatalogue()
:AMetaObject("Catalogue")
{
}



/*!
 * \class AMetaDataInfo
 *
 */
AMetaDataInfo::AMetaDataInfo()
:AMetaObject("Info")
{
    setAppName("new");
    setAuthor("unknown");
    setDate( QDate::currentDate() );
    setLastId( 100 );

}


AMetaDataInfo::AMetaDataInfo( const AMetaDataInfo &info )
:AMetaObject("Info")
{
}


AMetaDataInfo::~AMetaDataInfo()
{
}


AMetaDataInfo& 
AMetaDataInfo::operator=(const AMetaDataInfo&)
{
    return *this;
}


QString 
AMetaDataInfo::appName()
{
    return text("name");
}


void 
AMetaDataInfo::setAppName( const QString &name )
{
    setText( "name", name );
}


long 
AMetaDataInfo::lastId()
{
    return text( "lastid" ).toLong();
}


void 
AMetaDataInfo::setLastId( long id )
{
    setText( "lastid", QString::number( id ) );
}


QString 
AMetaDataInfo::author()
{
    return text("author");
}


void 
AMetaDataInfo::setAuthor( const QString &name )
{
    setText( "author", name );
}


QDate 
AMetaDataInfo::date()
{
    return QDate::fromString( text("date"), Qt::ISODate );
}


void 
AMetaDataInfo::setDate( QDate d )
{
    setText( "date", d.toString( Qt::ISODate ) );
}



/*!
 * \class AMetaDataGlobal
 *
 */
AMetaDataGlobal::AMetaDataGlobal()
:AMetaObject("Global")
{
    setSourceCode("function on_systemstart()\n{\n}\n");
}


QString 
AMetaDataGlobal::sourceCode()
{
    return text("sourcecode");
}


void 
AMetaDataGlobal::setSourceCode( const QString &src )
{
    setText( "sourcecode", src );
}

