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

//#include "qdesigner.h"
#include "qdesigner_objectinspector.h"
#include "qdesigner_workbench.h"

#include <QtDesigner/QtDesigner>
#include <QtDesigner/QDesignerComponents>

QDesignerObjectInspector::QDesignerObjectInspector(QDesignerWorkbench *workbench)
    : QDesignerToolWindow(workbench)
{
    setObjectName(QLatin1String("ObjectInspector"));
    QDesignerObjectInspectorInterface *widget = QDesignerComponents::createObjectInspector(workbench->core(), this);
    workbench->core()->setObjectInspector(widget);

    setCentralWidget(widget);

    setWindowTitle(tr("Object Inspector"));
}

QDesignerObjectInspector::~QDesignerObjectInspector()
{
}

QRect QDesignerObjectInspector::geometryHint() const
{
    QRect g = workbench()->availableGeometry();
    int margin = workbench()->marginHint();

    QSize sz(g.width() * 1/4, g.height() * 1/6);

    return QRect((g.width() - sz.width() - margin), margin,
                  sz.width(), sz.height());
}
