/****************************************************************************
** $Id: acfgrc.h,v 1.2 2008/11/09 21:09:11 leader Exp $
**
** Header file of the recorse work file of Ananas
** Designer and Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
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

#ifndef ACFGRC_H
#define ACFGRC_H

#include "ananasglobal.h"

#include <qobject.h>
#include <q3dict.h>
//Added by qt3to4:
#include <QPixmap>

QPixmap ANANAS_EXPORT rcIcon(const char *name);
void ANANAS_EXPORT setMessageHandler(bool GUI = false);
void ANANAS_EXPORT unsetMessageHandler();


/**
 * \ru
 *	\brief Класс для работы с файлом ресурсов.
 *
 *	Класс aCfgRc реализует интерфейс управления файлом ресурсов бизнес-схемы Ананаса,
 *	содержащим параметры соединения с SQL сервером, имя бизнес схемы, 
 *	отображаемое пользователю для выбора при запуске Ананаса, и ссылку на файл метаданных
 *	бизнес-схемы Ананаса.
 * \_ru
 */
class  ANANAS_EXPORT aCfgRc: public QObject
{
public:
	aCfgRc();
	int read(const QString &fname);
	int write(const QString &fname);
	int write();
	QString value(const QString &name, const QString &defValue = QString::null );
	void setValue(const QString &name, const QString &value);
	int count();
	QString getRcFileName();

private:
	QString filename;
	Q3Dict <QString> values;
};

#endif
