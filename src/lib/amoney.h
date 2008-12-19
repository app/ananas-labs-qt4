/****************************************************************************
** $Id: amoney.h,v 1.1 2008/11/05 21:16:28 leader Exp $
**
** Service functions header file of
** Ananas application library
**
** Copyright (C) 2007 Andrey Paskal.
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
**
**********************************************************************/
#ifndef AMONEY_H_
#define AMONEY_H_
#include	"ananas.h"
#include	"qstring.h"

class Degree;
class Currency;

/**
 * \ru
 * 		\brief	Класс для хранения денежных значений в различной валюте и получения их прописного представления.
 * \_ru
 */
class ANANAS_EXPORT AMoney{
	public:
		AMoney();
		AMoney( double amount, QString currency = "RUR");

		double 	getAmount();
		void 	setAmount( double );
		void 	setAmount( double,  QString currency);
		QString getCurrency();
		void 	setCurrency( QString );
		QString toString();
		QString toText();
		bool 	equal( AMoney &sum );

	private:
		double 		amount;
		Currency*	currency;
		Degree* 	degree;

		QString degreeText( QString , int );
		QString valueToText( qulonglong );
		QString decimalValueToText( qulonglong );
		QString singleToText( QString  );
		QString coupleToText( QString  );
		QString tripleToText( QString  );
		QString integerCurrencyName( QString );
		QString decimalCurrencyName( QString );
		QString firstUp( QString );

};



/**
 * \ru
 * 		\brief	содержит знания о роде (М/Ж) числовых порядков.
 * 			После задания порядка (сотни, тысячи, миллионы....)  можно узнать род
 * 			методом isMale()
 * \_ru
 */
class Degree {
	public:
		Degree( int = 0 );
		int getValue();
		void setValue( int value);
		bool isMale();

	private:
		int value;
		bool male;
};

/**
 * \ru
 * 		\brief	содержит знания о роде (М/Ж) валюты.
 * 			После задания одного из поддерживаемых типов валюты можно узнать род
 * 			методом isMale()
 * 		Поддерживаемые типы валют можно увидеть в описании конструктора.
 * \_ru
 */
class Currency {
	public:
		Currency( QString currensyId = "RUR");

		QString	getId();
		void 	setId( QString );
		bool 	isMale();
		bool	is( QString );

	private:
		QString id;
		bool 	male;
};


#endif /*AMONEY_H_*/
