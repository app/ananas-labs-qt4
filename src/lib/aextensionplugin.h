/****************************************************************************
** $Id: aextensionplugin.h,v 1.1 2008/11/05 21:16:28 leader Exp $
**
** Extension plugin object header file of
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

#ifndef AEXTENSIONPLUGIN_H
#define AEXTENSIONPLUGIN_H

#include <QtPlugin>
//#ifndef QT_H
//#include "qgplugin.h"
//#include "qstringlist.h"
//#endif // QT_H

#include "ananasglobal.h"
#include <QObject>
#include <QMetaObject>

class AExtension;
//class AExtensionPluginPrivate;

//#define A_EXPORT_PLUGIN(pluginobjectname) typedef AExtensionPlugin<pluginobjectname> pluginobjectname##Plugin; Q_EXPORT_PLUGIN(pluginobjectname)
#define A_EXPORT_PLUGIN(pluginobjectname) Q_EXPORT_PLUGIN2(pluginobjectname,pluginobjectname)

class  AExtensionInterface
{
public:
    virtual ~AExtensionInterface(){};
    virtual QString key() const = 0;
    virtual AExtension *create() = 0;
    virtual QMetaObject *metaObject() = 0;
};

Q_DECLARE_INTERFACE(AExtensionInterface,"Ananas.Extensions.AExtensionInterface/1.0")


/*!
 * \en
 * 	\brief Class for work with extensions
 * \_en
 * \ru
 * 	\brief Базовый класс в иерархии классов для работы с расширениями. Наследует QGPlugin.
 * \_ru
 */
class  ANANAS_EXPORT AExtensionPluginBase : public QObject, public AExtensionInterface
{
    Q_OBJECT
    Q_INTERFACES(AExtensionInterface)
public:

    AExtensionPluginBase();
    ~AExtensionPluginBase();
    virtual QString key() const = 0;
    virtual AExtension *create() = 0;
    virtual QMetaObject *metaObject() = 0;

//private:
//    AExtensionPluginPrivate *d;
};

/*!
 * \en
 *   \brief The AExtensionPlugin class provides an abstract base for custom AExtension plugins.
 *
 *   \ingroup plugins
 *   \mainclass
 *
 *   The Ananas Extension plugin is a simple plugin interface that makes it
 *   easy to create your own Ananas extension plugins that can be loaded
 *   dynamically by Ananas and Qt.
 *
 *   Writing a Ananas extension plugin is achieved by subclassing this base class,
 *   reimplementing the pure virtual functions keys() and create(), and
 *   exporting the class with the \c Q_EXPORT_PLUGIN macro. See the 
 *   Ananas extensions plugins that come with Ananas for example 
 *   implementations (in the
 *   \c{extensions} subdirectory of the source
 *   distribution). Read the \link plugins-howto.html plugins
 *   documentation\endlink for more information on plugins.
 * \_en
 * \ru
 * 	\brief Базовый класс для создания собственных расширений. Наследует AExtensionPluginBase
 * \_ru
 */
template<class type>
class  ANANAS_EXPORT AExtensionPlugin : public AExtensionPluginBase
{
//    Q_OBJECT
public:

    AExtensionPlugin()
    { 
	type o;
	extName = o.name();
    };
    ~AExtensionPlugin(){};
    QString key() const 
    {
	return extName;
    };
    AExtension *create() 
    {
        return new type();
    };
    QMetaObject *metaObject()
    {
	return (QMetaObject *) &type::staticMetaObject;
    };

private:
    QString extName;
};

#endif //AEXTENSIONPLUGIN_H
