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

#include "acombobox_plugin.h"

#include <QtPlugin>

#include "acombobox.h"


AComboBoxPlugin::AComboBoxPlugin(QObject *parent)
    : QObject(parent), m_initialized(false)
{
    m_initialized = false;
}

bool AComboBoxPlugin::isContainer() const
{
    return false;
}

bool AComboBoxPlugin::isInitialized() const
{
    return m_initialized;
}

void AComboBoxPlugin::initialize(QDesignerFormEditorInterface *core)
{
    if (m_initialized)
        return;
    Q_UNUSED(core);
    m_initialized = true;
}

QWidget *AComboBoxPlugin::createWidget(QWidget *parent)
{
    AComboBox *w = new AComboBox(parent, 0);
    return w;
}

QString AComboBoxPlugin::name() const
{
    return QLatin1String("AComboBox");
}

QString AComboBoxPlugin::group() const
{
    return QLatin1String("Ananas");
}

QString AComboBoxPlugin::toolTip() const
{
    return QString("Ananas for QT custom combobox");
}

QString AComboBoxPlugin::whatsThis() const
{
    return QString("");
}

QString AComboBoxPlugin::includeFile() const
{
    return QLatin1String("acombobox.h");
}

QIcon AComboBoxPlugin::icon() const
{
    //return QIcon();
    return QIcon(":/images/acombobox.png");
}

QString AComboBoxPlugin::domXml() const
{
    return "<widget class=\"AComboBox\" name=\"AComboBox_1\">\n</widget>\n";
}
