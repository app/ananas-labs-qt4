/****************************************************************************
** $Id: aextsql.cpp,v 1.1 2009/05/25 16:35:10 app Exp $
**
** Code file of the Report Result Object of Ananas
** Designer and Engine applications
**
** Created : 20070819
**
** Copyright (C) 2005-2007 Grigory Panov <grigory.panov at gmail.com>, Moscow.
** Copyright (C) 2005-2007 Ananas Project.
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

#include "aextsql.h"
#include "adatabase.h"

/**
 * \en
 * 		\brief Constructor
 * \_en
 * \ru
 *		\brief Конструктор класса
 * \_ru
 */
aExtSQL::aExtSQL() : AExtension("SQL")
{
    cursor = 0;
}

/**
 * \en
 *		\brief Init function, owerrides function in base class, creates inner database specific object and variables.
 * \_en
 * \ru
 *		\brief Функция иницализации, переопределяет функцию базового класса, создает внутренние объекты и переменные.
 * \_ru
 */
int
aExtSQL::init( aDatabase *database )
{
//	cursor = new Q3SqlSelectCursor(const QString::QString(""), database->db());
	const QString emptyString = "";
	cursor = new Q3SqlSelectCursor(emptyString, *(database->db()) );
	return AExtension::init(database);
}

/**
 * \en
 * 		\brief Destructor
 * \_en
 * \ru
 *		\brief Деструктор класса
 * \_ru
 */
aExtSQL::~aExtSQL()
{
	if(cursor) delete cursor;
	cursor = 0;
}

/**
 * \en
 * 		\brief Return inner cursor which contains result of last call method ExecQuery or ExecScalar
 * 
 * 		for bindings to external table (QDataTable etc.). Don't delete this cursor after use!
 * 		\return Inner cursor.
 * \_en
 * \ru
 *		\brief Функция фозвращает внутренний курсор, содержащий результаты выполнения последнего вызова функции ExecQuery или ExecScalar
 *  
 * 		для использования в качестве источника данных для таблиц (QDataTable и т.д). Не удаляйте этот курсор после использования!
 * 		\return Внутренний курсор.
 * \_ru
 * \see aExtSQL::ExecQuery()
 * \see aExtSQL::ExecScalar()
 */
Q3SqlSelectCursor *
aExtSQL::Cursor() const
{
	return cursor;
}

/**
 * \en
 * 	Gets value of column number \acol (zero based) from current record or QVariant::Invalid if error occured or \acol value out of range.
 * \_en
 * \ru
 *	Функция получения значения столбца \acol текущей записи или QVariant::Invalid если случилачь ошибка или значение \acol меньше 0 или больше, чем количество столбцов в результате.
 * \_ru
 * \param col (in) -	\en Column Index
 *			\ru Индекс столбца
 * \_ru
 * \return \en Column value \ru Значение столбца\_ru
 * \see aExtSQL::Count()
 */
QVariant
aExtSQL::Value(int col) const
{
	QVariant res = QVariant::Invalid;
	if(cursor->count() > col && col >= 0) res = cursor->value(col);
	if(res.type() == QVariant::ULongLong || res.type() == QVariant::LongLong)
	{
		res = res.toString();
	}
	return res;
}

/**
 * \en
 *		\brief Return record count in result.
 * \_en
 * \ru
 *		\brief Функция получения количества записей в результате.
 * \_ru
 */
int
aExtSQL::Size() const
{
	return cursor->size();
}

/**
 * \en
 *		\brief Return column count in result.
 * \_en
 * \ru
 *		\brief Функция получения количества столбцов в результате.
 * \_ru
 */
int 
aExtSQL::Count() const
{
	return cursor->count();
}

/**
 * \en
 *		\brief Function for query execution. 
 * 
 * 		Only select statement supported. If any error occured, returns false. Fom move to result records use First(), Next(), Last() and Prev() functions. For get information about last error occured use LastError() function.
 * 		\param query (in) - sql query for execution. 
 * 		\return true, if no error occured. 
 * \_en
 * \ru
 *		\brief Функция для выполнения sql запроса. 
 * 
 * 		Поддерживается только выражение select. Если случилась ошибка, возвращается false. Для перемещения по результатам используйте функции First(), Next(), Prev(), Last(). Для получения сведений об ошибке - функцией LastError(). 
 * 		\param query (in) - Sql запрос для выполнения.
 *		\return true, если запрос выполнился без ошибок. 
 * \_ru
 * \code var sql = new SQL();
       if( !sql.ExecQuery("select * from a_journ") ) 
       {
	   sys.Message(1,"Ошибка в вашем sql запросе:" + sql.LastError());
	   return;
       }
       if(sql.First())
       {
	    do{
		 sys.Message(0, sql.Value(0));
	    } while(sql.Next());
        }
 * \endcode
 * \see aExtSQL::ExecScalar()
 * \see aExtSQL::Value()
 * \see aExtSQL::LastError()
 */
bool
aExtSQL::ExecQuery( const QString & query)
{
	return cursor->exec(query);
}

/**
 * \en
 *		\brief This function similar to described above ExecQuery(), 
 * 
 * 		but return only first cell in first record or QVariant::Invalid, if no recornds returned or error occured. 
 * 		But if you know, that more than one record returned, you can navigate throw thouse records use function First(), Next() ...
 * 		\param query (in) - sql query for execution. 
 * 		\return Query result or QVariant::Invalid 
 * \_en
 * \ru
 *		\brief Эта функция похожа на описанную выше ExecQuery(), 
 * 
 * 		но возвращает не true или false, а значение первой ячейки первой строки результата или QVariant::Invalid, 
 * 		если не вернулось ни одной записи или случилась ошибка. Но если вы знаете, что в результате больше чем одна запись, 
 * 		вы можете перемещаться по ним используя функции First(), Next() и т.д.
 * 		\param query (in) - Sql запрос для выполнения. 
 * 		\return Результат запроса или QVariant::Invalid
 * \_ru
 * \code
	var sql = new SQL();
	var res = sql.ExecScalar("select count(*) from a_journ") ) 
	sys.Message(0, res + "records in system journal");
 * \endcode
 * \see aExtSQL::ExecQuery()
 */
QVariant 
aExtSQL::ExecScalar( const QString & query)
{
	cursor->exec(query);
	if(cursor->size() > 0)
		return cursor->value(0);
	else
		return QVariant::Invalid;
}

/**
 * \en
 *		\brief Make first record of result as current. 
 * 
 * 		You should first call ExecQuery for call this function have sence.
 * 		\return true if success  
 * \_en
 * \ru
 *		\brief Делает первую запись результата активной. 
 * 
 * 		Вы должны сначала вызвать ExecQuery, чтобы вызов этой функции имел смысл.
 * 		\return true в случае успеха
 * \_ru
 * \see aExtSQL::ExecuteQuery()
 */
bool
aExtSQL::First()
{
	return cursor->first();
}

/**
 * \en
 * 		\brief Make next record of result as current.
 * 
 * 		\return true if success
 * \_en
 * \ru
 *		\brief Делает следующую запись результата активной.
 * 
 * 		\return true в случае успеха 
 * \_ru
 * \see aExtSQL::ExecuteQuery()
 */
bool
aExtSQL::Next()
{
	return cursor->next();
}

/**
 * \en
 * 		\brief Make last record of result as current.
 * 
 * 		\return true if success  
 * \_en
 * \ru
 *		\brief Делает последнюю запись результата активной.
 * 
 * 		\return true в случае успеха
 * \_ru
 * \see aExtSQL::ExecuteQuery()
 */
bool
aExtSQL::Last()
{
	return cursor->last();
}


/**
 * \en
 * 		\brief Make previous record of result as current.
 * 
 *		\return true if success
 * \_en
 * \ru
 *		\brief Делает предыдущую запись результата активной.
 * 
 * 		\return true в случае успеха 
 * \_ru
 * \see aExtSQL::ExecuteQuery()
 */
bool
aExtSQL::Prev()
{
	return cursor->prev();
}


/**
 * \en
 * 		\brief Gets sql name of field by its user name. 
 * 
 * 		To every Ananas object user can add his own fields. Unfortunatly when you generate SQL query you should use 
 * 		sql field name instead of user field name. This function help do it easy. All what you need - create new object 
 * 		you want use in query, specify user field name and table (primary or one of secondary). See SqlTableName() 
 * 		for details about table naming. However, every sql table contains some system fields. That fields also can be used in query. 
 * 		See documentation on ananas object for details.
 * 		\param obj (in) -  Ananas object. Using as contex for field search.
 * 		\param userFieldName (in) - User field name
 * 		\param tableType (optional) - 	Table type. For catalogue group table mapped to "group" alias, 
 * 		For accumulation register every saldo table mapped to dimension user name alias.
 * 		\return sql field name 
 * \_en
 * \ru
 *		\brief Возвращает sql имя поля по его пользовательскому имени.
 * 
 *		К каждому объекту Ананаса пользователь может добавлять свои собственные пользовательские поля. 
 * 		К несчастью, когда вы генерируете SQL запрос, вы должны использовать sql-ные имена полей вместо пользовательских. 
 * 		Данная функция помогает делать это очень просто. Все что вам нужно - создать новый объект, 
 * 		который будет использоваться для получения имени, определить пользовательское имя поля и таблицу (основную или одну из запасных :). 
 * 		См SqlTableName() для детального описания имен и структуры таблиц. Кроме того, каждая таблица содержит системные поля 
 * 		для связи между объектами. Эти поля также могут быть использованы в запросе. См. документацию по каждому объекту Ананаса 
 * 		для детального описания.
 * 		\param obj (in) - Объект Ананасаю Используется в качестве контекста поиска.
 * 		\param userFieldName (in) - Пользовательское имя поля
 * 		\param tableType (optional) - Тип таблицы. Для каталога таблица групп имеет синоним "group", 
 * 		для накопительного регистра каждая из таблиц остатков по измерению имеет сининим, совпадающий 
 * 		с пользовательским именем этого измерения.
 * 		\return sql имя поля
 * \_ru
 * \code 
 *	var sql = new SQL();
	var cat = new Catalogue("Goods");
	sys.Message(0, sql.SqlFieldName(cat, "Full name"); // uf203
	sys.Message(0, sql.SqlFieldName(cat, "Group name", "group") //uf304 
	var doc = new Document("Order");
	sys.Message(0, sql.SqlFieldName(doc, "Organization"); //uf449
	var reg = new ARegister("Grossbuch");
	sys.Message(0, sql.SqlFieldName(reg, "Good"); // uf333
 * \endcode
 * \see aExtSQL::SqlTableName()
 */
QString 
aExtSQL::SqlFieldName(aObject *obj, const QString & userFieldName, const QString &tableType) const
{
if(!obj) return "Unknown object";
	aDataTable * dt = obj->table(tableType);
	if(dt)
	{
		return dt->sqlFieldName(userFieldName);
	}
	return "Unknown table type";
}


/**
 *	\en
 *		\brief Gets SQL name of primary or one of secondary object tables.
 * 
 * 		All Ananas objects except Report store theyr values in sql tables. For Catalogue this is group table and element table (pri), for Document - data header table, for Journal - always system journal table, for IRegister - one table for all data, for ARegister - main table for store operations (pri) and X table for store saldo, where X - dimensions count. So if you want get name of primary table, leave parameter \atableType or set in to empty string. If you want get name of secondary table, set parameter \atableType to "group" for catalogue group table or dimension user name for accumulation register saldo table for that dimension.
 *		\param obj (in) -  Object for getting table name. 
 *		\param tableType (optional) - Table type 
 *		\return sql table name 
 * \_en
 *	\ru
 *		\brief Возвращает SQL имя основной или вспомогательной таблицы объекта.
 * 
 *		Все объекты Ананаса, за исключением Отчетов, хранят данные в sql таблицах. Для Каталога это таблица групп и таблица элементов (осн.), для документа - таблица шапки, для журнала - всегда системная таблица системного журнала, для ИРегистра - одна таблица для хранения всех данных, для АРегистра - одна основная таблица для хранения записей об операциях (осн.)  и X вспомогательных для хранения остатков по разрезам, где X - количество измерений накопительного регистра. Так что если вы хотите получить имя основной таблицы объекта, не используйте параметр \atableType или установите его значение в "". Если вы хотите получить имя вспомогательной таблицы, установите параметру \atableType значение "group" для группы каталога или значение пользовательского имени измерения накопительного регистра для таблицы остатков по данному измерению.
 *		\param obj (in) - Объект для получения имени таблицы 
 *		\param tableType (optional) - Тип таблицы 
 *		\return sql имя таблицы 
 *	\_ru
 *	\code 
		var sql = new SQL();
		var cat = new Catalogue("Goods");
		sys.Message(0, sql.SqlTableName(cat)); //ce101
		sys.Message(0, sql.SqlTableName(cat, "")); //ce101
		sys.Message(0, sql.SqlTableName(cat, "group")); //cg105
		var doc = new Document("Order");
		sys.Message(0, sql.SqlTableName(doc)); //dh130
		var journ = new Journal("main journal");
		sys.Message(0, sql.SqlTableName(journ)); // a_journ
		var ireg = new IRegister("Info register"));
		sys.Message(0, sql.SqlTableName(ireg)); // ra303
		var areg = new ARegister("Acc Register");
		sys.Message(0, sql.SqlTableName(areg)); // ra330
		sys.Message(0, sql.SqlTableName(areg),"Goods") //ra330_d445
 *	\endcode 
 *	\see aExtSQL::SqlFieldName()
 */
QString 
aExtSQL::SqlTableName(aObject *obj, const QString &tableType) const
{
if(!obj) return "Unknown object";
	aDataTable * dt = obj->table(tableType);
	if(dt)
	{
		return dt->tableName;
	}
	return "Unknown table type";
}

/**
 * \en
 * 		\brief Not implemented yet.
 * \_en
 * \ru
 *		\brief Еще не реализовано.
 * \_ru
 */
QString
aExtSQL::SqlTableName(const QString & objName) const
{
	aCfg * md = &db->cfg;
	aCfgItem i = md->find(objName);
	if(!i.isNull())
	{
		QString oname = md->objClass(i);
		//if(oname == md_document) i = md->findChild(i, md_header);
		return aDatabase::tableDbName(*md, i);
	}
	return "not found!";
}

/**
 * \en
 * 		\brief Not implemented yet. 
 * \_en
 * \ru
 *		\brief Еще не реализовано.
 * \_ru
 */
QString
aExtSQL::SqlFieldName(const QString & fieldName) const
{
	aCfg * md = &db->cfg;
	aCfgItem i = md->find(fieldName);
	if(!i.isNull())
	{
		if(md->objClass(i) == md_field || md->objClass(i) == md_column)
		return QString("uf%1").arg(md->id(i));
	}
	return "not found";
}

/**
 * \en
 * 		\brief Returns text representation last error or emty text, if no error occured. 
 * 
 * 		Returns driver text only.
 * \_en
 * \ru
 *		\brief Возвращает текстовое представление последней ошибки или пустую строку, если ошибок не было. 
 * 
 * 		Возвращается текст драйвера базы данных.
 * \_ru
 */
QString
aExtSQL::LastError() const
{
	return cursor->lastError().databaseText();
}

#include <aextensionplugin.h>
typedef AExtensionPlugin<aExtSQL> aExtSQLPlugin;
A_EXPORT_PLUGIN( aExtSQLPlugin )
