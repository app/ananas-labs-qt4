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

#include "qdesigner_actions.h"
#include "qdesigner_formwindow.h"
#include "qdesigner_workbench.h"
#include "qdesigner_settings.h"

// sdk
#include <QtDesigner/QtDesigner>

// shared
#include "private/qtundo_p.h"

#include <QtCore/QEvent>
#include <QtCore/QFile>
#include <QtCore/QTimer>
#include <QtCore/qdebug.h>

#include <QtGui/QAction>
#include <QtGui/QCloseEvent>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

QDesignerFormWindow::QDesignerFormWindow(QDesignerFormWindowInterface *editor, QDesignerWorkbench *workbench, QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags),
      m_editor(editor),
      m_workbench(workbench)
{
    Q_ASSERT(workbench);

    if (m_editor) {
        m_editor->setParent(this);
    } else {
        m_editor = workbench->core()->formWindowManager()->createFormWindow(this);
    }

    setCentralWidget(m_editor);

    m_action = new QAction(this);
    m_action->setCheckable(true);

    connect((QObject*)m_editor->commandHistory(), SIGNAL(commandExecuted()), this, SLOT(updateChanged()));
    connect(m_editor, SIGNAL(fileNameChanged(QString)), this, SLOT(updateWindowTitle(QString)));
}

QDesignerFormWindow::~QDesignerFormWindow()
{
    if (workbench())
        workbench()->removeFormWindow(this);
}

QAction *QDesignerFormWindow::action() const
{
    return m_action;
}

void QDesignerFormWindow::changeEvent(QEvent *e)
{
    switch (e->type()) {
        case QEvent::ActivationChange: {
            if (isActiveWindow()) {
                m_action->setChecked(true);
                // ### raise();
            }
        } break;
        case QEvent::WindowTitleChange:
            m_action->setText(windowTitle().replace(QLatin1String("[*]"), ""));
            break;
        case QEvent::WindowIconChange:
            m_action->setIcon(windowIcon());
            break;
        default:
            break;
    }
    QMainWindow::changeEvent(e);
}

QRect QDesignerFormWindow::geometryHint() const
{
    return QRect(0, 0, 400, 300);
}

QDesignerFormWindowInterface *QDesignerFormWindow::editor() const
{
    return m_editor;
}

QDesignerWorkbench *QDesignerFormWindow::workbench() const
{
    return m_workbench;
}

void QDesignerFormWindow::updateWindowTitle(const QString &fileName)
{
    QString fn = fileName;

    if (fn.isEmpty()) {
        // Try to preserve its "untitled" number.
        QRegExp rx(QLatin1String("unnamed( (\\d+))?"));

        if (rx.indexIn(windowTitle()) != -1) {
            fn = rx.cap(0);
        } else {
            fn = QLatin1String("untitled");
        }
    }

    if (QWidget *mc = m_editor->mainContainer()) {
        setWindowIcon(mc->windowIcon());
        setWindowTitle(tr("%1 - %2[*]").arg(mc->windowTitle()).arg(fn));
    } else {
        setWindowTitle(fn);
    }
}

void QDesignerFormWindow::closeEvent(QCloseEvent *ev)
{
    if (m_editor->isDirty()) {
// Сохраняем автоматически
        bool ok = workbench()->saveForm(m_editor);
        ev->setAccepted(ok);
        m_editor->setDirty(!ok);
	
/*        raise();
        QMessageBox box(tr("Save Form?"),
                tr("Do you want to save the changes you made to \"%1\" before closing?")
                .arg(m_editor->fileName().isEmpty() ? action()->text() : m_editor->fileName()),
                QMessageBox::Information,
                QMessageBox::Yes | QMessageBox::Default, QMessageBox::No,
                QMessageBox::Cancel | QMessageBox::Escape, m_editor, Qt::Sheet);
        box.setButtonText(QMessageBox::Yes, m_editor->fileName().isEmpty() ? tr("Save...") : tr("Save"));
        box.setButtonText(QMessageBox::No, tr("Don't Save"));
        switch (box.exec()) {
            case QMessageBox::Yes: {
                bool ok = workbench()->saveForm(m_editor);
                ev->setAccepted(ok);
                m_editor->setDirty(!ok);
                break;
            }
            case QMessageBox::No:
                m_editor->setDirty(false); // Not really necessary, but stops problems if we get close again.
                ev->accept();
                break;
            case QMessageBox::Cancel:
                ev->ignore();
                break;
        }*/
    }

    if (m_workbench->core()->formWindowManager()->formWindowCount() == 1 && ev->isAccepted()
            && QDesignerSettings().showNewFormOnStartup())
        QTimer::singleShot(200, m_workbench->actionManager(), SLOT(createForm()));  // Use timer in case we are quitting.
}

void QDesignerFormWindow::updateChanged()
{
    setWindowModified(m_editor->isDirty());
    updateWindowTitle(m_editor->fileName());
}
