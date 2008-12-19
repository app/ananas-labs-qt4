#ifndef DSELECTROLE_H
#define DSELECTROLE_H

#include "ui_dselectrole.h"

class dSelectRole : public QDialog, public Ui::dSelectRole
{
    Q_OBJECT

public:
    dSelectRole(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~dSelectRole();

public slots:
    virtual void setData( aUser * user );
    virtual void onSelect();
    virtual Q_ULLONG getData();

protected slots:
    virtual void languageChange();

private:
    Q_ULLONG roleId;
    aUser *usr;
    QStringList listId;

};

#endif // DSELECTROLE_H
