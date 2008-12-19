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

#include "wjournal_plugin.h"

#include <QtPlugin>
#include <QDesignerFormEditorInterface>
#include <QExtensionManager>

#include "wjournal_taskmenu.h"
#include "wjournal.h"


wJournalPlugin::wJournalPlugin(QObject *parent)
    : QObject(parent), m_initialized(false)
{
    m_initialized = false;
}

bool wJournalPlugin::isContainer() const
{
    return true;
}

bool wJournalPlugin::isInitialized() const
{
    return m_initialized;
}

void wJournalPlugin::initialize(QDesignerFormEditorInterface *core)
{
    if (m_initialized)
        return;
    Q_UNUSED(core);

    QExtensionManager *manager = core->extensionManager();
    Q_ASSERT(manager != 0);
    manager->registerExtensions(new wJournalTaskMenuFactory(manager),
                                Q_TYPEID(QDesignerTaskMenuExtension));

    m_initialized = true;
}

QWidget *wJournalPlugin::createWidget(QWidget *parent)
{
    wJournal *w = new wJournal(parent, 0);
    return w;
}

QString wJournalPlugin::name() const
{
    return QLatin1String("wJournal");
}

QString wJournalPlugin::group() const
{
    return QLatin1String("Ananas");
}

QString wJournalPlugin::toolTip() const
{
    return QString();
}

QString wJournalPlugin::whatsThis() const
{
    return QString();
}

QString wJournalPlugin::includeFile() const
{
    return QLatin1String("wjournal.h");
}

QIcon wJournalPlugin::icon() const
{
    //return QIcon();
    return QIcon(":/images/wjournal.png");
}

QString wJournalPlugin::domXml() const
{
    return "<widget class=\"wJournal\" name=\"wJournal_1\">\n</widget>\n";
}
