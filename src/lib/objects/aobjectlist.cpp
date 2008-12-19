/****************************************************************************
** $Id: aobjectlist.cpp,v 1.3 2008/11/09 21:09:11 leader Exp $
**
** Code file of the Ananas Objects List of Ananas
** Designer and Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) 2005-2006 Grigory Panov <gr1313 at mail.ru>, Yoshkar-Ola.
**
** This file is part of the Library of the Ananas
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
#include <qdialog.h>
#include "adatabase.h"
#include "aobjectlist.h"
#include "aform.h"
#include "alog.h"


/*!
 *	\en
 *	Craeate abstract aObjectListList.
 *	\param parent - parent object
 *	\param name - name of object
 *	\_en
 *	\ru
 *	Создает абстрактный не связанный с базой данных объект управления данными.
 *	Созданный таким образом объект не использует информацию из метаданных о составе и
 *	типах полей. То есть он не является какой-либо моделью данных. И на самом деле малопригоден
 *	для использования. В дазе данных ни как не отражается создание этого объекта. Для того,
 *	что бы зарегистрировать вновь созданный абстрактный объект в базе данных, необходимо
 *	сначала проинициализировать его с использованием метаданных, а затем вызвать метод New().
 *	\_ru
 */
aObjectList::aObjectList( QObject *parent, const char *name )
:QObject( parent, name )
{
	db = 0;
	vInited = false;
	filtred = false;
	selectFlag = false;
}



/*!
 *	\en
 *	Create aObjectList, inited by md object.
 *	md object finding by name
 *	\param oname - md name of object, name contens prefix
 *				Document. for documents,
 *				InfoRegister. for information registers,
 *				Catalogue. for catalogues,
 *				AccumulationRegister. for Accumulation registers,
 *				DocJournal. for journals
 *	\param adb - link on object aDataBase used for work
 *	\param parent - parent object
 *	\param name - name of object
 *	\_en
 *	\ru
 *	Создает объект как модель данных, описанную в метаданных. На описание в метаданных
 *	указывает один из передаваемых при вызове параметров - имя элемента метаданных.
 *	После успешного создания объекта с ним можно работать как с объектом данных со структурой,
 *	описанной в метаданных, и индентифицируемой именем, переданным в параметрах вызова.
 *
 *	\_ru
 */
aObjectList::aObjectList( const QString &oname, aDatabase *adb, QObject *parent, const char *name )
:QObject( parent, name )
{
	vInited = false;
	filtred = false;
	selectFlag = false;
	db = adb;
	if ( adb )
	{
		obj = adb->cfg.find( oname );
		setObject( obj );
	}
}



/*!
 *	Create aObjectList, inited by md object.
 *	\param context - hi leve md object
 *	\param adb - link on object aDataBase used for work
 *	\param parent - parent object
 *	\param name - name of object
 */
aObjectList::aObjectList( aCfgItem context, aDatabase *adb, QObject *parent, const char *name )
:QObject( parent, name )
{
	filtred = false;
	vInited = false;
	db = adb;
	if ( adb )
	{
		setObject( context );
	}
}



/*!
 *	virtual destructor.
 */
aObjectList::~aObjectList()
{
}



/*!
 *	Tune on metadata object and it's database tables.
 *	\param adb - link on database object
 *	\return error code
 */
ERR_Code
aObjectList::init()
{
	if ( isInited() ) return err_noerror;
	return initObject();
}



/*!
 *	Set new object type after create
 *	/param newobject - new md object
 *	\return error code
 */
ERR_Code
aObjectList::setObject( aCfgItem newobject )
{
	setInited( false );
	obj = newobject;
	return init();
}



/*!
 *	Init object after create.
 *	Need setObject( id ), where id - if of the metadata object of the adb->cfg loaded Configuration.
 *	\return error code
 */
ERR_Code
aObjectList::initObject()
{
	aCfgItem fg, f;
	QString tname;

	setInited( true );
//	db = adb;
	md = 0;
	if ( db ) md = &db->cfg;
	else
	{
		aLog::print(aLog::Error, tr("aObjectList have no database!"));
		return err_nodatabase;
	}
	if ( obj.isNull() )
	{
		aLog::print(aLog::Error, tr("aObjectList md object not found"));
		return err_objnotfound;
	}
	return err_noerror;
}





/*!
 * Return field value of the primary object database table.
 */
QVariant
aObjectList::Value( const QString & name )
{

	aDataTable *t = table;
        if ( t ) return t->value( name );
        return QVariant("");
}



/*!
 * Set field value of the primary object database table.
 */
int
aObjectList::SetValue( const QString & name, const QVariant &value )
{
	aDataTable *t = table;
	if ( t )
	{
		t->setValue( name, value );
		return err_noerror;
	}
	return err_notable;
}



/*!
 *	Check object selecting.
 *	\return true if object record selected in database.
 */
bool
aObjectList::IsSelected()
{
	return selected();
}



/*!
 *
 */
bool
aObjectList::IsMarkDeleted()
{
        aDataTable *t = table;
        if ( t && t->sysFieldExists( "df" ) ) return t->sysValue( "df" ).toInt() == 1;
        return false;
}



/*!
 *
 */
bool
aObjectList::IsMarked()
{
        aDataTable *t = table;
        if ( t && t->sysFieldExists( "mf" ) ) return t->sysValue( "mf" ).toInt() == 1;
        return false;
}



/*!
 *
 */
int
aObjectList::SetMarkDeleted( bool Deleted )
{
	aDataTable *t = table;
	if ( t && t->sysFieldExists( "df" ) )
	{
		QString v = "0";
		if ( Deleted ) v = "1";
		t->setSysValue( "df", QVariant( v ) );
		return err_noerror;
	}
	else
	{
		aLog::print(aLog::Error, tr("aObjectList have no system field %1").arg("df"));
		return err_incorrecttype; // Object can not be mark deleted
	}
}



/*!
 *
 */
int
aObjectList::SetMarked( bool Marked )
{
        aDataTable *t = table;
        if ( t && t->sysFieldExists( "mf" ) ) {
                QString v = "";
                if ( Marked ) v = "1";
                t->setSysValue( "mf", QVariant( v ) );
//                t->printRecord();
                return err_noerror;
        }
	aLog::print(aLog::Error, tr("aObjectList have no system field %1").arg("mf"));
        return err_incorrecttype; // Object can not be marked
}





/*!
 * \ru
 *	Позиционирует указатель в БД на запись, соотвествующую объекту
 *	с указанным идентификатором.
 *	\param id - Идентификатор объекта.
 * 	\return возвращает код ошибки или 0 в случае успеха.
 * \_ru
 */
ERR_Code
aObjectList::select( qulonglong id )
{
	aDataTable * t = table;
	if ( !t ) return err_notable;
	setSelected (false);
	long otype = db->uidType( id );
	if ( !otype ) return err_objnotfound;
//	if ( concrete && ( otype != t->getMdObjId() ) ) return err_incorrecttype;
//	if ( !concrete )
//	{
		aCfgItem tmpObj = md->find( otype );
		if ( tmpObj.isNull() ) return err_objnotfound;
		setObject ( tmpObj );
//	}
	if ( t->select( QString("id=%1").arg(id), false ) )
		if ( t->first() )
		{
//			t->primeUpdate();
			setSelected (true);
//			t->printRecord();
			return err_noerror;
		}
		else return err_notselected;
	return err_selecterror;
}



/*!
 *
 */
ERR_Code
aObjectList::select(const QString & query )
{
	aDataTable * t = table;
	if ( !t ) return err_notable;
	if (t->select(query))
		if( t->first() )
		{
			setSelected (true);
			return err_noerror;
		}
		else return err_notselected;
	return err_selecterror;
}





/*!
 *
 */
bool
aObjectList::Next()
{
	return table->next();
}



/*!
 *
 */
bool
aObjectList::Prev()
{
	return table->prev();
}



/*!
 *
 */
bool
aObjectList::First()
{
	return table->first();
}



/*!
 *
 */
bool
aObjectList::Last()
{
	return table->last();
}

/*!
 *	\ru
 *	Возвращает уникальный идентификатор объекта из базы данных.
 *	В качестве объекта например может выступать "Приходная накладная" от такого-то числа за таким то номером.
 *	Каждый вновь созданный в системе документ или элемент справочника, включая группы справочника имеет свой уникальный
 *	неповторяющийся идентификатор. Если какое-либо поле, какого-либо объекта имеет тип Объект (например Document.Накладная),
 *	то в качестве значения ему нужно задавать уникальный идентификатор объекта, возвращаемый функцией Uid().
 *	Не существует возможности изменить существующий идентификатор какого-либо объекта. Созданием и управлением
 *	идентификаторами объектов занимается система.
 *	\return строка со значением уникального идентификатора.
 *	\_ru
 */
QString
aObjectList::Uid()
{
	return QString::number(getUid());
}


/*!
 *
 */
qulonglong
aObjectList::getUid()
{
	qulonglong Uid = 0;
	if ( selected() ) Uid = table->sysValue("id").toULongLong();
	return Uid;
}



/*!
 *
 */
void
aObjectList::setSelected( bool sel )
{
	selectFlag = sel;
}



/*!
 *
 */
bool
aObjectList::selected()
{
	return selectFlag;
}



/*!
 *
 */
ERR_Code
aObjectList::SetFilter( const QString & valname, const QVariant & value )
{
	aDataTable *t = table;
	if ( !t ) return err_notable;
	if ( t->setFilter( valname, value ) ) return err_noerror;
	else return err_fieldnotfound;
}



/*!
 *
 */
ERR_Code
aObjectList::ClearFilter()
{
	aDataTable * t = table;
	if ( !t ) return err_notable;
	t->clearFilter();
	return err_noerror;
}




/*!
 *
 */
QString
aObjectList::displayString()
{
	QString res="***";
        int stdfc = 0, fid;
        aCfgItem sw, f;

	sw = displayStringContext();
//	if ( md->objClass( obj ) == md_catalogue ) {
//        	sw = md->find( md->find( obj, md_element ), md_string_view );
//	} else {
//        	sw = md->find( obj, md_string_view );
//	}
        if ( !sw.isNull() ) {
                stdfc = md->attr( sw, mda_stdf ).toInt();
                switch ( stdfc ) {
                case 0:
                        fid = md->sText( sw, md_fieldid ).toInt();
                        res = table->sysValue( QString( "uf%1" ).arg( fid ) ).toString();
//printf("fid=%i res=%s\n",fid, ( const char *) res );
                        break;
                case 1:
                        break;
                case 2:
                        break;
                }
        }
	else
	{

		aLog::print(aLog::Debug, tr("aObjectList display string context is null"));
	}
//	res =
	return res;
}




aCfgItem
aObjectList::displayStringContext()
{
       return md->find( obj, md_string_view );
}



aObject *
aObjectList::currentObject() const
{
	return 0;
}
