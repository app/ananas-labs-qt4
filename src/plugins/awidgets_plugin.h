/****************************************************************************
**
** Header file
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

#ifndef AWIDGETS_PLUGIN_H
#define AWIDGETS_PLUGIN_H

#include <QDesignerCustomWidgetCollectionInterface>

#include "qwidgetplugin.h"


typedef QList<QDesignerCustomWidgetInterface*> aListInterfaces;

class QT_WIDGET_PLUGIN_EXPORT aWidgetsCollection: public QObject, public QDesignerCustomWidgetCollectionInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)

public:
    aWidgetsCollection(QObject *parent=0);

    virtual aListInterfaces customWidgets() const;

private:
    aListInterfaces widgets;
};

#endif // AWIDGETS_PLUGIN_H
