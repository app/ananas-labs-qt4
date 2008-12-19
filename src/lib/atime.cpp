/****************************************************************************
** $Id: atime.cpp,v 1.1 2008/11/05 21:16:28 leader Exp $
**
** Engine applications
**
** Created : 20041109
**
** Copyright (C) 2003-2004 Grigory Panov, Yoshkar-Ola.
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

#include	"atime.h"
//#include	"adatabase.h"
//#include	"adocjournal.h"



/*!
 *\en
 *	Constructor that create object aTime for work in ananas script.
 *\_en
 *\ru
 *	Конструктор, который создает aTime для работы в ананас скрипте.
 *\_ru
 */
aTime::aTime(): QObject()
{
 	time = new QTime;

}

/*!
 *\en
 *	Destructor
 *\_en
 *\ru
 *	Деструктор
 *\_ru
 */
aTime::~aTime()
{
 	delete time;
}

/*!
 *\en
 *	Starts timer.
 *\_en
 *\ru
 *	Запускает таймер.
 *\_ru
 */
void
aTime::start()
{
	time->start();
}


/*!
 *\en
 *	Restarts timer.
 *\_en
 *\ru
 *	Перезапускает таймер.
 *\_ru
 */
int
aTime::restart()
{
	time->restart();
	return 0;
}

/*!
 *\en
 *	Gets elapsed time from last call of start() or restart()
 *\_en
 *\ru
 *	Возвращает количество миллисекунд с последнего запуска таймера.
 *\_ru
 */
int
aTime::elapsed()
{
	return time->elapsed();
}

/*!
 *\en
 *	Return current date for ananas.script
 *\_en
 *\ru
 *	Возвращает текущую дату в ISO формате. Используется для работы с Ананас.Скриптом
 *\_ru
 */
QString
aTime::CurrentDate()
{
	return QDate::currentDate().toString(Qt::ISODate);
}

