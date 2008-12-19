/****************************************************************************
** $Id: wdateedit.h,v 1.1 2008/11/05 21:16:30 leader Exp $
**
** Header file of the field plugin of Ananas
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

#ifndef WDATEEDIT_H
#define WDATEEDIT_H

#include <qlabel.h>
#include <q3datetimeedit.h>
//Added by qt3to4:
#include <QFocusEvent>

/*!
 * \en Unknown field editor. \_en
 * \ru
 * 	\brief Плагин-виджет для отображения в экранной форме поля типа Unknown.
 * 	Наследует QLabel.
 * \_ru
 */
class wUnknownField : public QLabel
{
	Q_OBJECT
public:
    wUnknownField( QWidget *parent = 0, const char *name = 0):QLabel(parent, name){};

};


/*!
 * \en DateEdit field editor. \_en
 * \ru
 * 	\brief Плагин -- виджет используется в экранных формах в качестве поля ввода/редактирования
 * 	атрибута типа Дата бизнес объекта Ананаса.
 * 	Наследует QDateEdit.
 *
 * \_ru
 */
class wDateEdit : public Q3DateEdit
{
	Q_OBJECT
public:
	wDateEdit(QWidget* parent = 0, const char* name = 0);
	wDateEdit(const QDate &date,
			QWidget* parent = 0,
			const char* name = 0 );
	~wDateEdit();
protected:
	void focusOutEvent ( QFocusEvent * );
signals:
	void lostFocus();
};

#endif
