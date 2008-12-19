#ifndef DEDITREPORT_H
#define DEDITREPORT_H

#include "ui_deditreport.h"


class dEditReport : public Q3MainWindow, public Ui::dEditReport
{
    Q_OBJECT

public:
    dEditReport(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::WType_TopLevel);
    ~dEditReport();

    aListViewItem *item;
    aAliasEditor *al;

public slots:
    virtual void setData( aListViewItem * o );
    virtual void updateMD();

protected slots:
    virtual void languageChange();

    virtual void init();
    virtual void destroy();


};

#endif // DEDITREPORT_H
