/****************************************************************************
** $Id: aextguid.h,v 1.1 2009/05/24 18:34:42 app Exp $
**
** Header file of the Report Result Object of Ananas
** Designer and Engine applications
**
** Created : 20070819
**
** Copyright (C) 2005-2007 Grigory Panov <grigory.panov at gmail.com>, Moscow.
** Copyright (C) 2005-2007 Ananas Project.
**
** This file is part of the Library of the Ananas
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
#ifndef AEXTGUID_H
#define AEXTGUID_H

#include "aextension.h"
#include "aobject.h"

/**
 * \en
 * 	Class for generate GUID strings from Ananas Script.
 * \_en
 * \ru
 *	\brief Генерирует строку GUID в верхенм регистре.
 *
 *
 * \_ru
 */

class ANANAS_EXPORT aExtGUID: public AExtension
{
	Q_OBJECT
public:
	aExtGUID();
	~aExtGUID();


public slots:
	QString Generate() const;
};

#endif
