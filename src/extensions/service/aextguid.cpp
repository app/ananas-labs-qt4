/****************************************************************************
** $Id: aextguid.cpp,v 1.2 2007/09/19 06:32:06 app Exp $
**
** Code file of the Report Result Object of Ananas
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

#include <quuid.h>
#include "aextguid.h"

aExtGUID::aExtGUID()
	: AExtension("GUID")
{
}

aExtGUID::~aExtGUID()
{
}


/**
 * \en
 * 	Class for generate GUID strings from Ananas Script. Linux only !!!
 * \_en
 * \ru
 * Генерирует строку GUID (глобально-уникального идентификатора, Globally Unique IDentifier).
 * Только в Linux версиях. В Windows возвращает 00000000-0000-... (Для Qt3.2)
 *
 * GUID представляет собой 128-битное значение, уникальное для всех практических целей.
 * 	\brief Генерирует строку GUID в верхнем регистре.
 *
 * 	Пример использования функции Generate()
 * \code
 *  var newGUID = (new GUID).Generate();
 *  sysMessage (0, newGUID);
 *  возвращает строку вида {FC98E407-B3F7-4914-A2B1-36B4C2A93228}
 *
 *  Для преобразования в нижний регистр необходимо использовать
 *  newGUID.toLowerCase();
 * \endcode
 *
 * \_ru
 */
QString aExtGUID::Generate() const
{
	
#ifdef Q_OS_WIN32
	return QUuid().toString();
#else
	return QUuid::createUuid().toString().upper();
#endif
}

#include <aextensionplugin.h>
typedef AExtensionPlugin<aExtGUID> aExtGUIDPlugin;
A_EXPORT_PLUGIN( aExtGUIDPlugin )
