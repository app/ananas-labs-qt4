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

#ifndef QDESIGNER_FORMWINDOW_H
#define QDESIGNER_FORMWINDOW_H

#include <QtCore/QPointer>
#include <QtGui/QMainWindow>

class QDesignerWorkbench;
class QDesignerFormWindowInterface;

class QDesignerFormWindow: public QMainWindow
{
    Q_OBJECT
public:
    QDesignerFormWindow(QDesignerFormWindowInterface *formWindow, QDesignerWorkbench *workbench,
                        QWidget *parent = 0, Qt::WindowFlags flags = 0);

    virtual ~QDesignerFormWindow();

    QAction *action() const;
    QDesignerWorkbench *workbench() const;
    QDesignerFormWindowInterface *editor() const;

    virtual QRect geometryHint() const;

public slots:
    void updateWindowTitle(const QString &fileName);
    void updateChanged();

protected:
    virtual void changeEvent(QEvent *e);
    virtual void closeEvent(QCloseEvent *ev);

private:
    QDesignerFormWindowInterface *m_editor;
    QPointer<QDesignerWorkbench> m_workbench;
    QAction *m_action;
};

#endif // QDESIGNER_FORMWINDOW_H
