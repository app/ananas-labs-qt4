/****************************************************************************
** $Id: acatalogue.cpp,v 1.3 2008/11/09 21:09:11 leader Exp $
**
** Catalogue metadata object implementation file of
** Ananas application library
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) 2003-2004 Grigory Panov, Yoshkar-Ola.
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
#include	"aobject.h"
#include	"adatabase.h"
#include	"acatalogue.h"
#include 	"alog.h"
//Added by qt3to4:
#include <Q3ValueList>


/*!\en
 *\_en \ru
 *\_ru
 */
aCatalogue::aCatalogue( aCfgItem context, aDatabase * adb )
:aObject( context, adb, 0, "aCatalogue")
{
	concrete = !context.isNull();
	initObject();
}



/*!\en
 *\_en \ru
 *\_ru
 */
aCatalogue::aCatalogue(QString name, aDatabase * adb)
:aObject( "Catalogue."+name, adb, 0, "aCatalogue")
{
	if ( name.isEmpty() ) concrete = false;
	else concrete = true;
	initObject();
}



ERR_Code
aCatalogue::initObject()
{
	ERR_Code err = aObject::initObject();
	if ( err ) return err;
	aCfgItem g = md->find( obj, md_group ), e = md->find( obj, md_element );
	err = tableInsert( aDatabase::tableDbName( *md, e ), e );
	if ( err ) return err;
	return tableInsert( aDatabase::tableDbName( *md, g ), g, md_group );
}

/*
Q_ULLONG
aCatalogue::getCatGroup()
{
	aSQLTable * t = table();
	if ( !t ) return 0;
	if ( !selected() ) return 0;
	return t->sysValue("idg").toULongLong();
}*/

qulonglong
aCatalogue::getGroup()
{
	aSQLTable * t = table(md_group);
	if ( !t ) return 0;
	if ( !selected(md_group) ) return 0;
	return t->sysValue("id").toULongLong();
}

ERR_Code
aCatalogue::setGroup( qulonglong idg )
{
	aSQLTable * t = table();
	if ( !t ) return err_notable;
	if ( !selected() ) return err_notselected;
	t->setSysValue( "idg", idg );
	t->primeUpdate();
	t->update();
	return err_noerror;
}

ERR_Code
aCatalogue::SetGroup( aCatalogue * cat )
{
	if ( !cat ) return setGroup( 0 );
	else return setGroup(cat->getGroup());
}

qulonglong
aCatalogue::getOwner()
{
	aSQLTable * t = table();
	if ( !t ) return 0;
	if ( !selected() ) return 0;
	return t->sysValue("ido").toULongLong();
}

ERR_Code
aCatalogue::setOwner( qulonglong ido )
{
	aSQLTable * t = table();
	if ( !t ) return err_notable;
	if ( !selected() ) return err_notselected;;
	t->setSysValue( "ido", ido );
	t->primeUpdate();
	t->update();
	return err_noerror;
}

ERR_Code
aCatalogue::SetOwner( aCatalogue * cat )
{
	if ( !cat ) return setOwner( 0 );
	else return setOwner(cat->getUid());
}

ERR_Code
aCatalogue::New( bool child )
{
	qulonglong group = getGroup(), parent = getUid();
	ERR_Code err = aObject::New();
	if ( err ) return err;
	aSQLTable * t = table();
	t->setSysValue( "idg", group );
	if ( child ) t->setSysValue( "ido", parent );
	t->primeUpdate();
	t->update();
	if ( group ) groupSelect();
	setSelected(true);
	return err_noerror;
}


ERR_Code
aCatalogue::New( )
{
	return New(false);
}


ERR_Code
aCatalogue::Delete()
{
	aSQLTable * t = table();
	if ( !t ) return err_notable;
	if ( !selected() ) return err_notselected;
	qulonglong ido = t->sysValue("id").toULongLong();
	if ( ido )
	{
		aLog::print(aLog::Debug, tr("aCatalogue delete ido=%1").arg(ido));
		aCatalogue cat( obj, db );
		while ( !cat.selectByOwner(ido) )
		{
			cat.Delete();
		}
	}
	return aObject::Delete();
}

ERR_Code
aCatalogue::Update()
{
	ERR_Code err = aObject::Update();
	if ( !err )
	{

		aLog::print(aLog::Debug, tr("aCatalogue update"));
		err = TableUpdate( md_group );
		if(err)
		{
			aLog::print(aLog::Error, tr("aCatalogue update groups error %1").arg(err));
		}
		else
		{
			aLog::print(aLog::Debug, tr("aCatalogue update groups"));
		}
	}
	else
	{
		aLog::print(aLog::Error, tr("aCatalogue update elements error %1").arg(err));
	}
	return err;
}

ERR_Code
aCatalogue::Copy()
{
	return aObject::Copy();
}

ERR_Code
aCatalogue::Select( bool grouponly )
{
	aSQLTable * t = table();
	if ( !t ) return err_notable;
//	setSelected(true,md_group);
//	setSelected(true);
	qulonglong idg = getGroup();
	QString flt = "";
//	groupSelect();
	if ( grouponly ) flt = QString("idg=%1").arg( idg );
	if ( t->select( flt ) )
		if ( t->first() )
		{
			setSelected(true);
			qulonglong newidg = t->sysValue("idg").toULongLong();

			aLog::print(aLog::Debug, tr("aCatalogue select group id=%1").arg(idg));
			if ( newidg != idg ) groupSelect( newidg );
			return err_noerror;
		}
		else return err_notselected;
	else return err_selecterror;
}

/*!
 *\~english
 *	Select group.
 *	Get group id from current element and select group table
 *	with this id.
 *\~russian
 *\~
 * \return \~english Error code \~russian \~
 */
int
aCatalogue::GroupSelect()
{
	aSQLTable * t = table();
	if(!t) return err_notable;
	if ( !selected() ) return err_notselected;;
	qulonglong idg = t->sysValue("idg").toULongLong();
	return groupSelect(idg);
}
void
aCatalogue::UnSelect( bool grouponly )
{
	if (!grouponly )
	{
		setSelected(false);
	}
	setSelected(false, md_group);
}
ERR_Code
aCatalogue::groupSelect ( qulonglong idg )
{
	if ( !idg ) return err_noerror;
	aSQLTable * t = table( md_group );
	if ( !t ) return err_notable;
	setSelected( false, md_group );
	if ( t->select( idg ) )
		if ( t->first() )
		{
			setSelected( true, md_group );
			return err_noerror;
		}
		else return err_notselected;
	else return err_selecterror;
}


ERR_Code
aCatalogue::groupSelect ()
{
	aSQLTable * t = table(md_group);
	if ( !t ) return err_notable;
	qulonglong idg = t->sysValue("id").toULongLong();
	return groupSelect( idg );
}

ERR_Code
aCatalogue::selectByOwner ( qulonglong ido )
{
	aSQLTable * t = table();

	if ( !t ) return err_notable;
	if ( t->select(QString("ido=%1").arg(ido),false) )
		if ( t->first() )
		{
			setSelected(true);
			return err_noerror;
		}
		else return err_notselected;
	else return err_selecterror;
}

ERR_Code
aCatalogue::selectByGroup ( qulonglong idg )
{
	aSQLTable * t = table();
	if ( !t ) return err_notable;
	setSelected(false);
	if ( t->select(QString("idg=%1").arg(idg),false ) )
		if ( t->first() )
		{
			setSelected(true);
			return err_noerror;
		}
		else return err_notselected;
	else return err_selecterror;
}

ERR_Code
aCatalogue::groupByParent(qulonglong idp )
{
	aSQLTable * t = table( md_group );
	if ( !t ) return err_notable;
//f ( !selected(md_group) ) return err_notselected;
	setSelected( false, md_group );
	if ( t->select(QString("idp=%1").arg(idp),false) )
		if ( t->first() )
		{
			setSelected(true,md_group);
			return err_noerror;
		}
		else
		{
			return err_notselected;
		}
	else return err_selecterror;
}



/*!\en
 *	Select groups depth level.
 *	\param level (in) - level
\_en \ru
 *
  *\_ru
*/
int
aCatalogue::selectByLevel(int level )
{
	aSQLTable * t = table(md_group);
	if ( !t ) return err_notable;
//	if ( !selected(md_group) ) return err_notselected;
	setSelected( false, md_group );
	if ( t->select(QString("level=%1").arg(level),false) )
		if ( t->first() )
		{

			setSelected(true, md_group);
			return err_noerror;
		}
		else
		{
			return err_notselected;
		}
	else
	{
		return err_selecterror;
	}

}



/*!\en
 * Gets id of group on the id of element
 * \param ide (in) - id of element.
 * \return id of group.
 \_en \ru
 * Получает идентификационный номер группы по номеру элемента
 * \param ide (in) - идентификационный номер элемента.
 * \return идентификационный номер группы.
 *\_ru
 */
qulonglong
aCatalogue::idGroupByElement(qulonglong ide )
{
	aSQLTable * t = table();
	if ( !t ) return 0;
	t->select(QString("id=%1").arg(ide), false);
	if ( t->first() )
	{
		setSelected(true);
		return sysValue("idg").toLongLong();
	}
	else return 0;
}

ERR_Code
aCatalogue::GroupNew( bool reparent )
{
	aSQLTable * te = table(), *tg = table( md_group );
	if ( !te || !tg ) return err_notable;
//	groupSelect();
//	setSelected(true, md_group);
	qulonglong idp = getGroup(), level = tg->sysValue("level").toULongLong(),
			idg = tg->primeInsert()->value("id").toULongLong();
	if ( tg->insert() )
	{
		if ( idp ) level++;
		tg->select( idg );
		if ( !tg->first() ) return err_selecterror;
		tg->selected = true;
		aLog::print(aLog::Info,tr("aCatalogue  new group with id=%1").arg(idg) );
		tg->setSysValue("idp",idp);
		tg->setSysValue("level",level);
		if ( reparent ) te->setSysValue("idg",idg);
	}
	return Update();
}



/*!\en
 * Inserts new group in groups table
 * \param parentId (in) - id parent group, or 0 if no parent.
 \_en \ru
 * Вставляет новую группу в таблицу групп
 * \param parentId (in) - идентификационный номер группы предка или 0 если нет предка.
   *\_ru
*/
ERR_Code
aCatalogue::newGroup(qulonglong parentId )
{
	aSQLTable *tg = table( md_group );
	if (!tg ) return err_notable;
	setSelected(true, md_group);
	tg->select(parentId);
	setSelected(true,md_group);
	qulonglong level, idg;
	if(tg->first())
	{
		level = tg->sysValue("level").toULongLong()+1;
	}
	else level=0;
	QSqlRecord* rec = tg->primeInsert(); // get edit buffer for table groups
	idg = rec->value("id").toULongLong();
	aLog::print(aLog::Info,tr("aCatalogue  new group with id=%1").arg(idg) );
	rec->setValue("id",idg );
	rec->setValue("idp",parentId);
	rec->setValue("level",level);
	rec->setValue("df","0");
	tg->insert(); // insert record
	tg->select(QString("id=%1").arg(idg),false); // set cursor to inserted record
	tg->first();
	setSelected(true,md_group);
	//tg->update();
return groupSelect(idg);
}



/*!\en
 * Inserts new element in elements table
 * \param parentId (in) - id parent group.
 \_en \ru
 * Вставляет новый элемент в таблицу элементов
 * \param parentId (in) - идентификационный номер группы предка.
 *\_ru
 */
ERR_Code
aCatalogue::newElement(qulonglong parentId )
{
	aSQLTable *te = table();
	if (!te) return err_notable;
	QSqlRecord *rec;
	qulonglong ide;
	rec = te->primeInsert(); // get edit buffer for table elements
	ide = rec->value("id").toULongLong();
	rec->setValue("id",ide); // set defult values for all user fields = id
	//for(uint i=0; i< rec->count(); i++)
	//{
	//	rec->setValue(i,ide);
//		printf("%s\n",rec->fieldName(i).ascii());
	//}
	rec->setValue("idg",parentId);
	rec->setValue("df","0");
	rec->setNull("ido");
	te->insert(); // insert edit buffer as new line in table
	te->select(QString("id=%1").arg(ide),false); // set cursor to inserted record
	te->first();
	setSelected(true);
	return err_noerror;
}



/*!\en
 * Mark deleted element only. Don't supports link one to many.
 * \return id of deleted element.
 \_en \ru
 * Только выделяет удаляемый элемент. Не поддерживает связь один ко многим.
 * \return идентификационный номер удаляемого элемента.
 *\_ru
 */
qulonglong
aCatalogue::setMarkDeletedElement(qulonglong id_el,bool del)
{
	select(id_el);
	SetMarkDeleted(del);
	Update();
return table()->sysValue("id").toULongLong();
}
qulonglong
aCatalogue::setMarkDeletedGroup(qulonglong id_gr, bool del)
{
	groupSelect(id_gr);
	SetMarkDeleted(del, md_group);
	GroupUpdate();
return table()->sysValue("id").toULongLong();
}



/*!\en
 * Delets element only. Don't supports link one to many.
 * \return id of deleted element.
 \_en \ru
 * Только удаляет элемент. Не поддерживает связь один ко многим.
 * \return идентификационный номер удаляемого элемента.
 *\_ru
 */
qulonglong
aCatalogue::delElement()
{
	aSQLTable * t = table();
	qulonglong ide=0;
	if ( !t ) return ide;
	ide = t->sysValue("id").toULongLong();
	if ( ide )
	{
		aLog::print(aLog::Info,tr("aCatalogue delete element with id=%1").arg(ide));
		t->primeDelete();
		t->del();
		setSelected( false );
	}
	return ide;
}



/*!\en
 * Mark deleted group with child elements and groups.
 * \param idg (in) - id mark deleted group.
 * \param listDelId (in,out) - list of id mark deleted elements and groups.
 \_en \ru
 * Выделяет удаляемую группу с дочерними элементами и группами.
 * При первом вызове параметр listDelId должен быть пустой, он не обнуляется
 * автоматически при вызове этой функции.
 * Функция рекурсивно вызывает сама себя для всех дочерних подгрупп и
 * добавляет их id в список. Также туда добавляются и id элементов,
 * содержащихся в этих группах. Для изменения атрибута удаления используте функции
 * setElementMarkDeleted(id)(для элементов) и setGroupMarkDeleted(id) (для групп)
 * \param idg (in) - идентификационный номер выделенной для удаления группы.
 * \param listDelId (in,out) - список идентификационных номеров выделенных для удаления элементов и групп.
  *\_ru
 */
void
aCatalogue::getMarkDeletedList(qulonglong idg,
				Q3ValueList<qulonglong> &listDelId)
{
	Q3ValueList<qulonglong> lst;
	aSQLTable * tg = table ( md_group );
	if ( !tg ) return;
	qulonglong tmp;
	if ( idg )
	{

	//	delete elements in group;
			if(selectByGroup(idg)==err_noerror)
			{
				do
				{
					listDelId << sysValue("id").toULongLong();
				}
				while(Next());
			}
			if (groupByParent(idg)==err_noerror)
			{
				do
				{
				lst << GroupSysValue("id").toULongLong();
				}while(NextInGroupTable());

				Q3ValueList<qulonglong>::iterator it = lst.begin();
				while(it!= lst.end())
				{
					getMarkDeletedList((*it),listDelId);
					++it;
				}
			}

	}
	tg->select(QString("id=%1").arg(idg),false);
	if(tg->first())
	{
		listDelId << idg;
	}
	return;
}
bool
aCatalogue::isGroupMarkDeleted()
{
	return IsMarkDeleted(md_group);
}
bool
aCatalogue::isElementMarkDeleted()
{
	return IsMarkDeleted();
}



/*!\en
 * Delets group with child elements and groups.
 * \param idg (in) - id deleted group.
 * \param listDelId (in,out) - list of id deleted elements and groups.
 \_en \ru
 * Физически удаляет группу со всеми ее дочерними элементами и группами.
 * \param idg (in) - идентификационный номер выделенной для удаления группы.
 * \param listDelId (in,out) - список идентификационных номеров выделенных для удаления элементов и групп.
 *\_ru
 */
qulonglong
aCatalogue::delGroup(qulonglong idg, Q3ValueList<qulonglong> &listDelId)
{
	aSQLTable * tg = table ( md_group );
	if ( !tg ) return 0;
	//if ( !selected( md_group ) ) return err_notselected;
	qulonglong tmp;// idg = tg->sysValue("id").toULongLong();
	groupSelect(idg);
	if ( idg )
	{

		aLog::print(aLog::Info,tr("aCatalogue delete group with id=%1").arg(idg));
	//		printf("idg=" LLU_SPEC "\n",idg);
	//	delete elements in group;
			while(selectByGroup(idg)==err_noerror)
			{
				listDelId << delElement();
			}
			while (groupByParent(idg)==err_noerror)
			{
				delGroup(GroupSysValue("id").toULongLong(), listDelId);
				//printf(">>>idg=%lu\n",tmp);
			}
	}
	tg->select(QString("id=%1").arg(idg),false);
	if(tg->first())
	{
		tg->primeDelete();
		tg->del();
		listDelId << idg;
		setSelected( false, md_group );
	}
	return idg;
}
ERR_Code
aCatalogue::GroupDelete()
{
	aSQLTable * tg = table ( md_group );
	if ( !tg ) return err_notable;
	getGroup();
	if ( !selected( md_group ) )
	{

		aLog::print(aLog::Info,tr("aCatalogue delete without selection"));
		//debug_message(">>not seletc in delete\n");
		return err_notselected;
	}

	qulonglong idg = tg->sysValue("id").toULongLong();
	if ( idg )
	{
		//printf("idg=" LLU_SPEC "\n",idg);
		aCatalogue cat( obj,db );
		while ( !cat.selectByGroup(idg) ) cat.Delete();
		while ( !cat.groupByParent(idg) )
		{
			//printf("delete group\n");
			cat.GroupDelete();
		}
	}
	tg->primeDelete();
	tg->del();
	aLog::print(aLog::Info,tr("aCatalogue delete group with id=%1").arg(idg));
	if(tg->first())
	{
		setSelected( true, md_group );
	}
	else
	{
		setSelected( false, md_group );
	}
	return err_noerror;
}

ERR_Code
aCatalogue::GroupMarkDeleted()
{
	aSQLTable * t = table ( md_group );
	if ( !t ) return err_notable;
	if ( !selected( md_group ) ) return err_notselected;
	if ( SetMarkDeleted( true, md_group ) ) return err_noerror;
	else return err_deleteerror;
}

ERR_Code
aCatalogue::GroupUpdate()
{
	aLog::print(aLog::Debug, tr("aCatalogue update group"));
	return TableUpdate( md_group );
}

ERR_Code
aCatalogue::GroupSetGroup( aCatalogue * cat )
{
	aSQLTable * t = table( md_group );
	if ( !t ) return err_notable;
	qulonglong newidp, oldidp = t->sysValue("idp").toULongLong();
	if ( !cat ) newidp = 0;
	else newidp = cat->getGroup();
	if ( newidp == oldidp ) return err_noerror;
	t->setSysValue( "idp", newidp );
	t->primeUpdate();
	t->update();
	return err_noerror;
}

QVariant
aCatalogue::GroupValue( const QString & name )
{
	aSQLTable * t = table( md_group );
	if ( !t ) return QVariant::Invalid;
	return t->value( name );
}



/*!\en
 * Gets system field value in table of group.
 * \param name (in) - field name.
 * \return field value or QVariant::Invalid if no field.
 \_en \ru
 *\_ru
 */
QVariant
aCatalogue::GroupSysValue( const QString & name )
{
	aSQLTable * t = table( md_group );
	if ( !t ) return QVariant::Invalid;
	//if(name=="id") return t->sysValue(name).toString();
	return t->sysValue( name );//.toString();
}

ERR_Code
aCatalogue::GroupSetValue( const QString & name, const QVariant & value )
{
	aSQLTable * t = table( md_group );
	if ( !t ) return err_notable;
	if ( !selected( md_group ) ) return err_notselected;
	if ( t->setValue( name, value ) ) return err_noerror;
	else return err_fieldnotfound;

}


ERR_Code
aCatalogue::GroupSetSysValue( const QString & name, const QVariant & value )
{
	aSQLTable * t = table( md_group );
	if ( !t ) return err_notable;
	if ( !selected( md_group ) ) return err_notselected;
	 t->setSysValue( name, value);
	return err_noerror;
	//else return err_fieldnotfound;

}

QVariant
aCatalogue::GetElementValue(QVariant ide, const QString &fname)
{
	aSQLTable * t = table();
	if ( !t ) return "";
	t->select(ide.toULongLong());
	if ( !t->first() ) return "";
	else return t->value( fname );


}
bool
aCatalogue::Next()
{
	return  aObject::Next();  //return !groupSelect();
	//else return false;
}

bool
aCatalogue::Prev()
{
	return  aObject::Prev();  //return !groupSelect();
//	else return false;
}

bool
aCatalogue::First()
{
	return  aObject::First();  //return !groupSelect();
	//else return false;
}

bool
aCatalogue::Last()
{
	return aObject::Last(); //return !groupSelect();
	//else return false;
}

bool
aCatalogue::NextInGroupTable()
{
	return aObject::Next(md_group);
}

bool
aCatalogue::PrevInGroupTable()
{
	return aObject::Prev(md_group);
}

bool
aCatalogue::FirstInGroupTable()
{
	return aObject::First(md_group);
}

bool
aCatalogue::LastInGroupTable()
{
	return  aObject::Last(md_group);
}



/*!\en
 * Gets list user and calculation fields in table of elements.
 \_en \ru
  *\_ru
 */
QStringList
aCatalogue::getUserFields()
{
	QStringList l;
	aSQLTable *t = table();
	if(t) l = t->getUserFields();
	return l;
}



/*!\en
 * Gets list user and calculation fields in table of Groups.
 \_en \ru
  *\_ru
 */
QStringList
aCatalogue::getGroupUserFields()
{
	QStringList l;
	aSQLTable *t = table( md_group );
	if(t) l = t->getUserFields();
	return l;
}


aCfgItem
aCatalogue::displayStringContext()
{
       return md->find( md->find( obj, md_element ), md_string_view );
}



aCatElement::aCatElement(aCfgItem context, aDatabase * adb)
:aObject( context, adb, 0, "aElement")
{
}




aCatGroup::aCatGroup(aCfgItem context, aDatabase * adb)
:aObject( context, adb, 0, "aGroup")
{
	int err = initObject();
}




/*!
 *\ru
 *	Возвращает id родительской группы
 *\_ru
 */
qulonglong
aCatGroup::parentUid()
{
	if ( !selected() ) return 0;
	else return table()->sysValue("idp").toULongLong();
}



void
aCatGroup::setLevel( qulonglong newLevel )
{
	if ( !selected() ) return;
	aSQLTable * t = table();
	qulonglong level = t->sysValue("level").toULongLong();
	if ( level == newLevel ) return;
	aCatGroup tgr( obj, db );
	QString query;
	query = QString("UPDATE %1 SET level=%2 WHERE id=%3")\
					.arg(t->tableName).arg(newLevel).arg(getUid());
//	printf("%s\n",(const char*) query);
	db->db()->exec(query);
	if ( !tgr.SelectChild( this ));
		do
		{
			tgr.setLevel( newLevel + 1 );
		}
		while ( tgr.Next() );
}



QString
aCatGroup::trSysName( const QString & sname )
{
	if (( sname == "Level" )||( sname == QString::fromUtf8("Уровень") )) return "Level";
	return "";
}



QVariant
aCatGroup::sysValue( const QString & sname )
{
	if ( sname == "Level" )
		return table()->sysValue("level");
	return QVariant::Invalid;
}



/*!
 *\~english
 *	Init object.
 *	We can work with object only after inited.
 *	Auto call from constructor.
 *\~russian
 *\brief Инициализирует объект элементом конфигурации.
 *
 *	Мы можем работать с объектом после его инициализации.
 *	Функция вызывается из конструктора.
 *\~
 *	\return \en error code. \_en \ru
				код ошибки.\_ru
 */
ERR_Code
aCatGroup::initObject()
{
	ERR_Code err = aObject::initObject();
	if ( err ) return err;
	aCfgItem g = md->find( obj, md_group );
	return tableInsert( aDatabase::tableDbName( *md, g ), g );
}


/*!
 *\ru
 *\brief Ничего не делает. Возвращает 0.
 *
 *\return Код ошибки.
 *\_ru
 */

ERR_Code
aCatGroup::New(aCatGroup *group)
{
	int rc = 0;
	return rc;
}
/*!
 *\en
 *	Create new group in table.
 *	New group added in root group and have level 0.
 *	\return Error code.
 *\_en
 *\ru
 *	\brief Добавляет группу в справочник.
 *
 *	Группа добавляется как корневая и имеет уровень 0.
 *	\return  Код ошибки.
 *\_ru
 */
ERR_Code
aCatGroup::New()
{
	int rc = 0;
	qulonglong level = 0, idg=0, idp = 0;
	aSQLTable * t;
	aLog::print(aLog::Info, tr("aCatGroup new group"));
	int err = aObject::New();
	if(!err)
	{
		aLog::print(aLog::Info, tr( "aCatGroup new group ok"));
		setSelected(true);
	}
	else
	{
		aLog::print(aLog::Error, tr("aCatGroup new group error %1").arg(err));
	}
	return err;
}



/*!
 *\~english
 *	Select all groups.
 *	Select all groups use filter (if it's seted).
 *\~russian
 *	\brief Выбирает все группы.
 *
 *	Выбирает все группы используя фильтр (если он установлен).
 *\~
 *\return \~english Error code. \~russian Код ошибки.\~
 */
ERR_Code
aCatGroup::Select()
{
	return aObject::select("");
}



/*!
 *\~english
 *	Select all childs.
 *	Select childs ( not include ) for parent.
 *	If parent = 0 or not parent, selected all root groups.
 *\~russian
 *	Выбирает всех потомков.
 *	Выбирает всех потомков (первого уровня) для группы parent.
 *	Если параметр отсутствует или нулевой, находятся все группы 0 - уровня.
 *	Навигация по выбранным записям стандартная(Next(), Prev(), First(), Last())
 *\~
 *\param parent - \~english parent group. \~russian родительская группа.\~
 *\return \~english Error code. \~russian Код ошибки.\~
 */
ERR_Code
aCatGroup::SelectChild( aCatGroup * parent )
{
	qulonglong idp = 0;
	if ( parent ) idp = parent->getUid();
	QString query;
	query = QString("idp=%1").arg(idp);
//	printf("%s\n",(const char*)query);
	return select(query);
}



/*!
 *\~english
 *	Set parent for group.
 *	Set for current group new parent.
 *	Check for cycle parent and recalc levels.
 *\~russian
 *	Устанавливает родителя для группы.
 *	устанавливает текущей группе родителя переданного в качестве параметра.
 *	Выполняется проверка на циклическое присваивание и пересчет уровней вложенности.
 *\~
 *\param parent - \~english new parent group. \~russian новая родительская группа.\~
 *\return \~english Error code. \~russian Код ошибки.\~
 */

ERR_Code
aCatGroup::SetParent( aCatGroup * parent )
{
	aSQLTable * t = table();
	if ( !t ) return err_notable;
	qulonglong idp = 0, uid = getUid();
	if ( parent ) idp = parent->getUid();
	if ( idp == uid ) return err_cyclereparent;
	qulonglong level, tmpid = idp;
	aCatGroup tg( obj, db );
	while ( tmpid )
	{
		tg.select(tmpid);
		tmpid = tg.parentUid();
		if ( tmpid == uid ) return err_cyclereparent;
	}
	QString query;
	query = QString("UPDATE %1 SET idp=%2 WHERE id=%3").arg(t->tableName).arg(idp).arg(uid);
	level = parent->Value("Level").toULongLong();
//	printf("%s\n",(const char*)query);
	QSqlDatabase tdb = *db->db();
	tdb.exec(query);
	if ( !tdb.lastError().type() )
	{
		if (idp) setLevel(level+1);
		else setLevel(0);
		return err_noerror;
	}
	else return err_execerror;
}


aCfgItem
aCatGroup::displayStringContext()
{
       return md->find( md->find( obj, md_group ), md_string_view );
}
