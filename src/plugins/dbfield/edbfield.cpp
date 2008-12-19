#include "edbfield.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

#include "acfg.h"
#include "wdbfield.h"

/*
 *  Constructs a eDBField as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
eDBField::eDBField(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
eDBField::~eDBField()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void eDBField::languageChange()
{
    retranslateUi(this);
}


void eDBField::init()
{
	eName->clear();
	onames.clear();
	oids.clear();
}



void eDBField::setData( QWidget *o, aCfg *md )
{
//    const QObject *o = sender();
    if ( o ) {
	if ( o->className() != QString("wDBField") || !md ) {
	    reject();
	    return;
	}
    }
    else {
	reject();
	return;
    }
    int w=0, d=0, idx=0;
    unsigned int i;
    long oid, widgetId, fieldId;
    QString pn;
    aWidget *widget;
    wDBField *field = (wDBField*)o;
    QWidget *pWidget = field->parentWidget();

    while ( pWidget ) {
	pn = "";
	pn = pWidget->className();
	if ( pn == QString("wCatalogue") || pn == QString("wDocument") ) break;
	pWidget = pWidget->parentWidget();
    }
    if ( pn == QString("wCatalogue") ) widget = (wCatalogue*)pWidget; else
    if ( pn == QString("wDocument") ) widget = (wDocument*)pWidget; else
    return;

    widgetId = widget->getId();
    //fieldId = field->getId();
    if ( widgetId ) widget->setMDObject( md->find( widgetId ) );

//    printf( "parent widget id = %i\n", widgetId );
//    printf( "field id = %i\n", fieldId );
//    printf( "metadata id = %i\n", md->id( *widget->getMDObject() ) );

    QStringList tlist = md->types( md_field, widget->getMDObject() );
    otypes.clear();
    eType->clear();
    for ( QStringList::Iterator it = tlist.begin(); it != tlist.end(); ++it ) {
	otypes.append( (*it).section( "\t", 0, 0 ) );
	eName->insertItem( (*it).section("\t", 1, 1 ), idx++ );
    }
    for ( i = 0 ; i < otypes.count(); i++ ) {
	oid = 0;
	if( otypes[i][0] == 'O' ) {
	    sscanf( (const char *)otypes[ i ], "O %d", &oid );
	    if ( oid == fieldId ) {
		eName->setCurrentItem( i );
		break;
	    }
	}
    }
}


void eDBField::getData( QWidget *o )
{
//    const QObject *o = sender();
    if ( !o ) return;
    if ( o->className() != QString("wDBField") ) return;
    wDBField *f = ( wDBField*) o;

    int idx=eName->currentItem();
    long oid = 0;
    if (f) {
	if( otypes[idx][0] == 'O' ) {
	    sscanf( (const char *)otypes[ idx ], "O %d", &oid );
	   // f->setId( oid );
	}
    }
}


void eDBField::namechanged( const QString &s )
{
    int idx=eName->currentItem();
//	printf("new str=%s\n", (const char *) s.utf8());
    selotype=otypes[idx];
    seloid=oids[idx];
    eType->setText(onames[idx]);
}

