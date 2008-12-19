/****************************************************************************
**
** Copyright (C) 2006 Dmitriy Pavlyuk <dm-p@rambler.ru> All rights reserved.
**
** This file is part of the widgets plugin of the Ananas
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
****************************************************************************/

#include <QDesignerFormEditorInterface>

#include <QAction>

#include "wdbfield_taskmenu.h"
#include "wdbfield.h"
#include "addfdialog.h"


wDBFieldTaskMenu::wDBFieldTaskMenu(wDBField *widget, QObject *parent)
    : QObject(parent)
{
    m_widget = widget;

    m_editAction = new QAction(tr("Edit ..."), this);
    connect(m_editAction, SIGNAL(triggered()), this, SLOT(edit()));
}

void wDBFieldTaskMenu::edit()
{
    /*addfdialog e;

	e.setData(m_widget);
	if (e.exec() == QDialog::Accepted)
	{
		e.getData(m_widget);
	}*/
	m_widget->widgetEditor();
}

QAction *wDBFieldTaskMenu::preferredEditAction() const
{
    return m_editAction;
}

QList<QAction *> wDBFieldTaskMenu::taskActions() const
{
    QList<QAction *> list;
    list.append(m_editAction);
    return list;
}

wDBFieldTaskMenuFactory::wDBFieldTaskMenuFactory(QExtensionManager *parent)
    : QExtensionFactory(parent)
{
}

QObject *wDBFieldTaskMenuFactory::createExtension(QObject *object, const QString &iid, QObject *parent) const
{
    if (iid != Q_TYPEID(QDesignerTaskMenuExtension))
        return 0;

    if (wDBField *df = qobject_cast<wDBField*>(object))
        return new wDBFieldTaskMenu(df, parent);

    return 0;
}
