#ifndef CFGFORM_H
#define CFGFORM_H

#include "ui_cfgform.h"


class CfgForm : public Q3MainWindow, public Ui::CfgForm
{
    Q_OBJECT

public:
    CfgForm(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::WType_TopLevel);
    ~CfgForm();

    Q3IntDict<Q3IconViewItem> idList;
    InterfaceTreeView *interfacetree;
    aActionTreeView *actiontree;
    aCfg cfg;
    QWorkspace *ws;
    aMetadataTreeView *mdtree;
    InterfaceTreeView *toolbartree;
    QString rcfile;
    aWindowsList *wl;

public slots:
    virtual void fileNew();
    virtual void fileOpen();
    virtual void fileSave();
    virtual void fileSaveAs();
    virtual void filePrint();
    virtual void fileExit();
    virtual void init( QString & rcf, bool flagNew );
    virtual void initImageCollection();
    virtual void bAddImage_clicked();
    virtual void bRemoveImage_clicked();
    virtual void vImageCollection_itemRenamed( Q3IconViewItem * item, const QString & name );
    virtual void initLang();
    virtual void initRoles();
    virtual void save();
    virtual void bAdd_clicked();
    virtual void bDelete_clicked();
    virtual void bEdit_clicked();
    virtual void tLang_doubleClicked( int, int, int, const QPoint & );
    virtual void bKill_clicked();
    virtual void newObj();
    virtual void listCfg_onItem( Q3ListViewItem * );
    virtual void newField();
    virtual void newForm();
    virtual void newTable();
    virtual void tabWidget_selected( const QString & tab );
    virtual void bAddRole_clicked();
    virtual void bDeleteRole_clicked();
    virtual void bEditRole_clicked();
    virtual void bKillRole_clicked();
    virtual void tRole_doubleClicked( int, int, int, const QPoint & );

signals:
    void wActivate();
    void wDeactivate();

protected slots:
    virtual void languageChange();

private:
    void destroy();
    virtual void closeEvent( QCloseEvent * e );
    virtual int closeSubWindows();

};

#endif // CFGFORM_H
