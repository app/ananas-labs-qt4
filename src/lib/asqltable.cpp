/****************************************************************************
** $Id: asqltable.cpp,v 1.3 2008/11/09 21:09:11 leader Exp $
**
** Code file of the Ananas database table of Ananas
** Designer and Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
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
//#include <stream.h>

#include "asqltable.h"
#include <qdatetime.h>
//Added by qt3to4:
#include <QSqlError>
#include "adatabase.h"
#include <q3sqlcursor.h>
#include <Q3SqlFieldInfo>

#include "acatalogue.h"
#include "adocument.h"
#include "aaregister.h"
#include "alog.h"

/*!
 *	\~english
 *	Constructor
 *	\~russian
 *	Создает таблицу доступа к базе данных, позволяющую работать с одной
 *	sql таблицей или одним sql представлением (view).
 *	\param context - объект метаданных, который описывает таблицу.
 *	\param adb - ссылка на объект базы данных, которой
 *	принадлежит sql таблица.
 *	\~
 */
aDataTable::aDataTable( aCfgItem context, aDatabase *adb )
:Q3SqlCursor( QString::null, false, *adb->db() )
{
	db = adb;
	md = &db->cfg;
	mdobjId = 0;
	tableName = db->tableDbName( db->cfg, context, &mdobjId );
	if ( !tableName.isEmpty() ) {
		setName( tableName, true );
		init (context, adb );
	}
	selected = false;
	p_cat.setAutoDelete ( true );
	p_reg.setAutoDelete ( true );
	p_doc.setAutoDelete ( true );
}



/*!
 *	\~english
 *	Constructor
 *	\~russian
 *	Создает таблицу доступа к базе данных, позволяющую работать с одной
 *	sql таблицей или одним sql представлением (view).
 *	\param tname - имя sql таблицы в базе данных
 *	\param adb - ссылка на объект базы данных, которой
 *	принадлежит sql таблица.
 *	\~
 *
 */
aDataTable::aDataTable( const QString &tname, aDatabase *adb )
:Q3SqlCursor( tname, true, *adb->db() )
{
	db = adb;
	md = &db->cfg;
	tableName = tname;
	mdobjId = 0;
	selected = false;
	p_cat.setAutoDelete ( true );
	p_reg.setAutoDelete ( true );
	p_doc.setAutoDelete ( true );

}

/*!
 *	\~english
 *	Destructor
 *	\~russian
 *	Деструктор
 *	\~
 *
 */
aSQLTable::~aSQLTable()
{
//	delete p_reg;
//	delete p_cat;
	p_reg.clear();
	p_cat.clear();
	p_doc.clear();
}

/*!
 *	\~english
 *	Inits table object.
 *	\~russian
 *	Инициализирует объект.
 *	\see setObject()
 *	\param context - объект метаданных, который описывает таблицу.
 *	\param adb - ссылка на объект базы данных, которой
 *	принадлежит sql таблица.
 *	\~
 */
void
aDataTable::init( aCfgItem context, aDatabase *adb )
{
	db = adb;
	fnames.clear();
	fnames.setAutoDelete( true );
	userFilter.clear();
	userFilter.setAutoDelete( true );
	setObject( context );

}



/*!
 *	\~english
 *	Sets md object to table.
 *	\~russian
 *	Задает объект метаданных для таблицы.
 *	Заполняет внутренние объекты именами полей для последующего использования в
 *	функциях SetValue() и Value(), добавляет информацию о них в sql курсор.
 *	В случае, если добавляется накопительный регистр, к объетку добавляются также виртуальные поля - ресурсы регистра
 *	После вызова этой функции с таблицей можно начинать работу.
 *	\param context - объект метаданных, который описывает таблицу.
 *	\~
 */
void
aDataTable::setObject( aCfgItem context )
{
	aCfgItem cobj, parent;
	parent = obj = context;


	mdobjId = md->id(obj);
	while ( !mdobjId )
	{
		parent = md->parent(parent);
		mdobjId = md->id(parent);
	}
	if ( context.isNull() )
	{
		aLog::print(aLog::Error,QObject::tr("aDataTable try set mdobject to null"));
		return;
	}
	mapCat.clear();
	mapDoc.clear();
	mapReg.clear();
	mapDim.clear();
	mapSum.clear();
	//TODO:  test to memory leak
	p_cat.clear();
	p_reg.clear();
	p_doc.clear();
//	printf("before delete p_cat\n");
//	if(p_cat)
	//delete p_cat;

//	p_cat.clear();
//	printf("after delete p_cat\n");
//	if(p_reg)
	//delete p_reg;
//	p_reg.clear();
	if(md->objClass(context) == md_field && md->objClass(md->parent(context))== md_dimensions)
	{
		insertFieldInfo(context,false);
		aCfgItem res;
		aCfgItem ress = md->findChild(md->parent(md->parent(context)),md_resources);
		uint n = md->count( ress, md_field );
		for ( uint i = 0; i < n; i++ )
		{
			res = md->find( ress, md_field, i );
			insertFieldInfo(res,false);
		}
	}
	uint n = md->count( context, md_field );
	for ( uint i = 0; i < n; i++ )
	{
		cobj = md->find( context, md_field, i );
		insertFieldInfo(cobj);

	}

//	r = *this;
}


/*!
 *	\~english
 *	Appends info about field to object.
 *	\~russian
 *	Добавляет информацию о поле к объекту.
 *	\param obj - объект метаданных, который описывает поле.
 *	\param calculatd - указывает будет ли поле вычисляемым.
 *	\~
 */
void
aSQLTable::insertFieldInfo(aCfgItem cobj, bool calculated)
{
	QString fname, fdbname, objt;//, fid;
	int fid ;

	if ( !cobj.isNull() )
	{
		fid = md->id(cobj);
		fname = md->attr(cobj, mda_name);
		objt = md->attr( cobj, mda_type ).upper();
			fdbname = QString("uf%1").arg( fid );
                        if ( objt[0]=='O' )
			{
				fnames.insert( fname, new QString(fdbname) );
        			fdbname = QString("text_uf%1").arg( fid );
                		append( Q3SqlFieldInfo( fdbname, QVariant::String ) );
		             //   setGenerated( fdbname, false );
		                setCalculated( fdbname, calculated );
				int ftid = objt.section(" ", 1, 1 ).toInt();
				aCfgItem fto = md->find( ftid );
				if ( !fto.isNull() )
				{
					if ( md->objClass( fto ) == md_catalogue )
					{
						mapCat[fid] = fto;
					}
					if ( md->objClass( fto ) == md_document )
					{
						mapDoc[fid] = fto;
					}
				}
                        }
			else
                        if ( objt[0]==' ' )
			{
        			fdbname = QString("text_uf%1").arg( fid );
                		append( Q3SqlFieldInfo( fdbname, QVariant::String ) );
		                setCalculated( fdbname, calculated );
		               // setGenerated( fdbname, false );
				fnames.insert( fname, new QString(fdbname) );
				int ftid = objt.section(" ", 1, 1 ).toInt();
				aCfgItem fto = md->find( ftid );
				if ( !fto.isNull() )
				{
					if ( md->objClass( fto ) == md_aregister )
					{
					aCfgItem s_field = md->find(objt.section(" ",2,2).toInt());
					aCfgItem dim_fields = md->find(fto,md_dimensions);

						if(!dim_fields.isNull())
						{
							int cnt = md->count( dim_fields, md_field );
							for ( int k = 0; k < cnt; k++  )
							{
								aCfgItem dim_field = md->find( dim_fields, md_field, k );
								QString type =  md->attr(dim_field, mda_type);
								if(type[0]=='O')
								{

									if(type.section(" ",1,1).toInt()== mdobjId)
									{
										mapReg[fid]=fto;
										mapDim[fid]= md->attr(dim_field,mda_name);
										mapSum[fid] = md->attr(s_field,mda_name);
										break;
									}
								}
							}
						}
					}
				}

                        }
			else
			{
				fnames.insert( fname, new QString(fdbname) );
			}
	}
}


/*!
 *	\~english
 *	Gets metadata object.
 *	\~russian
 *	Получение объекта метаданных, которым инициализировали таблицу.
 *	\return объект метаданных, который описывает таблицу.
 *	\~
 */
qulonglong
aDataTable::getIdd(void)
{
	return 0;
}
long
aDataTable::getMdObjId()
{
	return mdobjId;
}



/*!
 *	\~english
 *	Stub.
 *	\~russian
 *	Заглушка.
 *	\~
 */
void
aDataTable::appendField( aSQLField * ) // fieldinfo )
{

}



/*!
 *	\~english
 *	Stub.
 *	\~russian
 *	Заглушка.
 *	\~
 */
void
aDataTable::insertField( int /*pos*/, aSQLField * ) // fieldinfo )
{

}



/*!
 *	\~english
 *	Stub.
 *	\~russian
 *	Заглушка.
 *	\~
 */
void
aDataTable::removeField( int ) // pos )
{

}



/*!
 *	\~english
 *	Stub.
 *	\~russian
 *	Заглушка.
 *	\~
 */
void
aDataTable::clearFields()
{

}



/*!
 *	\~english
 *	Stub.
 *	\~russian
 *	Заглушка.
 *	\~
 */
bool
aDataTable::checkStructure(  bool ) //update )
{
	bool rc = false;
	if ( name().isEmpty() ) {

	} else {

	}
	return rc;
}



/*!
 *	\~english
 *	Return field falue.
 *	\~russian
 *	Возвращает значение поля с номером \a i.
 *	\param i - номер поля.
 *	\return значение поля
 *	\~
 */
QVariant
aDataTable::value ( int i )
{
 //       QVariant v = QSqlCursor::value( i );

	QVariant v = sysValue(Q3SqlCursor::fieldName(i));
	return v;
}



/*!
 *	\~english
 *	Return field falue.
 *	\~russian
 *	Возвращает значение поля с именем \a name.
 *	Для получения значения необходимо указывать имя поля в метаданных
 *	\param name - имя поля в метаданных.
 *	\return значение поля или QVariant::Invalid, если поля не существует.
 *	\~
 */
QVariant
aDataTable::value ( const QString & name )
{
	QString fname;
        QVariant v;

	if ( !fnames[name] )
	{
		aLog::print(aLog::Error, QObject::tr("aDataTable get value of unknown field `%1'").arg(name));
		return QVariant::Invalid;
	}
	fname = * fnames[ name ];

        //v = QSqlCursor::value( fname );

	return sysValue(fname);
}



/*!
 * Return true if column name exists in database table.
 */
 /*!
 *	\~english
 *	Check field existing.
 *	\~russian
 *	Проверяет существование поля в таблице
 *	\param name - имя поля в таблице (не в метаданных!).
 *	\return \~engish true if column name exists in database table \~russian true, если столбец с данным именем есть в таблице. \~
 *	\~
 */

bool
aDataTable::sysFieldExists( const QString & name )
{
	return contains( name );
}



/*!
 * Return value of the column of the database table.
 */
QVariant
aDataTable::sysValue ( const QString & name )
{
	if(isCalculated(name)) return calcFieldValue(name);
	else return Q3SqlCursor::value( name );
}



/*!
 *
 */
void
aDataTable::setSysValue ( const QString & name, QVariant value )
{
	if ( name == QString("pnum") )
	{
		aLog::print(aLog::Info, QObject::tr("aDataTable get document prefix to `%1'").arg(value.toString()));
	//	debug_message("document prefix set to '%s'\n",(const char*)value.toString());
	}

	Q3SqlCursor::setValue( name, value );
}



/*!
 * Set value of the column of the database table.
 */
void
aDataTable::setValue ( int i, QVariant value )
{
	Q3SqlCursor::setValue( i, value );
}



/*!
 *
 */
bool
aDataTable::setValue ( const QString & name, QVariant value )
{
	QString fname;
	if ( !fnames[name] ) return false;
	fname = * fnames[ name ];
	if ( contains( fname ) ) {
		Q3SqlCursor::setValue( fname, value );
	}
	else return false;
	return true;
}



/*!
 *
 */
QSqlRecord *
aDataTable::primeInsert()
{
	QSqlRecord *rec;
	QVariant v;

	rec = Q3SqlCursor::primeInsert();
	if ( sysFieldExists("id") ) {
		rec->setValue("id", QVariant( db->uid( mdobjId ) ) );
	}
	return rec;
}



/*!
 *
 *//*
QSqlRecord *
aDataTable::primeUpdate()
{
	QSqlRecord *rec;

	rec = QSqlCursor::primeUpdate();
	*rec = r;
	return rec;
}
*/


/*!
 *
 */
bool
aDataTable::select( const QString & filter, bool usefltr )
{
	bool res;
	QString flt = getFilter();
	if ( usefltr )
	{
		if ( flt == "" ) flt = filter;
		else if ( filter != "" ) flt = flt + " AND " + filter;
	}
	else flt = filter;
	res = Q3SqlCursor::select( flt );
//	next();
//	r = *this;
	return res;
}



/*!
 *
 */
bool
aDataTable::select( qulonglong id )
{
	bool res;
//	printf("aSQLTable::select %llu\n",id);
	res = Q3SqlCursor::select( QString( "id=%1" ).arg( id ) );
	//setSelected(true);
//	next();
//	r = *this;
	return res;
}



/*!
 *
 */
void
aDataTable::clearFilter()
{
	aLog::print(aLog::Debug, QObject::tr("aDataTable clear filter"));
	userFilter.clear();
}


/*QDict<QVariant>
aDataTable::getUserFilter() {
	return this.userFilter;
};

void
setUserFilter( QDict<QVariant> newFilter) {
	this.userFilter = newFilter;
};
*/

/**
 *
 */
void
aDataTable::setFilter ( const QString & newFilter ) {
	Q3SqlCursor::setFilter( newFilter );
}


/*!
 *
 */
bool
aDataTable::setFilter( const QString& name, const QVariant& value )
{

	aLog::print(aLog::Debug, QObject::tr("aDataTable set filter %1='%2'").arg(name).arg(value.toString()));
	if ( !fnames[name] )
	{
		aLog::print(aLog::Error, QObject::tr("aDataTable set filter %1='%2', %3 not exist").arg(name).arg(value.toString()).arg(name));
		return false;
	}
	QVariant *v = new QVariant(value);
	userFilter.replace(*fnames[name], v );
	Q3SqlCursor::setFilter(getFilter());
	return true;
}



/*!
 *
 */
QString
aDataTable::getFilter()
{
	QString filter = "", fid, type;
	aCfgItem field;
	Q3DictIterator<QVariant>it( userFilter );
	if ( it.toFirst() )
	{
		fid = it.currentKey().mid(2);
		field = md->find(fid.toLong() );
		if ( !field.isNull() )
		{
			type = md->attr( field, mda_type );
			if ( type[0] == 'N' || type[0] == 'O' )
				filter = it.currentKey() + "=" + it.current()->toString();
			else
				filter = it.currentKey() + "='" + it.current()->toString() + "'";
		}
	}
	++it;
	for (;it.current();++it)
	{
		fid = it.currentKey().mid(2);
		field = md->find(fid.toLong() );
		if ( !field.isNull() )
		{
			type = md->attr( field, mda_type );
			if ( type[0] == 'N' || type[0] == 'O' )
				filter += " and " + it.currentKey() + "=" + it.current()->toString();
			else
				filter += " and " + it.currentKey() + "='" + it.current()->toString() + "'";
		}
	}
	return filter;
}



/*!
 *
 */
QString
aDataTable::getNFilter()
{
	QString filter = "", fid, type;
	aCfgItem field;
	Q3DictIterator<QVariant>it( userFilter );
	if ( it.toFirst() )
	{
		fid = it.currentKey().mid(2);
		field = md->find(fid.toLong() );
		if ( !field.isNull() )
		{
			type = md->attr( field, mda_type );
			if ( type[0] == 'N' || type[0] == 'O' )
				filter = tableName + "." + it.currentKey() + "=" + it.current()->toString();
			else
				filter = tableName + "." + it.currentKey() + "='" + it.current()->toString() + "'";
		}
	}
	++it;
	for (;it.current();++it)
	{
		fid = it.currentKey().mid(2);
		field = md->find(fid.toLong() );
		if ( !field.isNull() )
		{
			type = md->attr( field, mda_type );
			if ( type[0] == 'N' || type[0] == 'O' )
				filter += " and " + tableName + "." + it.currentKey() + "=" + it.current()->toString();
			else
				filter += " and " + tableName + "." + it.currentKey() + "='" + it.current()->toString() + "'";
		}
	}
	return filter;
}



/*!
 *
 */
void
aDataTable::printRecord(){
	unsigned int i;
        Q3DictIterator<QString> it( fnames );
	QString fname, sname;

	for (i=0; i< count(); i++){
		fname = "";
		sname = field( i ).name();
		it.toFirst();
	        for( ; it.current(); ++it ){
		    if ( *it.current() == sname ) {
			fname = it.currentKey();
			break;
		    }
		}
		printf("%s(%s)=%s\n",
		( const char *) fname,
		( const char *) sname,
		( const char *) value( i ).toString().local8Bit() );
	}

/*	for (i=0; i< r.count(); i++){
		fname = "";
		sname = r.field( i )->name();
		it.toFirst();
	        for( ; it.current(); ++it ){
		    if ( *it.current() == sname ) {
			fname = it.currentKey();
			break;
		    }
		}
		printf("r:%s(%s)=%s\n",
		( const char *) fname,
		( const char *) sname,
		( const char *) r.value( i ).toString().local8Bit() );
	}
*/
}



/*!
 *
*/
bool
aDataTable::exec( QString query )
{
	return Q3SqlCursor::exec( query );
}



QVariant
aDataTable::calc_rem(int fid, qulonglong id)
{
	aCfgItem o,fto;
	QString t,oclass;
	int ftid,oid;
	QVariant v="";
	fto = mapReg[fid];// = md->find( ftid );
	if ( !fto.isNull() )
	{
		if(p_reg[QString("%1").arg(fid)]==0)
		{
			p_reg.insert(QString("%1").arg(fid), new aARegister( fto, db ));
		}
		v = ((aARegister*)p_reg[QString("%1").arg(fid)])->getSaldo( QDateTime::currentDateTime(),
					mapDim[fid],
					id,
					mapSum[fid]);//md->attr(s_field,mda_name));
		if(!v.isValid()) v = "";
	}
	return v;
}


QVariant
aDataTable::calc_obj(int fid, qulonglong idd)
{
	aCfgItem o,fto;
	QString t,oclass;
	int ftid;
	QVariant v="";
//	printf("calculate cat %d, %llu\n",fid, idd);
	o = mapCat[fid];
	if( !o.isNull() )
	{
		aCatalogue *pCat = (aCatalogue*)p_cat[QString("%1").arg(fid)];
//		printf("obj not null\n");
		if(pCat==0)
		{
			p_cat.insert(QString("%1").arg(fid), new aCatalogue( o, db ));
			pCat = (aCatalogue*)p_cat[QString("%1").arg(fid)];
		}
		pCat->select( idd );

		if ( pCat->selected() )
		{
//			printf("select ok\n");
			v = QVariant( pCat->displayString() );
		}
	}
	else
	{
		o = mapDoc[fid];
		if(!o.isNull())
		{
			aDocument *pDoc = (aDocument*)p_doc[QString("%1").arg(fid)];
//			printf("obj not null\n");
			if(pDoc==0)
			{
				p_doc.insert(QString("%1").arg(fid), new aDocument( o, db ));
				pDoc = (aDocument*)p_doc[QString("%1").arg(fid)];
			}
			pDoc->select( idd );
			if ( pDoc->selected() )
			{
				v = QVariant( pDoc->displayString() );
			}
		}
	}

	return v;
}

/*!
 * Calculate value object type fields.
 */
QVariant
aDataTable::calculateField( const QString &name )
{
	QVariant v;
	if(name=="system_icon") return v;
	else return QVariant("");
//	return QVariant::Invalid;
}

/*!
 * Calculate value object type fields.
 */
QVariant
aDataTable::calcFieldValue( const QString &name )
{

        QVariant v="", fv=0;
	int fid = 0; //, ftid;
	qulonglong id = 0;
	aCfgItem o, fto;
	QString t, oclass;
        if ( name.left(5)=="text_" )
	{
                if(sysFieldExists(name.mid(5)))
		{
			fv = sysValue( name.mid( 5 ) );
			id = fv.toULongLong();
		}
		else
		{
			id = 0;
		}
		fid = name.mid( 7 ).toInt();
		v = QVariant("");
		if ( fid && id )
		{
			v=calc_obj(fid,id);
		}
		else
		{
			if(fid)
			{
				id = sysValue("id").toULongLong();
				v=calc_rem(fid,id);
			}
		}
        }
//	printf("calculate field %s\n", name.ascii());
        return v;
}



/*!
 * Gets list of user and calculation fields.
 */
QStringList
aDataTable::getUserFields()
{
   QStringList lst;
   uint i;
   const QString text_uf = "text_uf";
	for(i=0; i<count(); i++)
	{
		if(fieldName(i).left(2) == "uf"
		   || fieldName(i).left(text_uf.length()) == text_uf)
		{
			if(fieldName(i).left(text_uf.length()) == text_uf)
			{
				QStringList::iterator it  = lst.find("uf"+fieldName(i).mid(text_uf.length()));
				if(it!=lst.end())
				{
					lst.remove(it);
				}
			}
			lst << fieldName(i);
		}
	}
   return lst;
}



/*!
 *
 */
bool
aDataTable::isMarkDeleted()
{
        if ( sysFieldExists( "df" ) ) return sysValue( "df" ).toInt() == 1;
        return false;
}



/*!
 *
 */
ERR_Code
aDataTable::setMarkDeleted( bool Deleted )
{
        if ( sysFieldExists( "df" ) ) {
                QString v = "";
                if ( Deleted ) v = "1";
                setSysValue( "df", QVariant( v ) );
                return err_noerror;
        }
        return err_incorrecttype; // Object can not be mark deleted
}


bool
aDataTable::seek ( int i, bool relative )
{
	bool res = Q3SqlCursor::seek( i, relative );
//	if ( res ) r = *this;
	return res;
}

bool
aDataTable::next ()
{
	bool res = Q3SqlCursor::next();
//	if ( res ) r = *this;
	return res;
}

bool
aDataTable::prev ()
{
	bool res = Q3SqlCursor::prev();
//	if ( res ) r = *this;
	return res;
}

bool
aDataTable::first ()
{
	bool res = Q3SqlCursor::first();
//	if ( res ) r = *this;
	return res;
}


bool
aDataTable::last ()
{
	bool res = Q3SqlCursor::last();
//	if ( res ) r = *this;
	return res;
}



bool
aDataTable::New()
{
	QSqlRecord *rec;
	QVariant v;
	qulonglong Uid = 0;
	bool res = false;

	rec = Q3SqlCursor::primeInsert();
	if ( sysFieldExists("id") )
	{
		Uid = db->uid( mdobjId );
		aLog::print(aLog::Debug, QString("aDataTable new record with id=%1 for meta object with id=%2").arg(Uid).arg(mdobjId));

		rec->setValue("id", QVariant( Uid ) );
	}
	if ( insert() )
	{
		if ( select(QString("id=%1").arg(Uid), false) )
			if ( first() )
			{
				res = true;
			}
			else
			{
				aLog::print(aLog::Error, QString("aDataTable record with id=%1 not found").arg(Uid));
			}
	}
	return res;
}


bool
aDataTable::Copy()
{
	QSqlRecord *rec = new QSqlRecord( *editBuffer(true) );
	if ( New() )
	{
		for ( unsigned int i=0; i<rec->count(); i++ )
		{
			if ( rec->field( i ).name() != QString("id")  )
			{
				setValue( i, rec->value( i ) );
//				printf("field %s, before %s, after %s\n",rec->field( i )->name().ascii(), rec->value( i ).toString().ascii(), value( i ).toString().ascii());
			}
		}
	}
	delete rec;
	return Update();
//	Q_ULLONG Uid = db->uid( t->id );
//	r->setValue("id",Uid);
//	if ( t->insert() ) return Uid;

//	return true;
}


bool
aDataTable::Delete()
{
	QSqlRecord *rec;
	rec = primeDelete();
	del();
//	fNewNotUpdated = false;
	return true;
}


bool
aDataTable::Update()
{
	QSqlRecord *rec;

	rec = Q3SqlCursor::primeUpdate();
	//for ( unsigned int i=0; i<rec->count(); i++ ) rec->setValue( i, value( i ) );
	update();
	QSqlError err = lastError();
	if(!err.type() == QSqlError::None)
	{
		aLog::print(aLog::Error, QString("%1 %2").arg(err.text()).arg(err.driverText()) );
	}
	return true;
}

