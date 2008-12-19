#ifndef DEDITCOLUMNS_H
#define DEDITCOLUMNS_H

#include "ui_deditcolumns.h"


class dEditColumns : public Q3MainWindow, public Ui::dEditColumns
{
    Q_OBJECT

public:
    dEditColumns(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::WType_TopLevel);
    ~dEditColumns();

public slots:
    virtual void setData( ananasListViewItem * o );
    virtual void updateMD();
    virtual void bAddAction_clicked();
    virtual void bRemoveAction_clicked();
    virtual void bMoveUp_clicked();
    virtual void bMoveDown_clicked();

protected slots:
    virtual void languageChange();

private:
    aCfgItem obj;
    aCfg *md;
    aAliasEditor *al;
    ananasListViewItem * item;
    ananasTreeView *availableFields;
    ananasTreeView *usedFields;
    QStringList fieldsList;

    void destroy();
    void init();
    virtual void addField( aCfgItem addItem );

};

#endif // DEDITCOLUMNS_H
