/****************************************************************************
** $Id: asqlfield.cpp,v 1.1 2008/11/05 21:16:28 leader Exp $
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

#include <qobject.h>
#include "acfg.h"
#include "asqlfield.h"

/*!
Create Ananas database field contaner.
*/
/*
aField::aField(aCfg *newmd, aCfgItem newcontext )
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
Create Ananas field contaner.
*/
/*
aField::aField(const QString &name, const QString &type )
:QObject( 0, "aField" )
{
	QString t;

	fSys = true;
	Name = name;
//	Type  = type;
	aType = ( (const char *) type.section(" ",0,0).upper() )[0];
	Width = type.section(" ",1,1).toInt();
	Dec = type.section(" ",2,2).toInt();
	switch ( aType ){
	case 'C':
		Type = QVariant::String;
		field = QString("");
		break;
	case 'N':
		Type = QVariant::Double;
		field = ( double ) 0.0;
	default:
		Type = QVariant::Invalid;
	}
}
*/

/*!
Destroy object.
*/

//aField::~aField()
//{
//	delete field;
//}


/*!
Return pointer to asociated sql field.
*/
//QSqlField *
//aField::sqlField(){
//	return field;
//}


//QVariant
//aField::value()
//{
//	return field;
//}


//void
//aField::setValue( const QVariant &value)
//{
//	field = value;
//}


aSQLField::aSQLField( aCfg *newmd, aCfgItem newcontext )
: QObject( 0, "aSQLField" )
{

}


aSQLField::aSQLField( const QString &name, const QString &type, const QString &tname )
: QObject( 0, "aSQLField" )
{
	tName = tname;
}


aSQLField::~aSQLField()
{

}


QSqlField *
aSQLField::sqlField()
{
	return field;
}


QVariant
aSQLField::value()
{
	return "";
}


void
aSQLField::setValue( const QVariant &value)
{

}
QWidget *
aSQLField::editor()
{
	return NULL;
}
