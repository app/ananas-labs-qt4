/****************************************************************************
** $Id: awindowslist.h,v 1.1 2008/11/05 21:16:28 leader Exp $
**
** Main window of Ananas Designer applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
**
** This file is part of the Designer application  of the Ananas
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

#ifndef AWINDOWSLIST_H
#define AWINDOWSLIST_H

#include "ananasglobal.h"


#include <qwidget.h>
#include <q3dict.h>

/*!
 * \~english
 *	Contains list of opened window.
 * \~russian
 *	\brief Класс определяет интерфейс управления списком открытых окон.
 *	Наследует QWidget.
 *
 *	Объект класса содержит ссылки на все открытые окна. При открыти окна проверяется, есть ли оно в списке,
 *	и если есть, то ему просто передается фокус.
 * \~
 */
class  ANANAS_EXPORT aWindowsList : public QWidget
{
    Q_OBJECT
public:
    aWindowsList();
    virtual ~aWindowsList();

    void insert( int id, QWidget *window, qulonglong ido = 0 );
    void remove( int id, qulonglong ido = 0 );
    void remove( QWidget *window );
    bool find( int id, qulonglong ido = 0 );
    bool find( QWidget *window );
    QWidget *get( int id, qulonglong ido = 0 );

private:
    Q3Dict<QWidget> list;
    QString conv( int id, qulonglong ido );
};


#endif //AWINDOWSLIST_H
