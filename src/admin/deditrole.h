#ifndef DEDITROLE_H
#define DEDITROLE_H

#include "ui_deditrole.h"

class dEditRole : public QDialog, public Ui::dEditRole
{
    Q_OBJECT

public:
    dEditRole(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~dEditRole();

    virtual void setData( const QString & s );
    virtual QString getData();

protected slots:
    virtual void languageChange();

};

#endif // DEDITROLE_H
