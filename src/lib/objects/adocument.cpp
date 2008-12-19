/****************************************************************************
** $Id: adocument.cpp,v 1.3 2008/11/09 21:09:11 leader Exp $
**
** Document metadata object implementation file of
** Ananas application library
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
**
** This file is part of the Designer application of the Ananas
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


#include	"acfg.h"
#include 	"alog.h"
#include	"aobject.h"
#include	"adatabase.h"
#include	"adocument.h"
#include	"adocjournal.h"
#include	"airegister.h"
#include	"aaregister.h"
//Added by qt3to4:
#include <QSqlQuery>



/*!
 *\en
 *	Create object aDocument by ananas configuration object.
 *	New object initing by md ojbect
 *	Use for creating object if you have md object, for example in default document form
 *\_en
 *\ru
 *	\brief Создает объект и инициализирует его по метаданным бизнес схемы.
 *\_ru
 *	\param context - \en md object used for initing aDocument. \_en \ru
 *						элемент метаданных бизнес схемы, инициализирующий объект.\_ru
 *	\param adb - \en ananas database for working. \_en \ru
 *					ссылка на базу данных.\_ru
*/
aDocument::aDocument( aCfgItem context, aDatabase * adb )
:aObject( context, adb, 0, "aDocument")
{
	if ( context.isNull() ) concrete = false;
	else concrete = true;
	initObject();
	sysJournal = new aDocJournal(adb);
}



/*!
 *\en
 *	Create object aDocument by document metadata name.
 *	new object initing by md ojbect with some name
 *	Use for creating object if you know it's name
 *\_en
 *\ru
 *	Создает объект и инициализирует его по метаданным бизнес схемы.
 *	Документ инициализируется элементом метаданных, найденым по его имени,
 *	причем поиск осуществляется только среди документов.
 *\_ru
 *	\param name - \en md name of document.\_en \ru
 					имя документа в метаданных бизнес схемы.\_ru
 *	\param adb - \en ananas database for working. \_en \ru
 *					ссылка на базу данных\_ru
 */
aDocument::aDocument(const QString & name, aDatabase * adb)
:aObject( "Document."+name, adb, 0, "aDocument")
{
	if ( name.isEmpty() ) concrete = false;
	else concrete = true;
	initObject();
	sysJournal = new aDocJournal(adb);
}



/*!
 *\en
 *	Destroy object aDocument.
 *	Use for delete system journal and free memory.
 *\_en
 *\ru
 *	Уничтожает объект.
 *	Используется для удаления системного журнала и освобождения занятой под него памяти.
 *\_ru
 */
aDocument::~aDocument()
{
	delete sysJournal;
	sysJournal=0;
}



/*!
 *	\en
 *	Translate name of system field.
 *	Used for translate same system names from different languages ot english,
 *	result used from working with system fields
 *	\_en
 *	\ru
 *	Используется для перевода предопределенных имен для системных полей.
 *	Позволяет использовать для обращения к системным полям из скриптов назавания,
 *	близкие по смыслу к их назначению с учетом языка пользователя.
 *	Например к полю номер документа можно обратиться используя имя "НомерДок" или "DocNumber".
 *	\_ru
 *	\param sname - \en field name on same language. \_en \ru
 *					одно из предопределенных имен поля.\_ru
 *	\return \en field name on english \_en \ru имя поля\_ru
 */
QString
aDocument::trSysName( const QString & sname )
{
	if (( sname == "DocDate" )||( sname == QString::fromUtf8("ДатаДок") )) return "DocDate";
	if (( sname == "DocNumber" )||( sname == QString::fromUtf8("НомерДок") )) return "DocNumber";
	if (( sname == "SerialNumber" )||( sname == "ПНомДок" )) return "SerialNumber";
	if (( sname == "TableLine" )||( sname == QString::fromUtf8("ЛинияТабл") )) return "TableLine";
	return "";
}



/*!
 *	\en
 *	Init object aDocument.
 *	Used for insert table for document header and tables for all table part of document.
 *	After that we can work wthith document' tables by it name, name of heared table = ""
 *	\_en
 *	\ru
 *	Инициализирует объект элементом метаданных бизнес схемы.
 *	В процесе инициализации выполняется настройка на шапку документа
 *	и все его табличные части. После инициализации появляется возможность
 *	работать с полями и таблицами, используя их имена, определенные в бизнес схеме.
 *	\_ru
 *	\return \en error code. \_en \ru код ошибки.\_ru
 */
ERR_Code
aDocument::initObject()
{
	aCfgItem o, og;
	int i, nt;
	QString tdbname, tname;

	pref="";
	ERR_Code error = aObject::initObject();
	if ( error )
		return error;
	og = md->find( obj, md_header );
	error =  tableInsert( aDatabase::tableDbName( *md, og ), og );
	if ( error )
		return error;
        og = md->find( obj, md_tables );
        nt = md->count( og, md_table );
        for ( i=0; i< nt; i++ ) {
                o = md->find( og, md_table, i );
                tdbname = aDatabase::tableDbName( *md, o );
                tname = md->attr( o, mda_name );
                error = tableInsert( tdbname, o, tname );
				if ( error )
					return error;
        }
	return err_noerror;
}



/*!
 *\en
 *	Get value for some system field of document.
 *	Used for read some system field of document: DocDate - Date and time of document,
 *	DocNumber - Number of document.
 *\_en
 *\ru
 *	Читает значение системного поля.
 *	Используется для получения значения системного поля по его имени
 *\_ru
 *	\param sname - \en name of system field. \_en \ru
 *					имя ситемного поля. \_ru
 *	\return \en field value or QVariant::Invalid if used unknown field name.\_en \ru
 *				значение поля или ошибочное значение, если имя поля неизвестно.\_ru
 */
QVariant
aDocument::sysValue( const QString & sname, const QString & )
{
	if ( sname == "DocDate" )
		return sysJournal->getDate();
	if ( sname == "DocNumber" )
		return sysJournal->getNumber();
	if ( sname == "SerialNumber" )
		return sysJournal->getSerialNumber();
	return QVariant::Invalid;
}



/*!
 *	\en
 *	Set value for some system field of document.
 *	Used for write some system field of document: DocDate - Date and time of document,
 *	DocNumber - Number of document.
 *	\_en
 *	\ru
 *	Устанавливает значение системного поля.
 *	Используется для установки значения системного поля по его имени
 *	\_ru
 *	\param sname - \en name of system field\_en \ru
 					имя ситемного поля.\_ru
 *	\param value - \en new value of field\_en \ru
 					устанавливаемое значение\_ru
 *	\return \en error code. \_en \ru
				код ошибки.\_ru
 */
ERR_Code
aDocument::setSysValue( const QString & sname, QVariant value, const QString &)
{
	if ( sname == "DocDate" )
		return sysJournal->setDate( value );
	if ( sname == "DocNumber" )
		return sysJournal->setNumber( value );
	return err_fieldnotfound;
}



/*!
 *	\en
 *	Get some field value of current table line.
 *	Get value for field by metadata name or sustem value ln if use name "TableLine".
 *	\_en
 *	\ru
 *	\brief ScriptAPI. Читает значение поля табличной части документа.
 *
 *	Используется для получения значения поля таблицы по именам таблицы и поля.
 *	При вызове функции используются имена, указанные в бизнес схеме.
 *	Кроме этого может использоваться продопределенное имя "TableLine" для
 *	получения номера текущей записи
 *	\_ru
 *	\param tablename - \en metadata name of table\_en \ru
 						имя таблицы в бизнес схеме\_ru
 *	\param name - \en name of field\_en \ru
 					имя поля в бизнес схеме\_ru
 *	\return \en field value\_en \ru
 				значение поля\_ru
 */
QVariant
aDocument::TableValue( const QString & tablename, const QString & name )
{
	if ( trSysName(name) == "TableLine" ) return table(tablename)->sysValue("ln");
	else return tValue( tablename, name );
}



/*!
 *	\en
 *	Set some field value of current table line.
 *	Set value for field by metadata name
 *	\_en
 *	\ru
 *	\brief ScriptAPI. Записывает значение в поле текущей строки табличной части документа.
 *	\_ru
 *	\param tablename - \en metadata name of table,\_en \ru
 				имя таблицы в бизнес схеме,\_ru
 *	\param name - \en metadata name of field,\_en \ru
 				имя поля в бизнес схеме,\_ru
 *	\param value - \en new field value.\_en \ru
 				новое значение поля.\_ru
 *	\return \en error code. \_en \ru
				код ошибки.\_ru
 */
ERR_Code
aDocument::TableSetValue( const QString & tablename, const QString & name, const QVariant &value )
{
        return setTValue( tablename, name, value );
}



/*!
 * \en
 *	Create new document.
 *	Create new document header record by selected type in database,
 *	if object not seted, return error.
 *	New document also registred in system journal and make current
 * \_en
 * \ru
 *	\brief ScriptAPI. Создает и регистрирует в системном журнале объект aDocument.
 *
 *	Создает новую запись в  БД с данными из шапки документа. При этом резервируется уникальный Id записи таблицы.
 *	При этом документ регистрируется в системном журнале и получает следующий по порядку номер.
 * \_ru
 *	\return \en error code. \_en \ru
				код ошибки.\_ru
 */
ERR_Code
aDocument::New()
{
	if ( !sysJournal )
	{
		aLog::print(aLog::Error, tr("aDocument have no sysjournal"));
		return err_nosysjournal;
	}
	ERR_Code err = aObject::New();
	if ( err )
	{
		aLog::print(aLog::Error, tr("aDocument aObject::New() ended with error code = %1").arg(err));
		return err;
	}
	if ( obj.isNull() )
	{
		aLog::print(aLog::Error, tr("aDocument metaobject is null=%1"));
		return err_noobject;
	}
	qulonglong Uid = getUid();
	SetPrefix(md->attr(obj,mda_name));
	aLog::print(aLog::Debug, tr("aDocument new type = %1").arg(md->id(obj)));
	err = sysJournal->New( Uid, Prefix(), md->id(obj) );
	if ( err )
	{
		aLog::print(aLog::Error, tr("aDocument New() error while added record in sysJournal =%1").arg(err));
		table()->exec(QString("DELETE FROM %1 WHERE id=%2").arg(table()->tableName).arg(Uid));
		db->markDeleted( Uid );
		return err;
	}

	aLog::print(aLog::Debug, tr("aDocument new document"));
	return err_noerror;
}



/*!
 *	\en
 *	Create new table line.
 *	Create new table line for table of now document
 *	if document not selected, or no table with some name return error.
 *	New line begin current in table
 *	\_en
 *	\ru
 *	\brief ScriptAPI. Добавляет новую строку табличной части документа.
 *
 *	Строка добавляется в таблицу текущего документа, если документ не выбран
 *	или не имеет таблицы с таким именем - возникает ошибка.
 *	При создании строки ей автоматически присваивается очередной номер.
 *	\_ru
 *	\param tablename - \en metadata name of table.\_en \ru
 				имя таблицы в бизнес схеме.\_ru
 *	\return \en error code. \_en \ru
				код ошибки.\_ru
 */
ERR_Code
aDocument::TableNewLine( const QString & tablename )
{
	if ( IsConducted() ) return err_docconducted;
	aSQLTable *t = table( tablename );
	if ( !t ) return err_notable;
	qulonglong tUid = t->primeInsert()->value("id").toULongLong(), dUid = getUid();
	if ( !dUid ) return err_abstractobj;
	if ( t->insert() )
	{
		t->select( QString("id=%1").arg(tUid), false );
		if (!t->first())
			return err_selecterror;
		t->setSysValue("idd",dUid);
		QString  query;
		QVariant ln;
		query = QString("SELECT MAX(ln)+1 FROM %1 WHERE idd=%2").arg( t->tableName ).arg(dUid);
		QSqlQuery q = db->db()->exec( query );
		if ( q.first() )
		{
			ln = q.value( 0 );
			if ( !ln.toInt() ) ln = 1;
		}
		else
		{
			aLog::print(aLog::Error, tr("aDocument select new table line number"));
			return err_execerror;
		}
		t->setSysValue("ln",ln);
		t->selected = true;
		aLog::print(aLog::Info, tr("aDocument new table line number=%1").arg(ln.toString()));
		return TableUpdate( tablename );
	}
	aLog::print(aLog::Error, tr("aDocument insert new table line"));
	return err_inserterror;
}



/*!
 *	\en
 *	Delete current document.
 *	Delete document header, all tables and system journal record,
 *	If document conducted (turn on) it unconduct (make turn off).
 *	\_en
 *	\ru
 *	\brief ScriptAPI. Удаляет текущий документ.
 *
 *	Документ удаляется полностью вместе с табличными частями, также документ удаляется из системного журнала.
 *	Идентификатор документа не освободжается и повторно не используется.
 *	\_ru
 *	\return \en error code. \_en \ru
				код ошибки.\_ru
 */
ERR_Code
aDocument::Delete()
{
	if ( !selected() ) return err_notselected;
	if(IsConducted()) UnConduct();
	qulonglong uid = getUid();
	sysJournal->deleteDocument(uid);

	aLog::print(aLog::Debug, tr("aDocument delete from sysjournal"));
	db->markDeleted( uid );
	aLog::print(aLog::Debug, tr("aDocument delete from unicues"));
	aCfgItem tobj;
	uint n = md->count( obj, md_table );
	for ( uint i = 0; i < n; i++ )
	{
		tobj = md->find(obj,md_table,i);
		if ( tobj.isNull() ) continue;
		tableDeleteLines( md->attr(tobj,mda_name) );
		aLog::print(aLog::Debug, tr("aDocument delete table %1").arg(md->attr(tobj,mda_name)));
	}
	return aObject::Delete();
}



/*!
 *	\en
 *	Delete current table line.
 *	When deleted table line also deleted all records in all reegisters which contain link on this line
 *	Don't work if document conducted.
 *	\_en
 *	\ru
 *	\brief ScriptAPI. Удаляет текущую строку табличной части документа.
 *
 *	Также удаляются все записи, связанные с этой строкой во всех регистрах.
 *	Не работает для проведенных документов.
 *	\_ru
 *	\param tablename - \en metadata name of table\_en \ru
 				имя таблицы в бизнес схеме\_ru
 *	\return \en error code. \_en \ru
				код ошибки.\_ru
 */
ERR_Code
aDocument::TableDelete( const QString & tablename )
{
	if ( IsConducted() )
	{
		aLog::print(aLog::Error, tr("aDocument delete table of conducted document"));
		return err_docconducted;
	}
	aSQLTable * t = table(tablename);
	if ( !t )
	{
		aLog::print(aLog::Error, tr("aDocument deleted table not exist"));
		return err_notable;
	}
	if ( !t->selected )
	{
		//а эта проверка нужна???
		aLog::print(aLog::Error, tr("aDocument deleted table not selected"));
		return err_notselected;
	}
	qulonglong tUid = t->sysValue("id").toULongLong();
	aIRegister * reg = new aIRegister("",db);
	reg->deleteTable(tUid);
	delete reg;
	aLog::print(aLog::Debug, tr("aDocument delete table from info register"));
	//reg = new aARegister("",db);
	//reg->deleteTable(tUid);
	//delete reg;
	db->markDeleted( tUid );
	t->primeDelete();
	t->del();
	t->selected = false;
	aLog::print(aLog::Info, tr("aDocument delete table"));
	return err_noerror;
}




/*!
 *	\en
 *	Delete all table lines.
 *	Delete all table lines for current document.
 *	Delete all table lines, don't take document conductions and make any chanded in registers.
 *	\_en
 *	\ru
 *	Удаляет все строки таблицы.
 *	Удаляет все строки таблицы для текущего документа.
 *	Функция является внутренней для документа, не проверяет проведен ли документ
 *	и не вызывает никах изменения в регистрах.
 *	\_ru
 *	\param tablename - \en metadata name of table\_en \ru
 				имя таблицы в бизнес схеме\_ru
 *	\return \en error code. \_en \ru
				код ошибки.\_ru
 */
ERR_Code
aDocument::tableDeleteLines( const QString & tablename )
{
	qulonglong dUid = getUid();
	if ( !dUid )
	{
		aLog::print(aLog::Error, tr("aDocument not selected"));
		return err_notselected;
	}
	QString tdbname = table( tablename )->tableName;
	if ( tdbname == "" )
	{
		aLog::print(aLog::Error, tr("aDocument table name is empty"));
		return err_notable;
	}
	db->db()->exec(QString("DELETE FROM %1 WHERE idd=%2").arg(tdbname).arg(dUid));
	return err_noerror;
}



/*!
 *	\en
 *	Updete current record in database.
 *	Also update record for document system journal and all table parts.
 *	Don't work for conducted (Turn on) documents
 *	\_en
 *	\ru
 *	\brief ScriptAPI. Обновляет информацию в базе данных.
 *
 *	Заносит запись из буфера ОЗУ в базу данных. Обновляется информация в шапке документа,
 *	табличных частях, журнале документов.
 *	Не работает для проведенных документов.
 *	\_ru
 *	\return \en error code. \_en \ru
				код ошибки.\_ru
 */
ERR_Code
aDocument::Update()
{
	if ( IsConducted() ) return err_docconducted;
	ERR_Code e = aObject::Update();
	if ( e )
	{
		aLog::print(aLog::Error, tr("aDocument update error=%1").arg(e));
		return e;
	}
	e = sysJournal->Update();
	//TODO: may be it fix bug with saving values in first line;
	//e += TableUpdate();
	return e;
}



/*!
 *\en
 *	Update table.
 *	Use for insert information from bufer into database.
 *	Information is inserting in table with md name=teblename,
 *	if used TableUpdate() - updating all tables of document.
 *	If document already conducted return error.
 *\_en
 *\ru
 *	\brief SrciptAPI. Обновляет текущую запись табличной части документа.
 *
 *	Копирует информацию из буфера в текущую строку таблицы.
 *	Если имя не указано или задана пустая строка, обновляются все таблицы документа.
 *	Не работает для проведенного досумента.
 *\_ru
 *	\param tablename - \en metadata name of table\_en \ru
 				имя таблицы в бизнес схеме\_ru
 *	\return \en error code. \_en \ru
				код ошибки.\_ru
 */
ERR_Code
aDocument::TableUpdate( const QString & tablename )
{
	if ( IsConducted() )
	{
		aLog::print(aLog::Debug, tr("aDocument conducted table update"));
		return err_docconducted;
	}
	if ( tablename != "" ) return aObject::TableUpdate(tablename);
	aCfgItem tobj;
	uint n = md->count( obj, md_table );
	ERR_Code err = 0;
	for ( uint i = 0; i < n; i++ )
	{
		tobj = md->find(obj,md_table,i);
		if ( tobj.isNull() ) continue;
		err += aObject::TableUpdate( md->attr(tobj,mda_name) );
	}
	return err;
}



/*!
 *	\en
 *	Check document on mark deleted
 *	\_en
 *	\ru
 *	\brief ScriptAPI. Проверяет помечен ли документ как удаленный.
 *	\_ru
 *	\return \en true if document is mark deleted and fasle in othe event\_en \ru
 				значение флага удаления.\_ru
 */
bool
aDocument::IsMarkDeleted()
{
	return sysJournal->IsMarkDeleted();
}



/*!
 *	\en
 *	Check document on marked
 *	\_en
 *	\ru
 *	\brief ScriptAPI. Проверяет помечен ли документ.
 *	\_ru
 *	\return \en true if document is marked and fasle in othe event\_en \ru
 				значение флага.\_ru
 */
bool
aDocument::IsMarked()
{
        return sysJournal->IsMarked();
}



/*!
 *	\en
 *	Set document deleted flag
 *	\_en
 *	\ru
 *	\brief ScriptAPI. Устанавливает флаг удаления.
 *	\_ru
 *	\param Deleted - \en deleted flag\_en \ru
 				флаг удаления\_ru
 *	\return \en error code. \_en \ru
				код ошибки.\_ru
 */
ERR_Code
aDocument::SetMarkDeleted( bool Deleted )
{
	return sysJournal->SetMarkDeleted( Deleted );
}



/*!
 *	\en
 *	Set document marked flag
 *	\_en
 *	\ru
 *	\brief ScriptAPI Помечает документ.
 *
 *	В последствии помеченные документы можно рассматривать как группу документов,
 *	подобранных для обработки.
 *	\_ru
 *	\param Marked - \en marked flag\_en \ru
 				флаг отметки\_ru
 *	\return \en error code. \_en \ru
				код ошибки.\_ru
 */
ERR_Code
aDocument::SetMarked( bool Marked )
{
	return sysJournal->SetMarked( Marked );
}



/*!
 *	\en
 *	Set document prefix.
 *	Document prefix used for generate document number.
 *	\_en
 *	\ru
 *	\brief ScriptAPI. Задает префикс документа.
 *
 *	Префикс документа используется для генерации номера документа.
 *	\_ru
 *	\see Prefix() Select( const QString & num )
 *	\param pr - \en new document prefix.\_en \ru
 				Новый префикс документа.\_ru
 */
void
aDocument::SetPrefix( const QString & pr )
{
	pref = pr;
}



/*!
 *	\en
 *	Get document prefix.
 *	\_en
 *	\ru
 *	\brief ScriptAPI. Возвращает префикс документа.
 *	\_ru
 *	\see SetPrefix( const QString & pr ) Select( const QString & num )
 *	\return \en document prefix.\_en \ru
 				 текущий префикс документа.\_ru
 */
QString
aDocument::Prefix()
{
	return pref;
}



/*!
 *	\en
 *	Select document by dates.
 *	If set only one date (othe isNull) select records before or after date.
 *	If adocument inited by some document type from metadata selected only this documents from jpurnal,
 *	otherwise - select all documents from system journal with such date
 *	\_en
 *	\ru
 *	\brief ScriptAPI. Выбирает документы по интервалу времени.
 *
 *	Выбирает из системного журнала документы, значение поля "дата" которых
 *	лежит в указанном диапазоне. Если объект настроен на конкретный тип документа из бизнес схемы,
 *	выбираются только документы этого типа, в противном случае - все документы журнала,
 *	соответствующие условию.
 *	\_ru
 *	\param from - \en begin date of selection if isNull - selected from first date of system journal,\_en \ru
 				начальная дата диапазона, если дата isNull, первая дата журнала,\_ru
 *	\param to - \en end date of selection if isNull - selected to last date of system journal.\_en \ru
 				конечная дата диапазона, если isNull - до наиболее позднего документа.\_ru
 *	\return \en error code. \_en \ru
				код ошибки.\_ru
 */
ERR_Code
aDocument::Select( QDateTime from, QDateTime to )
{
	aSQLTable * t = table();
	if ( !t ) return err_notable;
	QString query, dname = "", jFltr;
	if ( concrete ) dname = md->attr(obj, mda_name);
	jFltr = sysJournal->selectionFilter( from, to, dname, true );
	if ( jFltr == "" ) return err_selecterror;
	query = QString("SELECT %1.* FROM %2, a_journ WHERE a_journ.idd=%3.id AND %4").arg(t->tableName).arg(t->tableName).arg(t->tableName).arg( jFltr );
	if ( IsFiltred() ) query = query + QString(" AND %1").arg(t->getNFilter());
	if ( t->exec(query) )
		if ( t->first() )
		{
			setSelected(true);
			sysJournal->findDocument(getUid());
			return err_noerror;
		}
		else return err_notselected;
	else
		return err_selecterror;
}



/*!
 *	\en
 *	Select document by number.
 *	Document number string which conteyn document prefix and some number
 *	If adocument inited by some document type from metadata selected only this documents from jpurnal,
 *	otherwise - select all documents from system journal with such date
 *	\_en
 *	\ru
 *	\brief ScriptAPI. Выбирает документы по номеру.
 *
 *	Номер задается как строка, которая содержит префикс и число.
 *	Если объект настроен на конкретный тип документа из бизнес схемы,
 *	выбираются только документы этого типа, в противном случае - все документы
 *	с таким номером.
 *	\_ru
 *	\see Prefix() SetPrefix()
 *	\param num - \en document number\_en \ru
 				\_ru
 *	\return \en error code. \_en \ru
				код ошибки.\_ru
 */
ERR_Code
aDocument::Select( const QString & num )
{
	aSQLTable * t = table();
	if ( !t ) return err_notable;
	QString query, dname = "", jFltr;
	if ( concrete ) dname = md->attr(obj, mda_name);
	jFltr = sysJournal->selectionFilter( num, dname, true );
	if ( jFltr == "" ) return err_selecterror;
	query = QString("SELECT %1.* FROM %2, a_journ WHERE a_journ.idd=%3.id AND %4").arg(t->tableName).arg(t->tableName).arg(t->tableName).arg( jFltr );
	if ( IsFiltred() ) query = query + QString(" AND %1").arg(t->getNFilter());
	if ( t->exec(query) )
		if ( t->first() )
		{
			setSelected(true);
			sysJournal->findDocument(getUid());
			return err_noerror;
		}
		else return err_notselected;
	else
		return err_selecterror;
}

bool
aDocument::Select()
{
	aSQLTable *t = table();
	if(!t) return false;
	t->select();
	if(t->first())
	{
		setSelected(true);
		return true;
	}
	else
	{
		return false;
	}
}


/*!
 *	\en
 *	Select table lines by name and document uid.
 *	If document uid = 0 selected table in current document.
 *	\_en
 *	\ru
 *	Выбирает строку таблицы по имени и uid документа.
 *	Если uid документа не указан или равен 0, выбирается строка текущего.
 *	\_ru
 *	\param name - \en table metadata name\_en \ru имя таблицы в бизнес схеме\_ru
 *	\param idd - \en document database id (uid)\_en \ru идентификатор экземпляра документа в базе\_ru
 *	\return \en error code. \_en \ru код ошибки.\_ru
 */
ERR_Code
aDocument::tableSelect( const QString & name, qulonglong idd )
{
	qulonglong dUid = idd;
	if ( !dUid ) dUid = getUid();
	if ( !dUid ) return err_abstractobj;
	aSQLTable * t = table( name );
	if ( !t ) return err_notable;
	if ( t->select(QString("idd=%1").arg(dUid)))
		if ( t->first() )
		{
			setSelected( true, name );
			return err_noerror;
		}
		else return err_notselected;
	else return err_selecterror;
}



/*!
 *	\en
 *	Get table by it's metadataname name.
 *	This function added for get access to document table by it name for othe objects.
 *	\_en
 *	\ru
 *	Возвращает таблицу дакумента по имени.
 *	Функция позволяет получить ссылку на таблизу документа по ее имени.
 *	\_ru
 *	\see aObject::table()
 *	\param tname - \en metadata table name.\_en \ru
 				имя таблицы в бизнес схеме.\_ru
 *	\return \en link on aSQLTable or 0 if table not found.\_en \ru
 				ссылка на таблицу.\_ru
 */
aSQLTable *
aDocument::getTable( const QString & tname )
{
	return table(tname);
}



/*!
 *	\en
 *	Select table by name.
 *	Make table with such name current
 *	\_en
 *	\ru
 *	\brief ScriptAPI. Выбирает табличную часть документа по ее имени.
 *
 *	Используется, если документ имеет более одной табличной части.
 *	\_ru
 *	\param name - \en table metadata name\_en \ru имя таблицы \_ru
 *	\return \en error code. \_en \ru
				код ошибки.\_ru
 */
ERR_Code
aDocument::TableSelect( const QString & name )
{
	return tableSelect( name );
}



/*!
 *	\en
 *	Copy current document.
 *	Create document with header and table pats like current document.
 *	New document have current date and next unical number.
 *	\_en
 * \ru
 *	\brief ScriptAPI. Копирует текущий документ
 * \_ru
 *	\return \en error code. \_en \ru
				код ошибки.\_ru
 */
ERR_Code
aDocument::Copy()
{
	if ( !sysJournal ) return err_nosysjournal;
	QString nowPrefix = sysJournal->getPrefix();
	qulonglong oldUid = getUid();
	ERR_Code err = aObject::Copy();
	if ( err ) return err;
	qulonglong Uid = getUid();
	err = sysJournal->New( Uid, nowPrefix, md->id(obj) );
	if ( err )
	{
		table()->exec(QString("DELETE FROM %1 WHERE id=%2").arg(table()->tableName).arg(Uid));
		db->markDeleted( Uid );
		return err;
	}
	aDocument doc( obj, db );
	err = doc.select(oldUid);
	if ( err ) return err;
	aCfgItem tObj, fObj;
	QString tname, query, tdbname, fname ;
	uint n = md->count(obj,md_table), m;
	for ( uint i = 0; i < n; i++ )
	{
		tObj = md->find( doc.obj, md_table, i );
		if ( tObj.isNull() ) continue;
		tname = md->attr( tObj, mda_name );
		if ( !doc.TableSelect(tname) )
		{
			TableNewLine(tname);
			m = md->count( tObj, md_field );
			for ( uint j = 0; j < m; j++ )
			{
				fObj = md->find( tObj, md_field, j );
				if ( fObj.isNull() ) continue;
				fname = md->attr( fObj, mda_name );
				TableSetValue( tname, fname, doc.TableValue( tname, fname ) );
			}
			TableUpdate( tname );
			while ( doc.TableNext( tname ) )
			{
				TableNewLine(tname);
				m = md->count( tObj, md_field );
				for ( uint j = 0; j < m; j++ )
				{
					fObj = md->find( tObj, md_field, j );
					if ( fObj.isNull() ) continue;
					fname = md->attr( fObj, mda_name );
					TableSetValue( tname, fname, doc.TableValue( tname, fname ) );
				}
				TableUpdate( tname );
			}
		}
	}
	return err_noerror;
}



/*!
 *	\en
 *	Make current next selected header.
 *	Use for navigation by selected documents.
 *	\_en
 *	\ru
 *	\brief ScriptAPI. Переход к следующему документу.
 *	\_ru
 *	\return \en operation result.\_en \ru
 			результат операции (успешно или нет)\_ru
 */
bool
aDocument::Next()
{
	if ( aObject::Next() ) if ( sysJournal->findDocument( getUid() ) ) return true;
	return false;
}



/*!
 *	\en
 *	Make current prev selected header.
 *	Use for navigation by selected documents.
 *	\_en
 *	\ru
 *	\brief ScriptAPI. Переход к предыдущему документу
 *	\_ru
 *	\return \en operation result.\_en \ru
 			результат операции (успешно или нет)\_ru
 */
bool
aDocument::Prev()
{
	if ( aObject::Prev() ) if ( sysJournal->findDocument( getUid() ) ) return true;
	return false;
}



/*!
 *	\en
 *	Make current first selected header.
 *	Use for navigation by selected documents.
 *	\_en
 *	\ru
 *	\brief ScriptAPI. Переходит к первому документу
 *	\_ru
 *	\return \en operation result.\_en \ru
 			результат операции (успешно или нет)\_ru
 */
bool
aDocument::First()
{

	printf("aDoc::first uid = %llu\n", getUid());
	if ( aObject::First() ) if ( sysJournal->findDocument( getUid() ) ) return true;
	return false;
}



/*!
 *	\en
 *	Make current last selected header.
 *	Use for navigation by selected documents.
 *	\_en
 *	\ru
 *	\brief ScriptAPI. Переходит к последнему документу
 *	\_ru
 *	\return \en operation result.\_en \ru
 			результат операции (успешно или нет)\_ru
 */
bool
aDocument::Last()
{
	if ( aObject::Last() ) if ( sysJournal->findDocument( getUid() ) ) return true;
	return false;
}



/*!
 *	\en
 *	Make current next selected line in table.
 *	Use for navigation in table.
 *	\_en
 *	\ru
 *	\brief ScriptAPI. Переход к следуещей записи в табличной части документа
 *	\_ru
 *	\param name - \en metadata table name.\_en \ru
 					имя таблицы из бизнес схемы.\_ru
 *	\return \en operation result.\_en \ru
 			результат операции (успешно или нет)\_ru
 */
bool
aDocument::TableNext( const QString & name )
{
	aSQLTable * t = table(name);
	if ( !t ) return false;
	if ( t->next() ) return true;
	else return false;
}



/*!
 *	\en
 *	Make current prev selected line in table.
 *	Use for navigation in table
 *	\_en
 *	\ru
 *	\brief ScriptAPI. Переход к предыдущей записи в табличной части документа
 *	\_ru
 *	\param name - \en metadata table name.\_en \ru
 					имя таблицы из бизнес схемы.\_ru
 *	\return \en operation result.\_en \ru
 			результат операции (успешно или нет)\_ru
 */
bool
aDocument::TablePrev( const QString & name)
{
	aSQLTable * t = table(name);
	if ( !t ) return false;
	if ( t->prev() ) return true;
	else return false;
}



/*!
 *	\en
 *	Make current first selected line in table.
 *	Use for navigation in table
 *	\_en
 *	\ru
 *	\brief ScriptAPI. Переход к первой записи в табличной части документа
 *	\_ru
 *	\param name - \en metadata table name.\_en \ru
 					имя таблицы из бизнес схемы.\_ru
 *	\return \en operation result.\_en \ru
 			результат операции (успешно или нет)\_ru
 */
bool
aDocument::TableFirst( const QString & name)
{
	aSQLTable * t = table(name);
	if ( !t ) return false;
	if ( t->first() ) return true;
	else return false;
}



/*!
 *	\en
 *	Make current last selected line in table.
 *	Use for navigation in table
 *	\_en
 *	\ru
 *	\brief ScriptAPI. Переход к последней записи в табличной части документа
 *	\_ru
 *	\param name - \en metadata table name.\_en \ru
 					имя таблицы из бизнес схемы.\_ru
 *	\return \en operation result.\_en \ru
 			результат операции (успешно или нет)\_ru
 */
bool
aDocument::TableLast( const QString & name )
{
	aSQLTable * t = table(name);
	if ( !t ) return false;
	if ( t->last() ) return true;
	else return false;
}




/*!
 *	\en
 *	Set filter for some field.
 *	If filter seted, on select selected only records where that field equal value.
 *	If seted filters for diferent fields it's group by and.
 *	If filter reseted it override previous value
 *	\_en
 *	\ru
 *	\brief ScripAPI. Устанавливает фильтр на доступные документы.
 *
 *	Доступными становяться только документы, удовлетворяющие условию.
 *	Например, для получения всех накладных с номером 0003 нужно сделать примерно следующее:
 *	\code
 *	aDocument * doc = new aDocument("Накладная", db);
 *	doc->SetFilter("Номер","0003");
 *	doc->First();
 *	do{
 *		//какие-то действия
 *	   }while(doc->Next());
 *	delete doc;
 *	\endcode
 *	\_ru
 *	\see ClearFilter()
 *	\param valname - \en metadata name of field.\_en \ru имя поля в метаданных \_ru
 *	\param value - \en filter value.\_en \ru Значение фильтра \_ru
 *	\return \en error code. \_en \ru код ошибки.\_ru
 */
ERR_Code
aDocument::SetFilter( const QString & valname, const QVariant & value )
{
	return aObject::SetFilter( valname, value );
}



/*!
 *	\en
 *	Clear all filters values.
 *	\_en
 *	\ru
 *	\brief ScriptAPI. Сбрасывает фильтр.
 *	\_ru
 *	\see SetFilter( const QString & valname, const QVariant & value )
 *	\return \en error code. \_en \ru код ошибки.\_ru
 */
ERR_Code
aDocument::ClearFilter()
{
	return aObject::ClearFilter();
}

/*
ERR_Code
aDicument::TableSetFilter( const QString & tname, const QString & valname, const QVariant & value )
{
}

ERR_Code
aDicument::TableClearFilter( const QString & tname )
{
}
*/



/*!
 *	\en
 *	Set conduct flag for current document.
 *	\_en
 *	\ru
 *	Устанавливает флаг проведения для текущего документа. Не используйте эту функцию напрямую. Используйте Conduct() и UnConduct()
 *	\_ru
 *	\param cond - \en conducted flag, flag set on 1 if cond true adn 0 if cond false\_en \ru значение флага \_ru
 *	\see Conduct() Unconduct() IsConducted()
 *	\return \en error code. \_en \ru код ошибки.\_ru
 */
ERR_Code
aDocument::setConduct( bool cond )
{
	qulonglong idd = getUid();

	if (!idd) return err_nodocument;
	QSqlDatabase *tdb = db->db();
	QString query;
	query = QString("UPDATE a_journ SET cf='%1' WHERE idd=%2").arg( ( int ) cond).arg( idd );
	tdb->exec(query);
	if ( !tdb->lastError().type() )
	{
		aLog::print(aLog::Debug, tr("aDocument conduct"));
		return err_noerror;
	}
	else
	{
		aLog::print(aLog::Error, tr("aDocument conduct"));
		return err_conducterror;
	}
}

/**
 * \brief Deprecated
 */
ERR_Code
aDocument::Conduct(){
	aLog::print(aLog::Debug, tr("Deprecated method call. aDocument::Conduct(...)"));
	return this->SignIn();
}

/*!
 *	\en
 *	Make document conducted.
 *	Work only for unconducted documents otherwise return error err_docconducted
 *	\_en
 *	\ru
 *	\brief ScriptAPI. Вызывает проведение документа.
 *	\_ru
 *	\see SignOut() IsSignedIn()
 *	\return \en error code. \_en \ru код ошибки.\_ru
 */
ERR_Code
aDocument::SignIn()
{
	if ( IsConducted() ) return err_docconducted;
	else return setConduct(true);

}


/**
 * \brief Deprecated
 */
ERR_Code
aDocument::UnConduct(){
	aLog::print(aLog::Debug, tr("Deprecated method call. aDocument::UnConduct()"));
	return this->SignOut();
}

/*!
 *	\en
 *	Make document unconducted.
 *	Work only for conducted documents otherwise return error err_docunconducted
 *	\_en
 *	\ru
 *	\brief ScriptAPI. Отменяет проведение документа.
 *	\_ru
 *	\see SignIn() IsSignedIn()
 *	\return \en error code. \_en \ru код ошибки.\_ru
 */
ERR_Code
aDocument::SignOut()
{
	if ( !IsConducted() ) return err_docunconducted;
	ERR_Code err = setConduct(false);
	if ( err ) return err;
	//Q_ULLONG uid = getUid();
	aIRegister * reg = new aIRegister("",db);
	reg->deleteDocument(this);
	delete reg;
	aCfgItem aReg, aRegs;
	aRegs= md->find(md->find(md->find(mdc_metadata),md_registers,0),md_aregisters,0);
	if ( aRegs.isNull() ) return 1; //TODO: set error code
	uint n = md->count( aRegs, md_aregister );
	aARegister *areg;
	for ( uint i = 0; i < n; i++  )
	{
		aReg = md->find( aRegs, md_aregister, i );
		if ( aReg.isNull() ) continue;
		areg = new aARegister(aReg,db);
		areg->deleteDocument(this);
		aLog::print(aLog::Debug, tr("aDocument delete from accumulation register"));
		delete areg;
		areg = 0;
	}
	return err_noerror;
}


/**
 * \brief Deprecated
 */
bool
aDocument::IsConducted(){
	aLog::print(aLog::Debug, tr("Deprecated method call. aDocument::IsConducted()"));
	return this->IsSignedIn();
}

/*!
 *	\en
 *	Check document conducting.
 *	\_en
 *	\ru
 *	\brief ScriptAPI. Проверяет, проведен ли документ.
 *	\_ru
 *	\see SignIn() SignOut()
 *	\return \en true, if document is conducted. \_en \ru true, если документ проведен \_ru
 */
bool
aDocument::IsSignedIn()
{
	qulonglong idd = getUid();
	if (!idd) return false;
	QSqlQuery q = db->db()->exec(QString("SELECT cf FROM a_journ WHERE idd=%1").arg(idd));
	if ( q.first() ) return q.value(0).toBool();
	else return false;
}



/*!
 *	\en
 *	Select document with id = \a uid.
 *	\_en
 *	\ru
 *	Выбирает документ с заданным идентификатором.
 *	\_ru
 */
ERR_Code
aDocument::select( qulonglong uid)
{
	ERR_Code res= err_noerror;
	qulonglong idd = 0;
	if ( !sysJournal ) return err_nosysjournal;
	res = aObject::select(uid);
	if(res != err_noerror) return res;
	idd = sysJournal->findDocument(uid);
	if (idd == 0) return err_nodocument;
	aCfgItem tobj;
	uint n = md->count( obj, md_table );
	for ( uint i = 0; i < n; i++ )
	{
		tobj = md->find(obj,md_table,i);
		if ( tobj.isNull() ) continue;
		tableSelect(md->attr(tobj,mda_name), uid);
//		aObject::select( QString("idd=%1").arg(uid), md->attr(tobj,mda_name) );
	}
	return res;
}

/**
 * \ru
 * 	\brief ScriptAPI. Выбирает документ с заданным идентификатором.
 * \_ru
 */
int
aDocument::SelectDocument( QVariant uid )
{
	return select(uid.toULongLong());
}

/*!
 *	\en
 *	Sets position in document table tname at \a pos.
 *	\_en
 *	\ru
 *	\brief ScriptAPI. Устанавливает позицию в табличной части документа на строку с заданным номером.
 *	\_ru
 *	\param pos - \en line number \_en \ru номер строки \_ru
 *	\param tname - \en table name \_en \ru имя таблицы \_ru
 */
void
aDocument::Seek(int pos, const QString &tname)
{
	aSQLTable * t = table(tname);
	if(t && t->first())
	{
		setSelected( true, tname );
		for(int i=0; i<pos; i++)
		{
			if(t->next()) continue;
			else break;
		}
	}
}

/**
 * \en
 * 	Get value of document field.
 * \_en
 * \ru
 * 	\brief ScriptAPI. Получение значения атрибута документа c указанным идентификатором.
 *
 * 	С помощью этого метода можно получить значение атрибута документа, не являющегося текущим.
 * 	Вызов метода приводит к смене текущего документа.
 * 	Для получения значения атрибута текущего документа используйте метод Value(...),
 * 	унаследованный от aObject.
 * \_ru
 *
 * \param docId - \en document id \_en \ru id документа \_ru
 * \param fname - \en md field name \_en \ru имя поля в метаданных \_ru
 *
 * \return  \en value of field \_en \ru значение поля или строку "Unknown", в случае ошибки \_ru
 *
 */
QVariant
aDocument::GetDocumentValue(QVariant docId, const QString &fname)
{
	//aSQLTable * t = table();
	//if ( !t ) return "";
	//t->select(docId.toULongLong());
	//if ( !t->first() ) return "";
	//else
	//{
	if(!select(docId.toULongLong()))
	{
		QVariant val = Value( fname );
		if(val.type()==QVariant::ULongLong ||
				val.type()==QVariant::LongLong)
		{
			return val.toString();
		}
		else
		{
			return val;
		}
	}
	else
	{
		aLog::print(aLog::Error, tr("aDocument::GetDocumentValue(...): Can't find aDocument"));
		return "Unknown";
	}
	//}

}
/*!
 *	\en
 *	Return field value with name = \a name of the primary object database table.
 *	\_en
 *	\ru
 *	Возвращает значения поля с именем = \a name основг\ной таблицы объекта.
 *	\_ru
 *	\param name - \en field name \_en \ru имя поля \_ru
 *	\return \en field value \_en \ru значение поля. Поля типа Q_ULLONG и DateTime переводятся в строки. \_ru
 */
QVariant
aDocument::Value( const QString & name, const QString &tableName )
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
