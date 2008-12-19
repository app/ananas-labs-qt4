#ifndef DEDITTOOLBAR_H
#define DEDITTOOLBAR_H

#include "ui_dedittoolbar.h"


class dEditToolbar : public Q3MainWindow, public Ui::dEditToolbar
{
    Q_OBJECT

public:
    dEditToolbar(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::WType_TopLevel);
    ~dEditToolbar();

    ananasTreeView *vComActions;
    aAliasEditor *al;
    InterfaceListViewItem *item;
    aActionTreeView *actiontree;
    aCfg *md;
    aCfgItem obj;

public slots:
    virtual void updateMD();
    virtual void setData( InterfaceListViewItem * o );
    virtual void bAddAction_clicked();
    virtual void bRemoveAction_clicked();
    virtual void bMoveUp_clicked();
    virtual void bMoveDown_clicked();

protected slots:
    virtual void languageChange();

private:
    void destroy();
    void init();

};

#endif // DEDITTOOLBAR_H
