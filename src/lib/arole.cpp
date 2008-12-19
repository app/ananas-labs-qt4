/****************************************************************************
** $Id: arole.cpp,v 1.1 2008/11/05 21:16:28 leader Exp $
**
** Role object implementation file of
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

#include	"arole.h"
#include 	"auser.h"
/*!
 *	\en
 *	Constructor.
 *	\_en
 *	\ru
 *	Конструктор. Создает экземпляр роли и настраивает его на работу с заданным id.
 *	\param roleId - идентификатор роли.
 *	\param adb - ссылка на базу данных.
 *	\_ru
 */
aRole::aRole( qulonglong roleId, aDatabase * adb ):aObject("",adb)
{

	initObject();
	Select(roleId);
}


/*!
 *	\en
 *	Constructor.
 *	\_en
 *	\ru
 *	Конструктор. Создает экземпляр роли. Для работы необходимо выбрать нужную роль с помощью функции Select( Q_ULLONG id ).
 *	\param adb - ссылка на базу данных.
 *	\_ru
 */
aRole::aRole( aDatabase * adb ):aObject("",adb)
{

	initObject();
}




ERR_Code
aRole::initObject()
{
	ERR_Code err = aObject::initObject();
	err = tableInsert( db_roles,"");
	if(err) return err;
	err = tableInsert( db_right_roles, md_right_roles );
	return err;
}



/*!
 *	\en
 *	Sets role name.
 *	\_en
 *	\ru
 *	Устанавливает имя роли.
 *	\_ru
 *	\param rname - \en role name \_en \ru имя роли \_ru
 *	\return \en error code \_en \ru код ошибки \_ru
*/
ERR_Code
aRole::SetName( const QString &rname )
{
	setSysValue( "name", rname );
	return err_noerror;
}


/*!
 *	\en
 *	Gets role name.
 *	\_en
 *	\ru
 *	Функция получения имени роли.
 *	\_ru
 *	\return \en role name \_en \ru имя роли \_ru
*/
QString
aRole::GetName( )
{

	return sysValue( "name" ).toString();
}


ERR_Code
aRole::Update()
{
	ERR_Code err = aObject::Update();
	if ( !err ){
		err = TableUpdate( md_right_roles );
	}
	return err;
}



/*!
 *	\en
 *	Sets primary table selected.
 *	\_en
 *	\ru
 *	Устанавливает основную таблицу выбранной.
 *	\_ru
 *	\return \en error code \_en \ru код ошибки \_ru
*/
ERR_Code
aRole::Select(  )
{
	aSQLTable * t = table();
	if ( !t ) return err_notable;
	QString flt = "";
	if ( t->select( flt ) )
		if ( t->first() )
		{
			setSelected(true);
		}
		else return err_notselected;
	else return err_selecterror;
	return err_noerror;
}


/*!
 *	\en
 *	Select role.
 *	\_en
 *	\ru
 *	Выбирает роль с заданным идентификатором.
 *	\_ru
 *	\param id - \en role id \_en \ru id роли \_ru
 *	\return \en error code \_en \ru код ошибки \_ru
*/
ERR_Code
aRole::Select( qulonglong id )
{
	aSQLTable * t = table();
	if ( !t ) return err_notable;
	QString flt;
	flt = QString("id=%1").arg(id);
	if ( t->select( flt ) )
		if ( t->first() )
		{
			setSelected(true);
		}
		else return err_notselected;
	else return err_selecterror;
	return err_noerror;
}


/*!
 *	\en
 *	Adds permission to role
 *	\_en
 *	\ru
 *	Добавляет к роли право \a p
 *	\_ru
 *	\param p - \en permission \_en \ru право \_ru
 *	\return \en error code \_en \ru код ошибки \_ru
*/
ERR_Code
aRole::addPermission( aPermission p )
{
	aSQLTable *tg = table( md_right_roles );
	if ( !tg ) return err_notable;
	setSelected(true, md_right_roles);
	qulonglong idg;
	QSqlRecord* rec = tg->primeInsert(); // get edit buffer for table groups
	idg = rec->value("id").toULongLong();
	rec->setValue("id",idg );
	rec->setValue("permission",p);
	tg->insert(); // insert record
	tg->update();
return  err_noerror;
}


/*!
 *	\en
 *	Removs permission from role
 *	\_en
 *	\ru
 *	Убирает у роли право \a p
 *	\_ru
 *	\param p - \en permission \_en \ru право \_ru
 *	\return \en error code \_en \ru код ошибки \_ru
*/
ERR_Code
aRole::delPermission( aPermission p )
{
/*	aSQLTable *tg = table( md_user_roles );
	if (!tg ) return err_notable;
	setSelected(true, md_right_roles);
//	tg->select(parentId);
//	setSelected(true,md_group);
	Q_ULLONG idg;
	QSqlRecord* rec = tg->primeInsert(); // get edit buffer for table groups
	idg = rec->value("id").toULongLong();
	//printf( "idg=%lu\n", idg );
	rec->setValue("id",idg );
	rec->setValue("permission",p);
	tg->insert(); // insert record
	tg->update();*/
return  err_noerror;
}


/*!
 *	\en
 *	Check permission \a p.
 *	\_en
 *	\ru
 *	Проверяет, есть ли у роли право \a p
 *	\_ru
 *	\param p - \en permission \_en \ru право \_ru
 *	\return \en true, if role has perission \_en \ru true, если у роли есть право \_ru
*/
bool
aRole::hasPermission( aPermission p )
{
	aSQLTable *tg = table( md_right_roles );
	if (!tg ) return err_notable;
	tg->select(QString("idr=%1").arg(sysValue("id").toULongLong()));
	if( tg->first() )
	{
		return true;
	}
	else
	{
		return false;
	}

}



/*!
 *	\en
 *	Adds user to role
 *	\_en
 *	\ru
 *	Добавляет к пользователю данную роль.
 *	\_ru
 *	\param userId - \en user id \_en \ru идентификатор пользовыателя \_ru
 *	\return \en error code \_en \ru код ошибки \_ru
*/

ERR_Code
aRole::addUser( qulonglong userId )
{
	aUser *user = new aUser(userId, db);
	user->addRole(sysValue("id").toULongLong());
	delete user;
return  err_noerror;
}


/*!
 *	\en
 *	Delete user from role
 *	\_en
 *	\ru
 *	Удаляет у пользователя данную роль
 *	\_ru
 *	\param userId - \en user id \_en \ru идентификатор пользовыателя \_ru
 *	\return \en error code \_en \ru код ошибки \_ru
*/
ERR_Code
aRole::delUser( qulonglong userId )
{
	aUser *user = new aUser(userId, db);
	user->delRole(sysValue("id").toULongLong());
	delete user;
return  err_noerror;
}


/*!
 *	\en
 *	Check user have role.
 *	\_en
 *	\ru
 *	Проверяет, принадлежит ли пользователь к данной роли.
 *	\_ru
 *	\param userId - \en user id \_en \ru id пользователя \_ru
 *	\return \en true, if role has user \_en \ru true, если у пользователя есть эта роль\_ru
*/
bool
aRole::hasUser(qulonglong userId)
{
	aUser *user = new aUser(userId, db);
	bool res;
	if(user->hasRole(sysValue("id").toULongLong()))
	{
		res = true;
	}
	else
	{
		res =false;
	}
	delete user;
	return res;
}


/*!
 *	\en
 *	Adds new pole.
 *	\_en
 *	\ru
 *	Добавляет новую роль.
 *	\_ru
 */
ERR_Code
aRole::New(const QString &rname)
{
	aSQLTable *te = table();
	if (!te) return err_notable;
	QSqlRecord *rec;
	qulonglong ide;
	rec = te->primeInsert(); // get edit buffer for table elements
	ide = rec->value("id").toULongLong();
	rec->setValue("id",ide); // set defult values for all user fields = id
	rec->setValue( "name", rname );
	te->insert(); // insert edit buffer as new line in table
	te->select(QString("id=%1").arg(ide),false); // set cursor to inserted record
	te->first();
	setSelected(true);
	return err_noerror;
}





/*!
 *	\en
 *	Delete this role.
 *	\_en
 *	\ru
 *	Удаляет эту роль. Не удаляет роль у пользователей.
 *	\_ru
 */
ERR_Code
aRole::Delete()
{
	aSQLTable * t = table();
	qulonglong ide=0;
	if ( !t ) return err_notable;
	ide = t->sysValue("id").toULongLong();
	if ( ide )
	{
		printf("ide=" LLU_SPEC "\n",ide);
		t->primeDelete();
		t->del();
		setSelected( false );
	}
	else return err_notselected;
	return err_noerror;
}



bool
aRole::Next()
{
	return  aObject::Next( );  //return !groupSelect();
	//else return false;
}

bool
aRole::Prev()
{
	return  aObject::Prev( );  //return !groupSelect();
//	else return false;
}

bool
aRole::First()
{
	return  aObject::First( );  //return !groupSelect();
	//else return false;
}

bool
aRole::Last()
{
	return aObject::Last( ); //return !groupSelect();
	//else return false;
}



