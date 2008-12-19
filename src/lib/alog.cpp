/****************************************************************************
** $Id: alog.cpp,v 1.2 2008/11/08 20:16:35 leader Exp $
**
** Log functions source file of 
** Ananas application library
**
** Created : 20060402
**
** Copyright (C) 2006 Grigory Panov, Yoshkar-Ola.
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

#include	"alog.h"
#include 	<qfile.h>
#include	<qdir.h>
#include	<qdatetime.h>
#include 	"aservice.h"


QString 	aLog::logName = "";
int		aLog::logLevel = aLog::Debug;
QFile		aLog::f;
bool		aLog::msg_was_show = false;
//const int	aLog::Error = 0;
//const int	aLog::Info = 1;
//const int	aLog::Debug = 2;


/*!
 *\~english
 *	Writes text to log, if /a status less init status.
 *	\param status - message status  ( aLog::Error, aLog::Info, aLog::Debug )
 *	\param text - message 
 *\~russian
 *	Пишет текст в лог, если статус сообщения меньше установленного при инициализации. К тексту добавляется дата и статус.
 *	\param status - статус сообщения  ( aLog::Error, aLog::Info, aLog::Debug ). 
 *	\param text - сообщение 
 *\~
*/
void
aLog::print(	int status,
		const QString &text)
{
	QString st;
//	switch(status)
//	{
//		case aLog::Error: 
		if(status == aLog::Error) st = "ERROR";
//		break;
//		case aLog::Info: 
		else if(status == aLog::Info) st = "INFO";
//		break;
//		default: 
		else st = "DEBUG";
//		break;
//	}
	if(status<=logLevel)
	{
	QString endline;
	
#ifdef Q_OS_WIN32
	endline = "\r\n";
#else
	endline = "\n";
#endif
		const QString toWrite = QString("%1 %2 %3%4")\
				  .arg(QDateTime::currentDateTime().toString(Qt::ISODate).replace('T',' '))\
				  .arg(st)\
				  .arg(text)\
				  .arg(endline);
		aLog::printr(toWrite);	
	}
}

/*!
 *\~english
 *	Writes raw text to log.
 *\~russian
 *	Пишет текст в лог без изменения.	
 *\~
 *	\param text - \~english text \~russian текст \~
*/
void
aLog::printr(	const QString &toWrite)
{
	if(aLog::f.handle () !=-1)
	{
		aLog::f.writeBlock((const char*)toWrite.local8Bit(),strlen((const char*)toWrite.local8Bit()));
		aLog::f.flush();
	}
	else
	{
		if(!aLog::msg_was_show)
		{
			printf("Log system not initialized!\n");
			aLog::msg_was_show = true;
		}
	}
}


/*!
 *\~english
 *	Inits log system.
 *\~russian
 *	Инициализирует систему логирования. Выполняется один раз при запуске програмы, если параметры не указаны, осуществляется попытка прочитать из из конфигурационного файла. Если это не удается, используются параметры по умолчанию.
 *\~
 *	\param log_name - \~english logfile name. Default ~/.ananas.ananas.log \~russian Путь к логу. Значение по умолчанию ~/.ananas/ananas.log \~
 *	\param status - \~english showing status  \~russian статус скрытия сообщений. Все сообщения, имеющие больший статус, не попадут в лог ( ERROR= 0, INFO= 1, DEBUG= 2 ). Значение по умолчанию DEBUG \~
*/
void
aLog::init(const QString &log_name, int show_up)
{
	bool ok;
	QString log;
	if(log_name =="" || logName==QString::null)
	{
		log = aService::readConfigVariable("log", &ok);	
		if(!ok) log = QDir::homeDirPath()+"/.ananas/ananas.log";
	}
	else
	{
		log = log_name;
	}
	
	aLog::logLevel = show_up;
	const QString logLev = aService::readConfigVariable("logLevel", &ok);
	
	if(logLev=="ERROR") aLog::logLevel = aLog::Error;
	if(logLev=="INFO") aLog::logLevel = aLog::Info;
	if(logLev=="DEBUG") aLog::logLevel = aLog::Debug;
	
	aLog::logName = log;

	if (aLog::f.isOpen()) f.close();	
	aLog::f.setName(getLogName());
	aLog::f.open( IO_WriteOnly | IO_Append );

}

QString
aLog::getLogName()
{
	return aLog::logName;
}

void 
aLog::close()
{
	aLog::f.close();
}

