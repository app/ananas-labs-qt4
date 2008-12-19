#ifndef DSELECTDB_H
#define DSELECTDB_H

#include "ui_dselectdb.h"


class ANANAS_EXPORT dSelectDB : public QDialog, public Ui::dSelectDB
{
    Q_OBJECT

public:
    dSelectDB(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~dSelectDB();

    QString rcfile;

public slots:
    void newItem();
    void editItem();
    void deleteItem();
    void onCancel();
    void ItemRenamed( Q3ListViewItem * item, int col );
    void onHelp();
    void createMenu();
    virtual void onDblClick(Q3ListViewItem *);

protected:
    QSettings settings;

    virtual void clearSettings();

protected slots:
    virtual void languageChange();

private:
    int withgroups;
    QMenuBar *menuBar;
    bool changes;
    int ro_mode;
    bool local;

private slots:
    void init();
    void readSettings( QStringList entryGroup );
    void itemSelect( Q3ListViewItem * item );
    void newGroup();
    void saveRC();
    void onOK();
    void importItem();
    void exportItem();

};

#endif // DSELECTDB_H
