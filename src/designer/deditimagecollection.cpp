#include "deditimagecollection.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

#include <qstatusbar.h>

/*
 *  Constructs a dEditImageCollection as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
dEditImageCollection::dEditImageCollection(QWidget* parent, const char* name, Qt::WindowFlags fl)
    : Q3MainWindow(parent, name, fl)
{
    setupUi(this);

    (void)statusBar();
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
dEditImageCollection::~dEditImageCollection()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void dEditImageCollection::languageChange()
{
    retranslateUi(this);
}

void dEditImageCollection::init()
{
    delete statusBar();
}

void dEditImageCollection::setData( )
{
    //--obj = o;

}
