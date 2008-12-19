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

#include "wdbtable_taskmenu.h"
#include "wdbtable.h"
#include "edbtable.h"


wDBTableTaskMenu::wDBTableTaskMenu(wDBTable *widget, QObject *parent)
    : QObject(parent)
{
    m_widget = widget;

    m_editAction = new QAction(tr("Edit ..."), this);
    connect(m_editAction, SIGNAL(triggered()), this, SLOT(edit()));
}

void wDBTableTaskMenu::edit()
{
    /*eDBTable e;

    m_widget->setAvailableTables();
    m_widget->checkFields();
	e.setData(m_widget);
	if (e.exec() == QDialog::Accepted)
	{
		e.getData(m_widget);
	}*/
	m_widget->OpenEditor();
}

QAction *wDBTableTaskMenu::preferredEditAction() const
{
    return m_editAction;
}

QList<QAction *> wDBTableTaskMenu::taskActions() const
{
    QList<QAction *> list;
    list.append(m_editAction);
    return list;
}

wDBTableTaskMenuFactory::wDBTableTaskMenuFactory(QExtensionManager *parent)
    : QExtensionFactory(parent)
{
}

QObject *wDBTableTaskMenuFactory::createExtension(QObject *object, const QString &iid, QObject *parent) const
{
    if (iid != Q_TYPEID(QDesignerTaskMenuExtension))
        return 0;

    if (wDBTable *dt = qobject_cast<wDBTable*>(object))
        return new wDBTableTaskMenu(dt, parent);

    return 0;
}
