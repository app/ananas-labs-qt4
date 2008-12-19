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

#ifndef WDBFIELD_TASKMENU_H
#define WDBFIELD_TASKMENU_H

#include <QDesignerTaskMenuExtension>
#include <QExtensionFactory>


class QExtensionManager;
class QAction;
class wDBField;

class wDBFieldTaskMenu: public QObject, public QDesignerTaskMenuExtension
{
    Q_OBJECT
    Q_INTERFACES(QDesignerTaskMenuExtension)
public:
    wDBFieldTaskMenu(wDBField *widget, QObject *parent);

    QAction *preferredEditAction() const;
    QList<QAction *> taskActions() const;

private slots:
    void edit();

private:
    QAction *m_editAction;
    wDBField *m_widget;
};

class wDBFieldTaskMenuFactory: public QExtensionFactory
{
    Q_OBJECT
public:
    wDBFieldTaskMenuFactory(QExtensionManager *parent = 0);

protected:
    QObject *createExtension(QObject *object, const QString &iid, QObject *parent) const;
};

#endif // WDBFIELD_TASKMENU_H
