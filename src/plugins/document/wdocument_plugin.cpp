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

#include "wdocument_plugin.h"

#include <QtPlugin>
#include <QDesignerFormEditorInterface>
#include <QExtensionManager>

#include "wdocument_taskmenu.h"
#include "wdocument.h"


wDocumentPlugin::wDocumentPlugin(QObject *parent)
    : QObject(parent), m_initialized(false)
{
    m_initialized = false;
}

bool wDocumentPlugin::isContainer() const
{
    return true;
}

bool wDocumentPlugin::isInitialized() const
{
    return m_initialized;
}

void wDocumentPlugin::initialize(QDesignerFormEditorInterface *core)
{
    if (m_initialized)
        return;
    Q_UNUSED(core);

    QExtensionManager *manager = core->extensionManager();
    Q_ASSERT(manager != 0);
    manager->registerExtensions(new wDocumentTaskMenuFactory(manager),
                                Q_TYPEID(QDesignerTaskMenuExtension));

    m_initialized = true;
}

QWidget *wDocumentPlugin::createWidget(QWidget *parent)
{
    wDocument *w = new wDocument(parent, 0);
    return w;
}

QString wDocumentPlugin::name() const
{
    return QLatin1String("wDocument");
}

QString wDocumentPlugin::group() const
{
    return QLatin1String("Ananas");
}

QString wDocumentPlugin::toolTip() const
{
    return QString();
}

QString wDocumentPlugin::whatsThis() const
{
    return QString();
}

QString wDocumentPlugin::includeFile() const
{
    return QLatin1String("wdocument.h");
}

QIcon wDocumentPlugin::icon() const
{
    //return QIcon();
    return QIcon(":/images/wdocument.png");
}

QString wDocumentPlugin::domXml() const
{
    return "<widget class=\"wDocument\" name=\"wDocument_1\">\n</widget>\n";
}
