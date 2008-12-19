#ifndef CATALOGFORM_H
#define CATALOGFORM_H

#include "ui_catalogform.h"

#include <Q3GridLayout>
#include <Q3VBoxLayout>


class CatalogForm : public Q3MainWindow, public Ui::CatalogForm
{
    Q_OBJECT

public:
    CatalogForm(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::WType_TopLevel);
    ~CatalogForm();

    aListView* ListView;
    QString fieldName;
    aLineEdit *LineEdit;
    aListBox* ListHint;
    Q3Frame* StatusFrame;
    aCatalogue* cat;
    bool FormToSelect;

    virtual Q_ULLONG getId();
    virtual void init();
    virtual void destroy();
    virtual long findFirst( const QString & s );
    virtual Q_ULLONG getIdg( Q_ULLONG ide );
    virtual QPixmap getGroupPixmap();
    virtual void edit( Q3ListViewItem * item, bool );
    virtual QPixmap getElementPixmap();
    virtual QPixmap getMarkDeletedPixmap();

public slots:
    virtual void setData( aCatalogue * catalog, QMap<Q_ULLONG, Q3ListViewItem *> mg, const QStringList & Fname, const QStringList & FnameGroup, Q_ULLONG idElForm, Q_ULLONG idGrForm, const bool toSelect );
    virtual void setId( Q_ULLONG idx );
    virtual void find( const QString & s );
    virtual void pressArrow();
    virtual void isPressArrow();
    virtual void setText( const QString & s );
    virtual void go();
    virtual void goToItem( Q3ListViewItem * item );
    virtual void loadElements( Q_ULLONG idGroup );
    virtual void onLoadElements( Q3ListViewItem * item );
    virtual void new_item( Q3ListViewItem * parentItem );
    virtual void new_group( Q3ListViewItem * parentItem );
    virtual void del_item( Q3ListViewItem * item );
    virtual void mark_deleted( Q3ListViewItem * item );
    virtual void undo_mark_deleted( Q3ListViewItem * item );
    virtual void edit( Q3ListViewItem * item, int );
    virtual void select( Q3ListViewItem * item );
    virtual void Refresh( Q_ULLONG id );

signals:
    void selected(Q_ULLONG);

protected:
    Q3GridLayout* GridLayout;
    Q3VBoxLayout* layout1;
    Q_ULLONG id,idElementForm,idGroupForm;
    QStringList fieldList, fieldListGroup;

    virtual long getGroupId( Q3ListViewItem * item );
    virtual Q_ULLONG getElementId( Q3ListViewItem * item );

protected slots:
    virtual void languageChange();

    virtual void keyPressEvent( QKeyEvent * e );


private:
    QMap<Q_ULLONG, Q3ListViewItem*> map_gr;
    QMap<Q_ULLONG, Q3ListViewItem*> map_el;
    QMap<Q_ULLONG,bool> map_deleted;

private slots:
    virtual void doOk();

};

#endif // CATALOGFORM_H
