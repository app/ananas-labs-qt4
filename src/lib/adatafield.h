/****************************************************************************
** $Id: adatafield.h,v 1.1 2008/11/05 21:16:28 leader Exp $
**
** Header file of the Ananas data field of Ananas
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

#ifndef ADATAFIELD_H
#define ADATAFIELD_H
//#include <qsqlfield.h>

#include <qobject.h>
#include <qvariant.h>
#include "ananasglobal.h"
#include "acfg.h"


//class aCfg;
//class aCfgItem;
//class QObject;
//#define

/*!
 * \en
 * 	aDataField object unite all ananas types for access them from
 * 	Ananas Script.
 * 	Object translate calls from script to special type object
 * 	with check correct call functions and parameters.
 * \_en
 * \ru
 * 	\brief Класс определяет программный интерфейс для доступа к полям бизнес объектов Ананаса.
 * 	Наследует QObject.
 *
 * 	Позволяет получить и задать значение поля любого типа. Предназначен для использования из Ананас-скрипта.
 * \_ru
 */

class  ANANAS_EXPORT aDataField : public QObject
{
	Q_OBJECT
//	Q_PROPERTY( Priority priority READ priority WRITE setPriority )
//	Q_ENUMS( Priority )
//	Q_CLASSINFO( "Author", "Oscar Peterson")
public:
//	enum fieldType ( Unknown, Numberic, Character, Date, Object );
	aCfgItem context;
	long id;
	aCfg *md;
	bool fSys;
	int Width, Dec;
	QString Name, fType;
	char aType;
	QVariant::Type Type;
//	QVariant field;
	int oType;


//	aField( aCfg *newmd, aCfgItem newcontext );
	aDataField( QObject *parent, const QString &fname = QString::null, const QString &ftype = QString::null );
	aDataField( const QString &fname = QString::null, const QString &ftype = QString::null );
	aDataField( const aDataField &field );
	~aDataField();

	aDataField& operator=( const aDataField& other );
	bool operator==( const aDataField& other ) const;
	bool operator!=( const aDataField& other ) const;
//	aDataField& operator[]( const int& i ){};

//	QSqlField *sqlField();
	virtual QVariant internalValue();
	virtual void setInternalValue( const QVariant &value);
public slots:
	virtual QString fieldName() const;
	virtual int ObjectType();
	virtual QVariant value();
	virtual void setValue( const QVariant &value);
private:
	QVariant fieldData;

	void init( const QString &fname = QString::null, const QString &ftype = QString::null );
};


#endif
