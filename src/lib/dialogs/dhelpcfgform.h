#ifndef HELPCFGFORM_H
#define HELPCFGFORM_H

#include "ui_dhelpcfgform.h"


class HelpCfgForm : public QDialog, public Ui::HelpCfgForm
{
    Q_OBJECT

public:
    HelpCfgForm(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~HelpCfgForm();

    virtual void init();

protected slots:
    virtual void languageChange();

};

#endif // HELPCFGFORM_H
