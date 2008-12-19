#ifndef EADDOBJ_H
#define EADDOBJ_H

#include "ui_eaddobj.h"


class eAddObj : public Q3MainWindow, public Ui::eAddObj
{
    Q_OBJECT

public:
    eAddObj(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::WType_TopLevel);
    ~eAddObj();

protected slots:
    virtual void languageChange();

private:
    void init();
    void destroy();

};

#endif // EADDOBJ_H
