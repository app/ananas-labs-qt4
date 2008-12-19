#ifndef EDBTABLE_H
#define EDBTABLE_H

#include "ui_edbtable.h"


class eDBTable : public QDialog, public Ui::eDBTable
{
    Q_OBJECT

public:
    eDBTable(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~eDBTable();

    QStringList deletedFields, deletedFieldsId;
    wDBTable *table;

    virtual QStringList findDeletedFields( int t_num, QStringList idls );

public slots:
    virtual void ColumnDel();
    virtual void ColumnL();
    virtual void ColumnR();
    virtual void ColumnSel( int col );
    virtual void ColumnUpd();
    virtual void getData( wDBTable * t );
    virtual void setData( wDBTable * t, aCfg * md );
    virtual void ColumnTextUpd( const QString & s );
    virtual void doOK();
    virtual void ColumnAdd();
    virtual void ColWidthChange( int value );
    virtual void doCancel();

signals:
    void closeEvent();

protected slots:
    virtual void languageChange();

private:
    QStringList otypes, fname, cwidth,idlist, tables;

    virtual void init();
    virtual void destroy();

};

#endif // EDBTABLE_H
