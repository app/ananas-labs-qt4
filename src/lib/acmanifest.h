/****************************************************************************
** $Id: acmanifest.h,v 1.1 2008/11/05 21:16:28 leader Exp $
**
** manifest.xml function header file of
** Ananas application library
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) 2006 Grigory Panov <gr1313 at mail.ru>, Yoshkar-Ola.
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

#ifndef ACMANIFEST_H
#define ACMANIFEST_H

#include "qdom.h"
#include "qstringlist.h"
#include "ananas.h"
#include <qobject.h>



/**
 * \en
 * 	Class for work with manifest.xml
 * \_en
 * \ru
 * \brief	Класс aCManifest предназначен для работы с файлом manifest.xml
 * 	Наследует QObject.
 *
 * \_ru
 */
#define mf_invalid 0
#define mf_metadata 1
#define mf_template 2
#define mf_dir 3
#define mf_script 4
#define mf_form 5
#define mf_picture 6
#define mf_other 100

class ANANAS_EXPORT aCManifest: public QObject
{
    Q_OBJECT
public:

	aCManifest();
	virtual ~aCManifest();
 //   	enum type{ mf_invalid, mf_metadata, mf_template, mf_dir, mf_script, mf_form, mf_picture, mf_other=100 };
	struct record
	{
		QString name;
		int type;
	};
	// place your castom types before mf_other

	virtual void create();
	virtual QString toString();
	virtual bool write(const QString& fname);
	virtual bool read(const QString& fname);
	virtual void clear();
	virtual bool isValid();

	virtual aCManifest::record first();
	virtual aCManifest::record next();

	virtual bool isExists( const QString &fname );
	virtual bool addEntry( const QString &fname, int type);
	virtual bool removeEntry( const QString &fname, bool withChilds );
	virtual QString lastError() const;

private:
	QString txtError;
	QDomDocument manifest;
	QDomElement rootNode, currentNode;

};


#endif //ACMANIFEST_H
