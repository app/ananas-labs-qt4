#include "dselectrole.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

/*
 *  Constructs a dSelectRole as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
dSelectRole::dSelectRole(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
dSelectRole::~dSelectRole()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void dSelectRole::languageChange()
{
    retranslateUi(this);
}

void
dSelectRole::setData( aUser *user )
{
 usr = user;
 listBox1->clear();
 listId.clear();
 Q3ValueList<aRole*> list = user->getRoles(false);
 Q3ValueList<aRole*>::iterator it;
 for ( it = list.begin(); it != list.end(); ++it )
 {
  listBox1->insertItem( (*it)->sysValue("name").toString() );
  listId.append((*it)->sysValue("id").toString());
  delete (*it);
 }
// delete list;
}


void
dSelectRole::onSelect()
{
 if(listBox1->currentItem()!=-1)
 {
  printf("add role\n");
 
  //emit( addRole(listId[listBox1->currentItem()].toULongLong()) );
  roleId =  listId[listBox1->currentItem()].toULongLong();
   usr->addRole(roleId);
  listBox1->removeItem(listBox1->currentItem());
  accept();
 // new aListViewItem(p_item,
 }
 else
 {
  printf("not selected\n");
 }
}


Q_ULLONG dSelectRole::getData()
{
    return roleId;
}
