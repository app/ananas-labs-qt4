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

#ifndef WDBTABLE_PLUGIN_H
#define WDBTABLE_PLUGIN_H

#include <QDesignerCustomWidgetInterface>

#include "qwidgetplugin.h"


class QT_WIDGET_PLUGIN_EXPORT wDBTablePlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    wDBTablePlugin(QObject *parent=0);

    bool isContainer() const;
    bool isInitialized() const;
    void initialize(QDesignerFormEditorInterface *core);
    QWidget *createWidget(QWidget *parent);

    QString name() const;
    QString group() const;
    QString toolTip() const;
    QString whatsThis() const;
    QString includeFile() const;
    QIcon icon() const;
    QString domXml() const;

private:
    bool m_initialized;
};

#endif // WDBTABLE_PLUGIN_H
