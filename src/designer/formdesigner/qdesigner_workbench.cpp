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
#include "qdesigner_workbench.h"
#include "qdesigner_actions.h"
#include "qdesigner_toolwindow.h"
#include "qdesigner_formwindow.h"
#include "qdesigner_settings.h"
#include "qdesigner_widgetbox.h"
#include "qdesigner_propertyeditor.h"
#include "qdesigner_objectinspector.h"
#include "qdesigner_signalsloteditor.h"
#include "qdesigner_actioneditor.h"
#include "qdesigner_resourceeditor.h"

#include <QtDesigner/QtDesigner>
#include <QtDesigner/QDesignerComponents>
#include "private/qdesigner_integration_p.h"
#include "private/pluginmanager_p.h"

#include <QtGui/QDockWidget>
#include <QtGui/QWorkspace>
#include <QtGui/QCloseEvent>
#include <QtGui/QDesktopWidget>
#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QToolBar>
#include <QtGui/QActionGroup>
#include <QtGui/QMessageBox>
#include <QtGui/QHeaderView>
#include <QtGui/QApplication>

#include <QtCore/QVariant>
#include <QtCore/QUrl>
#include <QtCore/QPluginLoader>
#include <QtCore/qdebug.h>

QDesignerWorkbench::QDesignerWorkbench()
    : m_mode(QDesignerWorkbench::NeutralMode), m_workspace(0)
{
    m_initializing = true;
    initialize();

    setUIMode(UIMode(/*QDesignerSettings().uiMode()*/DockedMode));
    m_initializing = false;
}

QDesignerWorkbench::~QDesignerWorkbench()
{
    if (m_mode == DockedMode) {
        Q_ASSERT(m_workspace != 0);
        QMainWindow *mw = qobject_cast<QMainWindow*>(m_workspace->window());
        Q_ASSERT(mw != 0);

        QDesignerSettings settings;
        settings.setMainWindowState(mw->saveState(2));
    }

    while (!m_toolWindows.isEmpty())
        delete m_toolWindows.takeLast();
}

QDesignerWorkbench::UIMode QDesignerWorkbench::mode() const
{
    return m_mode;
}

void QDesignerWorkbench::addToolWindow(QDesignerToolWindow *toolWindow)
{
    Q_ASSERT(m_toolWindowExtras.contains(toolWindow) == false);
    Q_ASSERT(toolWindow->windowTitle().isEmpty() == false);

    m_toolWindows.append(toolWindow);

    if (QAction *action = toolWindow->action()) {
        Q_ASSERT(m_toolMenu->actions().isEmpty() == false);

        QList<QAction*> lst = m_toolActions->actions();
        QAction *before = lst.count() ? lst.last() : m_toolMenu->actions().first();
        m_toolMenu->insertAction(before, action);
        m_toolActions->addAction(action);
    }
}

void QDesignerWorkbench::addFormWindow(QDesignerFormWindow *formWindow)
{
    Q_ASSERT(m_formWindowExtras.contains(formWindow) == false);
    // ### Q_ASSERT(formWindow->windowTitle().isEmpty() == false);

    m_formWindows.append(formWindow);
    if (m_windowActions->actions().isEmpty())
        m_windowMenu->addSeparator();

    if (QAction *action = formWindow->action()) {
        m_windowActions->addAction(action);
        m_windowMenu->addAction(action);
        action->setChecked(true);
    }

    m_actionManager->minimizeAction()->setEnabled(true);
    m_actionManager->editWidgets()->trigger();
}

void QDesignerWorkbench::initialize()
{
    /*QObjectList list = QPluginLoader::staticInstances();
    qDebug() << list.size();
    for (int i=0; i<list.size(); i++)
    {
        qDebug() << list[i]->objectName() << list[i]->metaObject()->className();
        qDebug() << qobject_cast<QDesignerCustomWidgetCollectionInterface *>(list[i]);
    }*/

    QDesignerSettings settings;
    m_core = QDesignerComponents::createFormEditor(this);

    (void) QDesignerComponents::createTaskMenu(core(), this);

    initializeCorePlugins();
    QDesignerComponents::initializePlugins(core());

    m_toolActions = new QActionGroup(this);
    m_toolActions->setExclusive(false);

    m_windowActions = new QActionGroup(this);
    m_windowActions->setExclusive(true);
    connect(m_windowActions, SIGNAL(triggered(QAction*)), this, SLOT(formWindowActionTriggered(QAction*)));

    m_actionManager = new QDesignerActions(this);

    m_globalMenuBar = new QMenuBar;

/*
    m_fileMenu = m_globalMenuBar->addMenu(tr("&File"));
    foreach (QAction *action, m_actionManager->fileActions()->actions()) {
        m_fileMenu->addAction(action);
        if (action->text() == QDesignerActions::tr("&Open Form...")) {
            QMenu *recentFilesMenu = m_fileMenu->addMenu(tr("&Recent Forms"));
            // Pop the "Recent Files" stuff in here.
            foreach(QAction *recentAction, m_actionManager->recentFilesActions()->actions())
                recentFilesMenu->addAction(recentAction);
        }
    }
*/
    m_editMenu = m_globalMenuBar->addMenu(tr("&Edit"));
    foreach (QAction *action, m_actionManager->editActions()->actions()) {
        m_editMenu->addAction(action);
    }

    m_editMenu->addSeparator();

    foreach (QAction *action, m_actionManager->toolActions()->actions()) {
        m_editMenu->addAction(action);
    }

    m_editMenu->addSeparator();
    QMenu *menu = m_editMenu->addMenu(tr("User Interface &Mode"));
    foreach (QAction *action, m_actionManager->uiMode()->actions())
        menu->addAction(action);


    m_formMenu = m_globalMenuBar->addMenu(tr("F&orm"));
    foreach (QAction *action, m_actionManager->formActions()->actions()) {
        m_formMenu->addAction(action);
    }

/*
    QMenu *previewSubMenu = new QMenu(tr("Preview in"), m_formMenu);
    m_formMenu->insertMenu(m_actionManager->previewFormAction(), previewSubMenu);

    foreach (QAction *action, m_actionManager->styleActions()->actions()) {
        previewSubMenu->addAction(action);
    }
*/
    m_toolMenu = m_globalMenuBar->addMenu(tr("&Tools"));

    m_toolMenu->addSeparator();

    m_windowMenu = m_globalMenuBar->addMenu(tr("&Window"));
    foreach (QAction *action, m_actionManager->windowActions()->actions()) {
        m_windowMenu->addAction(action);
    }

/*
    m_helpMenu = m_globalMenuBar->addMenu(tr("&Help"));
    foreach (QAction *action, m_actionManager->helpActions()->actions()) {
        m_helpMenu->addAction(action);
    }
*/
    QDesignerToolWindow *tw = new QDesignerWidgetBox(this);
    tw->setObjectName(QLatin1String("qt_designer_widgetbox"));
    addToolWindow(tw);
    tw = new QDesignerObjectInspector(this);
    tw->setObjectName(QLatin1String("qt_designer_objectinspector"));
    addToolWindow(tw);
    tw = new QDesignerPropertyEditor(this);
    tw->setObjectName(QLatin1String("qt_designer_propertyeditor"));
    addToolWindow(tw);
    tw = new QDesignerSignalSlotEditor(this);
    tw->setObjectName(QLatin1String("qt_designer_signalsloteditor"));
//    addToolWindow(tw);
    tw = new QDesignerResourceEditor(this);
    tw->setObjectName(QLatin1String("qt_designer_resourceeditor"));
//    addToolWindow(tw);
    tw = new QDesignerActionEditor(this);
    tw->setObjectName(QLatin1String("qt_designer_actioneditor"));
//    addToolWindow(tw);

    m_integration = new qdesigner_internal::QDesignerIntegration(core(), this);

    // create the toolbars
    m_editToolBar = new QToolBar;
    m_editToolBar->setObjectName(QLatin1String("editToolBar"));
    m_editToolBar->setWindowTitle(tr("Edit"));
    foreach (QAction *action, m_actionManager->editActions()->actions()) {
        if (action->icon().isNull() == false)
            m_editToolBar->addAction(action);
    }

    m_toolToolBar = new QToolBar;
    m_toolToolBar->setObjectName(QLatin1String("toolsToolBar"));
    m_toolToolBar->setWindowTitle(tr("Tools"));
    foreach (QAction *action, m_actionManager->toolActions()->actions()) {
        if (action->icon().isNull() == false)
            m_toolToolBar->addAction(action);
    }

    m_formToolBar = new QToolBar;
    m_formToolBar->setObjectName(QLatin1String("formToolBar"));
    m_formToolBar->setWindowTitle(tr("Form"));
    foreach (QAction *action, m_actionManager->formActions()->actions()) {
        if (action->icon().isNull() == false)
            m_formToolBar->addAction(action);
    }

    QMenu *toolbarMenu = m_toolMenu->addMenu(tr("Toolbars"));
    toolbarMenu->addAction(m_editToolBar->toggleViewAction());
    toolbarMenu->addAction(m_toolToolBar->toggleViewAction());
    toolbarMenu->addAction(m_formToolBar->toggleViewAction());

    m_geometries.clear();

    emit initialized();

    connect(m_core->formWindowManager(), SIGNAL(activeFormWindowChanged(QDesignerFormWindowInterface*)),
                this, SLOT(updateWindowMenu(QDesignerFormWindowInterface*)));
}

Qt::WindowFlags QDesignerWorkbench::magicalWindowFlags(const QWidget *widgetForFlags) const
{
    switch (m_mode) {
        case TopLevelMode: {
#ifdef Q_WS_MAC
            if (qobject_cast<const QDesignerToolWindow *>(widgetForFlags))
                return Qt::Tool;
#else
            Q_UNUSED(widgetForFlags);
#endif
            return Qt::Window;
        }
        case DockedMode:
            Q_ASSERT(m_workspace != 0);
            return Qt::Window | Qt::WindowShadeButtonHint | Qt::WindowSystemMenuHint | Qt::WindowTitleHint;
        case NeutralMode:
            return Qt::Window;
        default:
            Q_ASSERT(0);
            return 0;
    }
}

QWidget *QDesignerWorkbench::magicalParent() const
{
    switch (m_mode) {
        case TopLevelMode:
            return 0;
        case DockedMode:
            Q_ASSERT(m_workspace != 0);
            return m_workspace;
        case NeutralMode:
            return 0;
        default:
            Q_ASSERT(0);
            return 0;
    }
}

void QDesignerWorkbench::switchToNeutralMode()
{
    if (m_mode == NeutralMode) {
        return;
    } else if (m_mode == DockedMode) {
        Q_ASSERT(m_workspace != 0);
        QMainWindow *mw = qobject_cast<QMainWindow*>(m_workspace->window());
        QDesignerSettings settings;
        settings.setMainWindowState(mw->saveState(2));
    }

    QPoint desktopOffset = QPoint(0, 0);
    QPoint workspaceOffset = QPoint(0, 0);
    if (m_mode == TopLevelMode)
        desktopOffset = QApplication::desktop()->availableGeometry().topLeft();
    else if (m_mode == DockedMode)
        workspaceOffset = m_workspace->mapToGlobal(QPoint(0, 0));
    m_mode = NeutralMode;

    m_geometries.clear();
    m_visibilities.clear();

    foreach (QDesignerToolWindow *tw, m_toolWindows) {
        m_visibilities.insert(tw, tw->isVisible());
        if (tw->isVisible()) {
            // use the actual geometry
            QPoint pos = tw->window()->pos();
            if (!tw->isWindow()) {
                if (const QWidget *pw = tw->parentWidget()) {
                    pos = pw->mapTo(tw->window(), QPoint(0, 0));
                    pos += tw->window()->pos();
                }

            }
            m_geometries.insert(tw, QRect(pos - desktopOffset, tw->size()));
        }
        tw->setSaveSettingsOnClose(false);

        tw->setParent(0);
    }

    foreach (QDesignerFormWindow *fw, m_formWindows) {
        if (fw->isVisible()) {
            // use the actual geometry
            QPoint pos = fw->window()->pos();
            if (!fw->isWindow())
                if (QWidget *p = fw->parentWidget())
                    pos = p->pos(); // in workspace

            m_geometries.insert(fw, QRect(pos - desktopOffset + workspaceOffset,
                                            fw->size()));
        }

        fw->setParent(0);
    }

#ifndef Q_WS_MAC
    m_globalMenuBar->setParent(0);
#endif
    m_editToolBar->setParent(0);
    m_toolToolBar->setParent(0);
    m_formToolBar->setParent(0);
    m_core->setTopLevel(0);
    /*qDesigner->*/setMainWindow(0);

    if (m_workspace)
        delete m_workspace->parentWidget();

    m_workspace = 0;
}

void QDesignerWorkbench::switchToDockedMode()
{
    bool wasTopLevel = (m_mode == TopLevelMode);
    if (m_mode == DockedMode)
        return;

    switchToNeutralMode();
    m_mode = DockedMode;

    QDesignerToolWindow *widgetBoxWrapper = 0;
    if (0 != (widgetBoxWrapper = findToolWindow(core()->widgetBox()))) {
        widgetBoxWrapper->action()->setEnabled(true);
        widgetBoxWrapper->setWindowTitle(tr("Widget Box"));
    }

    Q_ASSERT(m_workspace == 0);

    QDesignerSettings settings;
    QDesignerToolWindow *mw = new QDesignerToolWindow(this); // Just to have a copy of
    mw->setSaveSettingsOnClose(true);
    mw->setObjectName(QLatin1String("MDIWindow"));
    mw->setWindowTitle(tr("Qt Designer"));
    m_workspace = new QWorkspace(mw);
    m_workspace->setAcceptDrops(true);
    m_workspace->installEventFilter(this);
    m_workspace->setScrollBarsEnabled(true);
    connect(m_workspace, SIGNAL(windowActivated(QWidget*)),
            this, SLOT(activateWorkspaceChildWindow(QWidget*)));
    mw->setCentralWidget(m_workspace);
    m_core->setTopLevel(mw);
    (void) mw->statusBar();
    if (m_geometries.isEmpty()) {
        settings.setGeometryFor(mw, qApp->desktop()->availableGeometry(0));
    } else {
        if (QDesignerToolWindow *widgetBox = findToolWindow(core()->widgetBox())) {
            QRect r = m_geometries.value(widgetBox, QRect(0, 0, 200, 200));
            mw->move(r.topLeft());
        }
        mw->setWindowState(mw->windowState() | Qt::WindowMaximized);
    }

#ifndef Q_WS_MAC
    mw->setMenuBar(m_globalMenuBar);
    m_globalMenuBar->show();
#endif
    mw->addToolBar(m_editToolBar);
    mw->addToolBar(m_toolToolBar);
    mw->addToolBar(m_formToolBar);
    m_editToolBar->show();
    m_toolToolBar->show();
    m_formToolBar->show();

    /*qDesigner->*/setMainWindow(mw);

    foreach (QDesignerToolWindow *tw, m_toolWindows) {
        if (wasTopLevel)
            settings.saveGeometryFor(tw);
        QDockWidget *dockWidget = magicalDockWidget(tw);
        if (dockWidget == 0) {
            dockWidget = new QDockWidget(mw);
            dockWidget->setObjectName(tw->objectName() + QLatin1String("_dock"));
            dockWidget->setWindowTitle(tw->windowTitle());
            mw->addDockWidget(tw->dockWidgetAreaHint(), dockWidget);
        }

        dockWidget->setWidget(tw);
    }

    foreach (QDesignerToolWindow *tw, m_toolWindows) {
        QDockWidget *dockWidget = magicalDockWidget(tw);
        tw->setVisible(true);
        dockWidget->setVisible(m_visibilities.value(tw, true));
    }

    mw->restoreState(settings.mainWindowState(), 2);

    foreach (QDesignerFormWindow *fw, m_formWindows)
        m_workspace->addWindow(fw, magicalWindowFlags(fw))->hide();
    // will be shown in adjustFormPositions

    changeBringToFrontVisiblity(false);

    mw->show();

    if (!m_initializing)
        QMetaObject::invokeMethod(this, "adjustFormPositions", Qt::QueuedConnection);
}

bool QDesignerWorkbench::eventFilter(QObject *object, QEvent *event)
{
    if (object == m_workspace) {
        if (event->type() == QEvent::DragEnter) {
            QDragEnterEvent *e = static_cast<QDragEnterEvent*>(event);
            if (e->mimeData()->hasFormat("text/uri-list")) {
                e->acceptProposedAction();
                return true;
            }
        } else if (event->type() == QEvent::Drop) {
            QDropEvent *e = static_cast<QDropEvent*>(event);
            if (!e->mimeData()->hasFormat("text/uri-list"))
                return false;
            foreach (QUrl url, e->mimeData()->urls()) {
                QString fileName = url.toLocalFile();
                if (fileName.endsWith(".ui"))
                    readInForm(url.toLocalFile());
                else
                    QMessageBox::critical(m_workspace, tr("Opening Form"),
                        tr("Cannot open file %1!").arg(fileName));
            }
            e->acceptProposedAction();
            return true;
        }
    }
    return false;
}

void QDesignerWorkbench::adjustFormPositions()
{
    if (m_workspace == 0)
        return;

    QPoint workspace_tl = m_workspace->mapToGlobal(QPoint(0, 0));

    foreach (QDesignerFormWindow *fw, m_formWindows) {
        QWidget *frame = fw->parentWidget();
        if (frame == 0)
            continue;
        QRect g = m_geometries.value(fw, fw->geometryHint());
        frame->move(g.topLeft() - workspace_tl);
        fw->resize(g.size());
        frame->show();
    }
}

void QDesignerWorkbench::changeBringToFrontVisiblity(bool visible)
{
    QAction *btf = m_actionManager->bringToFrontAction();
    QList<QAction *> actionList = m_actionManager->windowActions()->actions();
    btf->setVisible(visible);
    QAction *sep = actionList.at(actionList.indexOf(btf) - 1);
    if (sep->isSeparator())
        sep->setVisible(visible);
}

void QDesignerWorkbench::switchToTopLevelMode()
{
    if (m_mode == TopLevelMode)
        return;

    // make sure that the widgetbox is visible iff it is different from neutrol.
    if (m_mode != NeutralMode) {
        if (QDesignerToolWindow *widgetbox_tool = findToolWindow(core()->widgetBox())) {
            if (!widgetbox_tool->action()->isChecked())
                widgetbox_tool->action()->trigger();
        }
    }

    switchToNeutralMode();
    QPoint desktopOffset = QApplication::desktop()->availableGeometry().topLeft();
    m_mode = TopLevelMode;

    // The widget box is special, it gets the menubar and gets to be the main widget.

    QDesignerToolWindow *widgetBoxWrapper = 0;
    if (0 != (widgetBoxWrapper = findToolWindow(core()->widgetBox()))) {
        m_core->setTopLevel(widgetBoxWrapper);
#ifndef Q_WS_MAC
        widgetBoxWrapper->setMenuBar(m_globalMenuBar);
        widgetBoxWrapper->action()->setEnabled(false);
        widgetBoxWrapper->setSaveSettingsOnClose(true);
        /*qDesigner->*/setMainWindow(widgetBoxWrapper);
        widgetBoxWrapper->setWindowTitle(tr("Qt Designer"));
#endif
        widgetBoxWrapper->addToolBar(m_editToolBar);
        widgetBoxWrapper->addToolBar(m_toolToolBar);
        widgetBoxWrapper->addToolBar(m_formToolBar);

        widgetBoxWrapper->insertToolBarBreak(m_formToolBar);
    }

    QDesignerSettings settings;
    bool found_visible_window = false;
    foreach (QDesignerToolWindow *tw, m_toolWindows) {
        tw->setParent(magicalParent(), magicalWindowFlags(tw));
        settings.setGeometryFor(tw, tw->geometryHint());
        tw->action()->setChecked(tw->isVisible());
        found_visible_window |= tw->isVisible();
    }

    if (!m_toolWindows.isEmpty() && !found_visible_window)
        m_toolWindows.first()->show();

    changeBringToFrontVisiblity(true);

    foreach (QDesignerFormWindow *fw, m_formWindows) {
        fw->setParent(magicalParent(), magicalWindowFlags(fw));
        QRect g = m_geometries.value(fw, fw->geometryHint());
        fw->resize(g.size());
        fw->move(g.topLeft() + desktopOffset);
        fw->show();
    }
}

QDesignerFormWindow *QDesignerWorkbench::createFormWindow()
{
    QDesignerFormWindow *formWindow = new QDesignerFormWindow(/*formWindow=*/ 0, this);

    if (m_workspace) {
        m_workspace->addWindow(formWindow, magicalWindowFlags(formWindow));
    } else {
        formWindow->setParent(magicalParent(), magicalWindowFlags(formWindow));
    }

    formWindow->setAttribute(Qt::WA_DeleteOnClose, true);

    addFormWindow(formWindow);

    QRect g = formWindow->geometryHint();
    formWindow->resize(g.size());
    formWindow->move(availableGeometry().center() - g.center());

    if (m_workspace) {
        m_workspace->setActiveWindow(formWindow);
    }

    return formWindow;
}

QDesignerFormWindowManagerInterface *QDesignerWorkbench::formWindowManager() const
{
    return m_core->formWindowManager();
}

QDesignerFormEditorInterface *QDesignerWorkbench::core() const
{
    return m_core;
}

int QDesignerWorkbench::toolWindowCount() const
{
    return m_toolWindows.count();
}

QDesignerToolWindow *QDesignerWorkbench::toolWindow(int index) const
{
    return m_toolWindows.at(index);
}

int QDesignerWorkbench::formWindowCount() const
{
    return m_formWindows.count();
}

QDesignerFormWindow *QDesignerWorkbench::formWindow(int index) const
{
    return m_formWindows.at(index);
}

QRect QDesignerWorkbench::availableGeometry() const
{
    if (m_workspace)
        return m_workspace->geometry();

    QDesktopWidget *desktop = QApplication::desktop();

    QWidget *window = findToolWindow(core()->widgetBox());
    if (window)
        return desktop->availableGeometry(desktop->screenNumber(window));
    return desktop->availableGeometry(0);
}

int QDesignerWorkbench::marginHint() const
{
    return 20;
}

void QDesignerWorkbench::activateWorkspaceChildWindow(QWidget *widget)
{
    if (QDesignerFormWindow *fw = qobject_cast<QDesignerFormWindow*>(widget)) {
        core()->formWindowManager()->setActiveFormWindow(fw->editor());
        m_workspace->setActiveWindow(widget);
    }
}

void QDesignerWorkbench::removeToolWindow(QDesignerToolWindow *toolWindow)
{
    int index = m_toolWindows.indexOf(toolWindow);
    if (index != -1) {
        m_toolWindows.removeAt(index);
        m_toolWindowExtras.remove(toolWindow);
    }

    if (QAction *action = toolWindow->action()) {
        m_toolActions->removeAction(action);
        //m_toolMenu->removeAction(action); // FIXME Зависает в этом месте при выходе из программы
    }
}

void QDesignerWorkbench::removeFormWindow(QDesignerFormWindow *formWindow)
{
    int index = m_formWindows.indexOf(formWindow);
    if (index != -1) {
        m_formWindows.removeAt(index);
        m_formWindowExtras.remove(formWindow);
    }

    if (QAction *action = formWindow->action()) {
        m_windowActions->removeAction(action);
        m_windowMenu->removeAction(action);
    }

    if (formWindowCount() == 0) {
        m_actionManager->minimizeAction()->setEnabled(false);
        QList<QAction *> actions = m_windowMenu->actions();
        for (int i = actions.size() - 1; i >= 0; --i) {
            QAction *act = actions.at(i);
            if (act->isSeparator()) {
                delete act;
                break;
            }
        }
    }
}

void QDesignerWorkbench::initializeCorePlugins()
{
    QList<QObject*> plugins = QPluginLoader::staticInstances();
    plugins += core()->pluginManager()->instances();

    foreach (QObject *plugin, plugins) {
        if (QDesignerFormEditorPluginInterface *formEditorPlugin = qobject_cast<QDesignerFormEditorPluginInterface*>(plugin)) {
            if (!formEditorPlugin->isInitialized())
                formEditorPlugin->initialize(core());
        }
    }
}

void QDesignerWorkbench::saveSettings() const
{
    QDesignerSettings settings;
    if (m_mode == DockedMode) {
        if (qFindChild<QWorkspace *>(/*qDesigner->*/mainWindow())) {
            settings.saveGeometryFor(/*qDesigner->*/mainWindow());
            settings.setValue(/*qDesigner->*/mainWindow()->objectName() + QLatin1String("/visible"), false);
        }
    } else {
        foreach (QDesignerToolWindow *tw, m_toolWindows) {
            settings.saveGeometryFor(tw);
        }
    }
}

bool QDesignerWorkbench::readInForm(const QString &fileName) const
{
    return m_actionManager->readInForm(fileName);
}

bool QDesignerWorkbench::writeOutForm(QDesignerFormWindowInterface *formWindow, const QString &fileName) const
{
    return m_actionManager->writeOutForm(formWindow, fileName);
}

bool QDesignerWorkbench::saveForm(QDesignerFormWindowInterface *frm)
{
    return m_actionManager->saveForm(frm);
}

QDesignerToolWindow *QDesignerWorkbench::findToolWindow(QWidget *widget) const
{
    foreach (QDesignerToolWindow *toolWindow, m_toolWindows) {
        if (toolWindow->centralWidget() == widget)
            return toolWindow;
    }

    return 0;
}

QDesignerFormWindow *QDesignerWorkbench::findFormWindow(QWidget *widget) const
{
    foreach (QDesignerFormWindow *formWindow, m_formWindows) {
        if (formWindow->centralWidget() == widget)
            return formWindow;
    }

    return 0;
}

bool QDesignerWorkbench::handleClose()
{
    QList<QDesignerFormWindow *> dirtyForms;
    foreach (QDesignerFormWindow *w, m_formWindows) {
        if (w->editor()->isDirty())
            dirtyForms << w;
    }

    if (dirtyForms.size()) {
        if (dirtyForms.size() == 1) {
            if (!dirtyForms.at(0)->close()) {
                return false;
            }
        } else {
            QMessageBox box(tr("Save Forms?"),
                    tr("There are %1 forms with unsaved changes."
                        " Do you want to review these changes before quitting?")
                    .arg(dirtyForms.size()),
                    QMessageBox::Warning,
                    QMessageBox::Yes | QMessageBox::Default, QMessageBox::No,
                    QMessageBox::Cancel | QMessageBox::Escape, 0);
            box.setButtonText(QMessageBox::Yes, tr("Review Changes"));
            box.setButtonText(QMessageBox::No, tr("Discard Changes"));
            switch (box.exec()) {
            case QMessageBox::Cancel:
                return false;
            case QMessageBox::Yes:
               foreach (QDesignerFormWindow *fw, dirtyForms) {
                   fw->show();
                   fw->raise();
                   if (!fw->close()) {
                       return false;
                   }
               }
               break;
            case QMessageBox::No:
              foreach (QDesignerFormWindow *fw, dirtyForms) {
                  fw->editor()->setDirty(false);
                  fw->setWindowModified(false);
              }
              break;
            }
        }
    }

    foreach (QDesignerFormWindow *fw, m_formWindows)
        fw->close();

    saveSettings();
    return true;
}

QDesignerActions *QDesignerWorkbench::actionManager() const
{
    return m_actionManager;
}

void QDesignerWorkbench::setUIMode(UIMode mode)
{
    switch (mode) {
        case TopLevelMode:
            switchToTopLevelMode();
            break;
        case DockedMode:
            switchToDockedMode();
            break;

        default: Q_ASSERT(0);
    }
}

void QDesignerWorkbench::updateWindowMenu(QDesignerFormWindowInterface *fw)
{
    if (!fw)
        return;
    if (QDesignerFormWindow *dfw = qobject_cast<QDesignerFormWindow *>(fw->parentWidget()))
        dfw->action()->setChecked(true);
}

void QDesignerWorkbench::formWindowActionTriggered(QAction *a)
{
    QWidget *widget = a->parentWidget();
    Q_ASSERT(widget != 0);

    if (m_mode == DockedMode) {
        m_workspace->setActiveWindow(widget);
    } else {
        widget->setWindowState(widget->windowState() & ~Qt::WindowMinimized);
        widget->activateWindow();
        widget->raise();
    }
}

void QDesignerWorkbench::showToolBars()
{
    m_toolToolBar->show();
    m_formToolBar->show();
    m_editToolBar->show();
}

void QDesignerWorkbench::closeAllToolWindows()
{
    foreach (QDesignerToolWindow *tw, m_toolWindows)
        tw->hide();
}

QDockWidget *QDesignerWorkbench::magicalDockWidget(QWidget *widget) const
{
    if (!m_workspace)
        return 0;

    QDockWidget *dockWidget = qFindChild<QDockWidget*>(m_workspace->window(), widget->objectName() + QLatin1String("_dock"));
    return dockWidget;
}
