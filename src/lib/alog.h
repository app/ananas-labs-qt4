/****************************************************************************
** $Id: alog.h,v 1.2 2008/11/08 20:16:35 leader Exp $
**
** Log functions header file of 
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

#ifndef ALOG_H
#define ALOG_H

#include	"ananasglobal.h"
#include 	<qstring.h>
#include 	<qfile.h>


/*!
 *	\~english
 *	Class contain static functions for log system
 *	\~russian
 *	\brief Класс содержит статические функции для записи сообщений в лог файл Ананаса.
 *	\~
*/
class ANANAS_EXPORT aLog
{
	public:
	enum MessageType { Error, Info, Debug };

	static void 	print( int status, const QString &text);
	static void 	debug( QString const &text);
	static void 	error( QString const &message);
	static void 	info( QString const &text);
	static void 	printr(	const QString &text);
	static void	init( const QString &logname="", int loglevel=0, bool echoToConsole = true);
	static void 	close();
	static QString 	getLogName();
	
	
	private:
	aLog();
	static bool createDotAnanasDir();
	static QString logName;
	static int logLevel;
	static QFile logFileRef;
	static bool msg_was_show;
	static bool initialized;	
	static bool echoRequired;
};
#endif// ALOG_H
