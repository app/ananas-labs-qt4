/****************************************************************************
** $Id: acontainer.h,v 1.1 2008/11/05 21:16:28 leader Exp $
**
** Data container functions header file of
** Ananas application library
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) 2003-2006 Grigory Panov, Yoshkar-Ola.
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

#ifndef ACONTAINER_H
#define ACONTAINER_H

#include "qdom.h"
#include "qstringlist.h"
#include "ananas.h"
#include "acmanifest.h"
#include <qobject.h>


//class aDatabase;

/**
 * \en
 * 	Class for work with data container
 * \_en
 * \ru
 * \brief	Класс предназначен для работы с контейнерами данных.
 * 	Наследует QObject.
 *
 * \_ru
 */
class ANANAS_EXPORT aContainer: public QObject
{
    Q_OBJECT
public:

	aContainer();
	aContainer(const QString& name);
	virtual ~aContainer();

	virtual bool open();
	virtual bool open(const QString &name);
	virtual void create();
	virtual void close();

	virtual bool save();
	virtual bool save(const QString& name);
	virtual bool addFile(const QString& filename, const QString &newname, int type);
	virtual QString realPath(const QString &fname) const;

	virtual QString lastError() const;
	void	setLastError(const QString& errorText);

//signals:
	//void	progress(int,int);

protected:
	void generateName4TmpDir();
	bool createTmpDir();
	bool extractManifest(const QString& archName, aCManifest *mf);
	bool extractData(const QString& archName);
	bool compressFile(const QString& fileName);
	void cleanupTmpFiles();

private:
	QString txtError;
	aCManifest *manifest;
	QString tmpDirName;

};

#endif //ACONTAINER_H
