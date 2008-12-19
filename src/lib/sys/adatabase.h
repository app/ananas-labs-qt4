/****************************************************************************
** $Id: adatabase.h,v 1.2 2008/11/09 21:08:09 leader Exp $
**
** Header file of the ananas database of Ananas
** Designer and Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) 2003-2005 Grigory Panov <gr1313 at mail dot ru >, Yoshkar-Ola
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

#ifndef ADATABASE_H
#define ADATABASE_H
#include <QObject>
#include <qstringlist.h>
#include <qsqldatabase.h>
#include <q3dict.h>
//Added by qt3to4:
#include <QSqlError>
#include "acfg.h"
#include <qwidget.h>
#include <QMap>
#include <qdataschema.h>


/**
 *	Константы, используемые для формирования имен sql таблиц в зависимости от соответствия
 *	sql таблицы и бизнес объекта Ананаса
 */
#define db_document_header	"dh%1"
#define db_document_table	"dt%1_%2"
#define db_catalog_element	"ce%1"
#define db_catalog_group	"cg%1"
#define db_iregister		"ri%1"
#define db_aregister		"ra%1"
#define db_areg_remainder	"ra%1_rem"
#define db_user_roles		"usr_rl"
#define db_users		"usr"
#define db_roles		"rl"
#define db_right_roles		"r_rl"
#define db_indices		"idc"
#define db_areg_dim		"ra%1_d%2"

class aDatabase;
class aCfgRc;
class aDataTable;

/*!
 *\~english
 *	Database abstraction layer class.
 *	Used for working with SQL database on metadata context.
 *\~russian
 *	\brief Класс для работы с СУБД. Ananas Data Abstraction Layer.
 *	Наследует QObject.
 *
 *	Обеспечивает унифицированный программный интерфейс доступа к данным, независимый от используемой СУБД.
 *	Используется для работы с базой данных в контексте бизнес схемы.
 *\~
 */
class  ANANAS_EXPORT aDatabase: public QObject
{
	Q_OBJECT

public slots:

public:

        enum DBPermission {
            DBP_NoAccess    = 0,
            DBP_View        = 1 << 0,
            DBP_Update      = 1 << 1,
            DBP_Insert      = 1 << 2,
            DBP_Delete      = 1 << 3,
            DBP_Execute     = 1 << 4,
            DBP_TurnOn      = 1 << 5,
            DBP_TurnOff     = 1 << 6,
            DBP_All         = 0xFFFF
        };

	/*! \~english type of database drivers \~russian перечисление типов баз данных \~ */
	enum dbServer
	{
	/*!	\~english Unknown driver (not supported)\~russian   Неизвестный драйвер (не поддерживается)\~ */
	unknown,
	/*!	\~english MySQL driver \~russian MySQl драйвер \~ */
	mysql,
	/*!	\~english PostgreSQL driver \~russian PostgreSQL драйвер \~ */
	postgresql,
	/*!	\~english ODBC driver (not supported)\~russian ODBC драйвер (не поддерживается) \~ */
	odbc,
	/*!	\~english Oracle driver (not supported)\~russian Oracle драйвер (не поддерживается) \~ */
	oracle,
	/*!	\~english MSSQL driver (not supported)\~russian MSSQL драйвер (не поддерживается) \~ */
	mssql,
	/*!	\~english Sybase driver (not supported)\~russian Sybase драйвер (не поддерживается) \~ */
	sybase };
	/*! \~english link to database \~russian ссылка на базу данных \~ */
	QSqlDatabase ddb;
	/*! \~english metadata object \~russian метаданные \~ */
	aCfg cfg;
        QDataSchema *qds;

	aDatabase();
	~aDatabase();
	bool init( aCfgRc *rc, const QString &dbname = QString::null );
	bool init( const QString &rcname, const QString &dbname = QString::null );
	bool		prepareDatabaseConnect( aCfgRc *rc );
	void done();
	QSqlDatabase *db(const QString &dbname = QString::null );
	bool checkStructure();
	bool create();
	bool drop( const QString &);
	bool update();
	Q_ULLONG uid( int otype );
//	int otype( Q_ULLONG uid );
	int uidType ( Q_ULLONG uid );
	aDataTable *table( const QString & name = QString::null );
	bool tableExists( const QString & name );
	void markDeleted(qulonglong uid);
	void deleteMarked();
        QString driverName();

        bool isAccessRights( int md_type, Q_ULLONG obj_id, DBPermission req_operation );

	static QStringList supportedDriverList();
	static QStringList getUniqueIndices(const QString & flddef);
	static QString tableDbName( aCfg &md, aCfgItem context, long * tid);
	static QString tableDbName( aCfg &md, aCfgItem context);

	bool exchangeDataSystables	 ( QDomDocument &xml, bool import );
	bool exchangeDataCatalogues	 ( QDomDocument &xml, bool import );
	bool exchangeDataDocuments	 ( QDomDocument &xml, bool import );
	bool exchangeDataJournals	 ( QDomDocument &xml, bool import );
	bool exchangeDataInfoRegisters	 ( QDomDocument &xml, bool import );
	bool exchangeDataAccumulationRegisters	 ( QDomDocument &xml, bool import );
	bool exchangeDataUniques 	 ( QDomDocument &xml, bool import );

	bool transaction();
	bool commit();
	bool rollback();

        QStringList users();
        int  loginUsersCount();
        bool login( const QString &username, const QString &password, int applicationId = 0 );
        void logout();
        void netupdate();

        bool isObjectLocked( Q_ULLONG id );
        bool objectLock( Q_ULLONG id );
        void objectUnlock( Q_ULLONG id=0 );
//        QDataSchema *qds();

        int  rolePermission( int role_id, int md_id );
        void setRolePermission( int role_id, int md_id, int new_permission );
        
        static aDatabase *database();
signals:
        void refresh();
protected:
        void timerEvent ( QTimerEvent * );
private:
	/**
	 * \en
	 *	link to database 
	 * \_en \ru
	 *	\brief Ссылка на объект доступа к базе данных sql сервера.
	 * \_ru
	 */
	QSqlDatabase *dataBase;
        int v_user_id, v_app_id;
        int v_updatesCount;
        int v_timerId;

        int updatesCount();
	bool exportTableData(QDomDocument& xml, const QString &tableName);
	bool importTableData(QDomDocument& xml, const QString &tableName);
	void reportError(QSqlError er, const QString &query);

	bool isExists(const QString fname, QStringList *f_lst, QString &match);
	bool createdb( bool update );
        bool dumpTable( QDomDocument& xml, const QString &tableName);
	void fillFeatures();
	QString feature(const QString& fetureName);

	QMap<QString, QString> featuresMySQL, featuresSQLite, featuresPostgreSQL;
        QMap<int, int> accessRights;
};

#endif
