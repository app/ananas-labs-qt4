/****************************************************************************
** $Id: atests.h,v 1.1 2008/11/05 21:16:28 leader Exp $
**
** Tests functions header file of
** Ananas application library
**
** Created : 20051024
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

#ifndef ATESTS_H
#define ATESTS_H

#include	"ananas.h"
#include	<qmap.h>

/*!
 *	\~english
 *	Class contain static functions for test utilites
 *	\~russian
 *		\brief Класс содержит статические функции для утилит тестирования.
 *
 *		Предназначен для использования при отладке и тестировании кода.
 *	\~
*/
class ANANAS_EXPORT aTests
{
	public:

	static QMap<QString, QString>	readConfig(const QString &cfg_name, const QString &log_name=QString::null);
	static bool	writeConfig(const QString &cfg_name,QMap<QString, QString> cfg, const QString &log_name=QString::null);
	static void 	print2log(	const QString &log_name,
					const QString &test_name,
					const QString &status,
					const QString &text="");

	static void	printline2log(const QString &log_name);
	static QString 	parseCommandLine(int argc, char** argv, const QString requestedParam);

	private:
	aTests();
};
#endif// ATESTS_H
