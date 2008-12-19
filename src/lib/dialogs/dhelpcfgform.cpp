#include "dhelpcfgform.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

/*
 *  Constructs a HelpCfgForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
HelpCfgForm::HelpCfgForm(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
HelpCfgForm::~HelpCfgForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void HelpCfgForm::languageChange()
{
    retranslateUi(this);
}

void HelpCfgForm::init()
{
 textEdit1->setText( tr("Make group, item, set item paramert and press Ok button. See http://ananas.lrn.ru/index.php?title=Manual:Disigner for details" ));
}
