#ifndef DEDITCFG_H
#define DEDITCFG_H

#include "ui_deditcfg.h"


class dEditCfg : public Q3MainWindow, public Ui::dEditCfg
{
    Q_OBJECT

public:
    dEditCfg(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::WType_TopLevel);
    ~dEditCfg();

    aListViewItem *item;
    aCfgItem globals;

public slots:
    virtual void init();
    virtual void destroy();
    virtual void setData( aListViewItem * o );
    virtual void updateMD();

protected slots:
    virtual void languageChange();

};

#endif // DEDITCFG_H
