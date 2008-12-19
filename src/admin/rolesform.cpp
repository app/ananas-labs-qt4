#include "rolesform.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

/*
 *  Constructs a RolesForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
RolesForm::RolesForm(QWidget* parent, const char* name, Qt::WindowFlags fl)
    : QWidget(parent, name, fl)
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
RolesForm::~RolesForm()
{
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void RolesForm::languageChange()
{
    retranslateUi(this);
}
