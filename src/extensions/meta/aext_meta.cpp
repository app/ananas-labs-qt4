/****************************************************************************
** $Id: aext_meta.cpp,v 1.1 2008/11/05 21:16:27 leader Exp $
**
** Extension object implementation file of
** Ananas application library
**
** Created : 20031201
**
** Copyright (C) 2006 Andrey Paskal
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

//#include <qtextstream.h>
#include <qdom.h>
#include <qstringlist.h> 

#include "aext_meta.h"
#include "adatabase.h"
#include <aextensionplugin.h>

/**
 *\en
 *	Create object.
 *\_en \ru
 *	Создает объект.
 *\_ru
 */
AExtMeta::AExtMeta()
    : AExtension("Meta")
{
}


/**
 *\en
 *	Destroy object.
 *\_en \ru
 *	Уничтожает объект.
 *\_ru
 */
AExtMeta::~AExtMeta()
{
}

/**
 * \en
 * \_en 
 * \ru
 * 	\brief Возвращает имя класса бизнес объекта по идентификатору класса, присвоенному классу в метаданных.
 *
 * 	Имя класса может быть например "Справочник товаров", "Справочник счетов".
 * 	\param id - идентификатор класса в метаданных
 * 	\return имя класса в бизнес схеме
 *
 * 	Пример использования
 * 	\code
 * 	meta = new Meta();
 * 	sys.Message(0,"id=129, Class name = "+meta.GetClassName(129));
 * 	\endcode
 *
 * 	\see GetBaseClassName(...)
 * \_ru
 */
QString
AExtMeta::GetClassName(const long &id)
{
    return db->cfg.attr(db->cfg.find(id),"name");
}


/**
 * \en
 * \_en 
 * \ru
 * 	\brief Возвращает имя базового класса бизнес объекта по идентификатору класса, присвоенному классу в метаданных.
 *
 * 	Имя базового класса может быть, например "catalogue", "document", "aregister", "iregister","journal", "report".
 * 	\param id - идентификатор класса в метаданных
 * 	\return имя базового класса в бизнес схеме
 *
 * 	Пример использования
 * 	\code
 * 	meta = new Meta();
 * 	sys.Message(0,"id=129, Base class name = "+meta.GetBaseClassName(129));
 * 	\endcode
 *
 * 	\see GetClassName(...)
 * \_ru
 */
QString
AExtMeta::GetBaseClassName(const long &id)
{
    return db->cfg.objClass(id);
}

/**
 * \en
 * \_en 
 * \ru
 * 	\brief Возвращает имя базового класса бизнес объекта по уникальному идентификатору
 * 	бизнес объекта в базе данных.
 * 	
 * 	Имя базового класса может быть, например "catalogue", "document", "aregister", "iregister","journal", "report".
 * 	\param id - уникальный идентификатор бизнес обхъекта в базе данных
 * 	\return имя базового класса в бизнес схеме
 *
 * 	Пример использования
 * \code
 * 	meta = new Meta();
 * 	sys.Message(0,"uid=6, base class name ="+meta.GetBaseClassNameByObjectUid(6));
 * \endcode
 * \see GetClassNameByObjectUid(...)
 * \_ru
 */
QString
AExtMeta::GetBaseClassNameByObjectUid(const long &id)
{
    return db->cfg.objClass(db->uidType(id));
}

/**
 * \en
 * \_en 
 * \ru
 * 	\brief Возвращает имя класса бизнес объекта по уникальному идентификатору бизнес объекта в базе данных
 *
 * 	Имя класса может быть например "Справочник товаров", "Справочник счетов".
 * 	\param id - уникальный идентификатор бизнес объекта в базе данных
 * 	\return имя класса в бизнес схеме
 * 	
 * 	Пример использования
 * \code
 * 	meta = new Meta();
 * 	sys.Message(0,"uid=6, class name ="+meta.GetClassNameByObjectUid(6));
 * \endcode
 * \see GetBaseClassNameByObjectUid(...)
 * \_ru
 */
QString
AExtMeta::GetClassNameByObjectUid(const long &id)
{
    return db->cfg.attr(db->cfg.find(db->uidType(id)),"name");
}

/**
 * \en
 * \_en 
 * \ru
 * 	\brief Возвращает количество бизнес объектов указанного класса в бизнес схеме.
 *
 * 	Имя класса может быть, например, "catalogue", "document".
 * 	\param className - Имя класса
 * 	\return количество бизнес объектов
 * 	
 * 	Пример использования
 * \code
 * 	meta = new Meta();
 * \endcode
 * \_ru
 */
int
AExtMeta::CountByClassName(const QString &className) {
	return db->cfg.count(db->cfg.find(mdc_root),className);
}

/**
 * \en
 * \_en 
 * \ru
 * 	\brief Возвращает список имен бизнес объектов указанного класса в бизнес схеме.
 *
 * 	Имя класса может быть, например, "catalogue", "document".
 * 	TODO: Требуется проверить возможность передачи возвращаемого значения в Скрипт. Возможно списки и не поддерживаются.
 * 	\param className - Имя класса
 * 	\return Список названий
 * 	
 * 	Пример использования
 * \code
 * 	meta = new Meta();
 * \endcode
 * \_ru
 */
QStringList
AExtMeta::ListByClassName(const QString &className) {
	QStringList sList;
	QDomNodeList nodeList=db->cfg.find(mdc_root).elementsByTagName(className);
	int i,cnt = nodeList.count();
	for (i=0; i<cnt; i++) 
	{
		QDomElement el= nodeList.item(i).toElement();
		if (!el.isNull()) 
		{
			sList+=el.attribute("name");
		}
	}
	return sList;
}

typedef AExtensionPlugin<AExtMeta> AExtMetaPlugin;
A_EXPORT_PLUGIN( AExtMetaPlugin )
