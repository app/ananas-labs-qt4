#include "deditpermissions.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

//#include "deditpermissions.ui.h"
/*
 *  Constructs a dEditPermissions as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
dEditPermissions::dEditPermissions(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);
    
}

/*
 *  Destroys the object and frees any allocated resources
 */
dEditPermissions::~dEditPermissions()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void dEditPermissions::languageChange()
{
    retranslateUi(this);
}


void dEditPermissions::setData( int pm )
{
    int r,i;
    r = 1;
    QCheckBox *c;
    for (i=1;i<=7;i++)
    {
	c = (QCheckBox*) child( (const char*) QString("checkBox%1").arg(i),"QCheckBox");
	if ( c ) {
	  c->setChecked( (pm & r ) !=0 );  
	}
	r = r << 1;
    }
}


int dEditPermissions::getData()
{
    int r,i, res=0;
    r = 1;
    QCheckBox *c;
    for (i=1;i<=7;i++)
    {
	c = (QCheckBox*) child( (const char*) QString("checkBox%1").arg(i),"QCheckBox");
	if ( c ) {
	  if (c->isChecked()) res |= r;  
	}
	r = r << 1;
    }
    return res;
}


void dEditPermissions::setAll( bool v )
{
    int i;
    QCheckBox *c;
    for (i=1;i<=7;i++)
    {
	c = (QCheckBox*) child( (const char*) QString("checkBox%1").arg(i),"QCheckBox");
	if ( c ) c->setChecked( v );  
    }
}
