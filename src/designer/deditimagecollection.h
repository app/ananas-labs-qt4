#ifndef DEDITIMAGECOLLECTION_H
#define DEDITIMAGECOLLECTION_H

#include "ui_deditimagecollection.h"


class dEditImageCollection : public Q3MainWindow, public Ui::dEditImageCollection
{
    Q_OBJECT

public:
    dEditImageCollection(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::WType_TopLevel);
    ~dEditImageCollection();

    aCfgItem obj;

public slots:
    virtual void setData();

protected slots:
    virtual void languageChange();

private:
    void init();

};

#endif // DEDITIMAGECOLLECTION_H
