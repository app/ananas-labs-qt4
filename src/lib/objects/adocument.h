/****************************************************************************
** $Id: adocument.h,v 1.1 2008/11/05 21:16:29 leader Exp $
**
** Document metadata object header file of
** Ananas application library
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
**
** This file is part of the Designer application of the Ananas
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

#ifndef ADOCUMENT_H
#define ADOCUMENT_H

#include	<qdatetime.h>
#include	"acfg.h"
#include 	"aobject.h"
//#include	"adocjournal.h"

class aDocJournal;

class aDatabase;



/*!
 *	\en
 *	Not visual object used for working with documents in database.
 *	Used as interface between database and script or wObjects
 *	\_en
 *	\ru
 *	\brief Определяет программный интерфейс для работы с бизнес объектами типа Документ и их атрибутами.
 *	Наследует aObject.
 *
 *	Объект класса предоставляет навигационный доступ ко всем документам, имеющим тип, указанный при инициализации
 *	объекта. С помощью методов класса можно управлять как документом целиком, так и отдельными его реквизитами.
 *	В том числе табличными частями, если они определены в метаданных у документа.
 *	Обеспечивает единообразный доступ к полям и таблицам документов,
 *	хранимых в базе данных. Информация о составе и структуре полей и табличных частей документа
 *	храниться в метаданных.
 *	Позволяет создавать, удалять, изменять, копировать, проводить и распроводить документы.
 *	\_ru
 */
class  ANANAS_EXPORT aDocument :public aObject
{
	Q_OBJECT
	ERR_Code tableDeleteLines( const QString & tablename );
	ERR_Code tableSelect( const QString & tablename, qulonglong = 0 );
public:

	aDocument( aCfgItem context, aDatabase * adb = 0 );
	aDocument( const QString & name, aDatabase * adb = 0 );
	virtual ~aDocument();

	virtual ERR_Code select ( qulonglong uid );
	virtual ERR_Code initObject();
	virtual QString trSysName( const QString & sname );
	virtual QVariant sysValue( const QString & sname, const QString & = ""); // second parameter is here to make method declaration identical to aObject::sysValue
	virtual ERR_Code setSysValue( const QString & sname, QVariant value, const QString & = ""); // third parameter makes method declaration identical to aObject::setSysValue

	aSQLTable * getTable( const QString & tablename );
public slots:

	virtual ERR_Code New();
	virtual ERR_Code Delete();
	virtual ERR_Code Update();
	virtual ERR_Code Select( QDateTime from, QDateTime to );
	virtual bool	 Select();
	virtual ERR_Code Select( const QString & num );
	virtual int SelectDocument( QVariant uid );
	virtual ERR_Code Copy();

	virtual bool Next();
	virtual bool Prev();
	virtual bool First();
	virtual bool Last();

	virtual ERR_Code TableNewLine( const QString & tablename );
	virtual ERR_Code TableDelete(const QString & tablename);
	virtual ERR_Code TableSelect( const QString & name );

	virtual QVariant TableValue( const QString & tablename, const QString & name );
	virtual ERR_Code TableSetValue( const QString & tablename, const QString & name,
									const QVariant &value );
	virtual ERR_Code TableUpdate( const QString & tablename = "" );

	virtual bool TableNext( const QString & name);
	virtual bool TablePrev( const QString & name);
	virtual bool TableFirst( const QString & name);
	virtual bool TableLast( const QString & name);

	virtual bool IsMarkDeleted();
	virtual bool IsMarked();
	virtual ERR_Code SetMarkDeleted( bool Deleted );
	virtual ERR_Code SetMarked( bool Marked );

	void SetPrefix( const QString & pr );
	QString Prefix();

	virtual QVariant Value( const QString & name, const QString &tableName = "" );

	virtual ERR_Code SetFilter( const QString & valname, const QVariant & value );
	virtual ERR_Code ClearFilter();

	ERR_Code Conduct(); //depricated
	ERR_Code UnConduct(); //depricated
	bool IsConducted(); // depricated
	ERR_Code SignIn();
	ERR_Code SignOut();
	bool IsSignedIn();
//	virtual ERR_Code TableSetFilter( const QString & tname, const QString & valname, const QVariant & value );
//	virtual ERR_Code TableClearFilter( const QString & tname );
	void Seek(int pos, const QString &tname);
	QVariant GetDocumentValue(QVariant ide, const QString &fname);

private:
	QString pref;
	aDocJournal* sysJournal;
	ERR_Code setConduct( bool cond );
};

#endif// ADOCUMENT_H
