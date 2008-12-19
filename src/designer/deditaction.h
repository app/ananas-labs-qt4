#ifndef DEDITACTION_H
#define DEDITACTION_H

#include "ui_deditaction.h"


class dEditAction : public Q3MainWindow, public Ui::dEditAction
{
    Q_OBJECT

public:
    dEditAction(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::WType_TopLevel);
    ~dEditAction();

    ActionListViewItem * item;
    aAliasEditor *al;
    QStringList otypes;
    QStringList formslist;
    aCfg * md;
    aCfgItem obj;
    QPixmap *dp;

public slots:
    virtual void setData( ActionListViewItem * o );
    virtual void updateMD();
    virtual void bSetActive_clicked();
    virtual void bSetPassive_clicked();
    virtual void bRemoveActive_clicked();
    virtual void bRemovePassive_clicked();

protected:
    virtual void selectStandart();
    virtual void selectUser();

protected slots:
    virtual void languageChange();

    virtual void init();
    virtual void destroy();


private slots:
    virtual void cbTypeChange();
    virtual void cbObjectChange();

};

#endif // DEDITACTION_H
