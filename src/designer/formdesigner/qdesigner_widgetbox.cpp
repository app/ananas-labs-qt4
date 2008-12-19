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
#include "qdesigner_widgetbox.h"
#include "qdesigner_workbench.h"
#include "qdesigner_settings.h"

#include <QtDesigner/QtDesigner>
#include <QtDesigner/QDesignerComponents>

#include <QtCore/qdebug.h>

QDesignerWidgetBox::QDesignerWidgetBox(QDesignerWorkbench *workbench)
    : QDesignerToolWindow(workbench)
{
    setObjectName(QLatin1String("WidgetBox"));
    QDesignerWidgetBoxInterface *widget = QDesignerComponents::createWidgetBox(workbench->core(), this);
    widget->setFileName(QLatin1String(":/trolltech/widgetbox/widgetbox.xml"));
    widget->load();
    widget->setFileName(QDesignerSettings().defaultUserWidgetBoxXml());
    widget->load();

/*    int g, w;
    for ( g=0; g< widget->categoryCount(); g++){
	printf("cat %i=%s\n", g, widget->category( g ).name().toUtf8().data());
	for ( w=0; w < widget->widgetCount( g ); w++ ){
	    printf("   w%i=%s\n", w, widget->widget( g, w ).name().toUtf8().data());
	}
    } 
*/
    // Удалим лишние группы и объекты
    widget->removeCategory( 3 );
    
    workbench->core()->setWidgetBox(widget);

    setCentralWidget(widget);

    setWindowTitle(tr("Widget Box"));
}

QDesignerWidgetBox::~QDesignerWidgetBox()
{
}

QRect QDesignerWidgetBox::geometryHint() const
{
    QRect g = workbench()->availableGeometry();

    return QRect(workbench()->marginHint(), workbench()->marginHint(),
                 g.width() * 1/6, g.height() * 5/6);
}

Qt::DockWidgetArea QDesignerWidgetBox::dockWidgetAreaHint() const
{
    return Qt::LeftDockWidgetArea;
}
