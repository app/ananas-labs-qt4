/****************************************************************************
** $Id: areport.h,v 1.1 2008/11/05 21:16:29 leader Exp $
**
** Report metadata object header file of
** Ananas application library
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) 2003-2005 Grigory Panov <gr1313 at mail.ru>, Yoshkar-Ola.
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

#ifndef AREPORT_H
#define AREPORT_H

#include	<q3mainwindow.h>
#include	<q3textbrowser.h>
#include	<qworkspace.h>
#include	"acfg.h"
#include 	"aobject.h"
#include 	"engine.h"
#include 	"atemplate.h"
#include 	"aootemplate.h"

class aDatabase;

/**
 * \ru
 * 	\brief Класс для показа отчета в формате HTML. Наследует QMainWindow.
 *
 * 	\see aTemplate aReport
 * \_ru
 */
class  ANANAS_EXPORT aReportBrowser :public Q3MainWindow
{
	Q_OBJECT
public:
	QWorkspace *ws;
	Q3TextBrowser *textBrowser;
	aReportBrowser(  QWidget *parent = 0, const char *name = 0, Qt::WFlags f = Qt::WDestructiveClose );
	void append( const QString &text );
	void clear();
public slots:
	void print();

protected slots:
	virtual void languageChange();
};

/**
 * \ru
 * 	\brief Определяет программный интерфейс для работы со всеми типами отчетов.
 * 	Наследует aObject.
 *
 * 	Применяется для генерирования документов для печати в форматах OpenOffice *.odt,*ods и HTML.
 * 	\see aTemplate aOOTemplate
 * \_ru
 */

class  ANANAS_EXPORT aReport :public aObject
{
	Q_OBJECT
public:

/*!
 * \ru
 * Перечисление типов возможных отчетов.
 * \_ru
 */
	enum RT_type 	{
			RT_text/**отчет в формате HTML*/,
			RT_office_writer/**отчет в формате OpenOffice Writer*/,
			RT_office_calc/**отчет в формате OpenOffice Calc*/
			};

	aEngine *engine;
       	iTemplate *tpl;
	aReportBrowser *browser;
	RT_type type;

	aReport( aCfgItem context, RT_type r_type = RT_text, aEngine * e = 0 );
	aReport( QString name, RT_type r_type = RT_text, aEngine * e = 0 );
	virtual ~aReport();
	virtual ERR_Code initObject();

	QString path2workdir();

public slots:
	void setTable( const QString &name );
	void setValue( const QString &name, const QString &value );
	QString value( const QString &name );
	void clear();
	void exec( const QString &section );
	void close();
	void show();
protected:
	QString getName4NewTemplate();
private:
	QString tplName;

};

#endif// AREPORT_H
