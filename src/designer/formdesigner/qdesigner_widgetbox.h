/****************************************************************************
**
** Copyright (C) 1992-2006 Trolltech ASA. All rights reserved.
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

#ifndef QDESIGNER_WIDGETBOX_H
#define QDESIGNER_WIDGETBOX_H

#include "qdesigner_toolwindow.h"

class QDesignerWorkbench;

class QDesignerWidgetBox: public QDesignerToolWindow
{
    Q_OBJECT
public:
    QDesignerWidgetBox(QDesignerWorkbench *workbench);
    virtual ~QDesignerWidgetBox();

    virtual Qt::DockWidgetArea dockWidgetAreaHint() const;
    virtual QRect geometryHint() const;
};

#endif // QDESIGNER_WIDGETBOX_H
