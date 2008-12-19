/****************************************************************************
** $Id: aaregister.cpp,v 1.3 2008/11/09 21:09:11 leader Exp $
**
** Code file of the Accumulation Register of Ananas
** Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) Andrey Paskal <app at lrn dot ru>, Yoshkar-Ola
** Copyright (C) 2005 Grigory Panov <gr1313 at mail.ru>, Yoshkar-Ola
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

#include	"aaregister.h"
#include	"adatabase.h"
#include	"adocjournal.h"
#include 	"alog.h"
#include	<stdlib.h>
//Added by qt3to4:
#include <QSqlQuery>


/*!
 *\en
 *	Constructor that create object of aAregister by metadata object.
 *\_en
 *\ru
 * Конструктор, который создает объект по элементу метаданных.
 *\_ru
 *\param context -\en configuration item, \_en \ru
 					элемент конфигурации,\_ru
 *\param adb -\en link on ananas database.\_en \ru
 					ссылка на базу данных.\_ru
 */
aARegister::aARegister( aCfgItem context, aDatabase * adb )
:aIRegister( context, adb )
{
	concrete = !context.isNull();
	initObject();
}



/*!
 *\en
 *	Сonstructor that create object of aAregister by metadata name
 *\_en
 *\ru
 * Конструктор, который создает объект по имени рагистра остатков из конфигурации.
 *\_ru
 *\param context -\en md name of register,\_en \ru
 	имя регистра, заданное в конфигурации через Ананас-Дизайнер,\_ru
 *\param adb -
 	\en link on ananas database.\_en
	\ru ссылка на базу данных. Необязательный параметр. Если не задан или задано нулевое
	значение, создавамый объект для работы с регистром не будет связан с базой данный.
	То есть объект не будет заполнен данными из регистра, хранящимися в базе данных,
	и все изменения, вносимые в него, не будут сохранены в базе данных.
	\_ru
 */
aARegister::aARegister( QString name, aDatabase *adb  )
:aIRegister( name, adb, "AccumulationRegister.")
{
	if ( name.isEmpty() ) concrete = false;
	else concrete = true;
	initObject();
}

/*!
 *\en
 *	Destructor
 *\_en
 *\ru
 *	Деструктор.
 *\_ru
 */
aARegister::~aARegister()
{
}


/*!
 *\en
 *	Initing this object by metadata object.
 *	Сreating used tables and filters
 *\_en
 *\ru
 *	Инициализирует объект элементом конфигурации.
 *	В процесе инициализации выполняется настройка на таблицы базы данных.
 *	Этот метод вызывается конструктором.
 *\_ru
 *\return  -\en  error code.\_en \ru код ошибки.\_ru
 */
ERR_Code
aARegister::initObject()
{
	int len_uf;
	//resnames.clear();
	ERR_Code err = aIRegister::initObject();
	if ( err )
	{
		aLog::print(aLog::Error, tr("Accumulation Register init"));
		return err;
	}
	err = tableInsert( db->tableDbName(*md,obj), obj );
	if ( err )
	{
		aLog::print(aLog::Error, tr("Accumulation Register create main table"));
		return err;
	}
	aCfgItem field, dims = md->find(obj, md_dimensions,0);
	aCfgItem ress = md->find(obj, md_resources,0);
	//if ( res.isNull() ) return err_noresources;
	tablename = table()->tableName;
	uint n = md->count( dims, md_field );
	for ( uint i = 0; i < n; i++  )
	{
		field = md->find( dims, md_field, i );
		err = tableInsert( db->tableDbName(*md,field), field, md->attr(field,mda_name) );
		if ( err )
		{
			aLog::print(aLog::Error, tr("Accumulation Register create saldo table"));
			return err;
		}
		long mdid = md->id(field);
		QString f_name = md->attr(field,mda_name);
		dimnames[mdid]=f_name;
	}
	ress = md->find(obj, md_resources,0);
	n = md->count( ress, md_field );
	for ( uint i = 0; i < n; i++  )
	{
		field = md->find( ress, md_field, i );
		long mdid = md->id(field);
		QString f_name = md->attr(field,mda_name);
		resnames[mdid]=f_name;
		resSysNames[f_name]=QString("uf%1").arg(md->attr(field,mda_id));
	}
	return err_noerror;
}


/*!
 *\en
 *	\brief Adds new line to register table.
 *
 *	\return Error code
 *	\see SetDocument( aDocument * doc )
 *	\see SetTable( const QString & tblname )
 *\_en
 *\ru
 *	\brief Добавляет новую строку к таблице регистра.
 *
 *	\return Код ошибки
 *	\see SetDocument( aDocument * doc )
 *	\see SetTable( const QString & tblname )
 *\_ru
 */
int
aARegister::New()
{
	if ( !docseted ) return err_nodocument;
	aSQLTable * t = table();
	if ( !t ) return err_notable;
	qulonglong idd = nowDoc->getUid();
	if ( !idd ) return err_notselected;
	int err = aObject::New();
	if ( err ) return err;
	t->setSysValue( "idd", idd );
	t->setSysValue( "date", nowDoc->Value("DocDate") );
	if ( tableseted )
	{
		aSQLTable * tt = nowDoc->getTable ( tname );
		t->setSysValue("iddt",tt->sysValue("id"));
		t->setSysValue("ln",tt->sysValue("ln"));
	}
	err = Update();
	if ( err ) return err;
	else return select(getUid());
}

/*!
 *\en
 *	Updates information in database.
 *	Reimplements aObject function for recalc rests.
 *\_en
 *\ru
 *	Обновляет информацию в регитре.
 *	Все новые значения, заданные с помощью SetValue, сохраняются в базе данных.
 *	Выполняет пересчет остатков.
 *\_ru
 *\return  -\en  error code.\_en \ru код ошибки.\_ru
 */
int
aARegister::Update()
{
	aIRegister::Update();
	qulonglong uid = getUid();
	resum( table(), nowDoc->Value("DocDate").toDateTime(), true );
	aLog::print(aLog::Debug, tr("Accumulation Register update"));
	return err_noerror;//aIRegister::Update();
}


/*!
 *\en
 *	Deletes records from accumulation registers.
 *	Deletes all records from accumulation register tables that contents reference to doc.
 *\_en
 *\ru
 *	Удаляет все записи в регистре, ссылающиеся на досумент.
 *\_ru
 *\param doc - \en document to delete\_en \ru
	документ для удаления\_ru
 *\return  - \en false if can't find tag <aregisters/> in meta data or have no document.\_en \ru
	false если в конфигурации отсутствует раздел регистров остатков или нет документа.\_ru
 */
bool
aARegister::deleteDocument( aDocument * doc )
{
	qulonglong idd = doc->getUid();
	if ( !idd ) return false;
	aSQLTable *t = table();
	if ( t->select(QString("idd=%1").arg(idd) ))
	{
		if(t->first())
		{
			do
			{
				resum( t, doc->Value("DocDate").toDateTime(), false );
			}while(t->next());
		}
		db->db()->exec(QString("delete from %1 where idd=%2").arg(t->tableName).arg(idd));
		aLog::print(aLog::Info, QString("Accumulation register delete document with idd=%1").arg(idd));
	}
	return true;
}




/*!
 *\en
 *	Gets saldo.
 *	Helper metod.
 *\_en
 * \see getSaldo(const QDateTime &date, const QString & dimfieldname, QVariant dimvalue, const QString &resname)
 *\ru
 *	Функция получения остатка. Вспомогательный метод. Служит для совместимости со скриптами.
 *\_ru
 *\param strdate - \en date\_en \ru дата, за которую надо получить остаток. Дата задается в виде строки в ISO формате.\_ru
 *\param dimfieldname - \en name of dimensions field\_en \ru измерение, по которому нужно получить остаток\_ru
 *\param dimvalue - \en value of dimensions field\_en \ru значение измерения, по которому надо получить остаток\_ru
 *\param resname - \en name of resounces field\_en \ru ресурс, значение которого надо получить\_ru
 *\return  - \en Saldo value or QVariant::Invalid on error.\_en \ru
	Значение остатка или QVariant::Invalid при ошибке или неправильных параметрах.\_ru
 */
QVariant
aARegister::getSaldo(const QString & strdate, const QString & dimfieldname, QVariant  dimvalue, const QString &resname)
{
	QDateTime data = QDateTime::fromString(strdate,Qt::ISODate);

	if (!data.isValid()){
		QDate dt = QDate::fromString(strdate,Qt::ISODate);
		if(dt.isValid())
		{
			QTime tm;
			data.setDate(dt);
			data.setTime(tm);
			return getSaldo(data,dimfieldname, dimvalue,resname);
		}
		else
		{
			aLog::print(aLog::Error, QString(tr("Accumulation register convert datetime from string %1")).arg(strdate));
			return QVariant::Invalid;
		}
	}
	else
	{
		return getSaldo(data,dimfieldname, dimvalue,resname);
	}
}



/*!
 * \ru
 *  Выбирает остаток на заданную дату-время.
 *  Выбирает остаток на заданную дату-время для одного (любого из заданных в регистре)
 *  аналитического разреза (измерения) с указанным значением.
 *  Проще говоря, метод позволяет получить, например, остаток по дебету или кредиту требуемого счета
 *  на требуемую дату, если регистр используется для хранения бухгалтерских проводок, в каждой из которых
 *  указывается один дебетуемый и один кредитуемый счет.
 *  \param date - дата, на которую требуется получить остаток.
 *  \param dimfieldname - название аналитического разреза регистра.
 *  \param dimvalue - значение (точка) аналитического разреза, для которого требуется определить остаток.
 *  Например для Аналитического разреза "Счет по дебету" значение может быть "41.1" или любой другой
 *  номер счета бухгалтерского плана счетов.
 *  \return возвращает значение остатка.
 * \_ru
 */

QVariant
aARegister::getSaldo(const QDateTime &date, const QString & dimfieldname, QVariant dimvalue, const QString &resname)
{
	aSQLTable *t_dim = table(dimfieldname);
	if(!t_dim)
	{
		aLog::print(aLog::Error,QString(tr("Accumulation register not found table %1")).arg(dimfieldname));
		return QVariant::Invalid;
	}
	t_dim->clearFilter();
	if(!t_dim->setFilter(dimfieldname,dimvalue))
	{
		aLog::print(aLog::Error,tr("Accumulation register set filter"));
		return QVariant::Invalid;
	}
	QString flt = QString("date<='%1' and %2").arg(date.toString(Qt::ISODate)).arg(t_dim->getFilter());

	QString query = QString("select * from %1 where %2").arg(t_dim->tableName).arg(flt);
	t_dim->clearFilter();
	t_dim->select();

	QSqlQuery q = db->db()->exec(query);
	q.last();
	if(q.isValid())
	{
		return q.value(t_dim->position(resSysNames[resname]));
	}
	else
	{
		aLog::print(aLog::Debug,"Accumulation register record empty");
	}
	return QVariant::Invalid;// t_dim->value(resname);//QVariant::Invalid;
}


/*!
 * \ru
 *  Вычисляет сумму за определенный период.
 *  Выбирает сумму для одного (минимум) аналитического разреза (измерения) с указанным значением.
 *  Если необходимо сальдо по нескольким измерениям, установите по ним фильтры \see aIRegister::SetFilter()
 *  \param from - дата начала периода.
 *  \param to - дата окончания периода.
 *  \param dimfieldname - название аналитического разреза регистра.
 *  \param dimvalue - значение (точка) аналитического разреза, для которого требуется определить остаток.
 *  Например для Аналитического разреза "Счет по дебету" значение может быть "41.1" или любой другой
 *  номер счета бухгалтерского плана счетов.
 *  \return возвращает значение остатка.
 * \_ru
 */
QVariant
aARegister::getSaldoByManyDimensions(const QString &from, const QString &to, const QString & dimfieldname, QVariant dimvalue, const QString &resname)
{
	aSQLTable *t = table();
	QString currentFilter;
		
	if(!t)
	{
		aLog::print(aLog::Error,QString(tr("Accumulation register not found main table")));
		return QVariant::Invalid;
	}
	currentFilter = t->filter(); // QSqlCursor method call
	
	if(!t->setFilter(dimfieldname,dimvalue))
	{
		aLog::print(aLog::Error,tr("Accumulation register set filter"));
		t->setFilter(  currentFilter ); // QSqlCursor method call
		t->select();
		return QVariant::Invalid;
	}
	QString flt = QString("date>='%1' and date<='%2' and %3 ").arg(from).arg(to).arg(t->getFilter());

	QString query = QString("select sum(%1) from %2 where %3").arg(resSysNames[resname]).arg(t->tableName).arg(flt);
	
	aLog::print(aLog::Info,QString("Accumulation register query %1").arg(query));
	QSqlQuery q = db->db()->exec(query);
	q.last();
	if(q.isValid())
	{
		t->setFilter( currentFilter );
		t->select();
		return q.value(0);//t->position(resSysNames[resname]));
	}
	else
	{
		aLog::print(aLog::Debug,"Accumulation register record empty");
	}
	t->setFilter( currentFilter );
	t->select();
	return 0;
	
}
/*!
 *\en
 *	Recalculates rest tables.
 *\_en
 *\ru
 *	Пересчитывает таблицы остатков.
 *	Таблицы остатков пересчитывается для всех занчений с равной или более поздней датой.
 *	В передаваемой таблице должны быть выбрана запись, обусловившая пересчет.
 *\_ru
 *\param t - \en table with selected record \_en \ru
 *	таблица с выбранной записью\_ru
 *\param dd - \en date for start recalculation.\_en \ru
 *	дата, с которой пересчитаются остатки.\_ru
 *\param plus - \en true for new record, false for delete\_en \ru
 *	true для новой записи, false для удаления записи.\_ru
 */
void
aARegister::resum( aSQLTable * t, const QDateTime & dd, bool plus )
{
	aCfgItem dims;
	dims = md->find( obj, md_dimensions );
	int dims_count = md->count(dims,md_field);
	for(uint i=0; i<dims_count; i++)
	{
		aCfgItem dim = md->findChild(dims,md_field,i);
		long dimId = atoi(md->attr(dim,mda_id).ascii());
		aSQLTable *t_dim = table(md->attr(dim,mda_name));
		QVariant v = t->value(md->attr(dim,mda_name));
		if(v.isValid() && !v.isNull() && v!=QString())
		{
			recalculate_saldo(t_dim, t, dd, plus, dimId, v );
		}
	}
}



/*!
 *\ru
 *	Выбирает запись в таблице остатков с заданной датой, если ее нет, то создает новую.
 *	После этого вызывает функцию для обновления значений остатков.
 *\_ru
 *\param t_dim - \en saldo table \_en \ru таблица остатков для обновления\_ru
 *\param t - \en table \_en \ru таблица оборотов с выбранной записью, из которой берутся значения\_ru
 *\param dd - \en date for start recalculation.\_en \ru дата, с которой пересчитаются остатки.\_ru
 *\param plus - \en true for new record, false for delete\_en \ru true для новой записи, false для удаления записи.\_ru
 *\param dimId - \en dimension id\_en \ru id поля измерения в метаданных\_ru
 *\param dimValue - \en dimension value\_en \ru значение поля измерения в метаданных\_ru
 */
void //protected
aARegister::recalculate_saldo(aSQLTable *t_dim, aSQLTable *t, const QDateTime & dd, bool plus, long dimId, QVariant dimValue)
{
	QString query = QString("select * from %1 where date='%2' and uf%3='%4'").arg(t_dim->tableName).arg(dd.toString(Qt::ISODate)).arg(dimId).arg(dimValue.toString());

	QSqlQuery q =db->db()->exec(query);
	q.first();
	if(!q.isValid())
	{
		t_dim->select();
		QString where = QString("date<'%2' and uf%3='%4'").arg(dd.toString(Qt::ISODate)).arg(dimId).arg(dimValue.toString());
		query = QString("select * from %1 where %2").arg(t_dim->tableName).arg(where);
		QString values;
		q = db->db()->exec(query);
		q.last();
		if(q.isValid())
		{
			//t_dim->select(query,false);
			//t_dim->last();
			insert_values(&q,t_dim,dd,plus,dimId,dimValue);
		}
		else
		{
			insert_values(0,t_dim,dd,plus,dimId,dimValue);
		}
	}
	update_values(t_dim,dd,plus,dimId,dimValue,t);
}

/*!
 *\ru
 *	Добавляет запись в таблицу остатков.
 *\_ru
 *\param q - \en query with selected record \_en \ru запрос с данными для добавления. Если q!=0, то данные из q  попадут в добавляемую строку\_ru
 *\param t_dim - \en saldo table \_en \ru таблица остатков\_ru
 *\param dd - \en date for start recalculation.\_en \ru дата, с которой пересчитаются остатки.\_ru
 *\param plus - \en true for new record, false for delete\_en \ru true для новой записи, false для удаления записи.\_ru
 *\param dimId - \en dimension id\_en \ru id поля измерения в метаданных\_ru
 *\param dimValue - \en dimension value\_en \ru значение поля измерения в метаданных\_ru
 */

int //protected
aARegister::insert_values(QSqlQuery *q, aSQLTable *t_dim, const QDateTime & dd, bool plus, long dimId, QVariant dimValue)
{
	int res=0;
	QString ins_col = QString("date,uf%1,").arg(dimId);
	QString ins_val = QString("'%1','%2',").arg(dd.toString(Qt::ISODate)).arg(dimValue.toString());
	QMap<QString,QString>::Iterator it;
	QVariant res_value;
	for ( it = resSysNames.begin(); it != resSysNames.end(); ++it )
	{
		if(q)
		{
			res_value = q->value(t_dim->position(it.data()));
		}
		else
		{
			res_value = 0;
		}
		ins_col+=QString("%1,").arg(it.data());
		ins_val+=QString("%2,").arg(res_value.toString());
	}
	ins_col.truncate(ins_col.length()-1);
	ins_val.truncate(ins_val.length()-1);
	QString query = QString("insert into %1 (%2) values(%3)").arg(t_dim->tableName).arg(ins_col).arg(ins_val);
	db->db()->exec(query);
	t_dim->select();
	return res;
}



/*!
 *\ru
 *	Обновляет записи в таблице остатков с датой, большей или равной заданной.
 *\_ru
 *\param t_dim - \en saldo table \_en \ru таблица остатков для обновления\_ru
 *\param t - \en table \_en \ru таблица оборотов с выбранной записью, из которой берутся значения\_ru
 *\param dd - \en date for start recalculation.\_en \ru дата, с которой пересчитаются остатки.\_ru
 *\param plus - \en true for new record, false for delete\_en \ru true для новой записи, false для удаления записи.\_ru
 *\param dimId - \en dimension id\_en \ru id поля измерения в метаданных\_ru
 *\param dimValue - \en dimension value\_en \ru значение поля измерения в метаданных\_ru
 */
int //protected
aARegister::update_values(aSQLTable *t_dim, const QDateTime & dd, bool plus, long dimId, QVariant dimValue, aSQLTable *t)
{
	QMap<long,QString>::Iterator it;
	QString upd;
	QString where = QString("date>='%1' and uf%2='%3'").arg(dd.toString(Qt::ISODate)).arg(dimId).arg(dimValue.toString());
	QVariant res_value;
	QString sign;//="-";
	//if(plus) sign="+";
	for ( it = resnames.begin(); it != resnames.end(); ++it )
	{
		if(t)
		{
			res_value = t->value(it.data());
		}
		else
		{
			res_value = 0;
		}
		if(plus) sign="+";
		else sign ="-";
		upd+=QString("uf%1=uf%2%3%4,").arg(it.key()).arg(it.key()).arg(sign).arg(res_value.toString());
	}
	upd.truncate(upd.length()-1);
	QString query = QString("update %1 set %2 where %3").arg(t_dim->tableName).arg(upd).arg(where);
	db->db()->exec(query);
	return 0;
}





QString
aARegister::trSysName( const QString & sname)
{
	if(sname == tr("RecordDate") || sname == "RecordDate" )
	{
		return "date";
	}
	else return "";
}
