/****************************************************************************
** $Id: atests.cpp,v 1.2 2008/11/08 20:16:35 leader Exp $
**
** Tests functions source file of
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

#include	"atests.h"
#include	"alog.h"
#include 	<qfile.h>
#include	<qdatetime.h>
#include 	<qdir.h>

/*!
 *\~english
 *	Writes text to log.
 *\~russian
 *	Пишет текст в лог.
 *\~
 *	\param log_name - \~english Full path to logfile \~russian Полный путь к лог файлу \~
 *	\param test_name - \~english Test name \~russian Имя теста \~
 *	\param status - \~english OK, ERROR or SKIP \~russian OK, ОШИБКА или ПРОПУЩЕН \~
 *	\param text - \~english Comment \~russian Комментарий \~
*/
void
aTests::print2log(	const QString &log_name,
			const QString &test_name,
			const QString &status,
			const QString &text)
{
	QString toWrite = QString("%1::%2::%3::%4\r\n")\
			  .arg(QDateTime::currentDateTime().toString(Qt::ISODate).replace('T',' '))\
			  .arg(test_name)\
			  .arg(status)\
			  .arg(text);

	QFile f;
	if(log_name==QString::null)
	{
		f.open( QIODevice::WriteOnly, stdout );
		f.writeBlock((const char*)toWrite,strlen((const char*)toWrite));
	}
	else
	{
		f.setName(log_name);
		f.open( QIODevice::WriteOnly | QIODevice::Append );
		f.writeBlock((const char*)toWrite,strlen((const char*)toWrite));
		f.flush();
	}
	f.close();

//	else printf("error write to log\n");
}


/*!
 *\~english
 *	Reads config into QMap object.
 *\~russian
 *	Считывает конфиг в объект QMap.
 *\~
 *	\param conf_name - \~english configfile name \~russian Путь к конфигу \~
 *	\param log_name - \~english logfile name \~russian Путь к логу (не обязательно) \~
 *	\return - \~english map with config values \~russian map со значениями конфига \~
*/
QMap<QString,QString>
aTests::readConfig(const QString &conf_name,const QString &log_name)
{
	QMap<QString,QString> map;
	QString str, buff;
	QStringList list;
	QFile f(conf_name);
	if(!f.exists())
	{
		aLog::print(aLog::Error, QObject::tr("aTests file %1 not exists").arg(f.name()));
	}
	else
	{
		if(!f.open( QIODevice::ReadOnly ))
		{
			aLog::print(aLog::Error, QObject::tr("aTests file %1 not open for read").arg(f.name()));
		}
		else
		{
			while(!f.atEnd())
			{
				str=f.readLine(1024);
				if(!str.isEmpty())
				//--if(f.readLine(str,1024)!=-1)
				{
					if(str==QString::null || str[0]=='#' || str[0]=='\n') continue;
					QString s = str.section("=",0,0);
					map[s] = (str.right(str.length() - s.length()-1)).stripWhiteSpace();
					aLog::print(aLog::Debug, QString("map[%1] = %2").arg(s).arg(map[s]));
				}
				else
				{
					break;
				}
			}
			f.close();
		}
	}
	return map;
}


/*!
 *\~english
 *	Writes config.
 *\~russian
 *	Пишет конфиг из объекта QMap.
 *\~
 *	\param conf_name - \~english configfile name \~russian Путь к конфигу \~
 *	\param log_name - \~english logfile name \~russian Путь к логу (не обязательно) \~
 *	\return - \~english true, if successful \~russian true - успех \~
*/
bool
aTests::writeConfig(const QString &conf_name, QMap<QString,QString> map, const QString &log_name)
{
	QString str;
	QFile f(conf_name);
	if(!f.exists())
	{
		aLog::print(aLog::Error, QObject::tr("aTests file %1 not exists").arg(f.name()));
	}
	//else
	//{
		if(f.open( QIODevice::WriteOnly ))
		{
			QMap<QString,QString>::Iterator it;
			for ( it = map.begin(); it != map.end(); ++it )
			{
				str= QString("%1=%2\n").arg(it.key()).arg(it.data());
				f.writeBlock((const char*)str,strlen((const char*)str));
				f.flush();
			}
			f.close();
			if(log_name!=QString::null)
			{
				aTests::print2log(log_name,conf_name,"OK","write config");
			}
		}
		else
		{
			aLog::print(aLog::Error, QObject::tr("aTests file %1 not open for read").arg(f.name()));
			return 0;
		}
	//}
	return 1;
}
/*!
 *\~english
 *	Writes line in log with name \a log_name
 *\~russian
 *	Пишет строку в лог с именем \a log_name
 *\~
 *	\param log_name - \~english logfile name \~russian Путь к логу (не обязательно) \~
*/
void
aTests::printline2log(const QString &log_name)
{
	QString toWrite;
	toWrite.fill('=',60);
	toWrite+="\n";
	QFile f;
	if(log_name==QString::null)
	{
		f.open( QIODevice::WriteOnly, stdout );
		f.writeBlock((const char*)toWrite,strlen((const char*)toWrite));
	}
	else
	{
		f.setName(log_name);
		f.open( QIODevice::WriteOnly | QIODevice::Append );
		f.writeBlock((const char*)toWrite,strlen((const char*)toWrite));
		f.flush();
	}
	f.close();
}

/*!
 *\~english
 *	Find parament in command line.
 *\~russian
 *	Ищет параметр с именем \a requestedParam в коммандной строке и если находит, то возвращает его
 *	значение (то, что идет после знака = до первого пробела). Заменяет знак ~ на $(HOME)
 *\~
 *	\param argc - \~english parametr command line count \~russian число параметров коммандной строки \~
 *	\param argv - \~english command line paraments \~russian параметры коммандной строки \~
 *	\param requestedParam - \~english param for search \~russian параметр для поиска \~
 *	\return - \~english parametr value \~russian значение параметра \~
*/
QString
aTests::parseCommandLine(int argc, char** argv, const QString requestedParam)
{

	QString param,value;
	for(int j=1;j<argc;j++)
	{
		param = argv[j];
		if(param.section("=",0,0).lower()==requestedParam)
		{
			if(param.section("=",1)!=QString::null)
			{
				value = param.section("=",1);
				if(value[0]=='~')
				{
					value = value.mid(1);
					value = QDir::convertSeparators(QDir::homeDirPath()+value);
				}
				return value;
			}
		}
	}
	return QString::null;
}
