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

#include "awidgets_plugin.h"

#include <QtPlugin>

#include "acombobox_plugin.h"
#include "wfield_plugin.h"
#include "wdbfield_plugin.h"
#include "wtable_plugin.h"
#include "wdbtable_plugin.h"
#include "wcatalogue_plugin.h"
#include "wdocument_plugin.h"
#include "wjournal_plugin.h"
#include "wgrouptree_plugin.h"
#include "wactionbutton_plugin.h"
#include "wreport_plugin.h"


aWidgetsCollection::aWidgetsCollection(QObject *parent)
    : QObject(parent)
{
    widgets.append(new wActionButtonPlugin(this));
    widgets.append(new wFieldPlugin(this));
    widgets.append(new wDBFieldPlugin(this));
    widgets.append(new wTablePlugin(this));
    widgets.append(new wDBTablePlugin(this));
    widgets.append(new wGroupTreePlugin(this));
    widgets.append(new AComboBoxPlugin(this));
    widgets.append(new wCataloguePlugin(this));
    widgets.append(new wDocumentPlugin(this));
    widgets.append(new wJournalPlugin(this));
    widgets.append(new wReportPlugin(this));
}

QList<QDesignerCustomWidgetInterface*> aWidgetsCollection::customWidgets() const
{
    return widgets;
}

Q_EXPORT_PLUGIN2(aWidgetsCollection, aWidgetsCollection)
