/****************************************************************************
** $Id: asqlfield.h,v 1.1 2008/11/05 21:16:28 leader Exp $
**
** Header file of the Ananas database field of Ananas
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

#ifndef ASQLFIELD_H
#define ASQLFIELD_H
#include <qsqlfield.h>
#include "acfg.h"

/*
class  ANANAS_EXPORT aField : public QObject //SqlField
{
	Q_OBJECT
public:
//	enum fieldType ( Unknown, Numberic, Character, Date, Object );
	aCfgItem context;
	long id;
	aCfg *md;
	bool fSys;
	int Width, Dec;
	QString Name;
	char aType;
	QVariant::Type Type;


//	aField( aCfg *newmd, aCfgItem newcontext );
	aField( const QString &fname = QString::null, const QString &ftype = QString::null );
	~aField();

//	QSqlField *sqlField();
public slots:
	virtual QVariant value();
	virtual void setValue( const QVariant &value);
private:
	QVariant field;
};
*/

class  ANANAS_EXPORT aSQLField: public QObject //aField
{
	Q_OBJECT
public:
	QString tName;
	QWidget *editorWidget;

	aSQLField( aCfg *newmd, aCfgItem newcontext );
	aSQLField( const QString &name = QString::null, const QString &type = QString::null, const QString &tname = QString::null );
	~aSQLField();

	QSqlField *sqlField();
	virtual QWidget *editor();

public slots:
	virtual QVariant value();
	virtual void setValue( const QVariant &value);
private:
	QSqlField *field;
};

#endif
