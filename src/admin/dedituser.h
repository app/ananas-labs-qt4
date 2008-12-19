#ifndef DEDITUSER_H
#define DEDITUSER_H
#include "ui_dedituser.h"

class dEditUser : public QDialog, public Ui::dEditUser
{
    Q_OBJECT

public:
    dEditUser(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~dEditUser();

    virtual aUser * getData();

public slots:
    virtual bool check( const QString & login );
    virtual void setData( aUser * user, bool isNew );
    virtual void onOk();

protected slots:
    virtual void languageChange();

private:
    aUser *usr;
    bool newUser;

};

#endif // DEDITUSER_H
