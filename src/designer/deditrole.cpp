#include "deditrole.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

#include "acfg.h"
#include <qstatusbar.h>
#include <qmessagebox.h>

/*
 *  Constructs a dEditRole as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
dEditRole::dEditRole(QWidget* parent, const char* name, Qt::WindowFlags fl)
    : Q3MainWindow(parent, name, fl)
{
    setupUi(this);

    (void)statusBar();
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
dEditRole::~dEditRole()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void dEditRole::languageChange()
{
    retranslateUi(this);
}

extern aCfg cfg;

void dEditRole::setData( CfgForm *c, aCfgItem o )
{
//    aCfgItem alias;
//    int i, n, count;

    cf = c;
    obj = o;

    setCaption( tr("Role:") + cf->cfg.attr( obj, mda_name ) );
    eName->setText( cf->cfg.attr( obj, mda_name ) );
    eDescription->setText( cf->cfg.sText( obj, md_description ) );
}

void dEditRole::init()
{
    delete statusBar();
}

void dEditRole::destroy()
{
    updateMD();
    ( (MainForm*)this->topLevelWidget() )->removeTab(name());
}
void
dEditRole::updateMD()
{
    cf->cfg.setAttr( obj, mda_name, eName->text().stripWhiteSpace() );
    cf->cfg.setSText( obj, md_description, eDescription->text() );
    cf->initRoles();
    ( (MainForm*)this->topLevelWidget() )->wl->remove( this );
}

