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

#include "wreport_plugin.h"

#include <QtPlugin>
#include <QDesignerFormEditorInterface>
#include <QExtensionManager>

#include "wreport_taskmenu.h"
#include "wreport.h"


wReportPlugin::wReportPlugin(QObject *parent)
    : QObject(parent), m_initialized(false)
{
    m_initialized = false;
}

bool wReportPlugin::isContainer() const
{
    return true;
}

bool wReportPlugin::isInitialized() const
{
    return m_initialized;
}

void wReportPlugin::initialize(QDesignerFormEditorInterface *core)
{
    if (m_initialized)
        return;
    Q_UNUSED(core);

    QExtensionManager *manager = core->extensionManager();
    Q_ASSERT(manager != 0);
    manager->registerExtensions(new wReportTaskMenuFactory(manager),
                                Q_TYPEID(QDesignerTaskMenuExtension));

    m_initialized = true;
}

QWidget *wReportPlugin::createWidget(QWidget *parent)
{
    wReport *w = new wReport(parent, 0);
    return w;
}

QString wReportPlugin::name() const
{
    return QLatin1String("wReport");
}

QString wReportPlugin::group() const
{
    return QLatin1String("Ananas");
}

QString wReportPlugin::toolTip() const
{
    return QString();
}

QString wReportPlugin::whatsThis() const
{
    return QString();
}

QString wReportPlugin::includeFile() const
{
    return QLatin1String("wreport.h");
}

QIcon wReportPlugin::icon() const
{
    //return QIcon();
    return QIcon(":/images/wreport.png");
}

QString wReportPlugin::domXml() const
{
    return "<widget class=\"wReport\" name=\"wReport_1\">\n</widget>\n";
}
