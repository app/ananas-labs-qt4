/****************************************************************************
** $Id: afilter.h,v 1.1 2008/11/05 21:16:28 leader Exp $
**
** Filter declaration file of
** Ananas application library
**
** Created : 20070315
**
** Copyright (C) 2007 Ananas Project.
** Copyright (C) 2007 Grigory Panov <grigory.panov at gmail.com>, Moscow
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

#ifndef afilter_h
#define afilter_h

#include <qstring.h>
#include <q3valuelist.h>
#include <qdatetime.h>
#include "ananasglobal.h"

ANANAS_EXPORT enum OperationEnum{
		OperationEnumEquals,
		OperationEnumNotEquals,
		OperationEnumGreaterThen,
		OperationEnumLessThen,
		OperationEnumGreaterOrEquals,
		OperationEnumLessOrEquals,
		OperationEnumLike
	};

class ANANAS_EXPORT aFilter
{
public:
	aFilter();
	virtual ~aFilter();
	void Dump() const;

	void Add(const QString& fname, const char* value, OperationEnum op, bool AndOp = true, bool replace = true);
	void Add(const QString& fname, const Q_INT64 value, OperationEnum op, bool AndOp = true, bool replace = true);
	void Add(const QString& fname, const int value, OperationEnum op, bool AndOp = true, bool replace = true);
	void Add(const QString& fname, const double value, OperationEnum op, bool AndOp = true, bool replace = true);
	//void Add(const QString& fname, bool value, OperationEnum op, bool AndOp = true, bool replace = true);
	void Add(const QString& fname, const QString& value, OperationEnum op, bool AndOp = true, bool replace = true);
	void Add(const QString& fname, const QDateTime& value, OperationEnum op, bool AndOp = true, bool replace = true);
	void Add(const QString& fname, const QDate& value, OperationEnum op, bool AndOp = true, bool replace = true);
	QString toString(bool removeFirst = true) const;
	void Clear();
protected:
	void AddHelper(const QString& fname, const QString& value, OperationEnum op, bool And, bool replace);
	QString Escape(const QString& val);
private:
	struct filterCondition
	{
		QString fname;
		QString value;
		QString operation;
		QString AndOr;
	}f;
	//typedef struct filterCondition FilterCondition;
	Q3ValueList<filterCondition> conditions;
};

#endif
