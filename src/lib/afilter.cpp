/****************************************************************************
** $Id: afilter.cpp,v 1.1 2008/11/05 21:16:28 leader Exp $
**
** Filter implementation file of
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

#include "afilter.h"
#include <stdio.h>
//Added by qt3to4:
#include <Q3ValueList>

/*!
 *	\~english
 *	Constructor
 *	\~russian
 *	Конструктор.
 *	\~
 */
aFilter::aFilter()
{
}

/*!
 *	\~english
 *	Destructor
 *	\~russian
 *	Деструктор.
 *	\~
 */
aFilter::~aFilter()
{
}

/*!
 *	\~english
 *	Dump filter conditions to stdout
 *	\~russian
 *	Выводит содержимое фильтра на stdout.
 *	\~
 */
void
aFilter::Dump() const
{
	printf("%s\n", toString().ascii());
}

/*!
 *	\~english
 *	Converts filter into its sql retpesentation. \a removeFirst give you ability to remove first AND or OR operator
 *	\~russian
 *	Переводит фильтр в его sql представление. Параметр \a removeFirst отвечает за удаление ведущего оператора AND или OR
 *	\~
 */
QString
aFilter::toString(bool removeFirst) const
{
	QString str = QString::null;
	Q3ValueList<filterCondition>::const_iterator it;
	for ( it = conditions.begin(); it != conditions.end(); ++it )
	{
		filterCondition flt = (*it);
		if(it != conditions.begin() && removeFirst)
		{
			str+= flt.AndOr + " ";
		}
		str+= flt.fname + " ";
		str+= flt.operation + " ";
		str+= flt.value + " ";
	}
	return str;
}


void
aFilter::Add(const QString& fname, const Q_INT64 value, OperationEnum op, bool AndOp, bool replace)
{
	AddHelper(fname, QString::number(value), op, AndOp, replace);
}
void
aFilter::Add(const QString& fname, const int value, OperationEnum op, bool AndOp, bool replace)
{
	AddHelper(fname, QString::number(value), op, AndOp, replace);
}
void
aFilter::Add(const QString& fname, const double value, OperationEnum op, bool AndOp, bool replace)
{
	AddHelper(fname, QString::number(value), op, AndOp, replace);
}
/*
void
aFilter::Add(const QString& fname, bool value, OperationEnum op, bool AndOp, bool replace)
{
	AddHelper(fname, value?"true":"false", op, AndOp, replace);
}
*/
void
aFilter::Add(const QString& fname, const QString& value, OperationEnum op, bool AndOp, bool replace)
{
	// need escape string
	AddHelper(fname, Escape(value), op, AndOp, replace);
}

void
aFilter::Add(const QString& fname, const char* value, OperationEnum op, bool AndOp, bool replace)
{
	// need escape string
	AddHelper(fname, Escape(QString(value)), op, AndOp, replace);
}

void
aFilter::Add(const QString& fname, const QDateTime& value, OperationEnum op, bool AndOp, bool replace)
{
	AddHelper(fname, Escape(value.toString(Qt::ISODate)), op, AndOp, replace);
}

void
aFilter::Add(const QString& fname, const QDate& value, OperationEnum op, bool AndOp, bool replace)
{
	AddHelper(fname, Escape(value.toString(Qt::ISODate)), op, AndOp, replace);
}


QString
aFilter::Escape(const QString& val)
{
	QString res = val;
	res.replace("\"","\"\"");
	res.replace("'","''");
	res.insert(0,"\"");
	res.append("\"");
	//printf("%s\n",res.ascii());
	return res;
}

void
aFilter::AddHelper(const QString& fname, const QString& value, OperationEnum op, bool AndOp, bool replace)
{
	filterCondition flt;
	flt.AndOr = AndOp?"AND":"OR";
	flt.fname = fname;
	flt.value = value;
	switch(op)
	{
	case OperationEnumEquals:
		flt.operation = "=";
		break;
	case OperationEnumNotEquals:
		flt.operation = "<>";
		break;
	case OperationEnumGreaterThen:
		flt.operation = ">";
		break;
	case OperationEnumLessThen:
		flt.operation = "<";
		break;
	case OperationEnumGreaterOrEquals:
		flt.operation = ">=";
		break;
	case OperationEnumLessOrEquals:
		flt.operation = "<=";
		break;
	case OperationEnumLike:
		flt.operation = "like";
		break;
	default:
		printf("boo! operation not exists!");
		break;

	}
	if(replace) // replace all entries, append to end list of conditions, if nothing found
	{
		Q3ValueList<filterCondition>::iterator it;
		bool isFound = false;
		for ( it = conditions.begin(); it != conditions.end(); ++it )
		{
			if((*it).fname == fname)
			{
				(*it) = flt;
				isFound = true;
			}
		}
		if(!isFound) conditions.append(flt);
	}
	else
	{
		conditions.append(flt);
	}

}

void
aFilter::Clear()
{
	conditions.clear();
}
