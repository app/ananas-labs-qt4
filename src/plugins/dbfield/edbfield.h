#ifndef EDBFIELD_H
#define EDBFIELD_H

#include "ui_edbfield.h"


class eDBField : public QDialog, public Ui::eDBField
{
    Q_OBJECT

public:
    eDBField(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~eDBField();

    QStringList oids;
    QString selotype;
    QStringList otypes;
    QString seloid;

public slots:
    virtual void setData( QWidget * o, aCfg * md );
    virtual void getData( QWidget * o );
    virtual void namechanged( const QString & s );

protected:
    QStringList onames;

protected slots:
    virtual void languageChange();

private:
    void init();

};

#endif // EDBFIELD_H
