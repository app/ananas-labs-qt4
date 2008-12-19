#ifndef DEDITDOC_H
#define DEDITDOC_H

#include "ui_deditdoc.h"


class dEditDoc : public Q3MainWindow, public Ui::dEditDoc
{
    Q_OBJECT

public:
    dEditDoc(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::WType_TopLevel);
    ~dEditDoc();

    aListViewItem *item;
    aAliasEditor *al;
    Q3IntDict<int> fields;
    aRoleEditor *re;

public slots:
    virtual void setData( aListViewItem * o );
    virtual void updateMD();
    virtual void eSv_activated( int index );

protected slots:
    virtual void languageChange();

    virtual void init();
    virtual void destroy();


};

#endif // DEDITDOC_H
