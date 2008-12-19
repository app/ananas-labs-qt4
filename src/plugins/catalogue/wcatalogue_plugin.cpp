/****************************************************************************
**
** Code file
**
** Copyright (C) 2006 Dmitriy Pavlyuk <dm-p@rambler.ru>
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
**********************************************************************/

#include "wcatalogue_plugin.h"

#include <QtPlugin>
#include <QDesignerFormEditorInterface>
#include <QExtensionManager>

#include "wcatalogue_taskmenu.h"
#include "wcatalogue.h"


wCataloguePlugin::wCataloguePlugin(QObject *parent)
    : QObject(parent), m_initialized(false)
{
    m_initialized = false;
}

bool wCataloguePlugin::isContainer() const
{
    return true;
}

bool wCataloguePlugin::isInitialized() const
{
    return m_initialized;
}

void wCataloguePlugin::initialize(QDesignerFormEditorInterface *core)
{
    if (m_initialized)
        return;
    Q_UNUSED(core);

    QExtensionManager *manager = core->extensionManager();
    Q_ASSERT(manager != 0);
    manager->registerExtensions(new wCatalogueTaskMenuFactory(manager),
                                Q_TYPEID(QDesignerTaskMenuExtension));

    m_initialized = true;
}

QWidget *wCataloguePlugin::createWidget(QWidget *parent)
{
    wCatalogue *w = new wCatalogue(parent, 0);
    return w;
}

QString wCataloguePlugin::name() const
{
    return QLatin1String("wCatalogue");
}

QString wCataloguePlugin::group() const
{
    return QLatin1String("Ananas");
}

QString wCataloguePlugin::toolTip() const
{
    return QString();
}

QString wCataloguePlugin::whatsThis() const
{
    return QString();
}

QString wCataloguePlugin::includeFile() const
{
    return QLatin1String("wcatalogue.h");
}

QIcon wCataloguePlugin::icon() const
{
    //return QIcon();
    return QIcon(":/images/wcatalogue.png");
}

QString wCataloguePlugin::domXml() const
{
    return "<widget class=\"wCatalogue\" name=\"wCatalogue_1\">\n</widget>\n";
}
