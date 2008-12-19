#include "dedituser.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

/*
 *  Constructs a dEditUser as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
dEditUser::dEditUser(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
dEditUser::~dEditUser()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void dEditUser::languageChange()
{
    retranslateUi(this);
}


bool 
dEditUser::check( const QString &login )
{
    if(usr->sysValue("login").toString()==login)
    {
  return false; 
     }
    return true;
}


aUser * dEditUser::getData()
{
    return usr;
  
}


void dEditUser::setData( aUser *user, bool isNew )
{
    usr = user;
    newUser = isNew;
    //usr->Select();
    if(!isNew)
    {
 lineEdit1->setText(usr->sysValue("login").toString());
 lineEdit2->setText(usr->sysValue("password").toString());
 lineEdit3->setText(usr->sysValue("fname").toString());
 lineEdit4->setText(usr->sysValue("lname").toString());
}
}


void dEditUser::onOk()
{
    if(check(lineEdit1->text()))
    {
 if(newUser)
 {
    usr->New(lineEdit1->text(), lineEdit2->text(),lineEdit3->text(),lineEdit4->text());
 }
 else
 {
 usr->setSysValue("login",lineEdit1->text());
 usr->setSysValue("password",lineEdit2->text());
  usr->setSysValue("fname",lineEdit3->text());
  usr->setSysValue("lname",lineEdit4->text());
  usr->Update();
}
 this->accept();
   }
}
