/****************************************************************************
** $Id: awindowslist.cpp,v 1.1 2008/11/05 21:16:28 leader Exp $
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

#include "awindowslist.h"
#include "acfg.h"

/*!
 *	\~english
 *	Constructor
 *	\~russian
 *	Конструктор.
 *	\~
 */
aWindowsList::aWindowsList()
    : QWidget()
{
}


/*!
 *	\~english
 *	Destructor
 *	\~russian
 *	Деструктор.
 *	\~
 */
aWindowsList::~aWindowsList()
{
}


/*!
 *	\~english
 *	Insert window into list.
 *	\~russian
 *	Добавляет ссылку на окно в список.
 *	\~
 *	\param id - \~english object class id \~russian идентификатор класса объекта \~
 *	\param window - \~english link to form \~russian ссылка на форму \~
 *	\param ido - \~english object id (default 0) \~russian идентификатор объекта (по умолчанию 0) \~
 */
void
aWindowsList::insert( int id, QWidget *window, qulonglong ido )
{
    list.insert( conv( id, ido ), window );
}

/*!
 *	\~english
 *	Remove window from list.
 *	\~russian
 *	Удаляет окно из списка.
 *	\~
 *	\param id - \~english object class id \~russian идентификатор класса объекта \~
 *	\param ido - \~english object id (default 0) \~russian идентификатор объекта (по умолчанию 0) \~
 */
void
aWindowsList::remove( int id, qulonglong ido )
{
    list.remove( conv( id, ido ) );
}

/*!
 *	\~english
 *	Remove window from list.
 *	\~russian
 *	Удаляет окно из списка.
 *	\~
 *	\param window - \~english link to form \~russian ссылка на форму \~
 */
void
aWindowsList::remove( QWidget *window )
{
    Q3DictIterator<QWidget> it( list );
    for ( ; it.current(); ++it )
    {
	if ( it.current() == window ) {
	    list.remove( it.currentKey() );
	    break;
	}
    }
}

/*!
 *	\~english
 *	Find window with \a id and \a ido.
 *	\~russian
 *	Ищет окно по его \a id и \a ido
 *	\~
 *	\param id - \~english object class id \~russian идентификатор класса объекта \~
 *	\param ido - \~english object id (default 0) \~russian идентификатор объекта (по умолчанию 0) \~
 *	\return \~english true, if window found \~russian true, если окно найдено. \~
 */
bool
aWindowsList::find( int id, qulonglong ido )
{
    if ( !list.find( conv( id, ido ) ) ) return FALSE;
    return TRUE;
}

/*!
 *	\~english
 *	Find object in list with link
 *	\~russian
 *	Ищет объект в списке по ссылке.
 *	\~
 *	\param window - \~english link to form \~russian ссылка на форму \~
 *	\return \~english true, if window found \~russian true, если окно найдено. \~
 */
bool
aWindowsList::find( QWidget *window )
{
    Q3DictIterator<QWidget> it( list );
    for ( ; it.current(); ++it ) if ( it.current() == window ) return TRUE;
    return FALSE;
}


/*!
 *	\~english
 *	Get window with \a id and \a ido.
 *	\~russian
 *	Функция получения ссылки на окно по его \a id и \a ido
 *	\~
 *	\param id - \~english object class id \~russian идентификатор класса объекта \~
 *	\param ido - \~english object id (default 0) \~russian идентификатор объекта (по умолчанию 0) \~
 *	\return \~english link to window, or 0 if window not found \~russian ссылка на окно или 0\~
 */
QWidget *
aWindowsList::get( int id, qulonglong ido )
{
    return list.find( conv( id, ido ) );
}


/*!
 *	\~english
 *	Create key for store in list.
 *	\~russian
 *	Создает ключ из параметров \a id и \a ido
 *	\~
 *	\param id - \~english object class id \~russian идентификатор класса объекта \~
 *	\param ido - \~english object id (default 0) \~russian идентификатор объекта (по умолчанию 0) \~
 *	\return \~english key \~russian ключ \~
 */
QString
aWindowsList::conv( int id, qulonglong ido )
{
    return QString("%1_%2").arg( id ).arg( ido );
}

