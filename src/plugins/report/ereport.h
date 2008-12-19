#ifndef EREPORT_H
#define EREPORT_H

#include "ui_ereport.h"


class eReport : public QDialog, public Ui::eReport
{
    Q_OBJECT

public:
    eReport(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~eReport();

    QStringList otypes;

public slots:
    virtual void init();
    virtual void setData( QWidget * o, aCfg * md );
    virtual void getData( QWidget * o );

protected slots:
    virtual void languageChange();

};

#endif // EREPORT_H
