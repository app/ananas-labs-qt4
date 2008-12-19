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

#include "wgrouptree_plugin.h"

#include <QtPlugin>

#include "wgrouptree.h"


wGroupTreePlugin::wGroupTreePlugin(QObject *parent)
    : QObject(parent), m_initialized(false)
{
    m_initialized = false;
}

bool wGroupTreePlugin::isContainer() const
{
    return false;
}

bool wGroupTreePlugin::isInitialized() const
{
    return m_initialized;
}

void wGroupTreePlugin::initialize(QDesignerFormEditorInterface *core)
{
    if (m_initialized)
        return;
    Q_UNUSED(core);
    m_initialized = true;
}

QWidget *wGroupTreePlugin::createWidget(QWidget *parent)
{
    wGroupTree *w = new wGroupTree(parent);
    return w;
}

QString wGroupTreePlugin::name() const
{
    return QLatin1String("wGroupTree");
}

QString wGroupTreePlugin::group() const
{
    return QLatin1String("Ananas");
}

QString wGroupTreePlugin::toolTip() const
{
    return QString("Ananas for QT Group Tree Widget");
}

QString wGroupTreePlugin::whatsThis() const
{
    return QString("Entry to select group of Ananas's object");
}

QString wGroupTreePlugin::includeFile() const
{
    return QLatin1String("wgrouptree.h");
}

QIcon wGroupTreePlugin::icon() const
{
    //return QIcon();
    return QIcon(":/images/wgrouptree.png");
}

QString wGroupTreePlugin::domXml() const
{
    return "<widget class=\"wGroupTree\" name=\"wGroupTree_1\">\n</widget>\n";
}
