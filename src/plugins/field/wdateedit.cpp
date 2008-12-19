/****************************************************************************
** $Id: wdateedit.cpp,v 1.1 2008/11/05 21:16:30 leader Exp $
**
** Code file of the field plugin of Ananas
** Designer and Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Grigory Panov, Yoshkar-Ola.
**
** This file is part of the Ananas Plugins of the Ananas
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

#include <q3datetimeedit.h>
//Added by qt3to4:
#include <QFocusEvent>
#include "wdateedit.h"



/*!
 * \en	Constructor. \_en
 * \ru	Конструктор. \_ru
 */
wDateEdit::wDateEdit(QWidget* parent, const char* name ):Q3DateEdit(parent,name)
{}


/*!
 * \en	Constructor with setting date. \_en
 * \ru	Конструктор с инициированием даты. \_ru
 * \param date - \en date for set \_en \ru Дата для установки \_ru
 */
wDateEdit::wDateEdit(const QDate &date,
		     QWidget* parent,
		     const char* name) : Q3DateEdit(date,parent,name)
{}



/*!
 * \en 	Destructor \_en
 */
wDateEdit::~wDateEdit()
{
	//QDateEdit::~QDateEdit();
}


/*!
 * \en 	Focus out event handler. Emit signal lostFocus. \_en
 * \ru 	Обработчик сигнала выхода из фокуса. Испускает сигнал lostFocus. \_ru
 */
void
wDateEdit::focusOutEvent ( QFocusEvent * e)
{
	emit(lostFocus());
	QWidget::focusOutEvent(e);
}


