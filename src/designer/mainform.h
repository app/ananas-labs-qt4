#ifndef MAINFORM_H
#define MAINFORM_H

#include "ui_mainform.h"


class MainForm : public Q3MainWindow, public Ui::MainForm
{
    Q_OBJECT

public:
    MainForm(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::WType_TopLevel);
    ~MainForm();

    QLabel *msg;
    QWorkspace *ws;
    CfgForm *cfgform;
    QString rcfile;
    aCfg *md;
    int lastFormId;
    int lastTabId;
    aWindowsList *wl;
    //--QSettings designer_settings;

    virtual CfgForm * cfgForm();
    virtual QWidget * activeWindow();

public slots:
    virtual void filePrint();
    virtual void fileExit();
    virtual void editUndo();
    virtual void editRedo();
    virtual void editCut();
    virtual void editCopy();
    virtual void editPaste();
    virtual void editFind();
    virtual void helpIndex();
    virtual void helpContents();
    virtual void helpAbout();
    virtual void init();
    virtual void configNew();
    virtual void configOpen();
    virtual void configSave();
    virtual void configSaveAs();
    virtual void fileOpen();
    virtual void fileSave();
    virtual void fileSaveAs();
    virtual void fileNew();
    virtual void connectSignals( CfgForm * cf );
    virtual void getMd( aCfg * * md );
    virtual int getId();
    virtual void addTab( int uid, const QString & winName );
    // -- Help for add to workspace
    virtual void addTab(QWidget* window) {
        ws->addWindow(window);
        window->show();
        addTab(++lastTabId, window->objectName());
    };
    virtual void removeTab( const QString & winName );
    virtual void closeChildWindows();
    virtual void fileNewAction_activated();

signals:
    void tosave();

protected slots:
    virtual void languageChange();

private:
    Q3PopupMenu *windowsMenu;

private slots:
    virtual void windowsMenuActivated( int id );
    virtual void windowsMenuAboutToShow();
    virtual void tileHorizontal();
    virtual void closeEvent( QCloseEvent * e );

};

#endif // MAINFORM_H
