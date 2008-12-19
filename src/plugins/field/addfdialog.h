#ifndef ADDFDIALOG_H
#define ADDFDIALOG_H

#include "ui_addfdialog.h"


class addfdialog : public QDialog, public Ui::addfdialog
{
    Q_OBJECT

public:
    addfdialog(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~addfdialog();

    QStringList list,idlist;

    virtual void init();
    virtual void destroy();
    virtual const QString getData( bool name );

public slots:
    virtual void doOk();
    virtual void setData( QStringList displst, QStringList flst, QStringList ilds );
    virtual void setData(QWidget *o, aCfg *md);
    virtual void getData(QWidget *o);

protected slots:
    virtual void languageChange();

private:
    QString field;
    QString id;

};

#endif // ADDFDIALOG_H
