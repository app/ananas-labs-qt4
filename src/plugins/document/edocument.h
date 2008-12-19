#ifndef EDOCUMENT_H
#define EDOCUMENT_H

#include "ui_edocument.h"


class eDocument : public QDialog, public Ui::eDocument
{
    Q_OBJECT

public:
    eDocument(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~eDocument();

    QStringList otypes;

public slots:
    virtual void init();
    virtual void setData( QWidget * o, aCfg * md );
    virtual void getData( QWidget * o );

protected slots:
    virtual void languageChange();

};

#endif // EDOCUMENT_H
