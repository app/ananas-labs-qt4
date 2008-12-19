/****************************************************************************
** $Id: aobject.cpp,v 1.3 2008/11/09 21:09:11 leader Exp $
**
** Code file of the Ananas Object of Ananas
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
#include "aobject.h"
#include "aform.h"
#include "alog.h"


/*!
 *	\en
 *	Craeate abstract aObject.
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
aObject::aObject( QObject *parent, const char *name )
:QObject( parent, name )
{
	db = 0;
	vInited = false;
	filtred = false;
	selectFlag = false;
}



/*!
 *	\en
 *	Create aObject, inited by md object.
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
aObject::aObject( const QString &oname, aDatabase *adb, QObject *parent, const char *name )
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
 *	Create aObject, inited by md object.
 *	\param context - hi leve md object
 *	\param adb - link on object aDataBase used for work
 *	\param parent - parent object
 *	\param name - name of object
 */
aObject::aObject( aCfgItem context, aDatabase *adb, QObject *parent, const char *name )
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
aObject::~aObject()
{
}



/*!
 *	Tune on metadata object and it's database tables.
 *	\param adb - link on database object
 *	\return error code
 */
ERR_Code
aObject::init()
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
aObject::setObject( aCfgItem newobject )
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
aObject::initObject()
{
	aCfgItem fg, f;
	QString tname;

	setInited( true );
//	db = adb;
	md = 0;
	if ( db ) md = &db->cfg;
	else
	{
		aLog::print(aLog::Error, tr("aObject have no database!"));
		return err_nodatabase;
	}
	if ( obj.isNull() )
	{
		aLog::print(aLog::Error, tr("aObject md object not found"));
		return err_objnotfound;
	}
	return err_noerror;
}



/*!
 *
 */
bool
aObject::checkStructure()
{
	return false;
}



/*!
 *	Return the table of object by it's name.
 *	/param name - name of table for main table use name="" or empty parametr
 *	/return link on aDataTable or 0 if table not found
 */
aDataTable *
aObject::table( const QString &name )
{
	if ( !dbtables[ name ] )
	{
		if (name!="" && !name.isEmpty())
		{
			aLog::print(aLog::Error, tr("aObject table with name %1 not found").arg(name));
			cfg_message(1, tr("Table `%s' not found.\n").utf8(),(const char*) name);
		}
	//	else
	//	{
	//		cfg_message(1, tr("Table name is empty.\n").utf8());
	//	}
		return 0;
	}
	return dbtables[ name ];
}



/*!
 *	Insert table name and it link into internal buffer.
 *	used for finding table by it's md name or some default name
 *	/param dbname - database name of table
 *	/param obj - md object, used for aDataTable initing
 *	/param name - name of table, used for finding table in buffer
 *	/return error code
 */
ERR_Code
aObject::tableInsert( const QString &dbname,  aCfgItem obj, const QString &name )
{
	if ( db )
	{
		aDataTable *t = db->table( dbname );
		if ( !t )
			return err_notable;
		t->setObject( obj );
		dbtables.insert( name, t );
		return err_noerror;
	}
	aLog::print(aLog::Error, tr("aObject have no database!"));
	return err_nodatabase;
}



/*!
 *	Insert table name and it link into internal buffer.
 *	used for finding table by it's md name or some default name
 *	table not inited by md object
 *	/param dbname - database name of table
 *	/param name - name of table, used for finding table in buffer
 *	/return error code
 */
ERR_Code
aObject::tableInsert( const QString &dbname, const QString &name )
{
	if ( db ) {
		aDataTable *t = db->table( dbname );
		if ( !t ) return err_notable;
		dbtables.insert( name, t );
		return err_noerror;
	}
	aLog::print(aLog::Error, tr("aObject have no database!"));
	return err_nodatabase;
}



/*!
 *	Remove table from buffer.
 *	/param name - table name
 *	/return err_notable if table not found
 */
ERR_Code
aObject::tableRemove( const QString &name )
{
	if ( !dbtables[name] )
	{
		aLog::print(aLog::Error, tr("aObject table with name %1 not found").arg(name));
		return err_notable;
	}
	dbtables.remove( name );
	return err_noerror;
}



/*!
 *
 */
QString
aObject::trSysName( const QString & )
{
	return "";
}



/*!
 * Gets system field value.
 * \param name (in) -  field name.
 * \return field value or QVariant::Invalid if field no exist.
 */
QVariant
aObject::sysValue( const QString & name, const QString &tableName )
{
	aDataTable *t = table( tableName );
	if ( t && t->sysFieldExists( name ) )
	{
		return	t->sysValue(name);
	}
	else return QVariant::Invalid;
}



/*!
 * Sets system field value.
 * \param name (in) - field name.
 * \param value (in) - sets value.
 */
int
aObject::setSysValue( const QString & name, QVariant value, const QString &tableName )
{
	aDataTable *t = table( tableName );
	if ( t )
	{
		t->setSysValue( name, value );
		return err_noerror;
	}
	return err_notable;
}



/*!
 * Return field value of the primary object database table.
 */
QVariant
aObject::Value( const QString & name, const QString &tableName )
{

        aDataTable *t = table( tableName );
	QString trName = trSysName(name);

	if ( trName != "" ) return sysValue( trName );
	else
	{
	        if ( t ) return t->value( name );
	}
        return QVariant("");
}



/*!
 * Set field value of the primary object database table.
 */
int
aObject::SetValue( const QString & name, const QVariant &value, const QString &tableName )
{
	aDataTable *t = table( tableName );
	QString trName = trSysName(name);

	if ( trName != "" ) return setSysValue( trName, value );
	else
	{
		if ( t )
		{
			t->setValue( name, value );
			return err_noerror;
		}

	}
	return err_notable;
//	return setTValue( "", name, value );
}



/*!
 *	Check object selecting.
 *	\return true if object record selected in database.
 */
bool
aObject::IsSelected()
{
	return selected();
}



/*!
 *
 */
bool
aObject::IsMarkDeleted(const QString & tname)
{
        aDataTable *t = table( tname );
        if ( t && t->sysFieldExists( "df" ) ) return t->sysValue( "df" ).toInt() == 1;
        return false;
}



/*!
 *
 */
bool
aObject::IsMarked()
{
        aDataTable *t = table();
        if ( t && t->sysFieldExists( "mf" ) ) return t->sysValue( "mf" ).toInt() == 1;
        return false;
}



/*!
 *
 */
/*
int
aObject::TableSetMarkDeleted( bool Deleted, const QString & tname )
{
        aDataTable *t = table( tname );
        if ( t && t->sysFieldExists( "df" ) ) {
                QString v = "0";
                if ( Deleted ) v = "1";
                t->setSysValue( "df", QVariant( v ) );
                return err_noerror;
        }
        return err_incorrecttype; // Object can not be mark deleted
}
*/


/*!
 *
 */
int
aObject::SetMarkDeleted( bool Deleted, const QString & tname )
{
	aDataTable *t = table( tname );
	if ( t && t->sysFieldExists( "df" ) )
	{
		QString v = "0";
		if ( Deleted ) v = "1";
		t->setSysValue( "df", QVariant( v ) );
		return err_noerror;
	}
	else
	{
		aLog::print(aLog::Error, tr("aObject have no system field %1").arg("df"));
		return err_incorrecttype; // Object can not be mark deleted
	}
}



/*!
 *
 */
int
aObject::SetMarked( bool Marked )
{
        aDataTable *t = table();
        if ( t && t->sysFieldExists( "mf" ) ) {
                QString v = "";
                if ( Marked ) v = "1";
                t->setSysValue( "mf", QVariant( v ) );
//                t->printRecord();
                return err_noerror;
        }
	aLog::print(aLog::Error, tr("aObject have no system field %1").arg("mf"));
        return err_incorrecttype; // Object can not be marked
}



/*!
 * Add new object record in database.
 */
int
aObject::New()
{
	aDataTable *t = table();

	if ( !t ) return err_notable;
	setSelected ( t->New() );
/*	Q_ULLONG Uid = t->primeInsert()->value("id").toULongLong();
	if ( t->insert() )
	{
		if ( t->select(QString("id=%1").arg(Uid), false) )
			if ( t->first() )
			{
				setSelected(true);
				return err_noerror;
			}
		return err_selecterror;
	}
*/	if ( selected() ) return err_noerror;
	return err_inserterror;
}



/*!
 * Copy current selected object data in database.
 */
/*Q_ULLONG
aObject::copy( const QString & tablename )
{
	aDataTable * t = table( tablename );

	if ( !t ) return 0;
	if ( !selected(tablename) ) return 0;
	QSqlRecord * r = t->primeUpdate();
	Q_ULLONG Uid = db->uid( t->id );
	r->setValue("id",Uid);
	if ( t->insert() ) return Uid;
	else return 0;
}
*/


/*!
 *
 */
int
aObject::Copy()
{
//	QSqlRecord r;

//	Q_ULLONG Uid = copy();
//	if ( !Uid ) return err_copyerror;
	aDataTable *t = table();
	if ( t->Copy() ) return err_noerror;
//	if ( t->select(QString("id=%1").arg(Uid)) )
//		if ( t->first() )
//			return err_noerror;
	return err_copyerror;
}



/*!
* Delete curent selected object record from database.
*/
int
aObject::Delete()
{
	aDataTable * t = table();
	if ( !t ) return err_notable;
	db->markDeleted(getUid());
	t->Delete();
//	if ( !selected() ) return err_notselected;
//	t->primeDelete();
//	t->del();
	setSelected (false);
	return err_noerror;
}



/*!
 *\~english
 *	Update curent selected object record to database.
 *\~russian
 *\~
 */
int
aObject::Update()
{
	aDataTable *t = table();
	QSqlRecord *r;
	int i;

	if ( !t ) return err_notable;
	t->Update();
/*
	r = t->primeUpdate();
	t->printRecord();
	for ( i=0;i<r->count();i++ ) r->setValue( i, t->value( i ) );

	t->update();
*/
	if ( t->lastError().type() )
	{
		//debug_message("update error %i %s\n",t->lastError().type(), ( const char *)t->lastError().text());

		aLog::print(aLog::Error, tr("aObject update error. Driver message: %1").arg(t->lastError().text()));
		return err_updateerror;
	}
	else {
		return err_noerror;
	}
}



/*!
 *\~english
 *	Update object attributes from curent selected object database record.
 *\~russian
 *\~
 *//*
void
aObject::updateAttributes( const QString & tname )
{
	aDataTable *t = table();

}
*/

/*!
 *\~english
 *	Conduct document.
 *	Do nothing. Added for wDocument compatibility.
 *\~russian
 *	Проводит документ.
 *	Ничего не делает. Предназначена для совместимости и работы в wDocument.
 *\~
 *\return \~english error code - abstract object.\~russian код ошибки - абстрактный обект.\~
 */
int
aObject::Conduct()
{
	return err_abstractobj;
}

/*!
 *\~english
 *	UnConduct document.
 *	Do nothing. Added for wDocument compatibility.
 *\~russian
 *	Распроводит документ.
 *	Ничего не делает. Предназначена для совместимости и работы в wDocument.
 *\~
 *\return \~english error code - abstract object.\~russian код ошибки - абстрактный обект.\~
 */
int
aObject::UnConduct()
{
	return err_abstractobj;
}

bool
aObject::IsConducted()
{
	return 0;
}



/*!
 *\~english
 *	Return document database id.
 *	always return 0. Added for wJournal compatibility.
 *\~russian
 *	Возвращает id документа в базе данных.
 *	Всегда возвращает 0. Предназначена для совместимости и работы в wJournal.
 *\~
 *\return \~english 0.\~russian 0.\~
 */
qulonglong
aObject::docId()
{
	return 0;
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
aObject::select( qulonglong id )
{
	aDataTable * t = table();
	if ( !t ) return err_notable;
	setSelected (false);
	long otype = db->uidType( id );
//	debug_message("otype=%li\n",otype);
	if ( !otype ) return err_objnotfound;
	if ( concrete && ( otype != t->getMdObjId() ) ) return err_incorrecttype;
	if ( !concrete )
	{
		aCfgItem tmpObj = md->find( otype );
		if ( tmpObj.isNull() ) return err_objnotfound;
		setObject ( tmpObj );
	}
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
aObject::select(const QString & query, const QString &tableName)
{
	aDataTable * t = table(tableName);
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
 * Return field value of the secondary object database table.
 */
QVariant
aObject::tValue( const QString & tablename, const QString & name )
{
        aDataTable *t = table( tablename );
//CHECK_POINT
        if ( t ) return t->value( name );
        return QVariant("");
}



/*!
 * Set field value of the secondary object database table.
 */
ERR_Code
aObject::setTValue( const QString & tablename, const QString & name, const QVariant &value )
{
	aDataTable *t = table( tablename );
	if ( t )
	{
		t->setValue( name, value );
		return err_noerror;
	}
	return err_notable;
}



/*!
 *
 */
ERR_Code
aObject::decodeDocNum( QString nm, QString & pref, int & num)
{

	aLog::print(aLog::Debug, tr("aObject decode doc number %1").arg(nm));
	int pos = -1;
	for ( uint i = nm.length(); i > 0; i-- )
	{
		if ( ( nm.at(i-1) >='0' ) && ( nm.at(i-1) <= '9' ) )
			continue;
		else
		{
			pos = i;
			break;
		}
	}
	if ( pos == -1 )
	{
//CHECK_POINT
		pref = "";
		num = nm.toInt();
		return err_incorrectname;
	}
	if ( pos == ( int ) nm.length() )
	{
//CHECK_POINT
		pref = nm;
		num = -1;
		return err_incorrectname;
	}
//CHECK_POINT
	pref = nm.left( pos );
	num = nm.mid(pos).toInt();
	aLog::print(aLog::Debug, tr("aObject decode doc number ok, pref=%1 num=%2").arg(pref).arg(num));
	return err_noerror;
}



/*!
 *
 */
/*
bool
aObject::Next()
{
	return table()->next();
//	return dbtables[""]->next();
}
*/


/*!
 *
 */
/*
bool
aObject::Prev()
{
//	return dbtables[""]->prev();
	return table()->prev();
}
*/


/*!
 *
 */
/*
bool
aObject::First()
{
//	return dbtables[""]->first();
	return table()->first();
}
*/


/*!
 *
 */
/*
bool
aObject::Last()
{
//	return dbtables[""]->last();
	return table()->last();
}
*/


/*!
 *
 */
bool
aObject::Next( const QString& tableName )
{
	return table(tableName)->next();
}



/*!
 *
 */
bool
aObject::Prev( const QString& tableName )
{
	return table(tableName)->prev();
}



/*!
 *
 */
bool
aObject::First( const QString& tableName )
{
	return table(tableName)->first();
}



/*!
 *
 */
bool
aObject::Last( const QString& tableName )
{
	return table(tableName)->last();
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
aObject::Uid()
{
	return QString::number(getUid());
}


/*!
 *
 */
qulonglong
aObject::getUid()
{
	qulonglong Uid = 0;
	if ( selected() ) Uid = table()->sysValue("id").toULongLong();
	return Uid;
}



/*!
 *
 */
void
aObject::setSelected( bool sel, const QString & tablename )
{
	if ( tablename == "" ) selectFlag = sel;
	else table(tablename)->selected = sel;
}



/*!
 *
 */
bool
aObject::selected( const QString & tablename )
{
	if ( tablename == "" ) return selectFlag;
	else return table(tablename)->selected;
}



/*!
 *
 */
ERR_Code
aObject::setTFilter( const QString & tname, const QString & valname, const QVariant & value )
{
	aDataTable * t = dbtables[tname];
	if ( !t ) return err_notable;
	if ( t->setFilter( valname, value ) ) return err_noerror;
	else return err_fieldnotfound;
}



/*!
 *
 */
ERR_Code
aObject::clearTFilter( const QString & tname )
{
	aDataTable * t = dbtables[tname];
	if ( !t ) return err_notable;
	t->clearFilter();
	return err_noerror;
}



/*!
 *
 */
int
aObject::SetFilter( const QString & valname, const QVariant & value )
{
	int err = setTFilter( "", valname, value );
	filtred = !err;
	return err;
}



/*!
 *
 */
int
aObject::ClearFilter()
{
	filtred = false;
	return clearTFilter("");
}



/*!
 *
 */
int
aObject::TableSetFilter( const QString & tname, const QString & valname, const QVariant & value )
{
	return setTFilter( tname, valname, value );
}



/*!
 *
 */
int
aObject::TableClearFilter( const QString & tname )
{
	return clearTFilter(tname);
}



/*!
 * \ru
 * 	Обновляет базу данных данными табличной части объекта. Обычно вызывается
 * 	после метода TableSetValue.
 * 	\param tablename - имя таблицы. Необходим для указания имени, так как
 * 	в объекте возможно наличие нескольких табличных частей.
 * 	\return возвращает код ошибки или 0 в случае успеха.
 * \_ru
 */
int
aObject::TableUpdate( const QString & tablename )
{
	aDataTable *t = table( tablename );
	if ( !t )
	{

		aLog::print(aLog::Error, tr("aObject table update: no table found with name %1").arg(tablename));
		return err_notable;
	}
//	t->primeUpdate();
	t->Update();
	if (t->lastError().type())
	{

		aLog::print(aLog::Error, tr("aObject update error. Driver message: %1").arg(t->lastError().text()));
		return err_updateerror;
	}
	return err_noerror;
}



/*!
 *
 */
QString
aObject::displayString()
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
                        res = table()->sysValue( QString( "uf%1" ).arg( fid ) ).toString();
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

		aLog::print(aLog::Debug, tr("aObject display string context is null"));
	}
//	res =
	return res;
}




aCfgItem
aObject::displayStringContext()
{
       return md->find( obj, md_string_view );
}



/**
 * \ru
 * 	Вид объекта, так как он описан в метаданных.
 *  \_ru
 */
QString
aObject::Kind( const QString & name )
{
	QString wasKind = md->objClass( obj );
	if ( !name.isEmpty() ) {
	// Set new kind.

	}
	return wasKind;
}
