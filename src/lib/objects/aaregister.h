/****************************************************************************
** $Id: aaregister.h,v 1.1 2008/11/05 21:16:29 leader Exp $
**
** Header file of the Accumulation Register of Ananas
** Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) 2003-2004 Andrey Paskal <app at lrn dot ru>.
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

#ifndef AAREGISTER_H
#define AAREGISTER_H

#include	"acfg.h"
#include	"airegister.h"
//Added by qt3to4:
#include <QSqlQuery>



/*!
 *\en
 *	Object for working with accumulation register in database.
 *	Using for provide between database tables and wobjects or ananas scripts
 *\_en
 *\ru
 *\brief Класс aARegister определяет программный интерфейс Накопительных регистров. Наследует aIRegister.
 *
 * 	Накопительный регистр расширяет функциональность информационного регистра aIRegister,
 * 	автоматически расчитывая остатки по каждому измерению.
 *	В базе данных хранятся остатки по каждому измерению и на каждое время изменения значения ресурсов.
 *	Реквизиты накопительного регистра деляться на три вида: измерения, ресурсы, примечания.
 *	\see aIRegister
 *\_ru
 */
class  ANANAS_EXPORT aARegister : public aIRegister
{
	Q_OBJECT
public:
	aARegister(aCfgItem context, aDatabase * adb = 0);
	aARegister(QString name, aDatabase * adb = 0);
	virtual ~aARegister();

	virtual ERR_Code initObject();

	virtual bool deleteDocument( aDocument * doc );
	//virtual bool deleteTable( Q_ULLONG iddt );
	virtual QString trSysName( const QString & sname );
public slots:
	virtual ERR_Code New();
	virtual int Update();
	//int SelectCountByDoc();
	//int ClosePeriod( const QDateTime & date = QDateTime::currentDateTime() );
	//int SelectRemainders(const QDateTime & date = QDateTime::currentDateTime() );
	//int SelectRemainders(const QString & strdate);
	//int SelectRemainder(const QString & strdate, const QString & dimfieldname, QVariant dimvalue);
	QVariant getSaldo(const QDateTime &date, const QString & dimfieldname, QVariant dimvalue, const QString &resname);
	QVariant getSaldo(const QString &dateISO, const QString & dimfieldname, QVariant dimvalue, const QString &resname);

	QVariant getSaldoByManyDimensions(const QString &from, const QString &to, const QString & dimfieldname, QVariant dimvalue, const QString &resname);
	/*
	QVariant SumValue( const QString & name );
	bool SumNext();
	bool SumPrev();
	bool SumFirst();
	bool SumLast();
	*/
protected:
	void recalculate_saldo(aSQLTable *t, aSQLTable *t_dim, const QDateTime & dd, bool plus, long dimId, QVariant dimValue);
	int insert_values(QSqlQuery *q, aSQLTable *t_dim, const QDateTime & dd, bool plus, long dimId, QVariant dimValue);
	int update_values(aSQLTable *t_dim, const QDateTime & dd, bool plus, long dimId, QVariant dimValue, aSQLTable *t=0);
private:

	void resum( aSQLTable * t, const QDateTime & dd, bool plus );
	QString tablename;
	QSqlQuery sumQuery;
	QMap<long,QString> resnames;
	QMap<QString,QString> resSysNames;
	QMap<long,QString> dimnames;
	//QDateTime actualDate;
	//ERR_Code makeTmpTable( const QDateTime & date );
};

#endif// AAREGISTER_H
