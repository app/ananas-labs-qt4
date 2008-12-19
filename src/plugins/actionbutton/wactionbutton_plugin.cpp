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

#include "wactionbutton_plugin.h"

#include <QtPlugin>
#include <QDesignerFormEditorInterface>
#include <QExtensionManager>

#include "wactionbutton_taskmenu.h"
#include "wactionbutton.h"


wActionButtonPlugin::wActionButtonPlugin(QObject *parent)
    : QObject(parent), m_initialized(false)
{
    m_initialized = false;
}

bool wActionButtonPlugin::isContainer() const
{
    return false;
}

bool wActionButtonPlugin::isInitialized() const
{
    return m_initialized;
}

void wActionButtonPlugin::initialize(QDesignerFormEditorInterface *core)
{
    if (m_initialized)
        return;
    Q_UNUSED(core);

    QExtensionManager *manager = core->extensionManager();
    Q_ASSERT(manager != 0);
    manager->registerExtensions(new wActionButtonTaskMenuFactory(manager),
                                Q_TYPEID(QDesignerTaskMenuExtension));

    m_initialized = true;
}

QWidget *wActionButtonPlugin::createWidget(QWidget *parent)
{
    wActionButton *w = new wActionButton(parent);
    return w;
}

QString wActionButtonPlugin::name() const
{
    return QLatin1String("wActionButton");
}

QString wActionButtonPlugin::group() const
{
    return QLatin1String("Ananas");
}

QString wActionButtonPlugin::toolTip() const
{
    return QString("Ananas for QT Action button Widget");
}

QString wActionButtonPlugin::whatsThis() const
{
    return QString();
}

QString wActionButtonPlugin::includeFile() const
{
    return QLatin1String("wactionbutton.h");
}

QIcon wActionButtonPlugin::icon() const
{
    //return QIcon();
    return QIcon(":/images/wactionbutton.png");
}

QString wActionButtonPlugin::domXml() const
{
    return "<widget class=\"wActionButton\" name=\"wActionButton_1\">\n</widget>\n";
}
