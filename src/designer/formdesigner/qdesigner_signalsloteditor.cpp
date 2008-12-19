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
#include "qdesigner_signalsloteditor.h"
#include "qdesigner_workbench.h"
#include "qdesigner_settings.h"

#include <QtDesigner/QtDesigner>
#include <QtDesigner/QDesignerComponents>

#include <QtCore/qdebug.h>


QDesignerSignalSlotEditor::QDesignerSignalSlotEditor(QDesignerWorkbench *workbench)
    : QDesignerToolWindow(workbench)
{
    setObjectName(QLatin1String("SignalSlotEditorWindow"));
    QWidget *widget = QDesignerComponents::createSignalSlotEditor(workbench->core(), this);

    setCentralWidget(widget);

    setWindowTitle(tr("Signal/Slot Editor"));
}

QDesignerSignalSlotEditor::~QDesignerSignalSlotEditor()
{
}

QRect QDesignerSignalSlotEditor::geometryHint() const
{
    QRect g = workbench()->availableGeometry();
    int margin = workbench()->marginHint();

    QSize sz(g.width() * 1/3, g.height() * 1/6);
    QRect r(QPoint(0, 0), sz);
    r.moveCenter(g.center());
    r.moveTop(margin);

    return r;
}
