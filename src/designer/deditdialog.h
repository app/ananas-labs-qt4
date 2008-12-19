#ifndef DEDITDIALOG_H
#define DEDITDIALOG_H

#include "ui_deditdialog.h"


class dEditDialog : public Q3MainWindow, public Ui::dEditDialog
{
    Q_OBJECT

public:
    dEditDialog(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::WType_TopLevel);
    ~dEditDialog();

    int docId;
    aAliasEditor *al;
    aFormDesigner *fd;
    aListViewItem *item;

    virtual int getDocId();

public slots:
    virtual void init();
    virtual void destroy();
    virtual void setData( aListViewItem * o );
    virtual void EditForm();
    virtual void EditModule();
    virtual void updateMD();

protected slots:
    virtual void languageChange();

private slots:
    virtual void formPreview();

};

#endif // DEDITDIALOG_H
