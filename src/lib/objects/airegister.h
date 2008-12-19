/****************************************************************************
** $Id: airegister.h,v 1.1 2008/11/05 21:16:29 leader Exp $
**
** Header file of the Information Register of Ananas
** Engine applications
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

#ifndef AIREGISTER_H
#define AIREGISTER_H
#include	<q3valuelist.h>
#include	<qstringlist.h>
#include	<qvariant.h>
#include	<qdatetime.h>
#include	"acfg.h"
#include 	"aobject.h"

class aDatabase;

class aDocJournal;

class aDocument;



/*!
 * \en
 *	Object for working with information register in database.
 *	Using for provide between database tables and wobjects or ananas scripts
 * \_en
 * \ru
 * \class aIRegister aIRegister.h
 * \brief Класс aIRegister определяет программный интерфейс Информационных регистров.
 *
 *	Информационный регистр служит для хронологической записи данных.
 *	Для хранения остатков следует использовать Накопительные регистры aARegister
 *	\see aARegister
 * \_ru
 */
class  ANANAS_EXPORT aIRegister :public aObject
{
	Q_OBJECT
public:
	aIRegister(aCfgItem registerItem, aDatabase * adb = 0);
	aIRegister( const QString & name, aDatabase * adb = 0, const QString & type = "InfoRegister.");
	virtual ~aIRegister();
//	ERR_Code initObject(aDatabase * adb);
	virtual ERR_Code initObject();
	virtual bool deleteDocument( aDocument * doc );
	virtual bool deleteTable( qulonglong iddt );
public slots:
	virtual int SetDocument( aDocument * doc );
	virtual aDocument * GetDocument();
	virtual int SetTable( const QString & tblname );
	virtual int New();
	virtual int Select( const QString &from, const QString & to);
	virtual int Select( QDateTime from, QDateTime to );
	virtual int SelectByDoc();
	virtual QVariant Value( const QString & name, const QString &tableName = "" );

	virtual int SetFilter( const QString & fieldName, const QVariant & value );
	virtual bool Next();
	virtual bool Prev();
	virtual bool First();
	virtual bool Last();
protected:
	bool docseted, tableseted;
	QString tname;
	aDocument * nowDoc;
};

#endif// AIREGISTER_H
