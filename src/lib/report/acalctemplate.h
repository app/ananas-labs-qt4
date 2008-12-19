/****************************************************************************
** $Id: acalctemplate.h,v 1.1 2008/11/05 21:16:29 leader Exp $
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
#ifndef ACALCTEMPLATE_H
#define ACALCTEMPLATE_H

#include <qstring.h>
#include <q3ptrlist.h>
#include <q3valuestack.h>
#include <qdom.h>

#include "ananasglobal.h"
#include "aootemplate.h"

/**
 * \en
 * Class for work with pattern in OpenOffice Calc format
 * \_en
 * \ru
 * \brief Класс определяет интерфейс для работы с шаблонами в формате OpenOffice Calc.
 * \_ru
 */
class ANANAS_EXPORT aCalcTemplate : public aOOTemplate
{
public:
	aCalcTemplate();
	virtual ~aCalcTemplate();
protected:
	virtual	bool getNodeTags(QDomNode node, const QString &tagname, bool params );
	virtual void insertTagsValues(QDomNode node, const QString &tagName);
	QDomNode getCellNode(QDomNode node);
};
#endif // ACALCTEMPLATE_H
