/****************************************************************************
** $Id: arole.h,v 1.1 2008/11/05 21:16:28 leader Exp $
**
** Role object header file of
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

#ifndef AROLE_H
#define AROLE_H

#include 	"aobject.h"
#include 	"adatabase.h"
#include 	"acfg.h"
//#include 	"auser.h"

//class aUser;
//class aPermission;
//class aObject;



/*!
 *	\en
 *	Not visual object used for working with catalogues in database.
 *	Used as interface between database and script or wObjects*
 *	\_en
 * \ru
 *	\brief Класс предоставляет методы управления ролями. Не используется в настоящее время.
 *	Наследует aObject.
 *
 * \_ru
*/
class  ANANAS_EXPORT aRole: public aObject
{
	Q_OBJECT
public:

	aRole();
	aRole(qulonglong roleId, aDatabase * adb);
	aRole(aDatabase * adb);
	virtual ERR_Code initObject();

//public slots:

	virtual ERR_Code New(const QString &name);

	virtual ERR_Code Delete();
	virtual ERR_Code Update();
	virtual ERR_Code Select();
	virtual ERR_Code Select(qulonglong id);

	virtual ERR_Code addPermission( aPermission );
	virtual ERR_Code delPermission( aPermission );
	virtual bool hasPermission( aPermission );

	virtual ERR_Code addUser( qulonglong userId );
	virtual ERR_Code delUser( qulonglong userId );

//	QValueList< aUser *> getUsers( bool assigned);

	bool hasUser( qulonglong userId);

	virtual ERR_Code SetName( const QString & );
	virtual QString GetName();

	//virtual ERR_Code setLogin( const QString & );
	//virtual ERR_Code setFirstName( const QString & );
	//virtual ERR_Code setLastName( const QString & );

//irtual Q_ULLONG getUserId( const QString &login, const QString &password );
//irtual ERR_Code getRoles(QValueList<aRole> *lst);
	bool First();
	bool Next();
	bool Last();
	bool Prev();
};


#endif// AROLE_H
