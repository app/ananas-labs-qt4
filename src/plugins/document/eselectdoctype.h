#ifndef ESELECTDOCTYPE_H
#define ESELECTDOCTYPE_H

#include "ui_eselectdoctype.h"


class eSelectDocType : public QDialog, public Ui::eSelectDocType
{
    Q_OBJECT

public:
    eSelectDocType(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~eSelectDocType();

    QStringList dlist;
    int docId;

public slots:
    virtual void init();
    virtual void setJournal( aCfg * md, int id );
    virtual void accepted();

protected slots:
    virtual void languageChange();

};

#endif // ESELECTDOCTYPE_H
