/****************************************************************************
** $Id: wdocument.cpp,v 1.2 2008/11/08 20:16:36 leader Exp $
**
** Code file of the document plugin of Ananas
** Designer and Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
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
//--#include <qfocusdata.h>
#include "adatabase.h"
#include "adocument.h"
#include "asqltable.h"
#include "wdocument.h"
#include "edocument.h"
#include "wdbfield.h"
#include "wdbtable.h"
#include "wactionbutton.h"
#include "alog.h"


/*!
 *\~english
 *	Create wDocument object.
 *\~russian
 *	Создает объект wDocument.
 *\~
 *\param - \~english \~russian \~
 *\param - \~english \~russian \~
 */
wDocument::wDocument( QWidget *parent, Qt::WFlags fl )
:aWidget( parent, "wDocument", fl )
{
	setInited( false );
}



/*!
 *\~english
 *	Destructor for object.
 *\~russian
 *	Деструктор.
 *\~
 */
wDocument::~wDocument()
{

}



/*!
 *\~english
 *	Create aDocument database object.
 *\~russian
 *	Создает объект aDocument для работы с базой.
 *\~
 *\param - \~english \~russian \~
 *\param - \~english \~russian \~
 *\return \~english \~russian \~
 */
aObject *
wDocument::createDBObject(  aCfgItem obj, aDatabase *adb )
{
	return new aDocument( obj, adb );
}



/*!
 *\~english
 *\~russian
 *\~
 *\param - \~english \~russian \~
 */
void
wDocument::initObject( aDatabase *adb )
{
	aWidget::initObject( adb );
	QObject *obj;
	QObjectList lb = this->queryList( "wDBField" );
	QListIterator<QObject*> itb( lb ); // iterate over the buttons
	while ( itb.hasNext() )
	{
		obj = itb.next();
//		if ( (wActionButtton*) obj )->isActionUpdate() )
		connect( (wDBField *)obj, SIGNAL(valueChanged( const QVariant & )),
				this, SLOT(valueChanged( const QVariant & )) );
	}
	//--delete lb; // delete the list, not the objects
	lb = this->queryList( "wDBTable" );
	QListIterator<QObject*> itb1( lb ); // iterate over the buttons
	while ( itb1.hasNext() )
	{
		obj = itb1.next();
//		if ( (wActionButtton*) obj )->isActionUpdate() )
		connect( this, SIGNAL(changeObj(const QString &)),
			 (wDBTable *)obj, SLOT(newFilter(const QString &)));
		connect( this, SIGNAL(changeObjId(const qulonglong)),
			 (wDBTable *)obj, SLOT(newDataId(const qulonglong)));
	}
	//--delete lb; // delete the list, not the objects
	//--focusData()->next()->setFocus();
	focusNextChild();
}



/*!
 *\~english
 *\~russian
 *\~
 *\return \~english \~russian \~
 */
bool
wDocument::checkStructure()
{
	return false;
}



/*!
 *\~english
 *\~russian
 *\~
 *\param - \~english \~russian \~
 *\return \~english \~russian \~
 */
QDialog*
wDocument::createEditor( QWidget *parent )
{
	return new eDocument( parent );
}



/*!
 *\~english
 *\~russian
 *\~
 *\param - \~english \~russian \~
 *\return \~english \~russian \~
 */
int
wDocument::select( qulonglong id )
{
	id=id;
	return 0;
}



/*!
 *\~english
 *	Select object in database.
 *\~russian
 *	Выбирает объект в базе данных.
 *\~
 *\param id -\~english database record id.\~russian id объекта в базе данных.\~
 *\return id -\~english error code.\~russian код ошибки.\~
 */
ERR_Code
wDocument::Select( qulonglong id )
{
	ERR_Code err = aWidget::Select ( id );
/*	if ( err )
	{
		printf( "select document error %i\n", err );
		return err;
	}
*/	NewValues();
	emit( changeObj(QString("idd=%1").arg(id)));
	emit( changeObjId(id));
	return err_noerror;
}


/*!
 *\~english
 *\~russian
 *\~
 *\param - \~english \~russian \~
 *\return \~english \~russian \~
 */
qulonglong
wDocument::Insert()
{
	return 0;
}



/*!
 *\~english
 *	Update values in database.
 *	Udate values in current document header.
 *\~russian
 *	Обновляет значения в базе данных.
 *	Обновляет значения в текущей шапке документа.
 *\~
 *\return \~english error code.\~russian код ошибки.\~
 */
int
wDocument::Update()
{
	if ( dbobj ) {
		return dbobj->Update();
		NewValues();
	}
	else return err_abstractobj;
}



/*!
 *\~english
 *\~russian
 *\~
 *\param - \~english \~russian \~
 *\return \~english \~russian \~
 */
int
wDocument::MarkDelete()
{
	return 0;
}



/*!
 *\~english
 *\~russian
 *\~
 *\return \~english \~russian \~
 */
QString
wDocument::displayString()
{
	return"";
}



/*!
 *\~english
 *\~russian
 *\~
 *\return \~english \~russian \~
 */
bool
wDocument::isContainer()
{
	return true;
}



/*!
 *\~english
 *\~russian
 *\~
 *\return \~english \~russian \~
 */
int
wDocument::New()
{
	ERR_Code err = aWidget::New();
	qulonglong idd;
	if ( err ) return  err;
	NewValues();
	idd = uid();
//	printf("<<<<<<<<<<<<<<<NEW id=%llu\n",idd);
	aLog::print(aLog::Info, tr("wDocument new ok"));
	emit( changeObj(QString("idd=%1").arg(idd)));
	emit( changeObjId(idd));

	return err_noerror;
}



/*!
 *\~english
 *	Conduct current docment.
 *\~russian
 *	Проводит текущий документ.
 *\~
 *\return \~english error code.\~russian код ошибки.\~
 */
int
wDocument::TurnOn()
{
	if (!dbobj) return err_abstractobj;
	else return dbobj->Conduct();
}
/*!
 *\~english
 *	UnConduct current docment.
 *\~russian
 *	Распроводит текущий документ.
 *\~
 *\return \~english error code.\~russian код ошибки.\~
 */
int
wDocument::TurnOff()
{
	if (!dbobj) return err_abstractobj;
	else return dbobj->UnConduct();
}



/*!
 *\~english
 *	Set fields values from database.
 *	Used for insert values into fields when create or open document.
 *\~russian
 *	Устанавливает значения виджетов (кроме таблиц).
 *	Устанавливает значения виджетов (полей экранной формы таких как поля шапки документа)
 *	равными значениям из базы данных.
 *	Используется для при открытии формы, смене документа, обновлении значений виджетов документа.
 *\~
 */
void
wDocument::NewValues()
{
	QString fname;
	QObjectList l = this->queryList( "wDBField" );
	QListIterator<QObject*> it( l );
	QObject *obj;
	while ( it.hasNext() )
	{
		obj = it.next();
		fname=((wDBField *)obj)->getFieldName();
		((wDBField *)obj)->setValue(dbobj->Value(fname).toString());
		aLog::print(aLog::Debug, tr("wDocument set new value %1 for field %2 ").arg(dbobj->Value(fname).toString()).arg(fname));
	}
	//--delete l; // delete the list, not the objects
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
wDocument::valueChanged( const QVariant & value )
{
	if ( sender()->className() != QString("wDBField") ) return;
	wDBField * fld = ( wDBField * ) sender();
	QString fname=fld->getFieldName();
//	printf( "field name %s\n",(const char*)fname );
	if ( dbobj ) dbobj->SetValue( fname, value );
	emit aWidget::valueChanged( fname, value );
}

/*void
wDocument::on_abuttonclick()
{
	wActionButton *b = ( wActionButton *) sender();
	if ( !b ) return;
	if ( !b->isAction() ) return;
	if ( b->isActionUpdate() )
	{
		QString fname;
		QObjectList *l = this->queryList( "wDBField" );
		QObjectListIt it( *l );
		QObject *obj;
		uint i = 0;
		while ( (obj = it.current()) != 0 )
		{
			++it;
			fname=((wDBField *)obj)->getFieldName();
			dbobj->SetValue(fname,((wDBField *)obj)->value());
		}
		 update();
	}
	if ( b->isActionTurnOn() ) Conduct();
}*/

