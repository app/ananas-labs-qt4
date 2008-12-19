#ifndef DEDITLANG_H
#define DEDITLANG_H

#include "ui_deditlang.h"


class dEditLang : public Q3MainWindow, public Ui::dEditLang
{
    Q_OBJECT

public:
    dEditLang(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::WType_TopLevel);
    ~dEditLang();

public slots:
    virtual void setData( CfgForm * c, aCfgItem o );
    virtual void updateMD();

protected slots:
    virtual void languageChange();

private:
    aCfgItem obj;
    int langCount;
    CfgForm *cf;

    void init();
    void destroy();

};

#endif // DEDITLANG_H
