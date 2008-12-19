/****************************************************************************
** $Id: aobject.h,v 1.1 2008/11/05 21:16:29 leader Exp $
**
** Header file of the Ananas Object of Ananas
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

#ifndef AOBJECT_H
#define AOBJECT_H

#include <qdatetime.h>
#include "acfg.h"
#include "asqltable.h"


class aDatabase;



/*!
 * \en
 *	The parent object for hi level not visual objects
 *	for worcs with SQL database
 *	\_en
 *	\ru
 *	\brief Базовый класс для невизуальных классов (моделей в терминологии MVC) Ананаса,
 *	которым необходим доступ к базе данных.
 *	Наследует QObject.
 *
 *	Как правило такие классы предоставляют методы
 *	для работы с пользовательскими "объектами" типа "Журнал операций",
 *	первичный учетный документ "Приходная накладная", "Справочник товаров",
 *	"Элемент справочника товаров" и др.
 *
 *	Для доступа к данным базы данных через методы такого объекта
 *	сначала необходимо создать объект, указав в параметре вызова
 *	конструктора имя элемента метаданных, описывающего модель данных объекта,
 *	затем вызвать метод New().
 *	\_ru
 */
class  ANANAS_EXPORT aObject : public QObject
{
	Q_OBJECT
public:
	aCfg * md;
	aCfgItem obj;
	aDatabase * db;

	aObject( QObject *parent = 0, const char *name = 0 );
	aObject( const QString &oname, aDatabase *adb, QObject *parent = 0, const char *name = 0 );
	aObject( aCfgItem context, aDatabase *adb, QObject *parent = 0, const char *name = 0 );
	virtual ~aObject();

	virtual bool checkStructure();

	ERR_Code init();
//	aCfgItem *getMDObject();
//	void setMDObject( aCfgItem object );
	ERR_Code decodeDocNum( QString nm, QString & pref, int & num);
	virtual ERR_Code select( qulonglong id );
	qulonglong getUid();
	bool selected( const QString & tablename = "" );
	//void selectTable(const QString & tablename = "");
	ERR_Code select(const QString & query, const QString &tableName = "");
	aDataTable *table( const QString &name = "" );

//	void updateAttributes( const QString & tname = "" );
	virtual qulonglong docId();


	virtual bool Next(const QString &tableName = "" );
	virtual bool Prev(const QString &tableName = "" );
	virtual bool First(const QString &tableName = "" );
	virtual bool Last(const QString &tableName = "" );

public slots:
	virtual QString Uid();
	virtual bool IsSelected();
	virtual bool IsMarkDeleted( const QString & tname = "" );
	virtual bool IsMarked();
//	virtual int TableSetMarkDeleted( bool Deleted, const QString & tablename = "" );//depricated
	virtual int SetMarkDeleted( bool Deleted, const QString & tablename = "" );
	virtual int SetMarked( bool Marked );

//	virtual aObject *GetAttrib( const QString & AttribName );
//	virtual void SetAttrib( const QString & AttribName, const *aObject );

	virtual int New();
	virtual int Copy();
	virtual int Delete();
	virtual int Update();
	virtual int Conduct();
	virtual int UnConduct();
	virtual bool IsConducted();
	virtual QString Kind( const QString & name = QString::null );
//	virtual ERR_Code Select( QString num ){return err_incorrecttype;};
//	virtual ERR_Code Select( QDateTime from, QDateTime to, QString mdName){return err_incorrecttype;};
//	virtual ERR_Code Select( aObject* ){return err_incorrecttype;};

	virtual QVariant Value( const QString & name, const QString &tableName = "" );
	virtual int SetValue( const QString & name, const QVariant &value, const QString &tableName = "" );

	virtual QVariant sysValue( const QString & name, const QString &tableName = "" );
	virtual int setSysValue( const QString & name, QVariant value, const QString &tableName = "" );

//	virtual bool Next();
//	virtual bool Prev();
//	virtual bool First();
//	virtual bool Last();

	virtual int SetFilter( const QString & valname, const QVariant & value );
	virtual int ClearFilter();
	virtual int TableSetFilter( const QString & tname, const QString & valname, const QVariant & value );
	virtual int TableClearFilter( const QString & tname );
	virtual bool IsFiltred() {return filtred;};
	virtual int TableUpdate( const QString & tablename );
	virtual QString displayString();

protected:
	bool concrete;
	virtual aCfgItem displayStringContext();
//	Q_ULLONG lastUid;
	virtual ERR_Code setObject( aCfgItem newobject );
//	virtual ERR_Code initObject( aDatabase *adb );
	virtual ERR_Code initObject();
	bool isInited() { return vInited; };
	void setInited( bool flag ){ vInited = flag; };
	ERR_Code tableInsert( const QString &dbname, aCfgItem obj, const QString &name = "" );
	ERR_Code tableInsert( const QString &dbname, const QString &name = "" );
	ERR_Code tableRemove( const QString &name = "" );

	virtual QVariant tValue( const QString & tablename, const QString & name );
	virtual ERR_Code setTValue(const QString & tablename, const QString & name, const QVariant &value);
	virtual QString trSysName( const QString & sname );
//	Q_ULLONG copy(const QString & tablename = "");
	void	setSelected( bool sel, const QString & tablename = "" );
	virtual ERR_Code setTFilter( const QString & tname, const QString & valname, const QVariant & value );
	virtual ERR_Code clearTFilter( const QString & tname );

private:
	bool vInited, selectFlag, filtred;
	Q3Dict <aDataTable> dbtables;

//	QDict <aDataRecord> attributes;
};


#endif
