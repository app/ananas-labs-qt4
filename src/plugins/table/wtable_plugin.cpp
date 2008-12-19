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

#include "wtable_plugin.h"

#include <QtPlugin>

#include "wtable.h"


wTablePlugin::wTablePlugin(QObject *parent)
    : QObject(parent), m_initialized(false)
{
    m_initialized = false;
}

bool wTablePlugin::isContainer() const
{
    return false;
}

bool wTablePlugin::isInitialized() const
{
    return m_initialized;
}

void wTablePlugin::initialize(QDesignerFormEditorInterface *core)
{
    if (m_initialized)
        return;
    Q_UNUSED(core);
    m_initialized = true;
}

QWidget *wTablePlugin::createWidget(QWidget *parent)
{
    wTable *w = new wTable(parent, 0);
    return w;
}

QString wTablePlugin::name() const
{
    return QLatin1String("wTable");
}

QString wTablePlugin::group() const
{
    return QLatin1String("Ananas");
}

QString wTablePlugin::toolTip() const
{
    return QString("Ananas for QT Table Widget");
}

QString wTablePlugin::whatsThis() const
{
    return QString("Entry to modify tables values of Ananas's object");
}

QString wTablePlugin::includeFile() const
{
    return QLatin1String("wtable.h");
}

QIcon wTablePlugin::icon() const
{
    //return QIcon();
    return QIcon(":/images/wtable.png");
}

QString wTablePlugin::domXml() const
{
    return "<widget class=\"wTable\" name=\"wTable_1\">\n</widget>\n";
}
