/****************************************************************************
** $Id: airegister.cpp,v 1.3 2008/11/09 21:09:11 leader Exp $
**
** Code file of the Information Register of Ananas
** Engine applications
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

#include	"airegister.h"
#include	"adatabase.h"
#include	"adocjournal.h"
#include	"adocument.h"
#include	"alog.h"


/*!
 *\en
 *	\brief Create object aIRegister by ananas configuration object.
 *
 *	New object initing by md ojbect
 *	\param context - md object used for initing aDocument
 *	\param adb - ananas database for working
 *\_en
 *\ru
 *	\brief Создает новый экземпляр объекта информационного регистра по описанию в бизнес схеме.
 *
 *	\param context - Ссылка на объект бизнес схемы, описывающий регистр.
 *	\param adb - Можно не передавать. Значение по умолчанию = 0. Текущая база данных Ананаса.
 *
 *\_ru
 */
aIRegister::aIRegister( aCfgItem context, aDatabase * adb )
: aObject( context, adb )
{
	concrete = !context.isNull();
	initObject();
}



/*!
 *\en
 *	\brief Create object aIRegister by information register metadata name.
 *
 *	New object initing by md ojbect with some name.
 *	Use for creating object if you know it's name.
 *	\param name - md name of document
 *	\param adb - ananas database for working
 *	\param type - ananas database for working
 *\_en
 *\ru
 *	\brief Создает новый экземпляр объекта информационного регистра по имени.
 *
 *	Новый объект создается по описанию объекта с таким именем, хранимом в бизнес схеме.
 *	\param name - Имя Регистра в бизнес схеме
 *	\param adb - Можно не передавать. Значение по умолчанию = 0. Ссылка на текущую базу данных Ананаса
 *	\param type - Можно не передавать. Значение по умолчанию = "InfoRegister". Тип (класс) реристра в бизнес схеме
 *
 *\_ru
 */
aIRegister::aIRegister( const QString & name, aDatabase * adb,  const QString & type )
:aObject( type+name, adb )
{
	if ( name.isEmpty() ) concrete = false;
	else concrete = true;
	initObject();
}

/*!
 *	\ru
 *	Деструктор.
 *	\_ru
 */

aIRegister::~aIRegister()
{
}


/*!
 *\en
 *	\brief Initing this object by information registr from metadata object
 *
 *	\return Error code
 *\_en
 *\ru
 *	\brief Инициализирует объект информационного регистра.
 *
 *	\return Код ошибки
 *\_ru
 */
ERR_Code
aIRegister::initObject()
{
	docseted = false;
	tableseted = false;
	ERR_Code err = aObject::initObject();
	if ( err )
		return err;
	return tableInsert( db->tableDbName(*md,obj), obj );
}


/*!
 *\ru
 *	\brief Устанавливает фильтр на записи регистра.
 *
 *	После установки фильтра навигация происходит по записям значение данного поля которых равно указанному значению.
 *	\param name - Имя столбца в метаданных, на который устанавливается фильтр
 *	\param value - Значение для фильтрации
 *	\return Код ошибки
 *\_ru
 *\en
 *	\brief Sets filter.
 *
 *	\param name - Field name.
 *	\param value - Field value.
 *	\return Error code
 *\_en
 */
int
aIRegister::SetFilter( const QString & name, const QVariant & value )
{
	if ( (md->findName( md->find(obj,md_dimensions,0), md_field, name  )).isNull() )
		return err_incorrecttype;
	return aObject::SetFilter( name, value );
}



/*!
 *\ru
 *	\brief Отфильтровывает записи регистра, принадлежащие документу. Подготовливает регистр для добавления записи
 *	функцией New.
 *
 *
 *	Задав документ для регистра, получаем возможность работать только с записями регистра, указывающими на этот документом.
 *
 *	\param - Ссылка на существующий документ
 *	\return Код ошибки
 *	\see New()
 *
 *\_ru
 *\en
 *	\brief Sets document.
 *
 *	\param doc -  Reference to documetn.
 *	\return Error code
 *	\see New()
 *\_en
 */
int
aIRegister::SetDocument( aDocument * doc )
{
	nowDoc = doc;
	if ( !doc ) return err_nodocument;
	docseted = true;
	SetFilter( "idd", doc->getUid() );
	return err_noerror;
}

/*!
 *\ru
 *	\brief Возвращает документ, на основании которого была сделана текущая запись регистра.
 *
 *	Возвращяет хранящийся в текущей записи регистра указатель на документ (не идентификатор, а объект),
 *	позволяющий обратиться к реквизитам документа.
 *	\return Указатель на документ, если текущая запись регистра имеет ссылку на документ и этот докуменрт существует.
 *	Возврат ошибки будет свидетельствовать, что с данными не все в порядке, т.е. нарушена ссылочная целостность.
 *\_ru
 *\en
 *	\brief Gets document.
 *
 *	\return Document pointer
 *\_en
 */
aDocument*
aIRegister::GetDocument()
{
	//TODO: проверить, не происходит ли утечек памяти
	qulonglong doc_id = table()->sysValue("idd").toULongLong();
	aCfgItem o = md->find(db->uidType(doc_id));
	if(!o.isNull())
	{
		aDocument * d = new aDocument(o, db);
		if(d)
		{
			d->select(doc_id);
		}
		return d;
	}
	return 0;
}


/*!
 *\en
 *	\brief Sets table.
 *
 *	\param tblname - Table name.
 *	\return Error code
 *\_en
 *\ru
 *	\brief Задает табличную часть документа для использования при добавдении новых записей регистра методом New.
 *
 *	Вновь добавляемые записи регистра будут содержать ссылку на текущую запись указанной табличной части документа.
 *	Используется только когда нужно добавить строки из таблицы документа.
 *	До вызова этой функции должна быть вызвана функция установки документа SetDocument( aDocument * doc ).
 *	\param tblname - Имя таблицы документа.
 *	\return Код ошибки
 *	\see SetDocument( aDocument * doc )
 *\_ru
 */
int
aIRegister::SetTable( const QString & tblname )
{
	if ( tblname == "" )
	{
		tname = "";
		tableseted = false;
		return err_notable;
	}
	if ( !docseted ) return err_nodocument;
	tableseted = true;
	tname = tblname;
	return err_noerror;
}



/*!
 *\en
 *	\brief Adds new line to infomation register table.
 *
 *	\return Error code
 *	\see SetDocument( aDocument * doc )
 *	\see SetTable( const QString & tblname )
 *\_en
 *\ru
 *	\brief Добавляет новую строку к таблице информационного регистра.
 *
 * 	Для каждой строки информационного регистра обязательным является указание того, какая строка
 * 	табличной части какого документа с ней связана. То есть каждая запись в регистре обязательно связана
 * 	ровно с одной строкой табличной части какого-либо документа. И если при редактировании документа
 * 	пользователем удаляется строка 	в его табличной части, то происходит автоматическое удаление соответствующих
 * 	строк регистра.
 * 	Перед вызовом необходимо указать ссылку на документ и на одну из его (возможно нескольких) табличных частей.
 * 	Важно чтобы в табличной части документа была задана требуемая текущая запись (строка). Именно с ней будет связана
 * 	создаваемая запись регистра.
 *	Для указания документа и табличной части перед выховом этой функции нужно вызвать функции SetDocument() и SetTable()
 *	Иначе запись не будет создана и вернется ошибка.
 *	\return Код ошибки
 *	\see SetDocument( aDocument * doc )
 *	\see SetTable( const QString & tblname )
 *\_ru
 */
int
aIRegister::New()
{
	if ( !docseted ) return err_nodocument;
	aSQLTable * t = table();
	if ( !t ) return err_notable;
	qulonglong idd = nowDoc->getUid();
	if ( !idd ) return err_notselected;
	int err = aObject::New();
	if ( err ) return err;
	t->setSysValue( "idd", idd );
	//t->setSysValue( "date", nowDoc->Value("DocDate") );
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
 *	\brief Delete all record with document id from all information registers.
 *
 *	\param doc - document to delete.
 *	\return true - ok, false - error
 *\_en
 *\ru
 *	\brief Удаляет все записи с идентификатором данного документа из всех информационных регистров.
 *
 *	\param doc - Документ, записи о котором будут удалены.
 *	\return true - успех, false - ошибка
 *\_ru
 */
bool
aIRegister::deleteDocument( aDocument * doc )
{
	QString tdbname;
	aCfgItem iReg, iRegs = md->find(md->find(md->find(mdc_metadata),md_registers,0),md_iregisters,0);
	if ( iRegs.isNull() )
	{
		aLog::print(aLog::Error, tr("aIRegister metaobject is null"));
		return false;
	}
	qulonglong idd = doc->getUid();
	if ( !idd )
	{
		aLog::print(aLog::Error, tr("aIRegister deleted document have invalid idd"));
		return false;
	}
	uint n = md->count( iRegs, md_iregister );
	for ( uint i = 0; i < n; i++  )
	{
		iReg = md->find( iRegs, md_iregister, i );
		if ( iReg.isNull() ) continue;
		if ( md->attr( iReg, mda_no_unconduct)=="1") continue;
		tdbname = db->tableDbName( db->cfg, iReg );
		db->db()->exec(QString("DELETE FROM %1 WHERE idd=%2").arg(tdbname).arg(idd));
	}
	return true;
}



/*!
 *\en
 *	\brief Removes records with given table id
 *
 *	\param iddt - table id.
 *	\return true - ok, false - error
 *\_en
 *\ru
 *	\brief Удаляет все записи из всех информационных регистрах о таблице с данным идентификатором
 *
 *	\param iddt - id таблицы.
 *	\return true - успех, false - ошибка
 *\_ru
 */
bool
aIRegister::deleteTable( qulonglong iddt)
{
	QString tdbname;
	aCfgItem iReg, iRegs = md->find(md->find(md->find(mdc_metadata),md_registers,0),md_iregisters,0);
	if ( iRegs.isNull() )
	{
		aLog::print(aLog::Error, tr("aIRegister metaobject is null"));
		return false;
	}
	uint n = md->count( iRegs, md_iregister );
	for ( uint i = 0; i < n; i++  )
	{
		iReg = md->find( iRegs, md_iregister, i );
		if ( iReg.isNull() ) continue;
		if ( md->attr( iReg, mda_no_unconduct)=="1") continue;
		tdbname = db->tableDbName( db->cfg, iReg );
		db->db()->exec(QString("DELETE FROM %1 WHERE iddt=%2").arg(tdbname).arg(iddt));
	}
	return true;
}



/*!
 *\en
 *	\brief Selects records for date interval.
 *
 *	\return Error code
 *	\see Select( QDateTime from, QDateTime to )
 *\_en
 *\ru
 *	\brief Выбирает записи в информационном регистре за заданный период.
 *
 *	Вспомогательный метод для скриптов.
 *	\param from - дата начала периода в строке в фрмате ISO 8601 (ГГГГ-ММ-ДД)
 *	\param to - дата окончания периода в строке в формате ISO 8601 (ГГГГ-ММ-ДД)
 *	\return Код ошибки
 *	\see Select( QDateTime from, QDateTime to )
 *\_ru
 */
int
aIRegister::Select( const QString &from, const QString &to)
{
	//printf("from %s, to %s\n",from.ascii(),to.ascii() );
	aLog::print(aLog::Debug, tr("aIRegister select period from %1 to %2").arg(from).arg(to));
	return Select(QDateTime::fromString(from+"T00:00:00",Qt::ISODate), QDateTime::fromString(to+"t00:00:00",Qt::ISODate));
}


/*!
 *\en
 *	\brief Selects records for period
 *
 *	\param from - Period begin date
 *	\param to - Period end date
 *	\return Error code
 *\_en
 *\ru
 *	\brief Выбирает записи в информационном регистре за заданный период.
 *
 *	\param from - Дата начала периода
 *	\param to - Дата окончания периода
 *	\return Код ошибки
 *\_ru
 */
int
aIRegister::Select( QDateTime from, QDateTime to )
{
	aDocJournal journ(db);
	aSQLTable * t = table();
	if ( !t ) return err_notable;
	QString query, flt;
	flt = journ.selectionFilter( from, to, "", true );
	if ( flt == "" ) return err_selecterror;
	query = QString("SELECT %1.* FROM a_journ, %2 WHERE %3.idd=a_journ.idd AND %4").arg(t->tableName).arg(t->tableName).arg(t->tableName).arg(flt);
	if ( IsFiltred() )
	{
		QString tf = t->getNFilter();
		if ( tf != "" )
			query = query + QString(" AND %1").arg(tf);
	}
	if ( t->exec(query) )
		if ( t->first() )
		{
			setSelected(true);
			return err_noerror;
		}
		else return err_notselected;
	else
		return err_selecterror;
}


/*!
 *\en
 *
 *	\return Error code
 *\_en
 *\ru
 *	\brief Выбирает первую запись регистра по документу, установленному перед этим функцией SetDocument( aDocument * doc ).
 *
 *	\return Код ошибки
 *	\see SetDocument( aDocument * doc )
 *\_ru
 */
int
aIRegister::SelectByDoc()
{
	if ( !docseted ) return err_nodocument;
	qulonglong uidd = nowDoc->getUid();
	if ( !uidd ) return err_nodocument;
	aSQLTable * t = table();
	if ( !t ) return err_notable;
	if ( t->select(QString("idd=%1").arg(uidd)) )
		if (t->first()) return err_noerror;
		else return err_notselected;
	else return err_selecterror;
}

/*!
 *\en
 *	\brief Return field value with name = name of the primary object database table.
 *
 *	\param name - field name
 *	\return  field value
 *\_en
 *\ru
 *	\brief Возвращает значения поля с именем =  name основной таблицы объекта.
 *
 *	\param name - имя поля
 *	\return  значение поля. Поля типа Q_ULLONG и DateTime переводятся в строки.
 *\_ru
 */
QVariant
aIRegister::Value( const QString & name, const QString &tableName )
{
	QVariant res = aObject::Value(name, tableName);
	if(res.type() == QVariant::ULongLong || res.type() == QVariant::LongLong)
	{
		return QVariant(res.toString());
	}
	//if(res.type() == QVariant::DateTime)
	//{
	//	return QVariant(res.toDate().toString(Qt::ISODate));
	//}
	return res;
}

/**
 * \ru
 * 	\brief Переход к следующей записи регистра.
 * \_ru
 */
bool
aIRegister::Next()
{
	return aObject::Next();
}


/**
 * \ru
 * 	\brief Переход к предыдущей записи регистра.
 * \_ru
 */
bool
aIRegister::Prev()
{
	return aObject::Prev();
}


/**
 * \ru
 * 	\brief Переход к первой записи регистра.
 * \_ru
 */
bool
aIRegister::First()
{
	return aObject::First();
}


/**
 * \ru
 * 	\brief Переход к последней записи регистра.
 * \_ru
 */
bool
aIRegister::Last()
{
	return aObject::Last();
}
