#ifndef MAINFORM_H
#define MAINFORM_H

#include <QMainWindow>
#include <QPointer>

#include "ui_mainform.h"

class MainForm : public QMainWindow, public Ui::MainForm
{
    Q_OBJECT

public:
    MainForm(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::WType_TopLevel);
    ~MainForm();

    aWindowsList *wl;
    QMdiArea *ws;
    QString rcfile;
    QPointer<UsersForm> usersForm;
    QPointer<RolesForm> rolesForm;
    aDatabase *db;
    aCfg cfg;

public slots:
    virtual void fileNew();
    virtual void fileOpen();
    virtual void fileSave();
    virtual void fileSaveAs();
    virtual void filePrint();
    virtual void fileExit();
    virtual void adminUsers();
    virtual bool init();
    virtual void adminRoles();

protected slots:
    virtual void languageChange();

};

#endif // MAINFORM_H
