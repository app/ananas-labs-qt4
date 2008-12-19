#include "eactionbutton.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

#include <qseditor.h>
#include <stdlib.h>

/*
 *  Constructs a eActionButton as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
eActionButton::eActionButton(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
eActionButton::~eActionButton()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void eActionButton::languageChange()
{
    retranslateUi(this);
}

void eActionButton::Action_toggled( bool o )
{
    Script->setChecked( !o );
    //else
}


void eActionButton::Script_toggled( bool o )
{
    Action->setChecked( !o );
}

void eActionButton::setData(  wActionButton *b )
{
    if ( !b ) return;
    if ( b->isAction() ) Action->setChecked( TRUE );
    if ( b->isActionUpdate() ) cbUpdate->setChecked( TRUE );
    if ( b->isActionTurnOn() ) cbTurnon->setChecked( TRUE );
    if ( b->isActionClose() ) cbClose->setChecked( TRUE );
    if ( b->isScript() ) Script->setChecked( TRUE );
//    cbAction->setCurrentItem(b->getAction())
    // eCode->setText( b->getScriptCode() );
//    button = b;
    aWidget *wd = aWidget::parentContainer( b );
    if ( !strcmp(wd->name(),"Catalogue") )
    {
	    cbTurnon->setChecked( false );
	    cbTurnon->setHidden( TRUE );

//	b->setActionTurnOn( FALSE );
    }
    aCfg *md = wd->getMd();
    QStringList l_name;
    aCfgItem parent = md->find(mdc_actions);
    l_id.clear();
    loadActions(&l_name, &l_id, parent, md);
    cbAction->clear();
    cbAction->insertStringList(l_name);
    cbAction->setCurrentItem( l_id.findIndex( QString("%1").arg(b->getActionId()) ) );

}


void eActionButton::getData( wActionButton *button)
{
    if ( !button ) return;
    button->setAction( Action->isChecked() );
    button->setActionUpdate( cbUpdate->isChecked() );
    button->setActionTurnOn( cbTurnon->isChecked() );
    button->setActionClose( cbClose->isChecked() );
    button->setScript( Script->isChecked() );
    button->setActionUpdate( cbUpdate->isChecked() );
    if(cbAction->isEnabled())
    {

    	button->setActionId ( atoi(l_id[cbAction->currentItem()].ascii()));
    }
    else
    {
	    button->setActionId(0);
    }
//    button->setScriptCode( eCode->text() );
}

void eActionButton::destroy()
{
    //getData();
}


void
eActionButton::loadActions( QStringList *lst, QStringList *id,  aCfgItem p, aCfg *md )
{
	aCfgItem cobj;
	QString oclass;
	if(p.isNull())
	{
		p = md->find(mdc_actions);
	//	printf("parent were NULL, set parent to %s\n",md->objClass(p).ascii());
	}

	if(p.isNull()) return;
	cobj = md->firstChild ( p );
	while ( !cobj.isNull() )
	{
		oclass = md->objClass ( cobj );
		if ( oclass == md_actiongroup )
			loadActions ( lst, id, cobj, md );
		if ( oclass == md_action )
		{
			lst->append(md->attr(cobj,mda_name));
			id->append(md->attr(cobj,mda_id));
		}
		cobj = md->nextSibling ( cobj );
	}
}

