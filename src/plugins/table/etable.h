#ifndef ETABLE_H
#define ETABLE_H

#include "ui_etable.h"


class eTable : public QDialog, public Ui::eTable
{
    Q_OBJECT

public:
    eTable(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~eTable();

public slots:
    virtual void insertColumn( const char * Header, const char * Name, int idxt, int cw, int tw, int td );
    virtual void ColumnAdd();
    virtual void ColumnDel();
    virtual void ColumnL();
    virtual void ColumnR();
    virtual void ColumnSel( int col );
    virtual void ColumnUpd();
    virtual void setData( QWidget * o, aCfg * md );
    virtual void getData( QWidget * o );
    virtual void ColumnTextUpd( const QString & s );

protected slots:
    virtual void languageChange();

private:
    QStringList otypes, ftype, fname, cwidth;

    virtual void init();
    virtual void destroy();

};

#endif // ETABLE_H
