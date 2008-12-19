/****************************************************************************
** $Id: auser.cpp,v 1.1 2008/11/05 21:16:28 leader Exp $
**
** User object implementation file of
** Ananas application library
**
** Created : 20050527
**
** Copyright (C) 2003-2005 Grigory Panov, Yoshkar-Ola.
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

#include	"auser.h"
//Added by qt3to4:
#include <Q3ValueList>
//#include 	"ananas.h"


/*!\en Constructor for concrete object class creating
 * \_en
 * \ru Конструктор для создания конкретного объекта класса
 * \_ru
 */
aUser::aUser( qulonglong userId, aDatabase * adb ):aObject("",adb)
{

	initObject();
	Select(userId);

}
/*!\en Constructor for abstract object class creating
 * \_en
 * \ru Конструктор для создания абстрактного объекта класса
 * \_ru
 */
aUser::aUser( aDatabase * adb ):aObject("",adb)
{

//	Select(userId);
	initObject();
}




/*!\en Reimplement base class function
 * \_en
 * \ru Переопределяет функцию базового класса
 * \_ru
 */
ERR_Code
aUser::initObject()
{
	ERR_Code err = aObject::initObject(); // don't handle errors
	//if ( err ) return err;
	//aCfgItem e;// = md->find( obj, md_group ), e = md->find( obj, md_element );
	err = tableInsert( db_users, "");

	if(err) return err;
	err = tableInsert( db_user_roles, md_user_roles );
	return err;
}

/*!\en Returns list of roles, (un)assigned for this user. List elements must be deleted in called procedure.
 * \_en
 * \ru Возвращает список ролей, (не)назначенных данному пользователю. Элементы списка должны быть удалены в вызывающей процедуре
 * \_ru
 * \param assibned 	\en true for return assigned roles, false for retun unassigned roles\_en
 * 			\ru true для возврата назначенных ролей, false для возврата неназначенных ролей \_ru
 */
Q3ValueList< aRole *>
aUser::getRoles(bool assigned)
{
	Q3ValueList<aRole*> list;
	qulonglong rid;
	aRole *rl = new aRole(db);
	rl->Select();
	if(rl->First())
	{
		do
		{
			rid = rl->sysValue("id").toULongLong();
			if(!(hasRole(rid) ^ assigned))// !XOR
			{
				aRole *r = new aRole(rid,db);
				list.insert(list.end(),r);
			}
		}while(rl->Next());
	}
	delete rl;
	return list;
}

/*!\en Gets user id
 * \_en
 * \ru Получение id пользователя
 * \_ru
 * \param login \en user login \_en \ru логин пользователя\_ru
 * \param password \en user password \_en \ru пассворд пользователя\_ru
 * \return 	\en 0 if login and password has no match, user id otherwise \_en
 * 		\ru 0 если логин и пароль не совпадают, в противном случае id пользователя\_ru
 */
qulonglong
aUser::getUserId(const QString &login, const QString &password)
{
	aSQLTable * t = table();
	t->select(QString("login='%1' AND password='%2'").arg(login).arg(password));
	if(t->first())
	{
		return t->sysValue( "id").toULongLong();
	}
	else
	{
		return 0;
	}

}



/*!\en Updates table values
 * \_en
 * \ru Обновляет значения таблиц
 * \_ru
 */
ERR_Code
aUser::Update()
{
	ERR_Code err = aObject::Update();
	if ( !err ){
		err = TableUpdate( md_user_roles );
	}
	return err;
}


/*!\en Selected table and clear filter
 * \_en
 * \ru Выбирает таблицу и сбрасывает фильтр
 * \_ru
 */
ERR_Code
aUser::Select( )
{
	aSQLTable * t = table();
	if ( !t ) return err_notable;
//	setSelected(true,md_group);
//	setSelected(true);
	QString flt = "";
//	groupSelect();
	if ( t->select( flt ) )
		if ( t->first() )
		{
			setSelected(true);
		}
		else return err_notselected;
	else return err_selecterror;
	return err_noerror;
}

/*!\en Select table and set filter to user with given id
 * \_en
 * \ru Выбирает таблицу и устанавлиет фильтр на пользователя с переданным в параметре id
 * \_ru
 * \param id -	\en user id \_en \ru id пользователя \_ru
 */
ERR_Code
aUser::Select( qulonglong id)
{
	aSQLTable * t = table();
	if ( !t ) return err_notable;
//	setSelected(true,md_group);
//	setSelected(true);
	QString flt;
	flt = QString("id=%1").arg(id);
//	groupSelect();
	if ( t->select( flt ) )
		if ( t->first() )
		{
			setSelected(true);
		}
		else return err_notselected;
	else return err_selecterror;
	return err_noerror;
}

/*!\en Assign role to user
 * \_en
 * \ru Назначет пользователю роль
 * \_ru
 * \param roleId - 	\en id role to asssign \_en \ru id роли для назначения\_ru
 */
ERR_Code
aUser::addRole( qulonglong roleId )
{
	aSQLTable *tg = table( md_user_roles );
	if (!tg ) return err_notable;
	setSelected(true, md_user_roles);
//	tg->select(parentId);
//	setSelected(true,md_group);
	qulonglong idg;
	QSqlRecord* rec = tg->primeInsert(); // get edit buffer for table groups
	idg = sysValue("id").toULongLong();
	//printf( "idg=%lu\n", idg );
	rec->setValue("id",idg );
	rec->setValue("idr",roleId );
	tg->insert(); // insert record
	//tg->update();
return  err_noerror;
}

/*!\en Drop role
 * \_en
 * \ru Сбрасывает назначение роли пользователю
 * \_ru
 * \param roleId - 	\en id role to drop \_en
 * 			\ru id роли, назначение которой будет сброшено\_ru
 */
ERR_Code
aUser::delRole( qulonglong roleId )
{
	aSQLTable *t = table( md_user_roles );
	qulonglong ide=0;
	if ( !t ) return err_notable;
	ide = sysValue("id").toULongLong();
	t->select(QString("id=%1 and idr=%2").arg(ide).arg(roleId));
	if ( t->first() )
	{
		//printf("del usr_role \n");
		t->primeDelete();
		t->del();
//		setSelected( false );
	}
	else return err_notselected;
	return err_noerror;
return  err_noerror;
}

/*!\en Checks role assign
 * \_en
 * \ru Проверяет, назначена ли роль данному пользователю
 * \_ru
 * \param rid - 	\en role id to check \_en
 * 			\ru id роль для проверки \_ru
 * \return 	\en true, if role assigned, false otherwise \_en
 * 		\ru true, если роль назначена, false в противном случае \_ru
 */
bool
aUser::hasRole(qulonglong rid)
{
	aSQLTable *tg = table( md_user_roles );
	if (!tg ) return err_notable;
	tg->select(QString("id=%1 AND idr=%2").arg(sysValue("id").toULongLong()).arg(rid));
	if(tg->first())
	return true;
	else return false;
//	setSelected(true, md_user_roles);


}

/*!\en
 * Inserts new element in elements table
 \_en \ru
 * Вставляет новый элемент в таблицу элементов
 *\_ru
 * \param login - 	\en login \_en \ru логин \_ru
 * \param password -	\en password \_en \ru пароль \_ru
 * \param firstName -	\en first name \_en \ru имя пользователя \_ru
 * \param lastName -	\en last name \_en \ru фамилия пользователя \_ru
 */
ERR_Code
aUser::New(const QString &login,
		const QString &password,
		const QString &firstName,
		const QString &lastName)
{
	aSQLTable *te = table();
	if (!te) return err_notable;
	QSqlRecord *rec;
	qulonglong ide;
	rec = te->primeInsert(); // get edit buffer for table elements
	ide = rec->value("id").toULongLong();
//	printf(">>>id=%lu\n",ide);
	rec->setValue("id",ide); // set defult values for all user fields = id
	rec->setValue( "login", login );
	rec->setValue( "password", password );
	rec->setValue( "fname", firstName );
	rec->setValue( "lname", lastName );
	te->insert(); // insert edit buffer as new line in table
	te->select(QString("id=%1").arg(ide),false); // set cursor to inserted record
	te->first();
	setSelected(true);
	return err_noerror;
}





/*!\en
 * Delets user.
 \_en \ru
 * удаляет пользователя.
 *\_ru
 */
ERR_Code
aUser::Delete()
{
	aSQLTable * t = table();
	qulonglong ide=0;
	if ( !t ) return err_notable;
	ide = t->sysValue("id").toULongLong();
	if ( ide )
	{
	//	printf("ide=" LLU_SPEC "\n",ide);
		t->primeDelete();
		t->del();
//TODO: clear roles, assigned to user
		setSelected( false );
	}
	else return err_notselected;
	return err_noerror;
}


/*!\en Updates roles table values
 *\_en
 * \ru Обновляет значение таблицы ролей
 *\_ru
 */
ERR_Code
aUser::UpdateRole()
{
	return TableUpdate( md_user_roles );
}


/*!\en Next
 *\_en
 * \ru
 *\_ru
 */
bool
aUser::Next()
{
	return  aObject::Next(  );  //return !groupSelect();
	//else return false;
}

/*!\en Rpev
 *\_en \ru
 *\_ru
 */
bool
aUser::Prev()
{
	return  aObject::Prev(  );  //return !groupSelect();
//	else return false;
}

/*!\en First
 *\_en \ru
 *\_ru
 */
bool
aUser::First()
{
	return  aObject::First( );  //return !groupSelect();
	//else return false;
}

/*!\en Last
 *\_en \ru
 *\_ru
 */
bool
aUser::Last()
{
	return aObject::Last(  ); //return !groupSelect();
	//else return false;
}



