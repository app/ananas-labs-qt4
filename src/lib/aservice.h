/****************************************************************************
** $Id: aservice.h,v 1.1 2008/11/05 21:16:28 leader Exp $
**
** Service functions header file of
** Ananas application library
**
** Created : 20050425
**
** Copyright (C) 2003-2005 Grigory Panov, Yoshkar-Ola.
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

#ifndef ASERVICE_H
#define ASERVICE_H

//#include	<qmainwindow.h>
#include	"ananas.h"

/*!
 * \~english
 *	Class contains static functions for various destination: convertation, work with config file etc.
 * \~russian
 *	\brief Класс содержит статические методы различного назначения: конвертации, работы с конфигом и т.д.
 *	Не наследует свойства других классов.
 *
 *	Врапперы для некоторых функций для работы со скриптами есть в классе aForm
 * \~
 *	\see aForm aTests
 */
class ANANAS_EXPORT aService
{
	public:

	static QString	number2money(	double rubli,
					bool need_kopeyki=true,
					bool male=true,
					const QString &end1="рублей",
					const QString &end2="рубль",
					const QString &end3="рубля");
	static QString	number2money( QString currency, double rubli );
	static QString	parts2money(	qulonglong rubli,
					unsigned int kopeyki,
					bool need_kopeyki=true,
					bool positive = true,
					bool male=true,
					const QString &end1="рублей",
					const QString &end2="рубль",
					const QString &end3="рубля");
	static QString	Date2Print(const QString &ISODate);
	static void	saveSize2Config(QRect size, const QString &mdname);
	static QRect	loadSizeFromConfig(const QString &mdname);
	static QString	readConfigVariable(const QString &name, bool *ok);
	static void	writeConfigVariable(const QString &name, const QString &value);
	static QString  convertNumber2MoneyFormat(double number);
	static bool copyFile(const QString& srcFileName, const QString& destFileName, bool replaceIfExists=true);
	protected:

	static QString part2string(	unsigned int st3,
					unsigned int st2,
					unsigned int st1,
					int stepen,
					bool male,
					const QString &end1,
					const QString &end2,
					const QString &end3);
	private:
	aService();
};
#endif// AREPORT_H
