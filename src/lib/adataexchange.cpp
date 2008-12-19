/****************************************************************************
** $Id: adataexchange.cpp,v 1.2 2008/11/08 20:16:35 leader Exp $
**
** Import/export function implementation file of
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
#include "adataexchange.h"
#include "aservice.h"
#include <qapplication.h>
#include <qfile.h>
#include <qdir.h>
#include <q3process.h>
#include <QTextStream>
#include <stdio.h>
#include <stdlib.h>

//#define ERROR true

aBackup::aBackup():QObject()
{
}


aBackup::~aBackup()
{
}

bool
aBackup::importData(const QString& rcfile, const QString &archfile, bool dropBase, bool replaceTemplates)
{


	int prg=0;
	int totalSteps=10;
	QString tmpDirName;
	QString filename = archfile;

	QDir dir;
        QString temp;
	QStringList templatesName;

#ifndef _Windows
	temp = getenv("TMPDIR");
	if(temp=="" || temp.isEmpty())
		temp = P_tmpdir;
#else
	temp = getenv("TEMP");
#endif
	tmpDirName = QString(temp+"/%1").arg(QDateTime::currentDateTime().toTime_t());
	tmpDirName = QDir::convertSeparators(tmpDirName);
	//printf("tmp dir name = %s\n",tmpDirName.ascii());
	if(!dir.mkdir(tmpDirName))
	{
		setLastError(tr("Can't create temporary directory"));
		aLog::print(aLog::Error, "aBackup create temporary directory %1");
		return true;
	}
	else
	{
		aLog::print(aLog::Debug, tr("aBackup create temporary directory %1").arg(tmpDirName));
	}
	if(unzipArchive(archfile, tmpDirName)==true)
	{
		cleanupTmpFiles(tmpDirName, &templatesName);

		//printf("unzip error!\n");
		aLog::print(aLog::Error, tr("aBackup import data: unzip"));
		return true;
	}
	else
	{
		aLog::print(aLog::Debug, tr("aBackup unzip"));
	}


	QString srcDirName = QDir::convertSeparators(tmpDirName + "/templates/");
	dir.setPath(srcDirName);
	templatesName = dir.entryList("templ_*.odt;templ_*.ods");


	qApp->processEvents();
	emit(progress(++prg,totalSteps));

	filename.truncate( filename.length() - QString(".bsa").length() );
	aLog::print(aLog::Debug, tr("aBackup filename = %1").arg(filename));

	//printf("filename = %s\n",filename.ascii());
	changeRC(rcfile, tmpDirName + "/busines-schema.cfg");

	QFile f(tmpDirName+"/content.xml");
	QDomDocument xml;
	xml.setContent(&f);
//	printf("%s\n",xml.toString(4).ascii());
	aDatabase db;
	if(db.init(rcfile))
	{
		emit(progress(++prg,totalSteps));
		if(!dropBase)
		{
			db.done();
			cleanupTmpFiles(tmpDirName, &templatesName);
			return false;
		}
		db.drop(db.cfg.rc.value("dbname"));
		emit(progress(++prg,totalSteps));
		db.create();

		emit(progress(++prg,totalSteps));

		db.exchangeDataSystables ( xml, true);
		emit(progress(++prg,totalSteps));
		db.exchangeDataCatalogues( xml, true );
		emit(progress(++prg,totalSteps));
		db.exchangeDataDocuments ( xml, true );
		emit(progress(++prg,totalSteps));
		db.exchangeDataJournals ( xml, true );
		db.exchangeDataInfoRegisters ( xml, true );
		emit(progress(++prg,totalSteps));
		db.exchangeDataAccumulationRegisters ( xml, true );
		emit(progress(++prg,totalSteps));
		db.exchangeDataUniques ( xml, true );
	}
	else
	{
		setLastError(tr("Can't connect to database"));
		cleanupTmpFiles(tmpDirName, &templatesName);
		return true;
	}
	qApp->processEvents();

	QString destDirName = QDir::convertSeparators(db.cfg.rc.value("workdir"));

	//create template directory
	QDir destDir;
//	destDir.setPath(destDirName);
	if(!destDir.exists(destDirName))
	{
		aLog::print(aLog::Debug, tr("aBackup template dir `%1' not exists, try create").arg(destDirName));
		if(!destDir.mkdir(destDirName,true))
		{
			aLog::print(aLog::Error, tr("aBackup create template dir `%1' fail").arg(destDirName));
		}
		else
		{
			aLog::print(aLog::Debug, tr("aBackup create template dir `%1'").arg(destDirName));
		}
	}
	else
	{
		aLog::print(aLog::Debug, tr("aBackup template dir `%1' exists").arg(destDirName));
	}

	for(uint i=0; i<templatesName.count(); i++)
	{
	//	aTests::print2log("f:\\ERROR.log", "aBackup", tmpDirName + "/templates/"+templatesName[i]);
		aService::copyFile(QDir::convertSeparators(srcDirName+templatesName[i]), QDir::convertSeparators(destDirName +"/"+templatesName[i]), replaceTemplates);
	}

	db.done();

//	printf("copy %s to %s\n", QDir::convertSeparators(tmpDirName+"/busines-schema.cfg").ascii(), QDir::convertSeparators(filename+".cfg").ascii());
	if(!aService::copyFile( QDir::convertSeparators(tmpDirName+"/busines-schema.cfg"), QDir::convertSeparators(filename+".cfg"), true))
	{
		setLastError(tr("Can't copy .cfg file"));
		aLog::print(aLog::Error, tr("aBackup copy unzipped business schema file"));
		cleanupTmpFiles(tmpDirName, &templatesName);
		return true;
	}
	else
	{
		aLog::print(aLog::Debug, tr("aBackup copy business schema file"));
	}
	emit(progress(++prg,totalSteps));
//	printf("filename =%s\n",filename.ascii());
	changeRC(rcfile, filename+".cfg");
	cleanupTmpFiles(tmpDirName, &templatesName);
	setLastError(tr("Database import without errors"));
	aLog::print(aLog::Info, tr("aBackup import data ok"));
	return false;
}



bool
aBackup::exportData(const QString& rcfile, const QString &archfile, bool withTemplates )
{
	QDir dir;
	int prg=0;
	int totalSteps=10;
        QString temp;
	QString tmpDirName;
	QString srcDirName;
	QStringList templatesName;

#ifndef _Windows
	temp = getenv("TMPDIR");
	if(temp=="" || temp.isEmpty())
		temp = P_tmpdir;
#else
	temp = getenv("TEMP");
#endif
	tmpDirName = QString(temp+"/%1").arg(QDateTime::currentDateTime().toTime_t());
	tmpDirName = QDir::convertSeparators(tmpDirName);
	//printf("copy name = %s\n",tmpDirName.ascii());
	if(!dir.mkdir(tmpDirName))
	{
		setLastError(tr("Can't create directory %s").arg(tmpDirName));
		aLog::print(aLog::Error, tr("aBackup create temporary directory"));
		return true;
	}
	else
	{
		aLog::print(aLog::Debug, tr("aBackup create temporary directory %1").arg(tmpDirName));

	}
	if(!dir.mkdir(tmpDirName+"/META-INF"))
	{
		setLastError(tr("Can't create directory %s").arg(tmpDirName+"/META-INF"));
		aLog::print(aLog::Error, tr("aBackup create temporary directory"));
		cleanupTmpFiles(tmpDirName, &templatesName);
		return true;
	}
	else
	{
		aLog::print(aLog::Debug, tr("aBackup create temporary directory %1").arg(tmpDirName+"/META-INF"));
	}
	aCfg cfg;
	// create copy of metadata
	if(cfg.readrc( rcfile ))
	{
		setLastError(tr("Invalid resource file"));
		aLog::print(aLog::Error, tr("aBackup invalid *.rc file"));
		cleanupTmpFiles(tmpDirName, &templatesName);
		return true;
	}
	else
	{
		aLog::print(aLog::Debug, tr("aBackup valid *.rc file"));
	}
	qApp->processEvents();

	if(withTemplates)
	{
		srcDirName = QDir::convertSeparators(cfg.rc.value("workdir"));
		aLog::print(aLog::Debug, tr("aBackup workdir=%1").arg(srcDirName));
		dir.setPath(srcDirName);
		templatesName = dir.entryList("templ_*.odt;templ_*.ods");
		for(uint i=0; i<templatesName.count(); i++)
		{
			//ayTests::print2log("f:\\ERROR.log", "aBackup", tmpDirName + "/templates/"+templatesName[i]);
			aLog::print(aLog::Debug, tr("aBackup template %1 %2").arg(i).arg(tmpDirName + "/templates/"+templatesName[i]));
//			file.remove();
		}
	}


	emit(progress(++prg,totalSteps));
	if(cfg.write( tmpDirName+"/busines-schema.cfg" ))
	{
		setLastError(tr("Can't write resource file"));
		aLog::print(aLog::Error, tr("aBackup write %1 file").arg(tmpDirName+"/busines-schema.cfg"));
		cleanupTmpFiles(tmpDirName, &templatesName);
		return true;
	}
	else
	{
		aLog::print(aLog::Debug, tr("aBackup write %1 file").arg(tmpDirName+"/busines-schema.cfg"));

	}
	qApp->processEvents();
	emit(progress(++prg,totalSteps));
	// create dump
	if(dumpBase(rcfile,tmpDirName, prg, totalSteps )==true)
	{
		cleanupTmpFiles(tmpDirName, &templatesName);
		aLog::print(aLog::Error, tr("aBackup dump base error"));
		return true;
	}
	else
	{
		aLog::print(aLog::Debug, tr("aBackup bump base"));
	}

	if(writeXml(QDir::convertSeparators(tmpDirName+"/META-INF/manifest.xml"), createManifest(templatesName))==true)
	{
		setLastError(tr("Can't write file META-INF/manifest.xml"));
		aLog::print(aLog::Error, tr("aBackup write manifest.xml"));
		cleanupTmpFiles(tmpDirName, &templatesName);
		return true;
	}
	else
	{
		aLog::print(aLog::Debug, tr("aBackup write manifest.xml"));
	}
//	printf("%s\n",(createManifest(templatesName)).toString().ascii());
	if(withTemplates)
	{
		QString destNamePref = tmpDirName + "/templates";
		dir.mkdir(destNamePref);
		bool res = false;
		for(uint i=0; i<templatesName.count(); i++)
		{
//				printf("copy %s to %s\n", QString(srcDirName+"/"+templatesName[i]).ascii(), QString(destNamePref+"/"+templatesName[i]).ascii());
			if(!aService::copyFile(QDir::convertSeparators(srcDirName+"/"+templatesName[i]), QDir::convertSeparators(destNamePref+"/"+templatesName[i]), true))
			{
				setLastError(tr("Can't copy template file"));
				res&=true;
			}

		}
	}

	if(zipArchive(archfile + ".bsa", tmpDirName)==true)
	{
//		setLastError(tr("Can't zip archive"));
		cleanupTmpFiles(tmpDirName, &templatesName);
		aLog::print(aLog::Error, tr("aBackup zip archive"));
		return true;
	}
	else
	{
		aLog::print(aLog::Debug, tr("aBackup zip archive"));
	}
	// remove files and directories
	cleanupTmpFiles(tmpDirName, &templatesName);
	emit (progress(++prg,totalSteps));
	setLastError(tr("Data export done without errors"));
	aLog::print(aLog::Debug, tr("aBackup export data ok"));

	return false;
}

bool
aBackup::unzipArchive(const QString& archName, const QString& dir)
{
#ifndef _Windows
	Q3Process process( QString("unzip") );
//	process.setWorkingDirectory (dir);
	process.addArgument( archName );
	process.addArgument( "-d" );
	process.addArgument( dir );

#else
	Q3Process process( QString("7z") );
//	process.setWorkingDirectory ( templateDir);
//	printf("working dir = `%s'\n", QString(templateDir).ascii());
	process.addArgument( "x" );
	process.addArgument( "-y" );
	process.addArgument( QString("-o%1").arg(dir) );
	process.addArgument( archName );

#endif

//	printf("unzip to %s file `%s'\n",copyName.ascii(),fname.ascii());
	if( !process.start() )
	{
		//qWarning("FormTemplate::unzip(): failed to start unzip");
		setLastError(tr("Can't start zip"));
		aLog::print(aLog::Error, tr("aBackup start unzip"));

		return true;
	}

	while( process.isRunning() );

	if( !process.normalExit() )
	{
	//	qWarning("FormTemplate::unzip(): error extracting document content");
		setLastError(tr("Zip ended anormal"));
		aLog::print(aLog::Error, tr("aBackup unzip dead"));
		return true;
	}
	else
	{
		//printf("process end normal\n");
		aLog::print(aLog::Debug, tr("aBackup unzip normal"));
	}
	if( process.exitStatus() )
	{
		//qWarning("FormTemplate::unzip(): document content extracting ended with code " + QString::number(process.exitStatus()) );
		setLastError(tr("Zip ended with code %1").arg(process.exitStatus()));
		return true;
	}
	return false;
}

bool
aBackup::zipArchive(const QString& archName, const QString& dir)
{


#ifndef _Windows

	Q3Process processUpdate( QString("zip") );
	processUpdate.setWorkingDirectory(dir);
	processUpdate.addArgument( "-r" ); // recurce into subdirectories
	processUpdate.addArgument( "-0" ); // store only
	processUpdate.addArgument( archName ); // backup name
	processUpdate.addArgument(".");
#else
	Q3Process processUpdate( QString("7z") );
	processUpdate.setWorkingDirectory(dir);
	processUpdate.addArgument( "a" );
	processUpdate.addArgument( "-tzip" );
	processUpdate.addArgument( archName );
	processUpdate.addArgument( "-r" );
	processUpdate.addArgument(".");
#endif

	if( !processUpdate.start() )
	{
		setLastError(tr("Unable to start zip"));
		aLog::print(aLog::Error, tr("aBackup zip start error"));
		return true;
	}

	while( processUpdate.isRunning() );

	if( !processUpdate.normalExit() )
	{
		setLastError(tr("Zip ended with error"));
		aLog::print(aLog::Error, tr("aBackup zip dead"));
		return true;
	}

	if( processUpdate.exitStatus() )
	{
		setLastError(tr("Zip ended with code %1").arg(processUpdate.exitStatus()));
		return true;
	}

	return false;
}


bool
aBackup::dumpBase(const QString& rcfile, const QString& tmpDirName, int& prg, int totalSteps)
{
	QDomDocument xml;
	xml.setContent(QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>"));
	QDomElement root = xml.createElement( "AnanasDump" );
	xml.appendChild( root );
	aDatabase db;
	if(db.init(rcfile))
	{
		emit (progress(++prg,totalSteps));
		db.exchangeDataSystables ( xml, false );
		emit (progress(++prg,totalSteps));
		db.exchangeDataCatalogues( xml, false );
		emit (progress(++prg,totalSteps));
		db.exchangeDataDocuments ( xml, false );
		emit (progress(++prg,totalSteps));
		db.exchangeDataJournals ( xml, false );
		emit (progress(++prg,totalSteps));
		db.exchangeDataInfoRegisters ( xml, false );
		emit (progress(++prg,totalSteps));
		db.exchangeDataAccumulationRegisters ( xml, false );

		db.exchangeDataUniques ( xml, true );

		aLog::print(aLog::Debug, tr("aBackup dump tables ok"));
	}
	else
	{
		setLastError(tr("Can't connect to database"));
		aLog::print(aLog::Error, tr("aBackup init rc file  %1").arg(rcfile));
		db.done();
		return true;
	}
	qApp->processEvents();
	emit (progress(++prg,totalSteps));
	db.done();
	if(writeXml(tmpDirName+"/content.xml",xml)==true)
	{
		setLastError(tr("Can't write content.xml"));
		aLog::print(aLog::Error, tr("aBackup write content.xml"));
		return true;
	}
	else
	{
		aLog::print(aLog::Info, tr("aBackup dump base to xml compleet"));
	}
	return false;
}

bool
aBackup::writeXml(const QString & name2Save, QDomDocument xml)
{
	QFile file(name2Save);
	QByteArray buf( xml.toString(4).utf8() );
	if ( file.open( QIODevice::WriteOnly ) )
	{
		QTextStream ts( &file );
		//--ts.setEncoding(Q3TextStream::UnicodeUTF8);
		xml.save(ts, 4);
		file.close();
	}
	else
	{
		aLog::print(aLog::Error, tr("aBackup save xml %1").arg(name2Save));
		return true;
	}
	return false;
}

bool
aBackup::checkStructure()
{
	return false;
}

QDomDocument
aBackup::createManifest(const QStringList& templates)
{
	QDomDocument manifest;
	manifest.setContent(QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>"));

	QDomElement root = manifest.createElement("manifest:manifest");
	manifest.appendChild(root);

	QDomElement entry = manifest.createElement("manifest:file-entry");
	entry.setAttribute("manifest:full-path","business-schema.cfg");
	root.appendChild(entry);

	entry = manifest.createElement("manifest:file-entry");
	entry.setAttribute("manifest:full-path","content.xml");
	root.appendChild(entry);
	if(templates.count()>0)
	{
		QDomElement templatesDir = manifest.createElement("manifest:file-entry");
		templatesDir.setAttribute("manifest:full-path","templates/");

		for(uint i=0; i<templates.count();i++)
		{
			entry = manifest.createElement("manifest:file-entry");
			entry.setAttribute("manifest:full-path",QString("%1%2").arg("templates/").arg(templates[i]));
			templatesDir.appendChild(entry);
		}
		root.appendChild(templatesDir);
	}

	aLog::print(aLog::Debug, tr("aBackup create manifest.xml"));
	return manifest;
}


void
aBackup::changeRC(const QString& nameRC, const QString& newConfigName)
{
	QMap<QString,QString> cfg;
//	QString configFileName;

	cfg = aTests::readConfig(QDir::convertSeparators(nameRC));

//	configFileName = cfg["configfile"];
//	configFileName.truncate( configFileName.length() - QString(".bsa").length() );
//	cfg["configfile"] = configFileName + ".cfg";

	cfg["configfile"] = newConfigName;

	aTests::writeConfig(QDir::convertSeparators(nameRC), cfg);
}


void
aBackup::setLastError(const QString& errorText)
{
	txtError = errorText;
}


QString
aBackup::lastError() const
{
	return txtError;
}

void
aBackup::cleanupTmpFiles(const QString& tmpDirName, QStringList *files)
{
	QFile file;
	QDir dir;
	file.setName(QDir::convertSeparators(tmpDirName+"/content.xml"));
	aLog::print(aLog::Debug, tr("aBackup delete file %1").arg(file.name()));
	file.remove();
	file.setName(QDir::convertSeparators(tmpDirName+"/busines-schema.cfg"));
	aLog::print(aLog::Debug, tr("aBackup delete file %1").arg(file.name()));
	file.remove();
	file.setName(QDir::convertSeparators(tmpDirName+"/META-INF/manifest.xml"));
	aLog::print(aLog::Debug, tr("aBackup delete file %1").arg(file.name()));
	file.remove();
	for(uint i=0; i<files->count(); i++)
	{
			file.setName(QDir::convertSeparators(tmpDirName + "/templates/"+ (*files)[i]));
			aLog::print(aLog::Debug, tr("aBackup delete file %1").arg(file.name()));
			file.remove();
	}
	aLog::print(aLog::Debug, tr("aBackup delete directory %1").arg(tmpDirName + "/META-INF"));
	dir.rmdir(QDir::convertSeparators(tmpDirName + "/META-INF"));
	aLog::print(aLog::Debug, tr("aBackup delete directory %1").arg(tmpDirName + "/templates"));
	dir.rmdir(QDir::convertSeparators(tmpDirName + "/templates"));
	aLog::print(aLog::Debug, tr("aBackup delete directory %1").arg(tmpDirName));
	dir.rmdir(QDir::convertSeparators(tmpDirName));
	aLog::print(aLog::Info, tr("aBackup cleanup temporary files"));

}

