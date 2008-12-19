/****************************************************************************
** $Id: aservice.cpp,v 1.2 2008/11/08 20:16:35 leader Exp $
**
** Service functions source file of
** Ananas application library
**
** Created : 20050425
**
** Copyright (C) 2003-2006 Grigory Panov, Yoshkar-Ola.
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

#include	"qsettings.h"
#include	"aservice.h"
#include 	"alog.h"
#include	"amoney.h"
#include 	<qdir.h>
#include	<math.h>


/*!
 *\~english
 *	Converted double currency value to string representation (ru currency).
 *\~russian
 *	Конвертирует валюту, записанную в виде десятичного числа в ее текстовое представление.
 *\~
 *	\param rubli - \~english Value to convertions \~russian  Значение для конвертации \~
 *	\return - \~english Converted value \~russian Сконвертированное значение \~
 *	\see parts2money(Q_ULLONG rubli, unsigned int kopeyki, bool positive)
 *	\see part2string(unsigned int st3, unsigned int st2, unsigned int st1, int stepen)
 *	\see convertNumber2MoneyFormat(double number)
*/
QString
aService::number2money(	double rubli,
			bool need_kopeyki,
			bool male,
			const QString &end1,
			const QString &end2,
			const QString &end3)
{
	bool neg = rubli<0;
	//printf("chislo=%f\n",rubli);
	if(neg)
	{
		rubli = -rubli;
	}
	qulonglong r = QVariant(rubli).toULongLong();
	double tmp = rubli-r;

	uint kop;
#ifdef Q_OS_UNIX
	kop = round(tmp*100.0);
#else
	kop = tmp * 100.0;
#endif
	return parts2money(r,kop, need_kopeyki, !neg, male, end1, end2, end3);
}


/**
 * \ru
 * 		\brief	Конвертирует числовое представление денег в пропись с учетом указанной валюты.
 * 		\param amount - сумма денег для преобразование в пропись.
 * 		\param currency - строка с обозначением валюты. Допустимые значения: "RUR" - рубль, "USD" - доллар, "EUR" - евро.
 * 		\returns значение суммы прописью.
 * \_ru
 */
QString
aService::number2money(	 QString currency, double amount ) {
	AMoney* money = new AMoney( amount, currency );
	return money->toText();
}



/*!
 *\~english
 *	Converted currency value to string representation (ru currency).
 *\~russian
 *	Конвертирует валюту, записанную в виде рублей и копеек в ее текстовое представление.
 *\~
 *	\return - \~english Converted value \~russian Сконвертированное значение \~
 *	\see number2money(double rubli)
 *	\see part2string(unsigned int st3, unsigned int st2, unsigned int st1, int stepen)
*/
QString
aService::parts2money(qulonglong rubli,
			 unsigned int kopeyki,
			 bool need_kopeyki,
			 bool positive,
			 bool male,
			 const QString &end1,
			 const QString &end2,
			 const QString &end3)
{
//	if(cur!=ru) return QString("%1 dollars %2 cents").arg(rubli).arg(kopeyki);
	QString chislo = (QString("%1").arg(rubli));
	int len = chislo.length();
	//printf("length=%d\n",len);
	int count=0;
	int a,b,c;
	int stepen=(len-1)/3;
	int offset=(len%3);
	if(offset) offset = 3-offset;
	//printf("offset=%d\n",offset);
	QString res = "";
	bool has_sum=false;
	if(!positive) res+="минус ";
	while(count<len)
	{
		a=b=c=0;
		if(offset<=0)
		{
			a = chislo.at(count++).digitValue();
		}
		if(count<len)
		{
			if(offset<=1)
			{
				b = chislo.at(count++).digitValue();
			}
		}
		if(count<len)
		{
			if(offset<=2)
			{
				c = chislo.at(count++).digitValue();
			}
		}
//		printf("a=%d, b=%d, c=%d, stepen=%d\n",a,b,c,stepen);
		if(a==0 && b==0 && c==0)
		{
			if(stepen==0 && has_sum)
			{
				res+=part2string(a,b,c,stepen--,male,end1,end2,end3);
			}
			else
			{
				if(stepen==0)
				{
					res+=QString("ноль %1").arg(end1);
				}
				else
				{
					stepen--;
				}
			}
		}
		else
		{
			has_sum=true;
			res+=part2string(a,b,c,stepen--,male,end1,end2,end3);
		}
		offset=0;
	}
	if(need_kopeyki)
	{
		res+=part2string(0,kopeyki/10,kopeyki%10,-1,false,"копеек","копейка","копейки");
	}
	res = res.stripWhiteSpace();
	res = res.replace(0,1,res.at(0).upper());
	return res;
}


/*!
 *\~english
 *	Convertions implementation.
 *\~russian
 *	Реализует конвертацию в рубли и копейки. На вход подаются три числа и степень,
 *	в которую необходимо их возвести. Поддерживается суммы до 999 миллиардов.
 *	Для использования необходимо число разбить на группы чисел по 3 числа в группе и
 *	передать их этой функции, после чего собрать результат в одну строку.
 *	Функция добавляет в конце строки один пробел
 *\~
 *	\return - \~english Converted value \~russian Сконвертированное значение для переданных чисел\~
 *	\see number2money(double rubli)
 *	\see parts2money(Q_ULLONG rubli, unsigned int kopeyki, bool positive)
*/
QString
aService::part2string(unsigned int st3,
			 unsigned int st2,
			 unsigned int st1,
			 int stepen,
			 bool male,
			 const QString &end1,
			 const QString &end2,
			 const QString &end3 )
{
	QString part1, part2, part3, suff;
	QMap<int, QString> mapMaleCur;
	QMap<int, QString> mapFeMaleCur;
	QMap<int, QString> mapHungred;
	QMap<int, QString> mapDec;

	mapFeMaleCur[0]="";
	mapFeMaleCur[1]="одна";
	mapFeMaleCur[2]="две";
	mapFeMaleCur[3]="три";
	mapFeMaleCur[4]="четыре";
	mapFeMaleCur[5]="пять";
	mapFeMaleCur[6]="шесть";
	mapFeMaleCur[7]="семь";
	mapFeMaleCur[8]="восемь";
	mapFeMaleCur[9]="девять";

	mapFeMaleCur[10]="копеек";
	mapFeMaleCur[11]="копейка";
	mapFeMaleCur[12]= mapFeMaleCur[13]= mapFeMaleCur[14]="копейки";
	mapFeMaleCur[15]= mapFeMaleCur[16]= mapFeMaleCur[17]= mapFeMaleCur[18]=	mapFeMaleCur[19]="копеек";

	mapFeMaleCur[20]="тысяч";
	mapFeMaleCur[21]="тысяча";
	mapFeMaleCur[22]= mapFeMaleCur[23]= mapFeMaleCur[24]="тысячи";
	mapFeMaleCur[25]= mapFeMaleCur[26]= mapFeMaleCur[27]= mapFeMaleCur[28]= mapFeMaleCur[29]="тысяч";

	mapMaleCur[0]="";
	mapMaleCur[1]="один";
	mapMaleCur[2]="два";
	mapMaleCur[3]="три";
	mapMaleCur[4]="четыре";
	mapMaleCur[5]="пять";
	mapMaleCur[6]="шесть";
	mapMaleCur[7]="семь";
	mapMaleCur[8]="восемь";
	mapMaleCur[9]="девять";

	if(male==true)
	{
		mapMaleCur[10]=end1; //"рублей";
		mapMaleCur[11]=end2;//"рубль";
		mapMaleCur[12]=	mapMaleCur[13]=	mapMaleCur[14]=end3;//"рубля";
		mapMaleCur[15]= mapMaleCur[16]=	mapMaleCur[17]=	mapMaleCur[18]=	mapMaleCur[19]=end1;//"рублей";
	}
	else
	{
		mapFeMaleCur[10]= end1;//"копеек";
		mapFeMaleCur[11]= end2;//"копейка";
		mapFeMaleCur[12]= mapFeMaleCur[13]= mapFeMaleCur[14]=end3;//"копейки";
		mapFeMaleCur[15]= mapFeMaleCur[16]= mapFeMaleCur[17]= mapFeMaleCur[18]=	mapFeMaleCur[19]=end1;//"копеек";
	}
	mapMaleCur[30]="миллионов";
	mapMaleCur[31]="миллион";
	mapMaleCur[32]=	mapMaleCur[33]=	mapMaleCur[34]="миллиона";
	mapMaleCur[35]= mapMaleCur[36]=mapMaleCur[37]=mapMaleCur[38]=mapMaleCur[39]="миллионов";
	mapMaleCur[40]="миллиардов";
	mapMaleCur[41]="миллиард";
	mapMaleCur[42]=	mapMaleCur[43]=	mapMaleCur[44]="миллиарда";
	mapMaleCur[45]= mapMaleCur[46]=mapMaleCur[47]=mapMaleCur[48]=mapMaleCur[49]="миллиардов";
	mapHungred[0]="";
	mapHungred[1]="сто";
	mapHungred[2]="двести";
	mapHungred[3]="триста";
	mapHungred[4]="четыреста";
	mapHungred[5]="пятьсот";
	mapHungred[6]="шестьсот";
	mapHungred[7]="семьсот";
	mapHungred[8]="восемьсот";
	mapHungred[9]="девятьсот";

	mapDec[0]="десять";
	mapDec[1]="одиннадцать";
	mapDec[2]="двенадцать";
	mapDec[3]="тринадцать";
	mapDec[4]="четырнадцать";
	mapDec[5]="пятнадцать";
	mapDec[6]="шестнадцать";
	mapDec[7]="семнадцать";
	mapDec[8]="восемнадцать";
	mapDec[9]="девятнадцать";
	mapDec[10]="";
	mapDec[11]="";
	mapDec[12]="двадцать";
	mapDec[13]="тридцать";
	mapDec[14]="сорок";
	mapDec[15]="пятьдесят";
	mapDec[16]="шестьдесят";
	mapDec[17]="семьдесят";
	mapDec[18]="восемьдесят";
	mapDec[19]="девяносто";


	if(stepen==1 || stepen==-1 || stepen==0 && male==false)
	{
		part1 = mapHungred[st3];
		part2 = mapDec[10+st2];
		int mult = 20;
		if(stepen==-1) mult=10;
		if(stepen==0) mult=10;
		switch(st2)
		{
		case 0:
			part3=mapFeMaleCur[st1];
			suff=mapFeMaleCur[st1+mult];
			break;
		case 1:
			part3=mapDec[st1];
			suff=mapFeMaleCur[9+mult];
			break;
		default:
			part3=mapFeMaleCur[st1];
			suff=mapFeMaleCur[st1+mult];
			break;
		}
		if(stepen==-1 && st3==0 && st2==0 && st1==0)
		{
			part3="ноль";
		}
	}
	else
	{
		part1 = mapHungred[st3];
		part2 = mapDec[10+st2];
		int mult = 10*(stepen+1);
		switch(st2)
		{
		case 0:
			part2="";
			part3=mapMaleCur[st1];
			suff=mapMaleCur[st1+mult];
			break;
		case 1:
			part2="";
			part3=mapDec[st1];
			suff=mapMaleCur[9+mult];
			break;
		default:
			part3=mapMaleCur[st1];
			suff=mapMaleCur[st1+mult];
			break;
		}
	}
	if(stepen==-1) // если убрать этот блок, копейки будут выводиться прописью
	{
		part1 = part2 ="";
		part3 = QString("%1%2").arg(st2).arg(st1);
	}
return part1+" "+part2 +" "+part3+" "+suff+" ";
}


/*!
 *\~english
 *	Converted date from ISO format (yyyy-mm-dd) to local format.
 *\~russian
 *	Конвертирует дату из ISO формата в локальный формат.
 *\~
 *	\param ISODate - \~english Date to convertion \~russian  Дата для конвертации \~
 *	\return - \~english Converted value \~russian Сконвертированное значение \~
*/
QString
aService::Date2Print(const QString &ISODate)
{
	QString res;
	QDate dat = QDate::fromString(ISODate, Qt::ISODate);
	res = dat.toString(Qt::LocalDate);
	return res;
}


/*!
 *\~english
 *	Writes window size to config file.
 *\~russian
 *	Пишет размер окна в конфиг.
 *\~
 *	\param windowSize - \~english Rect of window geometry \~russian Прямоугольник, представляющий окно \~
 *	\param mdname - \~english Unical name \~russian Имя окна (должно быть уникальным) \~
 *	\see loadSizeFromConfig(const QString &mdname)
*/
void
aService::saveSize2Config(QRect windowSize, const QString &mdname)
{
	QSettings settings;
	//--settings.insertSearchPath( QSettings::Unix, QString(QDir::homeDirPath())+QString("/.ananas"));
	//--settings.insertSearchPath( QSettings::Windows, "/ananasgroup/ananas" );
	settings.beginGroup(QString("/config/%1").arg(mdname));
	settings.writeEntry("/left", windowSize.left());
	settings.writeEntry("/top", windowSize.top());
	settings.writeEntry("/right", windowSize.right());
	settings.writeEntry("/bottom", windowSize.bottom());
}


/*!
 *\~english
 *	Reads window size from config file.
 *\~russian
 *	Читает размер окна из конфиг.
 *\~
 *	\param mdname - \~english 	Window name.
 *					If config not contains info about mdname,
 *					return default value (400x300)
 *			\~russian 	Имя окна.
 *					Если конфиг не содержит информации об окне с таким именем,
 *					возвращает значение по умолчанию (400x300) \~
 *	\return - \~english Window size \~russian Размер окна \~
 *	\see saveSize2Config(QRect windowSize, const QString &mdname)
 *
*/
QRect
aService::loadSizeFromConfig(const QString &mdname)
{
	QSettings settings;
	//--settings.insertSearchPath( QSettings::Unix, QString(QDir::homeDirPath())+QString("/.ananas"));
	//--settings.insertSearchPath( QSettings::Windows, "/ananasgroup/ananas" );
	settings.beginGroup(QString("/config/%1").arg(mdname));
	int l = settings.readNumEntry("/left", 0);
	int t = settings.readNumEntry("/top", 0);
	int r = settings.readNumEntry("/right", 400);
	int b = settings.readNumEntry("/bottom", 300);
	return QRect(l,t,r,b);
}


/*!
 *\~english
 *	Reads variable from config file.
 *\~russian
 *	Читает переменную из конфига.
 *\~
 *	\param name - \~english variable name to read \~russian Имя переменной для чтения \~
 *	\param ok (out) -	\~english true if success, false otherwise
 				\~russian true если успешно иначе false \~
 *	\return - \~english Variable value \~russian Значение переменной \~
 *	\see writeConfigVariable(const QString &name, const QString &value)
 *	\see loadSizeFromConfig(const QString &mdname)
 *	\see saveSize2Config(QRect windowSize, const QString &mdname)
*/
QString
aService::readConfigVariable(const QString &name, bool *ok)
{
	QSettings settings;
	//--settings.insertSearchPath( QSettings::Unix, QString(QDir::homeDirPath())+QString("/.ananas"));
	//--settings.insertSearchPath( QSettings::Windows, "/ananasgroup/ananas" );
	settings.beginGroup(QString("/config/variables"));
	return settings.readEntry(QString("/%1").arg(name), "", ok);
}


/*!
 *\~english
 *	Writes variable to config file.
 *\~russian
 *	Пишет переменную в конфиг.
 *\~
 *	\param name - \~english Variable name \~russian Имя переменной \~
 *	\param value - \~english Variable value \~russian Значение переменной \~
 *	\see loadSizeFromConfig(const QString &mdname)
 *	\see saveSize2Config(QRect windowSize, const QString &mdname)
 *	\see readConfigVariable(const QString &name, bool *ok)
*/
void
aService::writeConfigVariable(const QString &name, const QString &value)
{
	QSettings settings;
	//--settings.insertSearchPath( QSettings::Unix, QString(QDir::homeDirPath())+QString("/.ananas"));
	//--settings.insertSearchPath( QSettings::Windows, "/ananasgroup/ananas" );
	settings.beginGroup(QString("/config/variables"));
	settings.writeEntry(QString("/%1").arg(name), value);
}

/*!
 *\~english
 *	Converted number to money format (publi.kk).
 *\~russian
 *	Конвертирует число с плавающей точкой в числовой денежный формат (рубли.кк)
 *\~
 *	\param number - \~english number to convertion \~russian число для конвертации \~
 *	\return - \~english Converted value \~russian Сконвертированное значение \~
*/
QString
aService::convertNumber2MoneyFormat(double number)
{
	return QString("%1").arg(number,0,'f',2);
}

bool
aService::copyFile(const QString& srcFileName, const QString& destFileName, bool replaceIfExists)
{

	QFile srcFile(srcFileName);
	QFile destFile(destFileName);

	if(!srcFile.exists())
	{
		aLog::print(aLog::Error, QObject::tr("aService copy: source file %1 not exist").arg(srcFileName));
		return false;
	}
	if(destFile.exists() && !replaceIfExists)
	{
		aLog::print(aLog::Error, QObject::tr("aService copy: replace destination file %1").arg(destFileName));
		return false;
	}

	if(!srcFile.open( QIODevice::ReadOnly ))
	{
		aLog::print(aLog::Error, QObject::tr("aService copy: source file %1 open for read error").arg(srcFileName));
		return false;
	}
	if(!destFile.open( QIODevice::WriteOnly))
	{
		aLog::print(aLog::Error, QObject::tr("aService copy: destination file %1 open for write error").arg(destFileName));
		return false;
	}

	const int BUFFER_SIZE = 1024;
	Q_INT8 buffer[BUFFER_SIZE];
	QDataStream srcStream(&srcFile);
	QDataStream destStream(&destFile);

	while(!srcStream.atEnd())
	{
		int i = 0;
		while(!srcStream.atEnd() && i < BUFFER_SIZE)
		{
			srcStream >> buffer[i];
			i++;
		}
		for(int k = 0; k < i; k++)
		{
			destStream << buffer[k];
		}
	}
	srcFile.close();
	destFile.close();

	aLog::print(aLog::Info, QObject::tr("aService copy file %1 to %2 ok").arg(srcFileName).arg(destFileName));
	return true;
}
