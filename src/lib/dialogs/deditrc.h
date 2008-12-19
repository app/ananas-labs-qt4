#ifndef DEDITRC_H
#define DEDITRC_H

#include "ui_deditrc.h"

class ANANAS_EXPORT dEditRC : public QDialog, public Ui::dEditRC
{
    Q_OBJECT

public:
    dEditRC(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~dEditRC();

public slots:
    virtual void setdata( QString nameRC, rcListViewItem * item );
    virtual void updatecfg();

signals:
    void onOk(QString rc);

protected slots:
    virtual void languageChange();

    virtual void onCFGFile();


private:
    rcListViewItem *it;

private slots:
    virtual void onOK();
    virtual void onRCFile();

};

#endif // DEDITRC_H
