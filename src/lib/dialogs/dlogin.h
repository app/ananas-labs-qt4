#ifndef DLOGIN_H
#define DLOGIN_H

#include "ui_dlogin.h"


class ANANAS_EXPORT dLogin : public QDialog, public Ui::dLogin
{
    Q_OBJECT

public:
    dLogin(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~dLogin();

    QString username, password;

public slots:
    virtual void init();
    virtual void buttonOK();

protected slots:
    virtual void languageChange();

};

#endif // DLOGIN_H
