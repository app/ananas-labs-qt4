#include "dlogin.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

/*
 *  Constructs a dLogin as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
dLogin::dLogin(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
dLogin::~dLogin()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void dLogin::languageChange()
{
    retranslateUi(this);
}

void dLogin::init()
{
	username = "";
	password = "";
}


void dLogin::buttonOK()
{
	username = eName->text();
	password = ePassword->text();
	accept();
}

