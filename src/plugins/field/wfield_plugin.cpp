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

#include "wfield_plugin.h"

#include <QtPlugin>
#include <QDesignerFormEditorInterface>
#include <QExtensionManager>

#include "wfield_taskmenu.h"
#include "wfield.h"


wFieldPlugin::wFieldPlugin(QObject *parent)
    : QObject(parent), m_initialized(false)
{
    m_initialized = false;
}

bool wFieldPlugin::isContainer() const
{
    return false;
}

bool wFieldPlugin::isInitialized() const
{
    return m_initialized;
}

void wFieldPlugin::initialize(QDesignerFormEditorInterface *core)
{
    if (m_initialized)
        return;
    Q_UNUSED(core);

    QExtensionManager *manager = core->extensionManager();
    Q_ASSERT(manager != 0);
    manager->registerExtensions(new wFieldTaskMenuFactory(manager),
                                Q_TYPEID(QDesignerTaskMenuExtension));

    m_initialized = true;
}

QWidget *wFieldPlugin::createWidget(QWidget *parent)
{
    wField *w = new wField(parent, 0);
    return w;
}

QString wFieldPlugin::name() const
{
    return QLatin1String("wField");
}

QString wFieldPlugin::group() const
{
    return QLatin1String("Ananas");
}

QString wFieldPlugin::toolTip() const
{
    return QString("Ananas for QT Field Edit Widget");
}

QString wFieldPlugin::whatsThis() const
{
    return QString("Entry to modify field value of Ananas's object");
}

QString wFieldPlugin::includeFile() const
{
    return QLatin1String("wfield.h");
}

QIcon wFieldPlugin::icon() const
{
//    return QIcon();
    return QIcon( ":/images/wfield.png" );
}

QString wFieldPlugin::domXml() const
{
    return "<widget class=\"wField\" name=\"wField_1\">\n</widget>\n";
}

