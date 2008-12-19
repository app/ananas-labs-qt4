#ifndef USERSFORM_H
#define USERSFORM_H
#include <QPointer>

#include "ui_usersform.h"

class UsersForm : public QWidget, public Ui::UsersForm
{
    Q_OBJECT

public:
    UsersForm(QWidget* parent = 0, Qt::WindowFlags fl = 0);
    ~UsersForm();

signals:
    void updateRoles();

protected:
    aCfg *cfg;
    QPointer<aMetadataTreeView> mdtree;
    QString rcfile;

protected slots:
    virtual void languageChange();

};

#endif // USERSFORM_H
