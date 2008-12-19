#ifndef DEDITJOURNAL_H
#define DEDITJOURNAL_H

#include "ui_deditjournal.h"


class dEditJournal : public Q3MainWindow, public Ui::dEditJournal
{
    Q_OBJECT

public:
    dEditJournal(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::WType_TopLevel);
    ~dEditJournal();

    aListViewItem *item;
    aAliasEditor *al;
    ananasTreeView * journalDocs;
    ananasTreeView * allDocs;
    aCfg *md;
    aCfgItem obj;
    QWidget * docPage;
    aRoleEditor *re;

public slots:
    virtual void setData( aListViewItem * o );
    virtual void updateMD();

protected slots:
    virtual void languageChange();

private:
    void init();
    void destroy();

private slots:
    virtual void typeChange();
    virtual void addDoc();
    virtual void removeDoc();
    virtual void moveUp();
    virtual void moveDown();

};

#endif // DEDITJOURNAL_H
