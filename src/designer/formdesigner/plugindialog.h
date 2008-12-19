/****************************************************************************
**
** Copyright (C) 2005-2006 Trolltech ASA. All rights reserved.
**
** This file is part of the Qt Designer of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef PLUGINDIALOG_H
#define PLUGINDIALOG_H

#include "ui_plugindialog.h"

class QDesignerFormEditorInterface;

class PluginDialog : public QDialog
{
    Q_OBJECT
public:
    PluginDialog(QDesignerFormEditorInterface *core, QWidget *parent = 0);

    QDesignerFormEditorInterface *core() const;

private:
    void populateTreeWidget();
    QIcon pluginIcon(const QIcon &icon);

private:
    QDesignerFormEditorInterface *m_core;
    Ui::PluginDialog ui;
    QIcon interfaceIcon;
    QIcon featureIcon;
};

#endif
