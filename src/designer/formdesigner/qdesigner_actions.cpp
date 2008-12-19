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
#include "qdesigner_toolwindow.h"
#include "qdesigner_actions.h"
#include "qdesigner_workbench.h"
#include "qdesigner_formwindow.h"
#include "qdesigner_settings.h"
#include "newform.h"
//#include "versiondialog.h"
//#include "oublietteview.h"
#include "saveformastemplate.h"
#include "plugindialog.h"
#include "formwindowsettings.h"

// sdk
#include <QtDesigner/QtDesigner>
#include "private/qdesigner_formbuilder_p.h"
#include "private/qtundo_p.h"
#include "private/pluginmanager_p.h"

#include <QtAssistant/QAssistantClient>

#include <QtGui/QStyleFactory>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QCloseEvent>
#include <QtGui/QFileDialog>
#include <QtGui/QMenu>
#include <QtGui/QMessageBox>
#include <QtGui/QIcon>

#include <QtCore/QLibraryInfo>
#include <QtCore/QBuffer>
#include <QtCore/QPluginLoader>
#include <QtCore/qdebug.h>

QDesignerActions::QDesignerActions(QDesignerWorkbench *workbench)
    : QObject(workbench),
      m_workbench(workbench), m_assistantClient(0), m_openDirectory(QString())
{
    Q_ASSERT(m_workbench != 0);

    m_core = m_workbench->core();
    Q_ASSERT(m_core != 0);

    QDesignerFormWindowManagerInterface *formWindowManager = m_core->formWindowManager();
    Q_ASSERT(formWindowManager != 0);

    QDesignerSettings settings;
    m_fileActions = new QActionGroup(this);
    m_fileActions->setExclusive(false);

    m_recentFilesActions = new QActionGroup(this);
    m_recentFilesActions->setExclusive(false);

    m_editActions = new QActionGroup(this);
    m_editActions->setExclusive(false);

    m_formActions = new QActionGroup(this);
    m_formActions->setExclusive(false);

    m_windowActions = new QActionGroup(this);
    m_windowActions->setExclusive(false);

    m_toolActions = new QActionGroup(this);
    m_toolActions->setExclusive(true);

    m_helpActions = new QActionGroup(this);
    m_helpActions->setExclusive(false);


//
// file actions
//
    m_newFormAction = new QAction(tr("&New Form..."), this);
    m_newFormAction->setShortcut(tr("CTRL+N"));
    connect(m_newFormAction, SIGNAL(triggered()), this, SLOT(createForm()));
    m_fileActions->addAction(m_newFormAction);

    m_openFormAction = new QAction(tr("&Open Form..."), this);
    m_openFormAction->setShortcut(tr("CTRL+O"));
    connect(m_openFormAction, SIGNAL(triggered()), this, SLOT(openForm()));
    m_fileActions->addAction(m_openFormAction);

    QAction *act;
    // Need to insert this into the QAction.
    for (int i = 0; i < MaxRecentFiles; ++i) {
        act = new QAction(this);
        act->setVisible(false);
        connect(act, SIGNAL(triggered()), this, SLOT(openRecentForm()));
        m_recentFilesActions->addAction(act);
    }
    updateRecentFileActions();

    act = new QAction(this);
    act->setSeparator(true);
    m_recentFilesActions->addAction(act);

    act = new QAction(tr("Clear &Menu"), this);
    connect(act, SIGNAL(triggered()), this, SLOT(clearRecentFiles()));
    m_recentFilesActions->addAction(act);

    QAction *sep = new QAction(this);
    sep->setSeparator(true);
    m_fileActions->addAction(sep);

    m_saveFormAction = new QAction(tr("&Save Form"), this);
    m_saveFormAction->setShortcut(tr("CTRL+S"));
    connect(m_saveFormAction, SIGNAL(triggered()), this, SLOT(saveForm()));
    m_fileActions->addAction(m_saveFormAction);

    m_saveFormAsAction = new QAction(tr("Save Form &As..."), this);
    connect(m_saveFormAsAction, SIGNAL(triggered()), this, SLOT(saveFormAs()));
    m_fileActions->addAction(m_saveFormAsAction);

    m_saveFormAsTemplateAction = new QAction(tr("Save Form As &Template..."), this);
    connect(m_saveFormAsTemplateAction, SIGNAL(triggered()), this, SLOT(saveFormAsTemplate()));
    m_fileActions->addAction(m_saveFormAsTemplateAction);

    sep = new QAction(this);
    sep->setSeparator(true);
    m_fileActions->addAction(sep);

    m_closeFormAction = new QAction(tr("&Close Form"), this);
    m_closeFormAction->setShortcut(tr("CTRL+W"));
    connect(m_closeFormAction, SIGNAL(triggered()), this, SLOT(closeForm()));
    m_fileActions->addAction(m_closeFormAction);

    sep = new QAction(this);
    sep->setSeparator(true);
    m_fileActions->addAction(sep);

    m_quitAction = new QAction(tr("&Quit"), this);
    m_quitAction->setShortcut(tr("CTRL+Q"));
    connect(m_quitAction, SIGNAL(triggered()),
            this, SLOT(shutdown()));
    m_fileActions->addAction(m_quitAction);

//
// edit actions
//
    m_undoAction = formWindowManager->actionUndo();
    m_undoAction->setShortcut(tr("CTRL+Z"));
    m_editActions->addAction(m_undoAction);

    m_redoAction = formWindowManager->actionRedo();
    m_redoAction->setShortcut(tr("CTRL+SHIFT+Z"));
    m_editActions->addAction(m_redoAction);

    sep = new QAction(this);
    sep->setSeparator(true);
    m_editActions->addAction(sep);

    m_cutAction = formWindowManager->actionCut();
    m_editActions->addAction(m_cutAction);

    m_copyAction = formWindowManager->actionCopy();
    m_editActions->addAction(m_copyAction);

    m_pasteAction = formWindowManager->actionPaste();
    m_editActions->addAction(m_pasteAction);

    m_deleteAction = formWindowManager->actionDelete();
    m_editActions->addAction(m_deleteAction);

    m_selectAllAction = formWindowManager->actionSelectAll();
    m_editActions->addAction(m_selectAllAction);

    sep = new QAction(this);
    sep->setSeparator(true);
    m_editActions->addAction(sep);

    m_sendToBackAction = formWindowManager->actionLower();
    m_editActions->addAction(m_sendToBackAction);

    m_bringToFrontAction = formWindowManager->actionRaise();
    m_editActions->addAction(m_bringToFrontAction);

//
// edit mode actions
//

    m_editWidgetsAction = new QAction(tr("Edit Widgets"), this);
    m_editWidgetsAction->setCheckable(true);
    m_editWidgetsAction->setShortcut(tr("F3"));
    m_editWidgetsAction->setIcon(QIcon(m_core->resourceLocation() + QLatin1String("/widgettool.png")));
    connect(formWindowManager, SIGNAL(activeFormWindowChanged(QDesignerFormWindowInterface*)),
                this, SLOT(activeFormWindowChanged(QDesignerFormWindowInterface*)));
    connect(m_editWidgetsAction, SIGNAL(triggered()), this, SLOT(editWidgetsSlot()));
    m_toolActions->addAction(m_editWidgetsAction);
    m_editWidgetsAction->setChecked(true);
    m_editWidgetsAction->setEnabled(false);
    QList<QObject*> builtinPlugins = QPluginLoader::staticInstances();
    builtinPlugins += m_core->pluginManager()->instances();
    foreach (QObject *plugin, builtinPlugins) {
        if (QDesignerFormEditorPluginInterface *formEditorPlugin = qobject_cast<QDesignerFormEditorPluginInterface*>(plugin)) {
            m_toolActions->addAction(formEditorPlugin->action());
            formEditorPlugin->action()->setCheckable(true);
        }
    }

    m_uiMode = new QActionGroup(this);
    m_uiMode->setExclusive(true);

    m_sdiAction = m_uiMode->addAction(tr("Multiple Top-Level Windows"));
    m_sdiAction->setCheckable(true);

    m_dockedMdiAction = m_uiMode->addAction(tr("Docked Window"));
    m_dockedMdiAction->setCheckable(true);

    switch (settings.uiMode()) {
        default: Q_ASSERT(0); break;

        case QDesignerWorkbench::TopLevelMode:
            m_sdiAction->setChecked(true);
            break;
        case QDesignerWorkbench::DockedMode:
            m_dockedMdiAction->setChecked(true);
            break;
    }

    connect(m_uiMode, SIGNAL(triggered(QAction*)), this, SLOT(updateUIMode(QAction*)));

//
// form actions
//
    m_layoutHorizontallyAction = formWindowManager->actionHorizontalLayout();
    m_formActions->addAction(m_layoutHorizontallyAction);

    m_layoutVerticallyAction = formWindowManager->actionVerticalLayout();
    m_formActions->addAction(m_layoutVerticallyAction);

    m_layoutHorizontallyInSplitterAction = formWindowManager->actionSplitHorizontal();
    m_formActions->addAction(m_layoutHorizontallyInSplitterAction);

    m_layoutVerticallyInSplitterAction = formWindowManager->actionSplitVertical();
    m_formActions->addAction(m_layoutVerticallyInSplitterAction);

    m_layoutGridAction = formWindowManager->actionGridLayout();
    m_formActions->addAction(m_layoutGridAction);

    m_breakLayoutAction = formWindowManager->actionBreakLayout();
    m_formActions->addAction(m_breakLayoutAction);

    m_adjustSizeAction = formWindowManager->actionAdjustSize();
    m_formActions->addAction(m_adjustSizeAction);

    sep = new QAction(this);
    sep->setSeparator(true);

    m_formActions->addAction(sep);

    m_previewFormAction = new QAction(tr("&Preview"), this);
    m_previewFormAction->setShortcut(tr("CTRL+R"));
    connect(m_previewFormAction, SIGNAL(triggered()), this, SLOT(previewFormLater()));
    m_formActions->addAction(m_previewFormAction);

    m_styleActions = new QActionGroup(this);
    m_styleActions->setExclusive(true);
    connect(m_styleActions, SIGNAL(triggered(QAction*)), this, SLOT(previewForm(QAction*)));

    QAction *sep2 = new QAction(this);
    sep2->setSeparator(true);
    m_formActions->addAction(sep2);

    m_formSettings = new QAction(tr("Form &Settings..."), this);
    m_formSettings->setEnabled(false);
    connect(m_formSettings, SIGNAL(triggered()), this, SLOT(showFormSettings()));
    m_formActions->addAction(m_formSettings);

    QStringList availableStyleList = QStyleFactory::keys();
    foreach (QString style, availableStyleList) {
        QAction *a = new QAction(this);
        a->setText(tr("%1 Style").arg(style));
        a->setObjectName(QLatin1String("__qt_action_style_") + style);

        m_styleActions->addAction(a);
    }

//
// window actions
//
    m_minimizeAction = new QAction(tr("&Minimize"), this);
    m_minimizeAction->setEnabled(false);
    m_minimizeAction->setShortcut(tr("CTRL+M"));
    connect(m_minimizeAction, SIGNAL(triggered()), this, SLOT(minimizeForm()));
    m_windowActions->addAction(m_minimizeAction);

    sep = new QAction(this);
    sep->setSeparator(true);
    m_windowActions->addAction(sep);

    m_bringToFrontAction = new QAction(tr("Bring All to Front"), this);
    connect(m_bringToFrontAction, SIGNAL(triggered()), this, SLOT(bringAllToFront()));
    m_windowActions->addAction(m_bringToFrontAction);

//
// Help actions
//

    m_mainHelpAction = new QAction(tr("Qt Designer &Help"), this);
    connect(m_mainHelpAction, SIGNAL(triggered()), this, SLOT(showDesignerHelp()));
    m_mainHelpAction->setShortcut(Qt::CTRL + Qt::Key_Question);
    m_helpActions->addAction(m_mainHelpAction);

    sep = new QAction(this);
    sep->setSeparator(true);
    m_helpActions->addAction(sep);

    m_widgetHelp = new QAction(tr("Current Widget Help"), this);
    m_widgetHelp->setShortcut(Qt::Key_F1);
    connect(m_widgetHelp, SIGNAL(triggered()), this, SLOT(showWidgetSpecificHelp()));
    m_helpActions->addAction(m_widgetHelp);

    sep = new QAction(this);
    sep->setSeparator(true);
    m_helpActions->addAction(sep);

    m_whatsNewAction = new QAction(tr("What's New in Qt Designer?"), this);
    connect(m_whatsNewAction, SIGNAL(triggered()), this, SLOT(showWhatsNew()));
    m_helpActions->addAction(m_whatsNewAction);

    // On Mac OS X, the about items are merged in so this separator is redundant.
#ifndef Q_WS_MAC
    sep = new QAction(this);
    sep->setSeparator(true);
    m_helpActions->addAction(sep);
    m_aboutPluginsAction = new QAction(tr("About Plugins"), this);
#else
    m_aboutPluginsAction = new QAction(tr("Plugin Information"), this);
#endif
    connect(m_aboutPluginsAction, SIGNAL(triggered()), this, SLOT(aboutPlugins()));
    m_helpActions->addAction(m_aboutPluginsAction);

    m_aboutDesignerAction = new QAction(tr("About Qt Designer"), this);
    connect(m_aboutDesignerAction, SIGNAL(triggered()), this, SLOT(aboutDesigner()));
    m_helpActions->addAction(m_aboutDesignerAction);

    m_aboutQtAction = new QAction(tr("About Qt"), this);
    connect(m_aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    m_helpActions->addAction(m_aboutQtAction);
//
// connections
//
    fixActionContext();
    activeFormWindowChanged(core()->formWindowManager()->activeFormWindow());
}

QDesignerActions::~QDesignerActions()
{
}

QActionGroup *QDesignerActions::toolActions() const
{ return m_toolActions; }

QDesignerWorkbench *QDesignerActions::workbench() const
{ return m_workbench; }

QDesignerFormEditorInterface *QDesignerActions::core() const
{ return m_core; }

QActionGroup *QDesignerActions::fileActions() const
{ return m_fileActions; }

QActionGroup *QDesignerActions::editActions() const
{ return m_editActions; }

QActionGroup *QDesignerActions::formActions() const
{ return m_formActions; }

QActionGroup *QDesignerActions::windowActions() const
{ return m_windowActions; }

QActionGroup *QDesignerActions::helpActions() const
{ return m_helpActions; }

QActionGroup *QDesignerActions::styleActions() const
{ return m_styleActions; }

QAction *QDesignerActions::newFormAction() const
{ return m_newFormAction; }

QAction *QDesignerActions::openFormAction() const
{ return m_openFormAction; }

QAction *QDesignerActions::saveFormAction() const
{ return m_saveFormAction; }

QAction *QDesignerActions::saveFormAsAction() const
{ return m_saveFormAsAction; }

QAction *QDesignerActions::saveFormAsTemplateAction() const
{ return m_saveFormAsTemplateAction; }

QAction *QDesignerActions::closeFormAction() const
{ return m_closeFormAction; }

QAction *QDesignerActions::quitAction() const
{ return m_quitAction; }

QAction *QDesignerActions::undoAction() const
{ return m_undoAction; }

QAction *QDesignerActions::redoAction() const
{ return m_redoAction; }

QAction *QDesignerActions::cutAction() const
{ return m_cutAction; }

QAction *QDesignerActions::copyAction() const
{ return m_copyAction; }

QAction *QDesignerActions::pasteAction() const
{ return m_pasteAction; }

QAction *QDesignerActions::selectAllAction() const
{ return m_selectAllAction; }

QAction *QDesignerActions::deleteAction() const
{ return m_deleteAction; }

QAction *QDesignerActions::sendToBackAction() const
{ return m_sendToBackAction; }

QAction *QDesignerActions::bringToFrontAction() const
{ return m_bringToFrontAction; }

QAction *QDesignerActions::mainHelpAction() const
{ return m_mainHelpAction; }

QAction *QDesignerActions::whatsNewAction() const
{ return m_whatsNewAction; }

QAction *QDesignerActions::aboutQtAction() const
{ return m_aboutQtAction; }

QAction *QDesignerActions::aboutDesignerAction() const
{ return m_aboutDesignerAction; }

QAction *QDesignerActions::layoutHorizontallyAction() const
{ return m_layoutHorizontallyAction; }

QAction *QDesignerActions::layoutVerticallyAction() const
{ return m_layoutVerticallyAction; }

QAction *QDesignerActions::layoutHorizontallyInSplitterAction() const
{ return m_layoutHorizontallyInSplitterAction; }

QAction *QDesignerActions::layoutVerticallyInSplitterAction() const
{ return m_layoutVerticallyInSplitterAction; }

QAction *QDesignerActions::layoutGridAction() const
{ return m_layoutGridAction; }

QAction *QDesignerActions::breakLayoutAction() const
{ return m_breakLayoutAction; }

QAction *QDesignerActions::adjustSizeAction() const
{ return m_adjustSizeAction; }

QAction *QDesignerActions::previewFormAction() const
{ return m_previewFormAction; }

QAction *QDesignerActions::formSettings() const
{ return m_formSettings; }

void QDesignerActions::editWidgetsSlot()
{
    QDesignerFormWindowManagerInterface *formWindowManager = core()->formWindowManager();
    for (int i=0; i<formWindowManager->formWindowCount(); ++i) {
        QDesignerFormWindowInterface *formWindow = formWindowManager->formWindow(i);
        formWindow->editWidgets();
    }
}

void QDesignerActions::createForm()
{
    NewForm *dlg = new NewForm(workbench(), 0);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->setAttribute(Qt::WA_ShowModal);
    dlg->show();
}

bool QDesignerActions::openForm()
{
    QString fileName = QFileDialog::getOpenFileName(
            core()->topLevel(),
            tr("Open Form"), m_openDirectory,
            tr("Designer UI files (*.ui)"), 0, QFileDialog::DontUseSheet);

    if (!fileName.isEmpty()) {
        return readInForm(fileName);
    }
    return false;
}

bool QDesignerActions::saveFormAs(QDesignerFormWindowInterface *fw)
{
    QString fileName = fw->fileName().isEmpty() ? QDir::current().absolutePath()
            + QLatin1String("/untitled.ui") : fw->fileName();
    QString saveFile = QFileDialog::getSaveFileName(fw, tr("Save form as"),
            fileName,
            tr("Designer UI files (*.ui)"));
    if (saveFile.isEmpty())
        return false;

    if (QFileInfo(saveFile).suffix() != QLatin1String("ui"))
        saveFile.append(QLatin1String(".ui"));

    fw->setFileName(saveFile);
    return writeOutForm(fw, saveFile);
}

void QDesignerActions::saveForm()
{
    if (QDesignerFormWindowInterface *fw = core()->formWindowManager()->activeFormWindow())
        saveForm(fw);
}

bool QDesignerActions::saveForm(QDesignerFormWindowInterface *fw)
{
    bool ret;
    if (fw->fileName().isEmpty())
        ret = saveFormAs(fw);
    else
        ret =  writeOutForm(fw, fw->fileName());
    return ret;
}

void QDesignerActions::closeForm()
{
    if (QDesignerFormWindowInterface *fw = core()->formWindowManager()->activeFormWindow())
        fw->parentWidget()->close();
}

void QDesignerActions::saveFormAs()
{
    if (QDesignerFormWindowInterface *fw = core()->formWindowManager()->activeFormWindow())
        saveFormAs(fw);
}

void QDesignerActions::saveFormAsTemplate()
{
    if (QDesignerFormWindowInterface *fw = core()->formWindowManager()->activeFormWindow()) {
        SaveFormAsTemplate dlg(fw, fw->window());
        dlg.exec();
    }
}

void QDesignerActions::notImplementedYet()
{
    QMessageBox::information(core()->topLevel(), tr("Designer"), tr("Feature not implemented yet!"));
}

void QDesignerActions::updateUIMode(QAction *act)
{
    QDesignerWorkbench::UIMode mode = QDesignerWorkbench::TopLevelMode;
    if (act == m_dockedMdiAction)
        mode = QDesignerWorkbench::DockedMode;

    QDesignerSettings settings;
    settings.setUIMode(mode);

    m_workbench->setUIMode(QDesignerWorkbench::UIMode(settings.uiMode()));
}

void QDesignerActions::previewFormLater(QAction *action)
{
    qRegisterMetaType<QAction*>("QAction*");
    QMetaObject::invokeMethod(this, "previewForm", Qt::QueuedConnection,
                                Q_ARG(QAction*, action));
}

void QDesignerActions::previewForm(QAction *action)
{
//return;

    if (QDesignerFormWindowInterface *fw = core()->formWindowManager()->activeFormWindow()) {
        qdesigner_internal::QDesignerFormBuilder builder(core(), qdesigner_internal::QDesignerFormBuilder::DisableScripts);
        builder.setWorkingDirectory(fw->absoluteDir());

        QByteArray bytes = fw->contents().toUtf8();
        QBuffer buffer(&bytes);

        QWidget *widget = builder.load(&buffer, 0);
        Q_ASSERT(widget);


        widget->setParent(fw->window(), Qt::Dialog);
        widget->setWindowModality(Qt::ApplicationModal);
        widget->setAttribute(Qt::WA_DeleteOnClose, true);
        widget->move(fw->mapToGlobal(QPoint(0, 0)) + QPoint(10, 10));

        QStyle *style = 0;

        if (action != 0 && action->objectName().startsWith(QLatin1String("__qt_action_style_"))) {
            QString styleName = action->objectName().mid(QString::fromUtf8("__qt_action_style_").count());
            style = QStyleFactory::create(styleName);

            if (style != 0) {
                style->setParent(widget);
                widget->setStyle(style);
                widget->setPalette(style->standardPalette());
                QList<QWidget*> lst = qFindChildren<QWidget*>(widget);
                foreach (QWidget *w, lst) {
                    if (w->windowType() == Qt::Popup)
                        w->setPalette(style->standardPalette());
                    w->setStyle(style);
                }
            }
        }

        widget->setWindowTitle(tr("%1 - [Preview]").arg(widget->windowTitle()));

        widget->show();
    }
}

void QDesignerActions::fixActionContext()
{
    QList<QAction*> actions;
    actions += m_fileActions->actions();
    actions += m_editActions->actions();
    actions += m_toolActions->actions();
    actions += m_formActions->actions();
    actions += m_windowActions->actions();
    actions += m_helpActions->actions();

    foreach (QAction *a, actions) {
        a->setShortcutContext(Qt::ApplicationShortcut);
    }
}

bool QDesignerActions::readInForm(const QString &fileName)
{
    // First make sure that we don't have this one open already.
    QDesignerFormWindowManagerInterface *formWindowManager = core()->formWindowManager();
    int totalWindows = formWindowManager->formWindowCount();
    for (int i = 0; i < totalWindows; ++i) {
        QDesignerFormWindowInterface *w = formWindowManager->formWindow(i);
        if (w->fileName() == fileName) {
            w->raise();
            formWindowManager->setActiveFormWindow(w);
            addRecentFile(fileName);
            return true;
        }
    }

    // Otherwise load it.
    QFile f(fileName);
    if (!f.open(QFile::ReadOnly)) {
        QMessageBox::warning(core()->topLevel(), tr("Read Error"), tr("Couldn't open file: %1\nReason: %2")
                .arg(f.fileName()).arg(f.errorString()));
        return false;
    }

    m_openDirectory = QFileInfo(f).absolutePath();

    QDesignerFormWindow *formWindow = workbench()->createFormWindow();
    if (QDesignerFormWindowInterface *editor = formWindow->editor()) {
        editor->setFileName(fileName);
        editor->setContents(&f);
        Q_ASSERT(editor->mainContainer() != 0);
        formWindow->updateWindowTitle(fileName);
        formWindow->resize(editor->mainContainer()->size());
        formWindowManager->setActiveFormWindow(editor);
    }
    formWindow->show();
    addRecentFile(fileName);
    formWindow->editor()->setDirty(false);
    return true;
}

bool QDesignerActions::writeOutForm(QDesignerFormWindowInterface *fw, const QString &saveFile)
{
    Q_ASSERT(fw && !saveFile.isEmpty());
    QFile f(saveFile);
    while (!f.open(QFile::WriteOnly)) {
        QMessageBox box(tr("Save Form?"),
                        tr("Could not open file: %1"
                                "\nReason: %2"
                                "\nWould you like to retry or change your file?")
                                .arg(f.fileName()).arg(f.errorString()),
                        QMessageBox::Warning,
                        QMessageBox::Yes | QMessageBox::Default, QMessageBox::No,
                        QMessageBox::Cancel | QMessageBox::Escape, fw, Qt::Sheet);
        box.setButtonText(QMessageBox::Yes, tr("Retry"));
        box.setButtonText(QMessageBox::No, tr("Select New File"));
        switch(box.exec()) {
            case QMessageBox::Yes:
                break;
                case QMessageBox::No: {
                    QString fileName = QFileDialog::getSaveFileName(fw, tr("Save form as"),
                            QDir::current().absolutePath(), QLatin1String("*.ui"));
                    if (fileName.isEmpty())
                        return false;
                    f.setFileName(fileName);
                    fw->setFileName(fileName);
                    break; }
            case QMessageBox::Cancel:
                return false;
        }
    }
    QByteArray utf8Array = fw->contents().toUtf8();
    while (f.write(utf8Array, utf8Array.size()) != utf8Array.size()) {
        QMessageBox box(tr("Save Form?"),
                        tr("Could not write file: %1\nReason:%2\nWould you like to retry?")
                                .arg(f.fileName()).arg(f.errorString()),
                        QMessageBox::Warning,
                        QMessageBox::Yes | QMessageBox::Default, QMessageBox::No, 0,
                        fw, Qt::Sheet);
        box.setButtonText(QMessageBox::Yes, tr("Retry"));
        box.setButtonText(QMessageBox::No, tr("Don't Retry"));
        switch(box.exec()) {
            case QMessageBox::Yes:
                f.resize(0);
                break;
            case QMessageBox::No:
                return false;
        }
    }
    addRecentFile(saveFile);
    fw->setDirty(false);
    fw->parentWidget()->setWindowModified(false);
    return true;
}

void QDesignerActions::shutdown()
{

    // Follow the idea from the Mac, i.e. send the Application a close event
    // and if it's accepted, quit.
    /*QCloseEvent ev;
    QApplication::sendEvent(qDesigner, &ev);
    if (ev.isAccepted())
        qDesigner->quit();*/
}

void QDesignerActions::activeFormWindowChanged(QDesignerFormWindowInterface *formWindow)
{
    bool enable = formWindow != 0;

    m_saveFormAction->setEnabled(enable);
    m_saveFormAsAction->setEnabled(enable);
    m_saveFormAsTemplateAction->setEnabled(enable);
    m_closeFormAction->setEnabled(enable);

    m_editWidgetsAction->setEnabled(enable);
    m_formSettings->setEnabled(enable);

    m_previewFormAction->setEnabled(enable);
    m_styleActions->setEnabled(enable);
}

void QDesignerActions::updateRecentFileActions()
{
    QDesignerSettings settings;
    QStringList files = settings.recentFilesList();
    int originalSize = files.size();
    int numRecentFiles = qMin(files.size(), int(MaxRecentFiles));
    QList<QAction *> recentFilesActs = m_recentFilesActions->actions();

    for (int i = 0; i < numRecentFiles; ++i) {
        QFileInfo fi(files[i]);
        // If the file doesn't exist anymore, just remove it from the list so
        // people don't get confused.
        if (!fi.exists()) {
            files.removeAt(i);
            --i;
            numRecentFiles = qMin(files.size(), int(MaxRecentFiles));
            continue;
        }
        QString text = fi.fileName();
        recentFilesActs[i]->setText(text);
        recentFilesActs[i]->setIconText(files[i]);
        recentFilesActs[i]->setVisible(true);
    }

    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFilesActs[j]->setVisible(false);

    // If there's been a change, right it back
    if (originalSize != files.size())
        settings.setRecentFilesList(files);
}

void QDesignerActions::openRecentForm()
{
    if (QAction *action = qobject_cast<QAction *>(sender())) {
        if (!readInForm(action->iconText()))
            updateRecentFileActions(); // File doesn't exist, remove it from settings
    }
}

void QDesignerActions::clearRecentFiles()
{
    QDesignerSettings settings;
    settings.setRecentFilesList(QStringList());
    updateRecentFileActions();
}

QActionGroup *QDesignerActions::recentFilesActions() const
{
    return m_recentFilesActions;
}

void QDesignerActions::addRecentFile(const QString &fileName)
{
    QDesignerSettings settings;
    QStringList files = settings.recentFilesList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > MaxRecentFiles)
        files.removeLast();

    settings.setRecentFilesList(files);
    updateRecentFileActions();
}

void QDesignerActions::minimizeForm()
{
    if (QDesignerFormWindowInterface *fw = core()->formWindowManager()->activeFormWindow()) {
        if (m_workbench->mode() == QDesignerWorkbench::DockedMode) {
            // Yuck, I need to get to the QWorkspaceChild::showShaded(), but there is no way
            // to do that legally, so I use the QMetaObject as my guide.
            QMetaObject::invokeMethod(fw->parentWidget()->parentWidget(), "showShaded");
        } else {
            fw->parentWidget()->showMinimized();
        }
    }
}

void QDesignerActions::bringAllToFront()
{
    int i;
    for (i = 0; i < m_workbench->formWindowCount(); ++i)
        m_workbench->formWindow(i)->raise();

    for (i = 0; i < m_workbench->toolWindowCount(); ++i)
        m_workbench->toolWindow(i)->raise();
}

QAction *QDesignerActions::minimizeAction() const
{
    return m_minimizeAction;
}

QAction *QDesignerActions::bringAllToFront() const
{
    return m_bringToFrontAction;
}

void QDesignerActions::showDesignerHelp()
{
    showHelp(QLatin1String("designer-manual.html"));
}

void QDesignerActions::showWhatsNew()
{
    showHelp(QLatin1String("qt4-designer.html"));
}

void QDesignerActions::showHelp(const QString &url)
{
    /*if (!m_assistantClient)
        m_assistantClient
            = new QAssistantClient(QLibraryInfo::location(QLibraryInfo::BinariesPath), this);

    QString filePath = QLibraryInfo::location(QLibraryInfo::DocumentationPath)
                                + QLatin1String("/html/") + url;

    QString cleanFilePath;
    int index = filePath.lastIndexOf(QLatin1Char('#'));
    if (index != -1)
        cleanFilePath = filePath.left(index);
    else
        cleanFilePath = filePath;

    if (!QFile::exists(cleanFilePath)) {
        filePath = QLibraryInfo::location(QLibraryInfo::DocumentationPath)
                                + QLatin1String("/html/designer-manual.html");
    }

    m_assistantClient->showPage(filePath);*/
}

void QDesignerActions::aboutDesigner()
{
    /*VersionDialog mb(core()->topLevel());
    mb.setWindowTitle(tr("About Qt Designer"));
    if (mb.exec()) {
        OublietteView *oubliette = new OublietteView;
        oubliette->setAttribute(Qt::WA_DeleteOnClose);
        oubliette->setMinimumSize(800, 600);
        oubliette->show();
    }*/
}

QActionGroup *QDesignerActions::uiMode() const
{
    return m_uiMode;
}


QAction *QDesignerActions::editWidgets() const
{
    return m_editWidgetsAction;
}

void QDesignerActions::showWidgetSpecificHelp()
{
    QDesignerFormWindowInterface *fw = core()->formWindowManager()->activeFormWindow();
    if (!fw) {
        showDesignerHelp();
        return;
    }

    QString className;
    QString currentPropertyName;

    currentPropertyName = core()->propertyEditor()->currentPropertyName();
    if (!currentPropertyName.isEmpty()) {
        QDesignerPropertySheetExtension *ps
            = qt_extension<QDesignerPropertySheetExtension *>(core()->extensionManager(),
                                                            core()->propertyEditor()->object());
        if (!ps)
            ps = qt_extension<QDesignerPropertySheetExtension *>(core()->extensionManager(),
                                                            fw->cursor()->selectedWidget(0));
        Q_ASSERT(ps);
        className = ps->propertyGroup(ps->indexOf(currentPropertyName));
    } else {
        QDesignerWidgetDataBaseInterface *db = core()->widgetDataBase();
        QDesignerWidgetDataBaseItemInterface *dbi = db->item(db->indexOfObject(fw->cursor()->selectedWidget(0), true));
        className = dbi->name();
    }

    // ### generalize using the Widget Data Base
    if (className == QLatin1String("Line"))
        className = QLatin1String("QFrame");
    else if (className == QLatin1String("Spacer"))
        className = QLatin1String("QSpacerItem");
    else if (className == QLatin1String("QLayoutWidget"))
        className = QLatin1String("QLayout");

    QString url = className.toLower();

    // special case
    url += QLatin1String(".html");

    if (!currentPropertyName.isEmpty())
        url += QLatin1Char('#') + currentPropertyName;

    showHelp(url);
}

QAction *QDesignerActions::widgetHelpAction() const
{
    return m_widgetHelp;
}

void QDesignerActions::aboutPlugins()
{
    PluginDialog dlg(core(), core()->topLevel());
    dlg.exec();
}

void QDesignerActions::showFormSettings()
{
    QDesignerFormWindowInterface *formWindow = core()->formWindowManager()->activeFormWindow();
    QDesignerFormWindow *window = m_workbench->findFormWindow(formWindow);
    FormWindowSettings dlg(formWindow);
    if (dlg.exec() && window) {
        formWindow->setDirty(true);
        window->updateChanged();
    }
}
