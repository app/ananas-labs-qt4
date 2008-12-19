/****************************************************************************
** $Id: aootemplate.h,v 1.1 2008/11/05 21:16:29 leader Exp $
**
** Report metadata object header file of
** Ananas application library
**
** Created : 20050202
**
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
#ifndef AOOTEMPLATE_H
#define AOOTEMPLATE_H

#include <qstring.h>
#include <q3ptrlist.h>
#include <q3valuestack.h>
#include <qdom.h>
#include <QMap>

#define open_token "<:"
#define close_token ":>"
#define open_token_section "\\[:"
#define close_token_section ":\\]"
#define PARAM "param"

#include "ananasglobal.h"
#include "itemplate.h"

/**
 * \en
 * Class for work with pattern in OpenOffice format
 * \_en
 * \ru
 * \brief Класс определяет интерфейс для работы с шаблонами в формате OpenOffice. Наследует iTemplate.
 *
 * Описание шаблона: \n
 * Шаблон представляет собой обычный документ OpenOffice.
 * Он предназначен для того, чтобы быстро создавать отчеты.
 * Внешне шаблон отличается от того документа, который необходимо сгенерировать
 * только наличием специальных тегов.
 * Эти теги при генерации заменяются на конкретные значения. \n
 * Предусмотрено два типа тегов: \n
 * 1) Тег секции \n
 * С помощью него можно размножать строки таблицы.
 * Для этого надо просто записать его в любую ячейку строки. \n
 * Формат тега [:имя тега:] \n
 * В имени допустимы любые символы. \n
 * При вызове exec() с параметром = имени этого тега к таблице будет добавлена еще
 * одна строка. Если в этой строке есть простые теги,
 * то произойдет замена тега на значение, заданное с помощью функции setValue() \n
 * 2) Простой тег. \n
 * С помощью него можно вставлять в документ обычный текст, дату, и т.п. \n
 * формат тега <:имя тега:> \n
 * При вызове exec() с параметром = имени этого тега, рядом допишется значение,
 * предварительно заданное с помощью функции setValue() \n
 * Функция setValue() задает значения для подстановки.
 * Для прстых тегов надо задавать setValue("param",значение для подстановки);
 * после чего надо выполнить exec(имя простого тега) \n
 * Для тегов секции setValue(имя простого тега, значение для подстановки);
 * после чего выполнить exec(имя секции);
 * \_ru
 */
class ANANAS_EXPORT aOOTemplate : public iTemplate
{
public:
	aOOTemplate();
	virtual ~aOOTemplate();
	virtual bool open( const QString &fname );
	virtual void close();
	virtual void clear();
	virtual QString getValue( const QString &name );
	virtual void setValue( const QString &name, const QString &value );
	virtual QString exec( const QString &sname );
	virtual QString result();
	virtual bool save( const QString & fname );
	virtual void setDir( const QString &dirName );
	virtual QString getDir();
	virtual void cleanUpTags();
protected:
/**
 * \en
 * directory name for unzip
 * \_en
 * \ru
 * Имя директории для распаковки
 * \_ru
 */
    	QString copyName;
	protected:
/**
 * \en
 * directory template name
 * \_en
 * \ru
 * Имя директории шаблонов
 * \_ru
 */
	QString templateDir;

	QMap<QString,QString> values;

	void searchTags(QDomNode node, const QString &sname );
	virtual bool getNodeTags(QDomNode node, const QString &tagname, bool params );
	void insertRowValues(QDomNode node);
	virtual void insertTagsValues(QDomNode node, const QString &tagName);
	void clearTags(QDomNode node, bool section );
	void clearRow(QDomNode node);
/**
 * \en
 * 	Variable for store unpacking teplate content
 * \_en
 * \ru
 *	Переменная для хранения содержимого распакованного шаблона.
 * \_ru
 */
	QDomDocument docTpl;

/**
 * \en
 * 	Variable for store unpacking teplate stales
 * \_en
 * \ru
 *	Переменная для хранения стилей (колонтитулы в т.ч.) распакованного шаблона.
 * \_ru
 */
	QDomDocument docStyle;
};
#endif // AOOTEMPLATE_H
