/****************************************************************************
** $Id: adocjournal.h,v 1.1 2008/11/05 21:16:29 leader Exp $
**
** Documents journal metadata object header file of
** Ananas application library
**
** Created : 20040701
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) Andrey Paskal <app at lrn dot ru>, Yoshkar-Ola
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

#ifndef ADOCJOURNAL_H
#define ADOCJOURNAL_H

#include	<qdatetime.h>
#include	"acfg.h"
#include	"aobject.h"
#include	"adocument.h"

class aDatabase;



/*!
 *\~english
 *	Not visual object used for working with system journal.
 *	Used as interface between database and script or wObjects
 *	Included in aDocument for linked document and system journal
 *\~russian
 *	\brief Определяет программный интерфейс управления общим (системным) журналом документов.
 *	Наследует aObject.
 *
 *	Невизуальный объект для работы с системным журналом документов базы данных.
 *	Все хранящиеся в базе данных Ананаса документы система регистрирует в журнале.
 *	Используется для доступа к системному журналу  скриптами и визуальными объектами.
 *	Включается в объект aDocument, для обеспечения работы документа с ситемным журналом.
 *\~
 */
class  ANANAS_EXPORT aDocJournal :public aObject
{
	Q_OBJECT
private:
	/*!\ru	Тип журнала. \n
	 *	0 - общий, содержит список документов всех типовб. \n
	 *	1 - специализированный журнал, содрежит список документов одного типа,
	 *	например  "Расходные накладные".
	 * \_ru
	 */
	int journalType;
public:
	aDocJournal(aCfgItem context, aDatabase * adb = 0);
	aDocJournal( const QString & name, aDatabase * adb = 0);
	aDocJournal( aDatabase * adb = 0);
	virtual ERR_Code initObject();
	ERR_Code selectDocument( qulonglong idd );
	virtual qulonglong docId();
	int docType();

	/*!\ru	\return тип журнала. \n
	 *	0 - общий, содержит список документов всех типовб. \n
	 *	1 - специализированный журнал, содержит список документов одного типа,
	 *	например  "Расходные накладные". \n
	 * \_ru
	 */
	int type() { return journalType; }
	QString nextNumber( const QString &prefix, int mdId );
	QVariant getDate();
	QString getNumber();
	// may be need return ULongLong ?
	int getSerialNumber();
	ERR_Code setDate( QVariant date );
	ERR_Code setNumber( QVariant number );

	virtual ERR_Code New ( qulonglong idd, const QString & docPrefix, int type );
	virtual ERR_Code deleteDocument ( qulonglong idd );
	qulonglong findDocument( qulonglong idd );
//	Q_ULLONG docId( Q_ULLONG id );
	qulonglong findDoc( const QString & num, int type );
	QString getPrefix();
	QString selectionFilter( QDateTime from, QDateTime to, const QString & mdName = "",
							bool full=false );
	QString selectionFilter( const QString & num, const QString & mdName = "", bool full=false );

public slots:
	aDocument* CurrentDocument();
	virtual ERR_Code Delete();
	virtual ERR_Code Select( QDateTime from, QDateTime to, const QString & mdName = "" );
	virtual ERR_Code Select( const QString & num, const QString & mdName = "" );
};

#endif// ADOCJOURNAL_H
