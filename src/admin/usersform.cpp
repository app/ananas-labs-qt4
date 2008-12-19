#include "usersform.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

/*
 *  Constructs a UsersForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
UsersForm::UsersForm(QWidget* parent, Qt::WindowFlags fl)
    : QWidget(parent, fl)
{
    setupUi(this);
    new QVBoxLayout( this );
    mdtree = new aMetadataTreeView( this, &aDatabase::database()->cfg ); // metadata tree creation
    mdtree->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    layout()->add( mdtree );
    updateGeometry();
}

/*
 *  Destroys the object and frees any allocated resources
 */
UsersForm::~UsersForm()
{

}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void UsersForm::languageChange()
{
    retranslateUi(this);
}


