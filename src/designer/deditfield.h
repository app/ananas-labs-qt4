#ifndef DEDITFIELD_H
#define DEDITFIELD_H

#include "ui_deditfield.h"


class dEditField : public Q3MainWindow, public Ui::dEditField
{
    Q_OBJECT

public:
    dEditField(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::WType_TopLevel);
    ~dEditField();

    QStringList otypes;
    QStringList artypes;
    aListViewItem *item;
    aAliasEditor *al;

public slots:
    virtual void setData( aListViewItem * o );
    virtual void updateMD();
    virtual void typeSelect( int idx );
    virtual void nameChanged();
    virtual void AARegSelect( int i );

protected slots:
    virtual void languageChange();

private:
    QString capt;

    void destroy();
    void init();

};

#endif // DEDITFIELD_H
