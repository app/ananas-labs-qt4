#ifndef DEDITIREG_H
#define DEDITIREG_H

#include "ui_deditireg.h"


class dEditIReg : public Q3MainWindow, public Ui::dEditIReg
{
    Q_OBJECT

public:
    dEditIReg(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::WType_TopLevel);
    ~dEditIReg();

    aListViewItem *item;
    aAliasEditor *al;

public slots:
    virtual void setData( aListViewItem * o );
    virtual void updateMD();

protected slots:
    virtual void languageChange();

private:
    void init();
    void destroy();

};

#endif // DEDITIREG_H
