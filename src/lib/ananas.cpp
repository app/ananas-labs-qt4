/****************************************************************************
** $Id: ananas.cpp,v 1.2 2008/11/09 21:09:11 leader Exp $
**
** Code file of the Ananas Library of Ananas
** Designer and Engine applications
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

#include "ananas.h"
#include "dselectdb.h"
#include "dlogin.h"



/*!
 * Library version number.
 */
const char *
ANANAS_EXPORT
ananas_libversion()
{
	static const char libver[]=ANANAS_LIBVERSION;
	return libver;
}


/*!
 * Select database and user login.
 */
bool
ANANAS_EXPORT
ananas_login( QString &rcfile, QString &username, QString &userpassword, aDatabase *db, int appId )
{
    dSelectDB dselectdb;
    dLogin dlogin;
        
    if ( rcfile.isEmpty() ) {
        if (dselectdb.exec()==QDialog::Accepted) rcfile = dselectdb.rcfile;
    }
    if ( !rcfile.isEmpty() ) {
    	if (dlogin.exec()==QDialog::Accepted) {
	    username = dlogin.username;
	    userpassword = dlogin.password;
            if ( !db ) db = aDatabase::database();
	    if ( !db->init( rcfile ) ) return false;
	    return ( db->login( username, userpassword, appId ) );
	} return false;
    }
    return false;
}


/*!
 * Select database and user login.
 */
void
ANANAS_EXPORT
ananas_logout( aDatabase *db )
{
    if ( !db ) db = aDatabase::database();
    db->logout();
}


/*!
 * Select database and user login.
 */
/*
bool
ANANAS_EXPORT
ananas_login( QString &rcfile, QString &username, QString &userpassword )
{
    dSelectDB dselectdb;
    //dLogin dlogin;

    if ( rcfile.isEmpty() ) {
        if (dselectdb.exec()==QDialog::Accepted) rcfile = dselectdb.rcfile;
    }
    if ( !rcfile.isEmpty() ) {
//    	if (dlogin.exec()==QDialog::Accepted) {
//	    username = dlogin.username;
//	    userpassword = dlogin.password;
	    return true;
//	} return false;
    }
    return false;
   // return true;
}
*/


/*!
 * \en
 * 	\brief Return object string presentation.
 * \_en
 * \ru
 * 	\brief Возвращает текстовое представление бизнес объекта Ананаса
 *
 * 	\param db - база данных бизнес объекта
 * 	\param uid - уникальный идентификатор бизнес объекта
 * 	\param oid - уникальный идентификатор типа данных, соответствующих бизнес объекту, как определено в метаданных.
 * \_ru
 */
QString
ANANAS_EXPORT
ananas_objectstr( aDatabase *db, qulonglong uid, int oid )
{
	QString res = "";

	//TODO: make this more faster
	aCfgItem fto;
	QString oclass;

//	printf("objstr uid = %Ld\n", uid );
	if ( !db ) return "<>";
	if ( !oid ) oid = db->uidType( oid );
	fto = db->cfg.find( oid );
	if ( !fto.isNull() ) {
		oclass = db->cfg.objClass( fto );
//		printf("oid = %i, oclass = %s\n", oid, ( const char *) oclass );
		if ( oclass == md_catalogue ) {
			aCatalogue cat( fto, db );
			cat.select( uid );
			if ( cat.selected() ) {
				res = cat.displayString();
		}
		}else if ( oclass == md_document ) {
			aDocument doc(fto, db);
			doc.select( uid );
			if( doc.selected() ){
				res = doc.displayString();
			}
		}
	}

	return res;
}


