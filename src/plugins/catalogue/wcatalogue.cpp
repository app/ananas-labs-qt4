/****************************************************************************
** $Id: wcatalogue.cpp,v 1.2 2008/11/08 20:16:36 leader Exp $
**
** Code file of the catlogue plugin of Ananas
** Designer and Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) 2003-2004 Grigory Panov, Yoshkar-Ola.
**
** This file is part of the Ananas Plugins of the Ananas
** automation accounting system.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.leaderit.ru/page=ananas or email sales@leaderit.ru
** See http://www.leaderit.ru/gpl/ for GPL licensing information.
**
** Contact org@leaderit.ru if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#include <qobject.h>
#include <q3sqlcursor.h>
#include <q3sqlpropertymap.h>
#include <q3toolbar.h>
#include <qaction.h>
//--#include <qfocusdata.h>
#include "adatabase.h"
#include "wcatalogue.h"
#include "ecatalogue.h"
#include "wdbfield.h"
#include "wdbtable.h"
#include "wgrouptree.h"
#include "alog.h"

//extern aCfg *plugins_aCfg;


/*!\en
 *\_en \ru
 *\_ru
 */
wCatalogue::wCatalogue( QWidget *parent, Qt::WFlags fl )
:aWidget( parent, "wCatalogue", fl )
{
	setInited( false );
	setFormMode( 0 );
}


wCatalogue::~wCatalogue()
{
}



void
wCatalogue::initObject( aDatabase *adb )
{
	aWidget::initObject( adb );
	QObject *obj;
	QObjectList lb = this->queryList( "QWidget" );
	QListIterator<QObject*> itb( lb ); // iterate over all wDBFields
	while ( itb.hasNext() )
	{
		obj = itb.next();
		if(obj->inherits("wDBField"))
		{
//		if ( (wActionButtton*) obj )->isActionUpdate() )
			connect( (wDBField *)obj, SIGNAL(valueChanged( const QVariant & )),
				this, SLOT(valueChanged( const QVariant & )) );
		}
		if(obj->inherits("wGroupTree"))
		{
			((wGroupTree*)obj)->setId(getId());
			connect( (wGroupTree *)obj, SIGNAL(selectionChanged( const qulonglong )),
				this, SLOT(selectionChanged( const qulonglong )) );

		}
		if(obj->inherits("wDBTable"))
		{
			connect( this, SIGNAL(newSelectionFilter( const QString&  )),
				(wDBTable*)obj, SLOT(newFilter( const QString& )) );
			connect( this, SIGNAL(newSelectionGroupId( const qulonglong  )),
				(wDBTable*)obj, SLOT(newDataId( const qulonglong )) );

		}

	}
	//--delete lb; // delete the list, not the objects
	//--focusData()->next()->setFocus();
	focusNextChild();

}



/*!
 *\~english
 *	Processed field value changed.
 *	Conected on signal valueChanged( const QVariant & ) for all
 *	wDBField insert in the widget.
 *	Decode field name and set new field value in database object.
 *	Emit signal value change with field name and value.
 *	Signal connectaed on aform slot, for future work.
 *\~russian
 *	Обрабатывает изменение значения поля
 *	Присоединяется к сигналу valueChanged( const QVariant & ),
 *	всех полей типа wDBField, вставленных в виджет.
 *	Декодирует имя поля и сохраняет его новое значение в объекте для
 *	работы с базой.
 *	Посылает сигнал об изменении значения, который содержит имя поля
 *	и его новое значение.
 *	Сигнал присоединяется к слоту формы для последующей обработки.
 *\~
 *\param value - \~english new field value. \~russian новое значение поля. \~
 */
void
wCatalogue::valueChanged( const QVariant & value )
{
	if ( sender()->className() != QString("wDBField") ) return;
	wDBField * fld = ( wDBField * ) sender();
	aCfgItem o_field,o_parent;
	QString parent_name;
	QVariant val = value;

	o_field = md->find(fld->getId());
	o_parent = md->parent(o_field);
	const QString fname = fld->getFieldName();

	parent_name = md->objClass( o_parent );
	if( parent_name == QString(md_group) )
	{

		if (dbobj)// && dbobj->className() == QString("aCatalogue") )
		{
			aLog::print(aLog::Debug, tr("wCatalogue group value changed to %1").arg(value.toString()));
			((aCatalogue*)dbobj)->GroupSetValue( fname, value );
		}


	}

	else
	{

		if (dbobj)
		{
			aLog::print(aLog::Debug, tr("wCatalogue element value changed to %1").arg(value.toString()));
			dbobj->SetValue( fname, val );
		}
	}

	emit aWidget::valueChanged( fname, value );
}


bool
wCatalogue::checkStructure()
{

	return false;
}


QVariant
wCatalogue::value( const QString &name )
{

	aLog::print(aLog::Error, tr("wCatalogue value for name %1 ").arg(name));
	return QVariant("");
}


void
wCatalogue::setValue( const QString &name, QVariant &value )
{
//	if ( formMode() == CATALOGUEFORMMODE_GROUP ) {
//		( aCatalogue *) dbobj->SetValue(name,value);
//	} else {
//	printf("wCatalogue::setValue(%s,%s)\n",name.ascii(),value.toString().ascii());
	aLog::print(aLog::Debug, tr("wCatalogue set value %1 for name %2 ").arg(value.toString()).arg(name));
	dbobj->SetValue(name,value);
//	}
	//dbobj->Update();
//	debug_message("wCatalogue value changed to %s\n",value.toString().ascii());

}


QDialog*
wCatalogue::createEditor( QWidget *parent )
{
    return new eCatalogue( parent );
}


int
wCatalogue::select( qulonglong )//id )
{
	return 0;
}


qulonglong
wCatalogue::insert()
{
    return 0;
}


int
wCatalogue::update()
{
	if ( dbobj )
	{
		aLog::print(aLog::Debug, tr("wCatalogue update"));
//		printf("wCatalogue:;update()\n");
		return dbobj->Update();
	}

	aLog::print(aLog::Error, tr("wCatalogue have not data source object "));
	return err_abstractobj;
}


int
wCatalogue::markDelete()
{
    return 0;
}


QString
wCatalogue::displayString()
{
	return "";
}

/*!
 * Create aDocument database object.
 */
aObject *
wCatalogue::createDBObject(  aCfgItem obj, aDatabase *adb )
{
//        printf("cat form mode = %i\n",formMode() );
//	if ( formMode() == CATALOGUEFORMMODE_GROUP ) return new aCatGroup( obj, adb );
        return new aCatalogue( obj, adb );
}


ERR_Code
wCatalogue::Select( qulonglong id )
{

	//if ( formMode()==0 ) {

	QObject *obj;
	QObjectList lb = this->queryList( "wDBTable" );
	QListIterator<QObject*> itb( lb ); // iterate over all wDBTable
	while ( itb.hasNext() )
	{
		obj = itb.next();
		( (wDBTable *)obj )->setFocus();
		( (wDBTable *)obj )->Select( id );
//		if ( (wActionButtton*) obj )->isActionUpdate() )
//		connect( (wDBTable *)obj, SIGNAL(valueChanged( const QVariant & )),
//				this, SLOT(valueChanged( const QVariant & )) );
	}
	//--delete lb; // delete the list, not the objects
	//--lb=0;
//	return 0;



//}
//
//
	blockSignals( true );
	ERR_Code err = aWidget::Select ( id ); //table()->select( id ); //aWidget::Select ( id );

	blockSignals( true );
	NewValues();
	blockSignals( false );
	return err_noerror;
}



ERR_Code
wCatalogue::SelectGroup( qulonglong id )
{

	ERR_Code err = ((aCatalogue*)dbobj)->groupSelect( id );
	if ( err )
	{
		aLog::print(aLog::Error, tr("wCatalogue select group error = %1 ").arg(err));
		return err;
	}
	blockSignals( true );
	NewValues();
	blockSignals( false );
	//emit( changeObj(QString("idd=%1").arg(id)));
	return err_noerror;
}


/*!
 *\~english
 *	Set fields values from database.
 *	Used for insert values into fields when create or open catalogue.
 *\~russian
 *	Устанавливает значения полей.
 *	Устанавливает значения полей из базы данных,
 *	используется при открытии формы или смене каталога.
 *\~
 */
void
wCatalogue::NewValues()
{
	aLog::print(aLog::Debug, tr("wCatalogue set new values for all fields"));
	QString fname;
	QObjectList l = this->queryList( "wDBField" );
	QListIterator<QObject*> it( l );
	QObject *obj;
	//--obj = it.toFirst();
	aCfgItem o_field,o_parent;
	QString parent_name;
	while ( it.hasNext() )
	{
		//++it;
		//obj = it.current();
		obj= it.next();
		fname=((wDBField *)obj)->getFieldName();
		o_field = md->find(((wDBField *)obj)->getId());
		o_parent = md->parent(o_field);
		//printf( "field name %s\n",(const char*)fname.local8Bit() );
		parent_name = md->objClass( o_parent );
		//printf("parent_name = %s\n", (const char*)parent_name );
		if( parent_name == QString(md_group) )
		{
//			debug_message("group set value\n");
			if (dbobj)// && dbobj->className() == QString("aCatalogue") )
			{

				aLog::print(aLog::Debug, tr("wCatalogue set new values for group field %1").arg(fname));
				((wDBField*)obj)->setValue(((aCatalogue*)dbobj)->GroupValue( fname ).toString());
			}

		}
		else
		{
			aLog::print(aLog::Debug, tr("wCatalogue value for element field %1 ").arg(fname));
			((wDBField*)obj)->setValue(dbobj->Value(fname).toString());
		}
	}
	//--delete l; // delete the list, not the objects
	//--l=0;
}



void
wCatalogue::setFormMode( int Mode )
{
	aWidget::setFormMode( Mode );
}


/*!
 * Create toolbar for Catalogue.
 */
Q3ToolBar*
wCatalogue::createToolBar( Q3MainWindow * owner )
{
	/*
	QAction *a;
	QToolBar *t = new QToolBar( owner, "CatalogueTools" );

	a = new QAction(
	QPixmap::fromMimeSource("doc_new.png"),
	tr("New"),
	QKeySequence(""),//Insert"),
	t,
	tr("New Element")
	);
	a->setToolTip(tr("New element (Ins)"));
	a->addTo( t );
	connect( a, SIGNAL( activated() ), this, SLOT( insert() ) );

	a = new QAction(
	QPixmap::fromMimeSource("doc_edit.png"),
	tr("Edit"),
	QKeySequence(""),//Return"),
	t,
	tr("Edit element")
	);
	a->setToolTip(tr("Edit element (Enter)"));
	a->addTo( t );
	connect( a, SIGNAL( activated() ), this, SLOT( update() ) );
*/
/*	a = new QAction(
	QPixmap::fromMimeSource("doc_view.png"),
	tr("View"),
	QKeySequence("Shifh+Return"),
	t,
	tr("View element")
	);
	a->setToolTip(tr("View element (Shift+Enter)"));
	a->addTo( t );
	connect( a, SIGNAL( activated() ), this, SLOT( view() ) );
*/
/*
	a = new QAction(
	QPixmap::fromMimeSource("doc_view.png"),
	tr("Delete"),
	QKeySequence(""),//Delete"),
	t,
	tr("Delete element")
	);
	a->setToolTip(tr("Delete element (Shift+Enter)"));
	a->addTo( t );
	connect( a, SIGNAL( activated() ), this, SLOT( markDelete() ) );
*/
	return 0;
}

void
wCatalogue::selectionChanged(const qulonglong groupId)
{
	aLog::print(aLog::Error, tr("wCatalogue selection changed to %1 ").arg(groupId));
	emit(newSelectionGroupId(groupId));
	emit(newSelectionFilter(QString("idg=%1").arg(groupId)));
}
