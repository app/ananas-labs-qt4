#ifndef ECATALOGUE_H
#define ECATALOGUE_H

#include "ui_ecatalogue.h"


class eCatalogue : public QDialog, public Ui::eCatalogue
{
    Q_OBJECT

public:
    eCatalogue(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~eCatalogue();

    QStringList otypes;

public slots:
    virtual void init();
    virtual void setData( QWidget * o, aCfg * md );
    virtual void getData( QWidget * o );

protected slots:
    virtual void languageChange();

};

#endif // ECATALOGUE_H
