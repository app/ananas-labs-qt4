#ifndef ROLESFORM_H
#define ROLESFORM_H

#include "ui_rolesform.h"

class RolesForm : public QWidget, public Ui::RolesForm
{
    Q_OBJECT

public:
    RolesForm(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = 0);
    ~RolesForm();

    QString rcfile;
    aMetadataTreeView *mdtree;
//    aCfg *cfg;

signals:
    void updateUsers();

protected slots:
    virtual void languageChange();

};

#endif // ROLESFORM_H
