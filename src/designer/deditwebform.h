#ifndef DEDITWEBFORM_H
#define DEDITWEBFORM_H

#include "ui_deditwebform.h"


class dEditWebForm : public Q3MainWindow, public Ui::dEditWebForm
{
    Q_OBJECT

public:
    dEditWebForm(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::WType_TopLevel);
    ~dEditWebForm();

    aAliasEditor *al;
    aListViewItem *item;

public slots:
    virtual void setData( aListViewItem * o );
    virtual void updateMD();

protected slots:
    virtual void languageChange();

private:
    void init();
    void destroy();

};

#endif // DEDITWEBFORM_H
