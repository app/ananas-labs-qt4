#include "mainform.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

/*
 *  Constructs a MainForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
MainForm::MainForm(QWidget* parent, const char* name, Qt::WindowFlags fl)
    : QMainWindow(parent, fl)
{
    setupUi(this);
    setObjectName( name );
    (void)statusBar();
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
MainForm::~MainForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void MainForm::languageChange()
{
    retranslateUi(this);
}

#include "usersform.h"
#include "rolesform.h"

void MainForm::fileNew()
{

}


void MainForm::fileOpen()
{

}


void MainForm::fileSave()
{

}


void MainForm::fileSaveAs()
{

}


void MainForm::filePrint()
{

}


void MainForm::fileExit()
{
 close();
}


/*!
 * Users functions.
 */
void MainForm::adminUsers()
{


    if( usersForm ) usersForm->show();//->isHidden())
    else {
        usersForm = new UsersForm( this );
//        usersForm->init(db);
//        usersForm->createTree();
        ws->addSubWindow( usersForm );
        usersForm->show();
    }
}



bool MainForm::init()
{
    ws = new QMdiArea;
    setCentralWidget( ws );
    db = aDatabase::database();
//    usersForm = new UsersForm;
//    usersForm->hide();
//    rolesForm = new RolesForm;
//    rolesForm->init(db);
//    rolesForm->createTree();
//    rolesForm->hide();
//    ws->addSubWindow( usersForm );
//    ws->addSubWindow( rolesForm );
    QSqlTableModel *t = new QSqlTableModel( this, *db->db() );
    t->setTable( db_users );
    t->setEditStrategy(QSqlTableModel::OnFieldChange );
    t->select();
    QTableView *dt = new QTableView;
    dt->setModel( t );
    ws->addSubWindow( dt );
    dt->show();
    return true;
} 



void MainForm::adminRoles()
{
    if ( rolesForm ) rolesForm->show();
    else {
      rolesForm = new RolesForm( this );
//      rolesForm->init(db);
//      rolesForm->createTree();
      ws->addSubWindow( rolesForm );
    }
}
