/****************************************************************************
** $Id: aextensionplugin.cpp,v 1.1 2008/11/05 21:16:28 leader Exp $
**
** Extension plugin class implementation file of
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

#include "aextensionplugin.h"

//#ifndef QT_NO_COMPONENT

//#include "aextensioninterface_p.h"

/*!
    \fn QStringList AExtensionPlugin::keys() const

    Returns the list of extensions (keys) this plugin supports.

    These keys are usually the class names of the custom extension that
    are implemented in the plugin.

    \sa create()
*/

/*!
    \fn AExtension* AExtensionPlugin::create( const QString& key )

    Creates and returns a AExtension object for the driver key \a key.
    The driver key is usually the class name of the required driver.

    \sa keys()
*/
/*
class AExtensionPluginPrivate : public AExtensionFactoryInterface
{
public:
    AExtensionPluginPrivate( AExtensionPluginBase *p )
	: plugin( p )
    {
    }
    virtual ~AExtensionPluginPrivate();

    QRESULT queryInterface( const QUuid &iid, QUnknownInterface **iface );
    Q_REFCOUNT;

    QStringList featureList() const;
    AExtension *create( const QString &key );

private:
    AExtensionPluginBase *plugin;
};

AExtensionPluginPrivate::~AExtensionPluginPrivate()
{
    delete plugin;
}

QRESULT AExtensionPluginPrivate::queryInterface( const QUuid &iid, QUnknownInterface **iface )
{
    *iface = 0;

    if ( iid == IID_QUnknown )
	*iface = this;
    else if ( iid == IID_QFeatureList )
	*iface = this;
    else if ( iid == IID_AExtensionFactory )
	*iface = this;
    else
	return QE_NOINTERFACE;

    (*iface)->addRef();
    return QS_OK;
}

QStringList AExtensionPluginPrivate::featureList() const
{
    return plugin->keys();
}

AExtension *AExtensionPluginPrivate::create( const QString &key )
{
    return plugin->create( key );
}
*/
/*!
    Constructs a Ananas extension plugin. This is invoked automatically by
    the \c Q_EXPORT_PLUGIN macro.
*/

AExtensionPluginBase::AExtensionPluginBase()
    :QObject()// QGPlugin( d = new AExtensionPluginPrivate( this ) )
{
}

/*!
    Destroys the Ananas extension plugin.

    You never have to call this explicitly. Qt destroys a plugin
    automatically when it is no longer used.
*/
AExtensionPluginBase::~AExtensionPluginBase()
{
    // don't delete d, as this is deleted by d
}

