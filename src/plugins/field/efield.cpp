#include "efield.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

#include "acfg.h"
#include <stdlib.h>

/*
 *  Constructs a eField as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
eField::eField(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
eField::~eField()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void eField::languageChange()
{
    retranslateUi(this);
}

void eField::init()
{



}



void eField::setData( QWidget *o, aCfg *cfg )
{
	md = cfg;
	if(!o)
	{
		reject();
		return;
	}
	if ( o->className() != QString("wField") || !md ) {
	    reject();
	    return;
	}
    wField *f = ( wField*) o;
    QString ts = f->getFieldType();
//    printf(">>> fieldType = %s;\n", ts.ascii() );
    char t=' ';
    int w=0, d=0, oid, idx=0;
    unsigned int i;

//    eName->setText(f->getName());
    otypes.clear();
    eType->clear();

    QStringList tlist = md->types();
    otypes.clear();
    eType->clear();
    for ( QStringList::Iterator it = tlist.begin(); it != tlist.end(); ++it ) {
	otypes.append( (*it).section( "\t", 0, 0 ) );
	eType->insertItem( (*it).section("\t", 1, 1 ), idx++ );
    }
    if ( !ts.isEmpty() ) {
	sscanf( ts, "%c %d %d", &t, &w, &d );
    } else {
	t = 'N';
	w = 10;
    }
    if ( t=='O' ) {
	for( i = 0 ; i < otypes.count(); i++ ) {
	    if( otypes[i][0] == 'O' ) {
		sscanf( (const char *)otypes[ i ], "O %d", &oid );
		if ( oid == w ) {
		    eType->setCurrentItem( i );
		    break;
		}
	    }
	}
    } else {
	eWidth->setValue( w );
	eDec->setValue( d );
	if ( t == ' ' ) eType->setCurrentItem( 0 );
	if ( t == 'N' ) eType->setCurrentItem( 1 );
	if ( t == 'C' ) eType->setCurrentItem( 2 );
	if ( t == 'D' ) eType->setCurrentItem( 3 );
	if ( t == 'B' ) eType->setCurrentItem( 4 );
    }
    typeSelect( eType->currentItem() );
}


void eField::getData( QWidget *o )
{
//    const QObject *o = sender();
    if ( !o ) return;
    if ( o->className() != QString("wField") ) return;
    wField *f = ( wField*) o;
    wField::tEditorType type;
    char ntype[40]="";
    int idx=eType->currentItem();

    if (f) {
	//f->setName(eName->text());
	sprintf(ntype, (const char *) otypes[idx], eWidth->value(), eDec->value());
	f->setFieldType(ntype);
	if(otypes[idx][0]==' ') type = wField::Unknown;
	if(otypes[idx][0]=='N') type = wField::Numberic;
	if(otypes[idx][0]=='C') type = wField::String;
	if(otypes[idx][0]=='D') type = wField::Date;
	if(otypes[idx][0]=='B') type = wField::Boolean;
	if(otypes[idx][0]=='O')
	{
		QString tmp = otypes[idx].section(' ',1,1);
		long tid = atol(tmp);
		aCfgItem item = md->find(tid);
		if(!item.isNull())
		{
			//gets object class
			tmp = md->objClass(item);
			if(tmp == md_catalogue)
				type = wField::Catalogue;
			if(tmp == md_document)
				type = wField::Document;
		}
	}
	f->setEditorType(type);
    }
}

void eField::typechanged(const QString &s )
{
}

void eField::typeSelect( int idx )
{
    bool w = false, d = false;
    if ( otypes[ idx ][0] == 'N' ){
	w = true;
	d = true;

    } else
	if ( otypes[ idx ][0] == 'C' ){
	w = true;

    }
    eWidth->setShown( w );
    tWidth->setShown( w );
    eDec->setShown( d );
    tDec->setShown( d );
}
