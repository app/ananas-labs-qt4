#ifndef DEDITAREG_H
#define DEDITAREG_H

#include "ui_deditareg.h"


class dEditAReg : public Q3MainWindow, public Ui::dEditAReg
{
    Q_OBJECT

public:
    dEditAReg(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::WType_TopLevel);
    ~dEditAReg();

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

#endif // DEDITAREG_H
