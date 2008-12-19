/****************************************************************************
** $Id: acontainer.cpp,v 1.2 2008/11/08 20:16:35 leader Exp $
**
** Data container implementation file of
** Ananas application
**
** Created : 20060209
**
** Copyright (C) 2006 Grigory Panov <gr1313 at mail.ru>, Yoshkar-Ola.
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

#include "alog.h"
#include "acontainer.h"
#include "acmanifest.h"
#include "aservice.h"
#include <qapplication.h>
#include <qfile.h>
#include <qdir.h>
#include <q3process.h>
#include <stdio.h>
#include <stdlib.h>


aContainer::aContainer():QObject()
{
}

aContainer::aContainer(const QString& name):QObject(0, name)
{
	manifest = new aCManifest();
}

aContainer::~aContainer()
{
	delete manifest;
	manifest = 0;
}

bool
aContainer::open()
{
	return open(name());
}

bool
aContainer::open(const QString& name)
{
	close();
	generateName4TmpDir();
	if(createTmpDir())
	{
		extractManifest(name, manifest);
		if(manifest)
		{
			if(extractData(name))
			{
				setName(name);
				aLog::print(aLog::Debug, tr("aContainer unzip"));
				return true;
			}
		}
		cleanupTmpFiles();
	}
	aLog::print(aLog::Error, tr("aContainer unzip"));
	return false;
}


void
aContainer::create()
{
	generateName4TmpDir();
	createTmpDir();
	manifest->create();
}

void
aContainer::close()
{
	cleanupTmpFiles();
}

void
aContainer::generateName4TmpDir()
{
	QString temp;
#ifndef _Windows
	temp = getenv("TMPDIR");
	if(temp=="" || temp.isEmpty())
		temp = P_tmpdir;
#else
	temp = getenv("TEMP");
#endif
	tmpDirName = QString(temp+"/%1").arg(QDateTime::currentDateTime().toTime_t());
	tmpDirName = QDir::convertSeparators(tmpDirName);

}

bool
aContainer::createTmpDir()
{
	QDir dir;
	if(!dir.mkdir(tmpDirName))
	{
		setLastError(tr("Can't create temporary directory"));
		aLog::print(aLog::Error, "aContainer create temporary directory %1");
		return false;
	}
	else
	{
		aLog::print(aLog::Debug, tr("aContainer create temporary directory %1").arg(tmpDirName));
		return true;
	}

}

bool
aContainer::save(const QString& name)
{
	setName(name);
	if(manifest)
	{
		aCManifest::record rec;
		rec = manifest->first();
		while(rec.type!=mf_invalid)
		{
			compressFile(rec.name);
			rec = manifest->next();
		}
	}
	else
	{
		setLastError(tr("No manifest found!"));
		aLog::print(aLog::Error, "aContainer save: manifest is null");
		return false;
	}
	return true;
}

bool
aContainer::save()
{
	return save(name());

}

bool
aContainer::addFile(const QString& filename, const QString &newname, int type)
{
	bool res = false;
	if(!aService::copyFile( QDir::convertSeparators(filename), QDir::convertSeparators(tmpDirName + newname), true))
	{
		aLog::print(aLog::Error, tr("aContainer error copy file"));
	}
	else
	{
		if(manifest)
		{
			manifest->addEntry(newname, type);
			res = true;
		}
		else
		{
			aLog::print(aLog::Error, tr("aContainer manifest is null"));
		}
	}
	return res;
}

QString
aContainer::realPath(const QString &fname) const
{
	return "";
}


bool
aContainer::extractManifest(const QString& archName, aCManifest *mf)
{
#ifndef Q_OS_WIN32
	Q3Process process( QString("unzip") );
//	process.setWorkingDirectory (dir);
	process.addArgument("-op");
	process.addArgument( archName );
	process.addArgument("/META-INF/manifest.xml");
	process.addArgument( "-d" );
	process.addArgument( tmpDirName );

#else
	Q3Process process( QString("7z") );
//	process.setWorkingDirectory ( templateDir);
//	printf("working dir = `%s'\n", QString(templateDir).ascii());
	process.addArgument( "x" );
	process.addArgument( "-y" );
	process.addArgument( QString("-o%1").arg(tmpDirName) );
	process.addArgument( archName );

#endif

	if( !process.start() )
	{
		setLastError(tr("Can't start unzip"));
		aLog::print(aLog::Error, tr("aContainer start unzip"));
		return false;
	}

	while( process.isRunning() );

	if( !process.normalExit() )
	{
		setLastError(tr("Unzip ended anormal"));
		aLog::print(aLog::Error, tr("aContainer unzip dead"));
		return false;
	}
	else
	{
		aLog::print(aLog::Debug, tr("aContainer unzip normal"));
	}
	if( process.exitStatus() )
	{
		setLastError(tr("Unzip ended with code %1").arg(process.exitStatus()));
		return false;
	}
	if(mf && mf->read(tmpDirName + QDir::convertSeparators("/META-INF/manifest.xml")))
	{
		return true;
	}
	return false;
}

bool
aContainer::extractData(const QString& archName)
{
#ifndef Q_OS_WIN32
	Q3Process process( QString("unzip") );
//	process.setWorkingDirectory (dir);
	process.addArgument("-op");
	process.addArgument( archName );
	process.addArgument( "-d" );
	process.addArgument( tmpDirName );

#else
	Q3Process process( QString("7z") );
//	process.setWorkingDirectory ( templateDir);
//	printf("working dir = `%s'\n", QString(templateDir).ascii());
	process.addArgument( "x" );
	process.addArgument( "-y" );
	process.addArgument( QString("-o%1").arg(tmpDirName) );
	process.addArgument( archName );

#endif

	if( !process.start() )
	{
		setLastError(tr("Can't start unzip"));
		aLog::print(aLog::Error, tr("aContainer start unzip"));
		return false;
	}

	while( process.isRunning() );

	if( !process.normalExit() )
	{
		setLastError(tr("Unzip ended anormal"));
		aLog::print(aLog::Error, tr("aContainer unzip dead"));
		return false;
	}
	else
	{
		aLog::print(aLog::Debug, tr("aContainer unzip normal"));
	}
	if( process.exitStatus() )
	{
		setLastError(tr("Unzip ended with code %1").arg(process.exitStatus()));
		return false;
	}
	return true;
}

bool
aContainer::compressFile(const QString& fileName)
{

#ifndef Q_OS_WIN32

	Q3Process processUpdate( QString("zip") );
	processUpdate.setWorkingDirectory(tmpDirName);
//	processUpdate.addArgument( "-r" ); // recurce into subdirectories
//	processUpdate.addArgument( "-0" ); // store only
	processUpdate.addArgument( name() ); // cfg name
	processUpdate.addArgument(".");
	processUpdate.addArgument("-i");
	processUpdate.addArgument(fileName);
#else
	Q3Process processUpdate( QString("7z") );
	processUpdate.setWorkingDirectory(tmpDirName);
	processUpdate.addArgument( "a" );
	processUpdate.addArgument( "-tzip" );
	processUpdate.addArgument( fileName );
	processUpdate.addArgument( "-r" );
	processUpdate.addArgument(".");
#endif

	if( !processUpdate.start() )
	{
		setLastError(tr("Unable to start zip"));
		aLog::print(aLog::Error, tr("aContainer zip start error"));
		return false;
	}

	while( processUpdate.isRunning() );

	if( !processUpdate.normalExit() )
	{
		setLastError(tr("Zip ended with error"));
		aLog::print(aLog::Error, tr("aContainer zip dead"));
		return false;
	}

	if( processUpdate.exitStatus() )
	{
		setLastError(tr("Zip ended with code %1").arg(processUpdate.exitStatus()));
		return false;
	}

	return true;
}




void
aContainer::setLastError(const QString& errorText)
{
	txtError = errorText;
}


QString
aContainer::lastError() const
{
	return txtError;
}

void
aContainer::cleanupTmpFiles()
{
	QFile file;
	QDir dir;

	if(manifest && tmpDirName!="")
	{
		aCManifest::record rec;
		rec = manifest->first();
		while(rec.type!=mf_invalid)
		{
			if(rec.type!=mf_dir && rec.type!=mf_invalid)
			{
				file.setName(tmpDirName + QDir::convertSeparators(rec.name));
				if(file.remove())
					aLog::print(aLog::Debug, tr("aContainer delete file %1").arg(rec.name));
			}
			rec = manifest->next();
		}
		rec = manifest->first();
		while(rec.type!=mf_invalid)
		{
			if(rec.type==mf_dir)
			{
				if(dir.rmdir(tmpDirName + QDir::convertSeparators(rec.name) ))
					aLog::print(aLog::Debug, tr("aContainer delete directory %1").arg(rec.name));
			}
			rec = manifest->next();
		}
	}


	file.setName(QDir::convertSeparators(tmpDirName+"/META-INF/manifest.xml"));
	if(file.remove()) aLog::print(aLog::Debug, tr("aContainer delete file %1").arg(file.name()));
	if(dir.rmdir(QDir::convertSeparators(tmpDirName))) aLog::print(aLog::Debug, tr("aContainer delete directory %1").arg(tmpDirName + "/META-INF"));
	//aLog::print(aLog::Info, tr("aContainer cleanup temporary files"));

}

