/****************************************************************************
** $Id: adatafield.cpp,v 1.1 2008/11/05 21:16:28 leader Exp $
**
** Code file of the Ananas database field of Ananas
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

//#include <qobject.h>
//#include "acfg.h"
#include "adatafield.h"

/*!
Create Ananas database field contaner.
*/
/*
aDataField::aDataField(aCfg *newmd, aCfgItem newcontext )
:QObject( 0, "aField" ) // name )
{
	md = newmd;
	context = newcontext;
//	fType = type;
	fTName = "";
//	field = new QSqlField( fTName );
	fSys = false;
}
*/

/*!
 * Create Ananas field contaner.
 */
aDataField::aDataField( QObject *parent, const QString &name, const QString &type )
:QObject( parent, "aField" )
{
	init( name, type );
}


/*!
 * Create Ananas field contaner.
 */
aDataField::aDataField(const QString &name, const QString &type )
:QObject( 0, "aField" )
{
	init( name, type );
}


aDataField::aDataField( const aDataField &field )
:QObject( 0, "aField" )
{
	init( field.fieldName(), field.fType );
}

/*!
 * Destroy object.
 */

aDataField::~aDataField()
{
//	delete field;
}

aDataField&
aDataField::operator=( const aDataField& other )
{
	Type = other.Type;
	context = other.context;
	id = other.id;;
	md = other.md;
	fSys = other.fSys;
	Width = other.Width;
	Dec = other.Dec;
	Name = other.Name;
	fType = other.fType;
	aType = other.aType;
	fieldData = other.fieldData;
	return *this;
}

bool
aDataField::operator==( const aDataField& other ) const
{
	return (
	fSys == other.fSys &&
	Name == other.Name &&
	fType == other.fType &&
	id == other.id &&
	context == other.context
	);
}


bool
aDataField::operator!=( const aDataField& other ) const
{
	return !( other == *this );
}

void
aDataField::init( const QString &name, const QString &type )
{
	QString t;

	fSys = true;
	Name = name;
	fType = type;
	Type = QVariant::Invalid;
	if ( !type.isNull() ) {
		aType = ( (const char *) type.section(" ",0,0).upper() )[0];
		Width = type.section(" ",1,1).toInt();
		Dec = type.section(" ",2,2).toInt();
		switch ( aType ){
		case 'C':
			Type = QVariant::String;
			fieldData = QString("");
			break;
		case 'N':
			Type = QVariant::Double;
			fieldData = ( double ) 0.0;
		default:
			Type = QVariant::Invalid;
		}
	}
}

/*!
Return pointer to asociated sql field.
*/
//QSqlField *
//aField::sqlField(){
//	return field;
//}


/*!
 *
 */
QVariant
aDataField::internalValue()
{
	return fieldData;
}


/*!
 *
 */
void
aDataField::setInternalValue( const QVariant &value)
{
	fieldData = value;
}



/*!
 * Возвращает значение поля данных.
 */
QVariant
aDataField::value()
{
	return fieldData;
}


/*!
 * Устанавливает значение поля данных.
 */
void
aDataField::setValue( const QVariant &value)
{
	fieldData = value;
}



/*!
 *
 */
int
aDataField::ObjectType()
{
	return oType;
}


QString
aDataField::fieldName() const
{
	return Name;
}
