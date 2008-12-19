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

#ifndef QDESIGNER_ACTIONS_H
#define QDESIGNER_ACTIONS_H

#include <QtCore/QPointer>
#include <QtCore/QObject>
#include <QtGui/QAction>

class QDesignerMainWindow;
class QDesignerWorkbench;

class QAction;
class QActionGroup;
class QDesignerFormEditorInterface;
class QDesignerFormWindowInterface;
class QAssistantClient;

class QDesignerActions: public QObject
{
    Q_OBJECT
public:
    QDesignerActions(QDesignerWorkbench *mainWindow);
    virtual ~QDesignerActions();

    QDesignerWorkbench *workbench() const;
    QDesignerFormEditorInterface *core() const;

    bool saveForm(QDesignerFormWindowInterface *fw);
    bool readInForm(const QString &fileName);
    bool writeOutForm(QDesignerFormWindowInterface *formWindow, const QString &fileName);

    QActionGroup *fileActions() const;
    QActionGroup *recentFilesActions() const;
    QActionGroup *editActions() const;
    QActionGroup *formActions() const;
    QActionGroup *windowActions() const;
    QActionGroup *toolActions() const;
    QActionGroup *helpActions() const;
    QActionGroup *uiMode() const;
    QActionGroup *styleActions() const;

//
// file actions
//
    QAction *newFormAction() const;
    QAction *openFormAction() const;
    QAction *saveFormAction() const;
    QAction *saveFormAsAction() const;
    QAction *saveFormAsTemplateAction() const;
    QAction *closeFormAction() const;
    QAction *quitAction() const;

//
// edit actions
//
    QAction *undoAction() const;
    QAction *redoAction() const;
    QAction *cutAction() const;
    QAction *copyAction() const;
    QAction *pasteAction() const;
    QAction *deleteAction() const;
    QAction *selectAllAction() const;
    QAction *sendToBackAction() const;
    QAction *bringToFrontAction() const;

//
// edit mode actions
//
    QAction *editWidgets() const;
    QAction *editConnections() const;
    QAction *editTabOrders() const;
    QAction *editBuddies() const;

//
// form actions
//
    QAction *layoutHorizontallyAction() const;
    QAction *layoutVerticallyAction() const;
    QAction *layoutHorizontallyInSplitterAction() const;
    QAction *layoutVerticallyInSplitterAction() const;
    QAction *layoutGridAction() const;
    QAction *breakLayoutAction() const;
    QAction *adjustSizeAction() const;
    QAction *previewFormAction() const;
    QAction *formSettings() const;

//
// window actions
//
    QAction *minimizeAction() const;
    QAction *bringAllToFront() const;

//
// help actions
//
    QAction *mainHelpAction() const;
    QAction *widgetHelpAction() const;
    QAction *whatsNewAction() const;
    QAction *aboutQtAction() const;
    QAction *aboutDesignerAction() const;

public slots:
    void activeFormWindowChanged(QDesignerFormWindowInterface *formWindow);
    void createForm();
    bool openForm();

signals:
    void useBigIcons(bool);

private slots:
    void saveForm();
    void saveFormAs();
    void saveFormAsTemplate();
    void previewForm(QAction *action = 0);
    void previewFormLater(QAction *action = 0);
    void notImplementedYet();
    void shutdown();
    void editWidgetsSlot();
    void openRecentForm();
    void clearRecentFiles();
    void closeForm();
    void minimizeForm();
    void bringAllToFront();
    void showDesignerHelp();
    void showWhatsNew();
    void aboutPlugins();
    void aboutDesigner();
    void updateUIMode(QAction *act);
    void showWidgetSpecificHelp();
    void showFormSettings();

private:
    bool saveFormAs(QDesignerFormWindowInterface *fw);
    void fixActionContext();
    void updateRecentFileActions();
    void addRecentFile(const QString &fileName);
    void showHelp(const QString &help);

private:
    enum { MaxRecentFiles = 10 };
    QDesignerWorkbench *m_workbench;
    QDesignerFormEditorInterface *m_core;
    QAssistantClient *m_assistantClient;
    QString m_openDirectory;

    QActionGroup *m_fileActions;
    QActionGroup *m_recentFilesActions;
    QActionGroup *m_editActions;
    QActionGroup *m_formActions;
    QActionGroup *m_windowActions;
    QActionGroup *m_toolActions;
    QActionGroup *m_helpActions;
    QActionGroup *m_uiMode;
    QActionGroup *m_styleActions;

    QAction *m_editWidgetsAction;

    QAction *m_newFormAction;
    QAction *m_openFormAction;
    QAction *m_saveFormAction;
    QAction *m_saveFormAsAction;
    QAction *m_saveFormAsTemplateAction;
    QAction *m_closeFormAction;

    QAction *m_quitAction;
    QAction *m_undoAction;
    QAction *m_redoAction;
    QAction *m_cutAction;
    QAction *m_copyAction;
    QAction *m_pasteAction;
    QAction *m_deleteAction;
    QAction *m_sendToBackAction;
    QAction *m_bringToFrontAction;
    QAction *m_selectAllAction;


    QAction *m_layoutHorizontallyAction;
    QAction *m_layoutVerticallyAction;
    QAction *m_layoutHorizontallyInSplitterAction;
    QAction *m_layoutVerticallyInSplitterAction;
    QAction *m_layoutGridAction;
    QAction *m_breakLayoutAction;
    QAction *m_adjustSizeAction;
    QAction *m_previewFormAction;
    QAction *m_formSettings;

    QAction *m_minimizeAction;
    QAction *m_bringAllToFrontAction;

    QAction *m_mainHelpAction;
    QAction *m_widgetHelp;
    QAction *m_whatsNewAction;
    QAction *m_aboutQtAction;
    QAction *m_aboutPluginsAction;
    QAction *m_aboutDesignerAction;
    QAction *m_sdiAction;
    QAction *m_dockedMdiAction;
};

#endif // QDESIGNER_ACTIONS_H

