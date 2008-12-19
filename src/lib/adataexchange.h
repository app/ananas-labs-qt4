/****************************************************************************
** $Id: adataexchange.h,v 1.1 2008/11/05 21:16:28 leader Exp $
**
** Import/export data function header file of
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

#ifndef ADATEXCHANGE_H
#define ADATEXCHANGE_H

#include "qdom.h"
#include "qstringlist.h"
#include "ananas.h"
#include <qobject.h>


class aDatabase;

/**
 * \en
 * 	Class for work make backup and restore databases
 * \_en
 * \ru
 * \brief	Класс aBackup определяет программный интерфейс для бэкапа и востановления базы данных.
 * 	Наследует QObject.
 *
 * \_ru
 */
class ANANAS_EXPORT aBackup: public QObject
{
    Q_OBJECT
public:

	aBackup();
	virtual ~aBackup();

	virtual bool importData(const QString& rcfile, const QString &archfile, bool dropDB, bool replaceTemplates=0);
	virtual bool exportData(const QString& rcfile, const QString &archfile, bool withTemplates=0 );
	virtual QString lastError() const;

signals:
	void	progress(int,int);

protected:
	virtual void		setLastError(const QString & errorText);
	virtual bool		unzipArchive(const QString& archName, const QString& tmpDirName);
	virtual bool		zipArchive(const QString& archName, const QString& tmpDirName);
	virtual bool		checkStructure();
	virtual QDomDocument	createManifest(const QStringList& templates);
	virtual void		changeRC(const QString& nameRC, const QString& newConfigName);
	virtual bool		dumpBase(const QString& rcfile, const QString& tmpDirName, int &progress, int totalSteps);
	virtual bool 		writeXml(const QString & fileName, QDomDocument xml);
	void			cleanupTmpFiles(const QString& tmpDirName, QStringList *files);

private:
	QString txtError;
//	QString tmpDirName;

};

#endif //ADATEXCHANGE_H
