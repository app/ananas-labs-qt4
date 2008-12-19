#ifndef DEDITPERMISSIONS_H
#define DEDITPERMISSIONS_H

#include <ui_deditpermissions.h>
//#include <qvariant.h>

class dEditPermissions : public QDialog, public Ui::dEditPermissions
{
    Q_OBJECT

public:
    dEditPermissions(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~dEditPermissions();

public slots:
    virtual void setData( int pm );
    virtual int getData();
    void setAll( bool b );

protected slots:
    virtual void languageChange();

};

#endif // DEDITPERMISSIONS_H
