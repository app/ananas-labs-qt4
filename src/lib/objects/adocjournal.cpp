/****************************************************************************
** $Id: adocjournal.cpp,v 1.3 2008/11/09 21:09:11 leader Exp $
**
** Documents journal metadata object implementation file of
** Ananas application library
**
** Created : 20040701
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

#include	<qdatetime.h>
//Added by qt3to4:
#include <QSqlQuery>

#include	"acfg.h"
#include	"aobject.h"
#include	"adatabase.h"
#include	"adocjournal.h"
#include 	"alog.h"

/*!
 *	\en
 *	Constructor. Init object with \a context.
 *	\_en
 *	\ru
 *	Конструктор. Инициализирует объект объектом метаданных.
 *	\_ru
 *	\param context - \en md object\_en \ru объект метаданных \_ru
 *	\param adb - \en link to database\_en \ru ссылка на базу данных \_ru
*/
aDocJournal::aDocJournal( aCfgItem context, aDatabase * adb )
:aObject( context, adb, 0, "aDocJournal")
{
	initObject();
}


/*!
 *	\en
 *	Constructor. Init object with md object, founded by \a name.
 *	\_en
 *	\ru
 *	Конструктор. Инициализирует объект объектом метаданных с именем \a name.
 *	\_ru
 *	\param name - \en md object name\_en \ru имя объекта метаданных \_ru
 *	\param adb - \en link to database\_en \ru ссылка на базу данных \_ru
*/
aDocJournal::aDocJournal( const QString & name, aDatabase * adb)
:aObject( name, adb, 0, "aDocJournal")
{
	db = adb;
	initObject();
}


/*!
 *	\en
 *	Constructor. Creates system journal.
 *	\_en
 *	\ru
 *	Конструктор. Создает экземпляр системного журнала, в котором хранятся ссылки на все созданные документы.
 *	\_ru
 *	\param adb - \en link to database\_en \ru ссылка на базу данных \_ru
*/
aDocJournal::aDocJournal( aDatabase * adb )
{
	db = adb;
	initObject();
}



ERR_Code
aDocJournal::initObject()
{
	ERR_Code err = err_noerror;
	aCfgItem mditem, docitem, header;


	journalType=0;

	setInited( true );
	md = 0;
	if ( db ) md = &db->cfg;
	if(!md)
	{
		 aLog::print(aLog::Error, tr("aDocJournal md object not exists"));
		 return err_objnotfound;
	}
//	aObject::initObject();
	journalType= md->attr(obj,mda_type).toInt();
	if (journalType)
	{
		mditem= md->find(obj,md_fieldid);
		if (mditem.isNull())
		{
			aLog::print(aLog::Error, tr("aDocJournal columns not defined"));
			return err;
		}
		else
		{
			aLog::print(aLog::Debug, tr("aDocJournal column defined"));
		}
		docitem= md->parent( md->parent( md->find(md->text(mditem).toLong())));

		header = md->find( docitem, md_header );
		if(header.isNull()) aLog::print(aLog::Error, tr("aDocJournal invalid column define"));
	//table for special journal
		err =  tableInsert( aDatabase::tableDbName( *md, header ), header );
//		printf("table name is %s\n", aDatabase::tableDbName( *md, header ).ascii());
	}
	else
	{
	//table for common journal
		err = tableInsert( "a_journ" );
	}

	return err;
}



/*!
 *\~english
 *	Get current document id.
 *	Get database id for current document.
 *\~russian
 *	Возвращает id текущего документа.
 *	Возвращает id документа в базе данных.
 *\~
 *\return \~english database document id. \~russian id документа в базе данных.\~
 */
qulonglong
aDocJournal::docId()
{
	if (!journalType) {
		return table()->sysValue("idd").toULongLong();
	} else {
		return table()->sysValue("id").toULongLong();
	}
}



/*!
 *\~english
 *	Gets current document type.
 *\~russian
 *	Получение типа текущего документа.
 *\~
 *\return \~english database document type. \~russian тип документа в базе данных.\~
 */
int
aDocJournal::docType()
{
	return db->uidType( docId() );
}



/*!
 *\~english
 *	Delets current document.
 *\~russian
 *	Удаляет текущий документ.
 *\~
 *\see deleteDocument( Q_ULLONG idd )
 *\return \~english error code \~russian код ошибки.\~
 */
ERR_Code
aDocJournal::Delete()
{
	ERR_Code err;
	aDocument * doc = CurrentDocument();
	if ( !doc ) return err_notselected;
	else err = doc->Delete();
	delete doc;
	doc = 0;
	return err;
}



/*!
 *\~english
 *	Delets document with id =\a idd.
 *\~russian
 *	Удаляет документ с заданным идентификатором.
 *\~
 *\see Delete()
 *\param idd - \~english document id \~russian идентификатор документа \~
 *\return \~english error code \~russian код ошибки.\~
 */
ERR_Code
aDocJournal::deleteDocument( qulonglong idd )
{
	aSQLTable * t = table();
	qulonglong Uid = findDocument(idd);
	db->markDeleted( Uid );
	if (t->exec("DELETE FROM a_journ WHERE idd =" + QString("%1").arg(idd)))
	{
		aLog::print(aLog::Info, tr("aDocJournal delete document with idd=%1").arg(idd));
		return err_noerror;
	}
	else
	{
		aLog::print(aLog::Error, tr("aDocJournal delete document with idd=%1").arg(idd));
		return err_execerror;
	}
}



/*!
 *\~english
 *	Create new record in system journal.
 *	Record create afte crate new document and use information about doc's
 *	database id, prefix and metadata id(type).
 *	Document number generate automatically by nextNumber().
 *\~russian
 *	Добавляет запись в системный журнал.
 *	Запись добавляется после создания документа и использует информацию о его
 *	id в базе данных, префиксе номера и id в конфигурации.
 *	Номер досумента генерируется функцией nextNumber()
 *\~
 *\param idd - \~english database document id.\~russian id базы данных.\~
 *\param docPrefix - \~english document number prefix.
 					\~russian перфикс номера документа.\~
 *\param type - \~english document metadata id.
 				\~russian id документа в конфигурации.\~
 *\return \~english error code.\~russian код ошибки.\~
 */
ERR_Code
aDocJournal::New( qulonglong idd, const QString & docPrefix, int type )
{
	//qulonglong Uid =0;// db->uid( md_systemjournal );
	aDataTable * t = table();
	if ( !t ) return err_notable;
	//t->exec("LOCK TABLE a_journ WRITE");
	//printf("insert into ajourn id=%llu idd=%llu, docPrefix=%s, type=%d\n",Uid,idd, (const char*)docPrefix.local8Bit(), type);
//	t->exec("LOCK TABLE a_journ WRITE");
	QSqlRecord *rec;
	rec = t->primeInsert(); // get edit buffer for table elements
//	t->primeInsert();
	//ide = rec->value("id").toULongLong();
	//rec->setValue("id",Uid);
	rec->setValue("idd",idd);
	rec->setValue("typed",type);
	rec->setValue("num",nextNumber(docPrefix,type));
	rec->setValue("pnum",docPrefix);
	rec->setValue("ddate",QDateTime::currentDateTime());

	t->insert(); // insert edit buffer as new line in table
//TODO: error handle
	aLog::print(aLog::Info, tr("aDocJournal new document with idd=%1").arg(idd));
	/*
	t->primeInsert();
	t->insert();
	t->setSysValue( "id", Uid );
	t->setSysValue( "idd", idd );
	t->setSysValue( "typed", type );
	t->setSysValue( "num", nextNumber(docPrefix,type) );
	t->setSysValue( "pnum", docPrefix );
	t->setSysValue( "ddate", QDateTime::currentDateTime() );
	Update();
	t->exec("UNLOCK TABLE");
	*/

	int err = selectDocument( idd );
	setSelected(!err);
	return err;
}



/*!
 *\~english
 *	Select document with id =\a idd.
 *\~russian
 *	Выбирает документ с заданным идентификатором.
 *\~
 *\param idd - \~english document id \~russian идентификатор документа \~
 *\see findDocument( Q_ULLONG idd )
 *\return \~english error code \~russian код ошибки.\~
 */
ERR_Code
aDocJournal::selectDocument( qulonglong idd )
{
	aDataTable * t = table();
	if ( !t ) return err_notable;
	QString flt;
	flt = QString("idd=%1").arg(idd);
	if ( t->select(flt) )
		if ( t->first() )
		{
			setSelected(true);
			return err_noerror;
		}
		else return err_notselected;
	return err_selecterror;
}




/*!
 *\~english
 *	Generate next document number.
 *\~russian
 *	Генерирует следующий номер для документа.
 *	Номер представляет собой строку, состоящую из префикса номера и числа.
 *	Номер документа генерируется используя префикс номера документа
 *	и его тип (id в конфигурации), как следующий за максимальным для даного сочетания.
 *	Номер является уникальным в для доументов одного типа.
 *\~
 *\param prefix - \~english \~russian префикс номера.\~
 *\param mdId - \~english \~russian id документа в конфигурации.\~
 *\return \~english document number \~russian номер документа (только число).\~
 */
QString
aDocJournal::nextNumber( const QString &prefix, int mdId )
{
	QString num="0", query;
	query = QString("SELECT MAX(num)+1 FROM a_journ where pnum='%1' AND typed=%2").arg( prefix ).arg(mdId);
	QSqlQuery q = db->db()->exec( query );
	if ( q.first() ) num = q.value( 0 ).toString();
        if ( num == "0" ) num = "1";
	aLog::print(aLog::Info, tr("aDocJournal generated next number for %1 is %2").arg(prefix).arg(num));
	//debug_message("<<<Next document number query>>>\n%s\nreturn: %s\n",
		//	(const char*)query,(const char*)num);
	return num;
}



/*!
 *\~english
 *	Gets current document date.
 *\~russian
 *	Возвращает дату текущего документа.
 *\~
 *\see setDate( QVariant date )
 *\return \~english document date \~russian дата документа \~
 */
QVariant
aDocJournal::getDate()
{
	aSQLTable * t = table();
	if ( !t || !selected() ) return QVariant::Invalid;
	else return t->sysValue("ddate");
}


/*!
 *\~english
 *	Gets current document prefix + number.
 *\~russian
 *	Возвращает строку префикс + номер текущего документа.
 *\~
 *\see setNumber( QVariant number )
 *\return \~english document number \~russian номер документа \~
 */
QString
aDocJournal::getNumber()
{
	aSQLTable * t = table();
	if ( !t || !selected() ) return "";
	else return t->sysValue("pnum").toString() + t->sysValue("num").toString();
}

/*!
 *\~english
 *	Gets current document number.
 *\~russian
 *	Возвращает номер текущего документа.
 *\~
 *\see getNumber( QVariant number )
 *\return \~english document number \~russian номер документа \~
 */
int
aDocJournal::getSerialNumber()
{
	aSQLTable * t = table();
	if ( !t || !selected() ) return 0;
	else return t->sysValue("num").toInt();
}

/*!
 *\~english
 *	Sets current document date.
 *\~russian
 *	Устанавливает дату текущего документа.
 *\~
 *\see getDate()
 *\param date - \~english document date \~russian дата документа \~
 *\return \~english error code \~russian код ошибки.\~
 */
ERR_Code
aDocJournal::setDate( QVariant date )
{
	aSQLTable * t = table();
	if ( !t ) return err_notable;
	if ( !selected() ) return err_notselected;
	t->setSysValue( "ddate", date );
	return err_noerror;
}



/*!
 *\~english
 *	Sets current document number.
 *\~russian
 *	Устанавливает номер текущего документа.
 *\~
 *\see getNumber()
 *\param number - \~english document number \~russian номер документа \~
 *\return \~english error code \~russian код ошибки.\~
 */
ERR_Code
aDocJournal::setNumber( QVariant number  )
{
	aSQLTable * t = table();
	if ( !t ) return err_notable;
	if ( !selected() ) return err_notselected;
	QString pr;
	int num;
	decodeDocNum( number.toString(), pr, num  );
	t->setSysValue( "pnum", pr );
	t->setSysValue( "num", num );
	return err_noerror;
//	return Update();
	/*QString query;
	Q_ULLONG idd = docId();
	QSqlQuery q = db->db()->exec ( QString("SELECT pnum, num FROM a_journ WHERE idd=%1").arg( idd ) );
	if ( q.first() ) return q.value(0).toString()+q.value(1).toString();
	else return "";*/
}



/*!
 *\~english
 *	Find document by id.
 *\~russian
 *	Ищет документ в системном журнале по его идентификатору.
 *\~
 *\see findDoc( const QString & number, int type ) selectDocument( Q_ULLONG idd )
 *\param idd - \~english document number \~russian номер документа \~
 *\return 	\~english document id or 0 if document not found
 *		\~russian id документа или 0, если документ не найден.\~
 */
qulonglong
aDocJournal::findDocument( qulonglong idd )
{
	aSQLTable * t = table();
	if ( !t ) return 0;
	if (t->exec(QString("SELECT * FROM a_journ WHERE idd=%1").arg(idd)) )
		if ( t->first() )
		{
			setSelected( true );
			return getUid();
		}
		else
		{
			aLog::print(aLog::Debug, tr("aDocJournal document not found with idd=%1").arg(idd));
		}
	return 0;
}



/*!
 *\~english
 *	Find document by number and type.
 *\~russian
 *	Ищет документ по его номеру и типу.
 *\~
 *\see findDocument( Q_ULLONG idd )
 *\param number - \~english document number \~russian номер документа, состоящий из префикса и номера \~
 *\param type - \~english document type \~russian тип документа \~
 *\return 	\~english document id or 0 if document not found
 *		\~russian id документа или 0, если документ не найден.\~
 */
qulonglong
aDocJournal::findDoc( const QString & number, int type )
{
	QString pref;
	int num;
	decodeDocNum( number, pref, num );
	if ( table()->select(QString("pnum='%1' AND num=%2 AND typed=%3").arg(pref).arg(num).arg(type)) )
		if ( table()->first() )
		{
			setSelected(true);
			return table()->sysValue("idd").toULongLong();
		}
		else
		{
			aLog::print(aLog::Debug, tr("aDocJournal document not found with number=%1 and type=%2 ").arg(number).arg(type));
		}
	return 0;
}



/*!
 *\~english
 *	Gets current document.
 *\~russian
 *	Возвращает текущий документ. Полученный документ необходимо удалить после использования.
 *	\code
 *	aDocument *doc = sysJournal->CurrentDocument();
 *	//какие-то действия с документом
 *	delete doc;
 *	doc=0;
 *	\endcode
 *\~
 *\return 	\~english current document
 *		\~russian текущий документ \~
 */
aDocument*
aDocJournal::CurrentDocument()
{
	aCfgItem i = md->find( docType() );
	if(!i.isNull())
	{
		aDocument *d = new aDocument( i, db );
		if(!d->select( docId() ))
		{
			return d;
		}
		else
		{
			delete d;
			return 0;
		}
	}
	else
	{
		return 0;
	}
}



/*!
 *\~english
 *	Select documents at some period.
 *\~russian
 *	Выбирает документы определенного типа за некоторый период.
 *\~
 *\param from - \~english begin date \~russian дата начала периода \~
 *\param to - \~english end date \~russian дата окончания периода \~
 *\param nmName - \~english document type \~russian тип документа \~
 *\return 	\~english error code
 *		\~russian код ошибки \~
 */
ERR_Code
aDocJournal::Select( QDateTime from, QDateTime to, const QString & mdName )
{
	aSQLTable  * t = table();
	if ( !t )
		return err_notable;
	QString fltr, docType = "";
	if ( mdName != "" )
	{
		aCfgItem item = md->find( "Document." + mdName );
		if ( item.isNull() )
			return err_objnotfound;
		int type = md->id( item );
		docType = QString(" AND typed=%1").arg(type);
	}
	if ( from.isNull() )
		if ( !to.isNull() )
			fltr = "ddate<='" + to.toString("yyyy-MM-dd hh:mm:ss") + "'";
		else
			return err_condition;
	else
		if ( !to.isNull() )
			fltr = "ddate>='" + from.toString("yyyy-MM-dd hh:mm:ss") + "' AND ddate<='" +\
					 to.toString("yyyy-MM-dd hh:mm:ss") + "'";
		else
			fltr = "ddate>='" + from.toString("yyyy-MM-dd hh:mm:ss") + "'";
	if ( t->select( fltr + docType ) )
		if ( t->first() )
		{
			setSelected(true);
			return err_noerror;
		}
		else return err_notselected;
	return err_selecterror;
}



/*!
 *\~english
 *	Select document by number and type.
 *\~russian
 *	Выбирает документ определенного типа и номера.
 *\~
 *\param number - \~english document number \~russian составной номер документа \~
 *\param nmName - \~english document type \~russian тип документа \~
 *\return 	\~english error code
 *		\~russian код ошибки \~
 */
ERR_Code
aDocJournal::Select( const QString & number, const QString & mdName )
{
	aSQLTable * t = table();
	if ( !t ) return err_notable;
	QString dFilter = "", pref;
	if ( mdName != "" )
	{
		aCfgItem tObj = md->find( "Document."+mdName );
		if ( tObj.isNull() ) return err_objnotfound;
		else dFilter = QString(" AND typed=%1").arg(md->attr(tObj,mda_id));;
	}
	int num;
	//printf(">>>>>>>>>>>>decode doc num %s\n",number.ascii());
	decodeDocNum( number, pref, num );
	if ( t->select(QString("pnum='%1' AND num=%2").arg(pref).arg(num) + dFilter) )
		if ( t->first() )
		{
//			printf(">document selected!\n");

			aLog::print(aLog::Debug, tr("aDocJournal select document with number=%1 and md name=%2").arg(number).arg(mdName));
			setSelected(true);
			return err_noerror;
		}
		else return err_notselected;
	return err_selecterror;

}



/*!
 *\~english
 *	Gets current document prefix
 *\~russian
 *	Выбирает префикс текущего документа.
 *\~
 *\return 	\~english prexix
 *		\~russian префикс \~
 */
QString
aDocJournal::getPrefix()
{
	QString pref = "";
	qulonglong dUid = docId();
	if ( dUid )
	{
		QSqlQuery q = db->db()->exec(QString("SELECT pnum FROM a_journ WHERE idd=%1").arg(dUid));
		if ( q.first() ) pref = q.value(0).toString();
	}
	return pref;
}



QString
aDocJournal::selectionFilter( QDateTime from, QDateTime to, const QString & mdName, bool full )
{
	QString fltr="", docType = "", journ="";
	if ( full ) journ="a_journ.";
	if ( mdName != "" )
	{
		aCfgItem item = md->find( "Document." + mdName );
		if ( item.isNull() )
			return "";
		int type = md->id( item );
		docType = " AND " + journ + QString("typed=%1").arg(type);
	}
	if ( from.isNull() )
		if ( !to.isNull() )
			fltr = journ + "ddate<='" + to.toString("yyyy-MM-dd hh:mm:ss") + "'";
		else
			return "";
	else
		if ( !to.isNull() )
			fltr = journ + "ddate>='" + from.toString("yyyy-MM-dd hh:mm:ss") + "' AND " + journ + "ddate<='" + to.toString("yyyy-MM-dd hh:mm:ss") + "'";
		else
			fltr = journ + "ddate>='" + from.toString("yyyy-MM-dd hh:mm:ss") + "'";
	return fltr + docType;
}

QString
aDocJournal::selectionFilter( const QString & num, const QString & mdName, bool full )
{
	QString pref, fltr="", journ="";
	int nm;
	decodeDocNum( num, pref, nm );
	if ( full )
	{
		fltr=QString("a_journ.num=%1 AND a_journ.pnum='%2'").arg(nm).arg(pref);
		journ = "a_journ.";
	}
	else fltr=QString(" num=%1 AND pnum='%2'").arg(nm).arg(pref);
	if ( mdName != "" )
	{
		aCfgItem tObj = md->find( "Document."+mdName );
		if ( tObj.isNull() ) return "";
		else fltr += QString(" AND %1typed=%2").arg(journ).arg(md->id(tObj));
	}
	return fltr;
}
