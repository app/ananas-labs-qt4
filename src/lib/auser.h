/****************************************************************************
** $Id: auser.h,v 1.1 2008/11/05 21:16:28 leader Exp $
**
** User  object header file of
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

#ifndef AUSER_H
#define AUSER_H

#include	"arole.h"
#include 	"adatabase.h"
#include 	"acfg.h"
#include 	"aobject.h"
#include 	<q3valuelist.h>
//class aDatabase;
//class aRole;
//class aObject;



/*!
 * \en
 *	Not visual object used for working with users in database.
 * \_en
 * \ru
 *	\brief Определяет программный интерфейс для управления пользователями. Не используется в настоящий момент.
 *	Наследует aObject.
 * \_ru
*/
class  ANANAS_EXPORT aUser: public aObject
{
	Q_OBJECT
public:
	aUser();
	aUser(qulonglong userId, aDatabase * adb);
	aUser(aDatabase * adb);
	virtual ERR_Code initObject();

//public slots:

	virtual ERR_Code New(	const QString &login,
				const QString &passwd="",
				const QString &Name="",
				const QString &LastName="");

	virtual ERR_Code Delete();
	virtual ERR_Code Update();
	virtual ERR_Code Select();
	virtual ERR_Code Select( qulonglong userId );

	virtual ERR_Code addRole( qulonglong roleId );
	virtual ERR_Code delRole( qulonglong roleId );
	virtual ERR_Code UpdateRole();

	Q3ValueList< aRole *> getRoles( bool assigned);

	bool hasRole( qulonglong roleId);
//	virtual ERR_Code setPassword( const QString & );
	//virtual ERR_Code setLogin( const QString & );
	//virtual ERR_Code setFirstName( const QString & );
	//virtual ERR_Code setLastName( const QString & );

	virtual qulonglong getUserId( const QString &login, const QString &password );
	//virtual ERR_Code getRoles(QValueList<aRole> *lst);
	bool First();
	bool Next();
	bool Last();
	bool Prev();
};


#endif// AUSER_H
