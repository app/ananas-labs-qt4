/****************************************************************************
** $Id: aextensionfactory.cpp,v 1.1 2008/11/05 21:16:28 leader Exp $
**
** ...
**
** Created : 20060209
**
** Copyright (C) 2003-2006 Leader InfoTech.  All rights reserved.
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

#include "aextensionfactory.h"
#include <aextension.h>
#include <aextensionplugin.h>
#include <qapplication.h>
#include <qobject.h>
#include <QHash>
#include <QStringList>
#include <QDir>
#include <QPluginLoader>

class AExtensionFactoryPrivate : public QObject
{
public:
    AExtensionFactoryPrivate();
    ~AExtensionFactoryPrivate();

    QHash<QString, AExtensionInterface *> hash;
    QStringList extensionKeys;
};

static AExtensionFactoryPrivate *instance = 0;

AExtensionFactoryPrivate::AExtensionFactoryPrivate()
: QObject( qApp )
{
    QString extensionsDirName;
    AExtensionInterface *iface = 0;
    QObject *plugin = 0;
    QPluginLoader pluginLoader;

#ifdef Q_OS_WIN32
    extensionsDirName = qApp->applicationDirPath()+"/extensions";
#else
    extensionsDirName = "/usr/lib/ananas4/extensions";
#endif

    hash.clear();
    extensionKeys.clear();
    QDir extensionsDirectory( extensionsDirName );
    QStringList extensionFiles = extensionsDirectory.entryList( QDir::Files );
    QString fileName = extensionFiles.first();
    for (int i=0; i<extensionFiles.count(); i++){
      fileName = extensionFiles[i];
      pluginLoader.setFileName( extensionsDirectory.absoluteFilePath( fileName ) );
      plugin = pluginLoader.instance();
      if (plugin) {
            iface = qobject_cast<AExtensionInterface *>(plugin);
            if( iface ) {
                extensionKeys.append( iface->key());
                hash[ iface->key()] = iface;
            }
      }
    }
}

AExtensionFactoryPrivate::~AExtensionFactoryPrivate()
{
//    delete instance;
//    instance = 0;
}

/*!
    Creates a AExtension object that matches \a key. This is either a
    built-in extensions, or a extension from a extension plugin.

    \sa keys()
*/
AExtension *AExtensionFactory::create( const QString& key )
{
    AExtension *ret = 0;
    AExtensionInterface *iface = 0;
//    QString extension = key;

    if ( !instance ) instance = new AExtensionFactoryPrivate();
    if ( instance->hash.contains( key ) ){
        iface = instance->hash.value( key );
        if ( iface ) ret = iface->create(); 
    }
    if (ret) ret->setName(key);
    return ret;
}



QMetaObject *AExtensionFactory::metaObject( const QString &key )
{
    QMetaObject *ret = 0;
    AExtensionInterface *iface = 0;

    if ( !instance ) instance = new AExtensionFactoryPrivate();
    if ( instance->hash.contains( key ) ){
        iface = instance->hash.value( key );
        if ( iface ) ret = iface->metaObject(); 
    }
    return ret;
};

/*!
    Returns the list of keys this factory can create styles for.

    \sa create()
*/
QStringList AExtensionFactory::keys()
{
    if ( !instance ) instance = new AExtensionFactoryPrivate();
    return instance->extensionKeys;
}

