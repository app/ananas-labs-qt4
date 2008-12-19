#ifndef EFIELD_H
#define EFIELD_H

#include "ui_efield.h"


class eField : public QDialog, public Ui::eField
{
    Q_OBJECT

public:
    eField(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~eField();

    QStringList otypes;

public slots:
    virtual void init();
    virtual void setData( QWidget * o, aCfg * cfg );
    virtual void getData( QWidget * o );
    virtual void typechanged( const QString & s );
    virtual void typeSelect( int idx );

protected:
    aCfg * md;

protected slots:
    virtual void languageChange();

};

#endif // EFIELD_H
