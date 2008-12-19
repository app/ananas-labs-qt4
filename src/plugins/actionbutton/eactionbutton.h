#ifndef EACTIONBUTTON_H
#define EACTIONBUTTON_H

#include "ui_eactionbutton.h"


class eActionButton : public QDialog, public Ui::eActionButton
{
    Q_OBJECT

public:
    eActionButton(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~eActionButton();

public slots:
    virtual void Action_toggled( bool o );
    virtual void Script_toggled( bool o );
    virtual void setData( wActionButton *o );
    virtual void getData( wActionButton *o );

protected:
    virtual void loadActions( QStringList *lst, QStringList *id,  aCfgItem parent, aCfg *md);

protected slots:
    virtual void languageChange();

private:
    QStringList l_id;

    void destroy();

};

#endif // EACTIONBUTTON_H
