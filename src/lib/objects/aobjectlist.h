/****************************************************************************
** $Id: aobjectlist.h,v 1.1 2008/11/05 21:16:29 leader Exp $
**
** Header file of the Ananas Objects List of Ananas
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

#ifndef AOBJECTLIST_H
#define AOBJECTLIST_H

#include <qdatetime.h>
#include "acfg.h"
#include "asqltable.h"


class aDatabase;
class aObject;


/*!
 * \en
 *	The parent object for hi level not visual objects lists
 *	for worcs with SQL database
 *	\_en
 *	\ru

*	\_ru
 */
class  ANANAS_EXPORT aObjectList : public QObject
{
	Q_OBJECT
//	Q_PROPERTY( aObject currentObject READ currentObject )
public:
	aCfg * md;
	aCfgItem obj;
	aDatabase * db;

	aObjectList( QObject *parent = 0, const char *name = 0 );
	aObjectList( const QString &oname, aDatabase *adb, QObject *parent = 0, const char *name = 0 );
	aObjectList( aCfgItem context, aDatabase *adb, QObject *parent = 0, const char *name = 0 );
	virtual ~aObjectList();

	ERR_Code init();
	ERR_Code decodeDocNum( QString nm, QString & pref, int & num);
	virtual ERR_Code select( qulonglong id );
	qulonglong getUid();
	bool selected();
	void setSelected( bool sel );
	ERR_Code select(const QString & query );

//	virtual Q_ULLONG docId();


public slots:
	virtual aObject *currentObject() const;

	virtual QString Uid();
	virtual bool IsSelected();
	virtual bool IsMarkDeleted();
	virtual bool IsMarked();
	virtual int SetMarkDeleted( bool Deleted );
	virtual int SetMarked( bool Marked );
	virtual QVariant Value( const QString & name );
	virtual int SetValue( const QString & name, const QVariant &value );

	virtual bool Next();
	virtual bool Prev();
	virtual bool First();
	virtual bool Last();

	virtual int SetFilter( const QString & valname, const QVariant & value );
	virtual int ClearFilter();
	virtual bool IsFiltred() {return filtred;};
	virtual QString displayString();

protected:
	virtual aCfgItem displayStringContext();
	virtual ERR_Code setObject( aCfgItem newobject );
	virtual ERR_Code initObject();
	bool isInited() { return vInited; };
	void setInited( bool flag ){ vInited = flag; };
private:
	aDataTable *table;
	bool vInited, selectFlag, filtred;
	Q3Dict <aDataTable> dbtables;
//	QDict <aDataRecord> attributes;
};


#endif
