/****************************************************************************
** $Id: itemplate.h,v 1.1 2008/11/05 21:16:29 leader Exp $
**
** Report metadata object header file of
** Ananas application library
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
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

#ifndef ITEMPLATE_H
#define ITEMPLATE_H

#include "ananasglobal.h"

#include <qobject.h>
//#include <qdict.h>
//#include <qstringlist.h>

/**
 * \~english
 * Interface for work with report
 * \~russian
 * \brief Интерфейс для работы с отчетами. Наследует QObject.
 *
 * Классы, генерирующие отчеты, должны использовать этот интерфейс.
 * Содержит чистые виртуальные функции, не допускается создание объектов этого класса напрямую.
 * Пример:
 * \code
 * iTemplate *tpl = new aTemplate(); // aTemplate должен реализовывать интерфейс iTemplate
 * tpl->open("test.tpl");
 * tpl->close();
 * delete tpl;
 * \endcode
 * \~
 */
class  ANANAS_EXPORT iTemplate : public QObject
{
    Q_OBJECT
public:
/**
 * \en
 * Constructor
 * \_en
 * \ru
 * Конструктор.
 * \_ru
 */
    iTemplate();
/**
 * \en
 * Destructor
 * \_en
 * \ru
 * Деструктор
 * \_ru
 */
    virtual ~iTemplate();

/**
 * \en
 * Opens pattern
 * \_en
 * \ru
 * Открывает шаблон.
 * \_ru
 */
    virtual bool open( const QString &fname )=0;
/**
 * \en
 * Closes pattern.
 * \_en
 * \ru
 * Закрывает шаблон.
 * \_ru
 */
    virtual void close();
/**
 * \en
 * Clears internal variable.
 * \_en
 * \ru
 * Обнуляет внутренние переменные.
 * \_ru
 */
    virtual void clear();
/**
 * \en
 * Get value of parameter `name'
 * \_en
 * \ru
 * Возвращает значение параметра по его имени.
 * \_ru
 */
    virtual QString getValue( const QString &name );
/**
 * \en
 * Set value of parameter `name'
 * \_en
 * \ru
 * Устанавливает значение параметра по его имени.
 * \_ru
 */
    virtual void setValue( const QString &name, const QString &value );
/**
 * \en
 * Replaces tads to values
 * \_en
 * \ru
 * Выполняет подстановку значения параметра в шаблоне.
 * Может вызываться несколько раз.
 * \_ru
 */
    virtual QString exec( const QString &sname );
/**
 * \en
 * Returns document.
 * \_en
 * \ru
 * Возвращает результат, т.е. шаблон в текстовом виде. Включена в интерфейс временно.
 * \_ru
 */
    virtual QString result();
/**
 * \en
 * Saves document to file
 * \_en
 * \ru
 * Сохраняет документ в файл с заданным именем.
 * \_ru
 */
    virtual bool save( const QString & fname );
/**
 * \en
 * Clears all tags
 * \_en
 * \ru
 * Очищает все теги
 * \_ru
 */

    virtual void cleanUpTags();
/**
 * \en
 * Sets  working directory
 * \_en
 * \ru
 * Устанавливает рабочую директорию
 * \_ru
 */
    virtual void setDir(const QString &dir);

 /**
 * \en
 * Gets  working directory
 * \_en
 * \ru
 * Возвращает рабочую директорию
 * \_ru
 */
    virtual QString getDir();
};

#endif //ITEMPLATE_H
