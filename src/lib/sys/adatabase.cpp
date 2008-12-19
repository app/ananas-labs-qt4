/****************************************************************************
** $Id: adatabase.cpp,v 1.6 2008/12/10 21:04:03 leader Exp $
**
** Database abstaraction layer
** of Ananas Project
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) 2003-2005 Grigory Panov <gr1313 at mail.ru >, Yoshkar-Ola
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

/*
*/
/*
for mysql server set variable in /var/lib/mysql/my.cnf
default-character-set=utf8

*/
#include <QObject>
#include <qfile.h>
#include <qdom.h>
#include <qstringlist.h>
#include <q3textstream.h>
#include <qsqlrecord.h>
#include <qstringlist.h>
//Added by qt3to4:
#include <QSqlQuery>
#include <Q3SqlCursor>
#include <QSqlError>
#include <Q3SqlRecordInfo>
#include <QMessageBox>

#include "alog.h"

#include "asqltable.h"
#include "aapplication.h"

#include "adatabase.h"


QStringList dd;

static aDatabase aDatabase_default;

/*!
 *
 */
QString
qds_field ( const QString fname, const QString &tdef, const QString &descr = "" )
{
        int n1, n2;
        bool notnull, serial;
        char t=' ';
        QString st, dt="F="+fname+"|"+descr+"|";

        if ( tdef.isEmpty() ) return "";
        n1=0; n2=0;
        st = tdef.section ( " ", 0, 0 ).lower();
        n1 = tdef.section ( " ", 1, 1 ).toInt();
        n2 = tdef.section ( " ", 2, 2 ).toInt();
        if ( !st.isEmpty() ) t = ( ( const char * ) st ) [0];
        serial = ( st.mid ( 1 ).contains ( 's' ) > 0 );
        notnull = ( st.mid ( 1 ).contains ( 'n' ) > 0 );
//      if ( serial && t=='l') t='i';
        switch ( t )
        {
                case 'i':
                        dt.append ( "I|0|0|" );
                        break;
                case 'l':
                        dt.append ( "L|0|0|" );
                        break;
                case 'c':
                        dt.append ( QString ( "S|%1|0|" ).arg ( n1 ) );
                        break;
                case 'n':
                        dt.append ( QString ( "N|%1|%2|" ).arg ( n1 ).arg ( n2 ) );
                        break;
                case 'd':
                        dt.append ( "D|0|0|" );
                        break;
                case 'o':
                        dt.append ( "L|0|0|" );
                        break;
                case 'b':
                        dt.append ( "C|1|0|" );
                        break;
                default:
                        dt.append ( "I|0|0|" );
                        break;
        }
        if ( !serial && notnull ) dt.append ( "N" );
        if ( serial ) dt.append ( "PS" );
        return dt;
}



void
qds_fields ( aCfg &cfg, aCfgItem context )
{

        int j, ifld;
        QString t, flddef="";
        aCfgItem of, item;
        int id, n=0;
        aCfgItem dim_field;

        /*
                if(cfg.objClass(context) == md_field && cfg.objClass(cfg.parent(context))== md_dimensions)
                {
                        dim_field = context;
                        t = cfg.attr( dim_field, mda_type );
                        if ( !t.isEmpty() )
                        {
                                if(t[0]!=' ')
                                {
                                        flddef.append( QString("uf%1 %2,").arg(cfg.id(dim_field)).arg(t) );
                                }
                        }
                }
        */
        ifld = cfg.count ( context, md_field );
        for ( j = 0; j < ifld; j++ )
        {
                item = cfg.find ( context, md_field, j );
                id = cfg.id ( item );
                if ( id )
                {
                        t = cfg.attr ( item, mda_type );
                        if ( !t.isEmpty() )
                        {
                                if ( t[0]!=' ' )
                                {
                                        dd<<qds_field ( QString ( "uf%1" ).arg ( id ), t, cfg.attr ( item, mda_name ) );
                                        if ( t.section ( " ", 3, 3 ).lower() =="i" )
                                        {
                                                dd<<QString ( "I=IDX_uf%1||uf%2|" ).arg ( id ).arg ( id );
                                        };
                                        n++;
                                }
                        }
                }
        }
}



void
qds_dd ( aCfg &cfg )
{

        dd.clear();
        dd<<"D=ANANAS|Ananas data schema|UTF-8|0000-0000-0000-0000";

        //================== Системные таблицы

        dd<<"T=uniques|Uniques table|U";
        dd<<"F=id|ID|L|0|0|NSP";
        dd<<"F=otype|Object type|I|0|0|";
        dd<<"F=df|Deleted flag|C|1|0|";
        dd<<"I=I1|Index 1|otype|";
        dd<<"I=I2|Index 2|df|";

        dd<<"T=netusers|Network users|U";
        dd<<"F=users|Users count|I|0|0|";
        dd<<"F=updates|Updates count|I|0|0|";

        dd<<"T=locks|Locked objects|U";
        dd<<"F=userid|User ID|I|0|0|";
        dd<<"F=id|Object id|L|1|0|NP";
        dd<<"I=I1|Index 1|userid|";

        dd<<"T=a_journ|Journals|U";
        dd<<"F=id|ID|L|0|0|PN";
        dd<<"F=typej|Journal type|I|0|0|";
        dd<<"F=idd|ID of document|L|0|0|";
        dd<<"F=typed|Document type|I|0|0|";
        dd<<"F=ddate|Document date|D|0|0|";
        dd<<"F=pnum|Prefix number|C|254|0|N";
        dd<<"F=num|Document number|I|0|0|";
        dd<<"F=mf|Marked flag|C|1|0|";
        dd<<"F=df|Deleted flag|C|1|0|";
        dd<<"F=cf|Conducted flag|C|1|0|";
        dd<<"I=I1|Index 1|typej|";
        dd<<"I=I2|Index 2|idd|";
        dd<<"I=I3|Index 3|typed|";
        dd<<"I=I4|Index 4|ddate|";
        dd<<"I=I5|Index 5|pnum|";
        dd<<"I=I6|Index 6|num|";
        dd<<"I=I7|Index 7|mf|";
        dd<<"I=I8|Index 8|df|";
        dd<<"I=I9|Index 9|cf|";

        dd<<QString ( "T=%1|Users|U" ).arg ( db_users );
        dd<<"F=id       |ID                           |L|0|0|NP";
        dd<<"F=fname    |First name                   |C|25|0|";
        dd<<"F=lname    |Last name                    |C|30|0|";
        dd<<"F=login    |Login name                   |C|20|0|";
        dd<<"F=password |Password                     |C|100|0|";
        dd<<"F=active0  |Active Ananas unknown app    |I|0|0|";
        dd<<"F=active1  |Active Ananas engine         |I|0|0|";
        dd<<"F=active2  |Active Ananas designer       |I|0|0|";
        dd<<"F=active3  |Active Ananas Administrator  |I|0|0|";
        dd<<"I=I1|Index 1|fname|";
        dd<<"I=I2|Index 2|lname|";
        dd<<"I=I3|Index 3|login|";
        dd<<"I=I4|Index 4|password|";

        dd<<QString ( "T=%1|User Roles|U" ).arg ( db_user_roles );
        dd<<"F=id|ID|L|0|0|P";
        dd<<"F=idr|ID role|I|0|0|";
        dd<<"I=I1|Index 1|idr|";


        dd<<QString ( "T=%1|Roles|U" ).arg ( db_roles );
        dd<<"F=id|ID|L|0|0|P";
        dd<<"F=name|Role name|C|50|0|";
        dd<<"I=I1|Index 1|name|";

        dd<<QString ( "T=%1|Roles rights|U" ).arg ( db_right_roles );
        dd<<"F=id|ID|L|0|0|";
        dd<<"F=permission|Role permission|I|0|0|";
        dd<<"F=object|Role object|L|0|0|";
        dd<<"I=I1|Index 1|id|";
        dd<<"I=I2|Index 2|object|";

        dd<<QString ( "T=%1|???|U" ).arg ( db_indices );
        dd<<"F=tname|Table name|C|40|0|";
        dd<<"F=uindices|???|C|240|0|";
        dd<<"F=idxname|???|C|64|0|";
        dd<<"I=I1|Index 1|tname|";
        dd<<"I=I2|Index 2|uindices|";
        dd<<"I=I3|Index 3|idxname|";

        //================== Справочники

        aCfgItem gcont, cont, item;
        long id;
        int n, i;

        gcont = cfg.find ( cfg.find ( mdc_metadata ), md_catalogues, 0 );
        n = cfg.count ( gcont, md_catalogue );
        for ( i = 0; i<n; i++ )
        {
                item = cfg.find ( gcont, md_catalogue, i );
                if ( !item.isNull() )
                {
                        id = cfg.id ( item );
                        cont = cfg.findChild ( item, md_element, 0 );
                        if ( !cont.isNull() )
                        {
                                dd<<QString ( "T=%1|Elements:%2|U" ).arg ( aDatabase::tableDbName ( cfg, cont ) ).arg ( cfg.attr ( item, mda_name ) );
                                dd<<"F=id|ID|L|0|0|NP";
                                dd<<"F=df|Mark deletede flag|C|1|0|";
                                dd<<"F=idg|ID group|L|0|0|";
                                dd<<"F=ido|ID owner|L|0|0|";
                                dd<<"I=I1|Index 1|df|";
                                dd<<"I=I2|Index 2|idg|";
                                dd<<"I=I3|Index 3|ido|";
                                qds_fields ( cfg, cont );
                        }
                        cont = cfg.findChild ( item, md_group, 0 );
                        if ( !cont.isNull() )
                        {
                                dd<<QString ( "T=%1|Groups:%2|U" ).arg ( aDatabase::tableDbName ( cfg, cont ) ).arg ( cfg.attr ( item, mda_name ) );
                                dd<<"F=id|ID|L|0|0|NP";
                                dd<<"F=df|Mark deletede flag|C|1|0|";
                                dd<<"F=level|tree level (0..n)|L|0|0|";
                                dd<<"F=idp|Parent group ID ( 0 = root )|L|0|0|";
                                dd<<"I=I1|Index 1|df|";
                                dd<<"I=I2|Index 2|level|";
                                dd<<"I=I3|Index 3|idp|";
                                qds_fields ( cfg, cont );
                        }
                }
        }

        //================= Документы

        aCfgItem rcont, tcont;
        int tn, ti;

        rcont = cfg.find ( cfg.find ( mdc_metadata ), md_documents, 0 );
        n = cfg.count ( rcont, md_document );
        for ( i = 0; i<n; i++ )
        {
                item = cfg.find ( rcont, md_document, i );
                if ( !item.isNull() )
                {
                        cont = cfg.findChild ( item, md_header, 0 );
                        if ( !cont.isNull() )
                        {
                                dd<<QString ( "T=%1|%2|U" ).arg ( aDatabase::tableDbName ( cfg, cont ) ).arg ( cfg.attr ( item, mda_name ) );
                                dd<<"F=id|ID|L|0|0|NP";
                                qds_fields ( cfg, cont );
                        }
                        tcont = cfg.find ( item, md_tables, 0 );
                        tn = cfg.count ( tcont, md_table );
                        for ( ti = 0; ti < tn; ti++ )
                        {
                                cont = cfg.findChild ( tcont, md_table, ti );
                                if ( !cont.isNull() )
                                {
                                        dd<<QString ( "T=%1|Groups:%2|U" ).arg ( aDatabase::tableDbName ( cfg, cont ) ).arg ( cfg.attr ( item, mda_name ) );
                                        dd<<"F=id|ID|L|0|0|NP";
                                        dd<<"F=idd|Document ID|L|0|0|";
                                        dd<<"F=ln|Line number|L|0|0|";
                                        dd<<"I=I1|Index 1|idd|";
                                        dd<<"I=I2|Index 2|ln|";
                                        qds_fields ( cfg, cont );
                                }
                        }
                }
        }

        //=================== Регистр оборотный

        QString filds;

        rcont = cfg.find ( cfg.find ( mdc_metadata ), md_iregisters, 0 );
        n = cfg.count ( rcont, md_iregister );
        for ( i = 0; i<n; i++ )
        {
                item = cfg.find ( rcont, md_iregister, i );
                if ( !item.isNull() )
                {
                        dd<<QString ( "T=%1|%2|U" ).arg ( aDatabase::tableDbName ( cfg, item ) ).arg ( cfg.attr ( item, mda_name ) );
                        dd<<"F=id|ID|L|0|0|NP";
                        dd<<"F=idd|Document ID|L|0|0|";
                        dd<<"F=iddt|Document table line ID|L|0|0|";
                        dd<<"F=ln|Line number|L|0|0|";
                        dd<<"I=I1|Index 1|idd|";
                        dd<<"I=I2|Index 2|iddt|";
                        dd<<"I=I3|Index 3|ln|";
                        qds_fields ( cfg, item );
                }
        }

        //=================== Регистр накопительный

        aCfgItem res, dim;

        rcont = cfg.find ( cfg.find ( mdc_metadata ), md_aregisters, 0 );
        n = cfg.count ( rcont, md_aregister );
        for ( i = 0; i<n; i++ )
        {
                item = cfg.find ( rcont, md_aregister, i );
                if ( !item.isNull() )
                {
                        dd<<QString ( "T=%1|%2|U" ).arg ( aDatabase::tableDbName ( cfg, item ) ).arg ( cfg.attr ( item, mda_name ) );
                        dd<<"F=id|ID|L|0|0|NP";
                        dd<<"F=idd|Document ID|L|0|0|";
                        dd<<"F=iddt|Document table line ID|L|0|0|";
                        dd<<"F=ln|Line number|L|0|0|";
                        dd<<"F=date|Document date|D|0|0|";
                        dd<<"I=I1|Index 1|idd|";
                        dd<<"I=I2|Index 2|iddt|";
                        dd<<"I=I3|Index 3|ln|";
                        dd<<"I=I4|Index 4|date|";
                        qds_fields ( cfg, item );

                        res = cfg.find ( item, md_resources );
                        dim = cfg.find ( item, md_dimensions );
                        if ( !res.isNull() && !dim.isNull() )
                        {
                                aCfgItem d;
                                for ( int k=0; k<cfg.count ( dim,md_field );k++ )
                                {
                                        d = cfg.findChild ( dim,md_field,k );
                                        dd<<QString ( "T=%1|%2.%3|U" ).arg ( aDatabase::tableDbName ( cfg, d ) ).arg ( cfg.attr ( item, mda_name ) ).arg ( cfg.attr ( d, mda_name ) );
                                        dd<<"F=date|Document date|D|0|0|";
                                        dd<<"I=I1|Index 1|date|";
                                        qds_fields ( cfg, dim );
                                        qds_fields ( cfg, d );
                                        qds_fields ( cfg, res );
                                }
                        }
                }
        }
};

//==========================================================================

/*!
 *	\~english
 *	Constructor has no arguments.
 *	\~russian
 *	Конструктор без параметров.
 *	\~
 */
aDatabase::aDatabase() : QObject()
{
    dataBase = 0;
    qds = 0;
    v_updatesCount = 0;
    v_timerId = 0;
}



/*!
 *	\~english
 *	Destructor.
 *	\~russian
 *	Деструктор.
 *	\~
 */
aDatabase::~aDatabase()
{
        done();
//        if ( qds ) delete qds;
}



/*!
 *	\~english
 *	Return current database driver name.
 *	Translate name for unicode MySQL driver to standart name.
 *	\~russian
 *	Возвращает имя драйвера базы данных.
 *	Переводит имя для драйвера MySQL с поддержкой utf8 в стандартное имя.
 *	\~
 *	\return \~english QT current Database driver name \~russian имя драйвера. \~
 */
QString
aDatabase::driverName()
{
        QString name = db()->driverName();
        if ( name == "QMYSQL" ) name = "QMYSQL";
        return name;
}


/*!
 *	\~english
 *	Return text feature for dynamic SQL by feature name.
 *	\~russian
 *	Возвращает код реализации какой либо осбенности сервера БД по ее ключу.
 *	\~
 *	\return \~english Code for dynamic SQL creation  \~russian Код для формирования динамического SQL \~
 */
QString
aDatabase::feature(const QString& featureName)
{
	QString res = QString::null;
        if(driverName() == "QMYSQL")
	{
		res = featuresMySQL[featureName];
	}
        if(driverName() == "QSQLITE")
	{
		res = featuresSQLite[featureName];
	}
        if(driverName() == "QPSQL7")
	{
		res = featuresPostgreSQL[featureName];
	}
        return res;
}

/*!
 *	\~english
 *	This is overloaded method, provided for convenience.
 *	\~russian
 *	Перегружет метод init( aCfgRc *rc, const QString &dbname )
 *	\~
 *	\see init( aCfgRc *rc, const QString &dbname )
 *	\param rcname (in) - \~english rc file name. \~russian имя файла ресурса \~
 *	\param dbname (in) - \~english parameter to pass trough. \~russian имя базы данных \~
 *	\return \~english true, if no error \~russian  true, если ошибок не было \~
 */
bool
aDatabase::init( const QString &rcname, const QString &dbname )
{
	if ( rcname.isEmpty() ) return false;
	if ( cfg.readrc( rcname ) ) return false;
	return init( &cfg.rc, dbname );
}

/**
 * \en
 *      \brief Creates database on SQL server.
 *
 *      Uses rc file parameters values and makes connection to database server.
 *      Then creates database with name obtained from rc file.
 *      \param rc (in) - rc object reference.
 *      \param dbname (in) - unused now.
 *      \return true, if no error
 * \_en \ru
 *      \brief Открывает соединение с базой данных на SQL сервере или создает базу данных.
 *
 *      Если БД не удалось открыть, метод пытается создать базу данных на SQL сервере,
 *      используя переданные параметры соединения с SQL сервером. При этом создается пустая база данных
 *      без таблиц и других элементов.
 *      \param rc (in) - ссылка на инициализированный объект,
 *      хранящий данные файла ресурсов, необходимые для установления соединения. Проще говоря, объект
 *      с параметрами соединения.
 *      \param dbname (in) - не используется
 *      \return frue, если не было ошибок
 * \_ru
 *      \see init( const QString &rcname, const QString &dbname )
 */
bool
aDatabase::init ( aCfgRc *rc, const QString &dbname )
{
        fillFeatures();
        if ( !rc ) return false;
        if ( !prepareDatabaseConnect ( rc ) ) return false;

        qds_dd ( cfg );
        qds->setDataDictionary ( dd );
        if ( qds->open() )
        {
                aLog::print ( aLog::Info,tr ( "aDatabase open connection to %1" ).arg ( rc->value ( "dbname" ) ) );
        }
        else
        {
                cfg_message ( 3, ( const char * ) tr ( "Can't open database connection\n" ).utf8() );
                aLog::print ( aLog::Error,tr ( "aDatabase open connection to %1" ).arg ( rc->value ( "dbname" ) ) );
        };
        QSqlQuery q = db()->exec ( QString ( "SELECT * FROM %1" ).arg ( qds->tableName ( "netusers" ) ) );
        if ( !q.first() )
        {
                db()->exec ( QString ( "INSERT INTO %1 (users,updates) VALUES (0,0)" ).arg ( qds->tableName ( "netusers" ) ) );
        }
        v_updatesCount = updatesCount();
        v_timerId = startTimer ( 5000 );
        return true;
}

/**
 * \ru
 *      \brief  Инициализирует драйвер sql соединения.
 *
 *      Для инициализации используются параметры из файла ресурсов бизнес схемы.
 *      \param dbParams - объект хранящий данные файла ресурсов.
 *      \return true, если драйвер успешно инициализирован. false, если возникла ошибка.
 * \_ru
 */
bool
aDatabase::prepareDatabaseConnect ( aCfgRc* dbParams )
{
        QString driver = "UNKNOWN";
        QString dbtype;

        dbtype = dbParams->value ( "dbtype" );
        done();

        /**/
        QString qds_dbtype = "QSQLITE";
        if ( dbtype=="internal" ) qds_dbtype = "QSQLITE";
        if ( dbtype=="mysql" )  qds_dbtype = "QMYSQL";
        if ( dbtype=="postgres" ) qds_dbtype = "QPOSTGRESQL";

        if ( qds==0 ) qds = new QDataSchema ( qds_dbtype, "ANANAS" );
        //qds->setNameSpace("qds_");

        qds->db()->setDatabaseName ( dbParams->value ( "dbname" ) );
        qds->db()->setUserName ( dbParams->value ( "dbuser" ) );
        qds->db()->setPassword ( dbParams->value ( "dbpass" ) );
        qds->db()->setHostName ( dbParams->value ( "dbhost","localhost" ) );
        qds->db()->setPort ( dbParams->value ( "dbport","0" ).toInt() );

        dataBase = qds->db();
        return true;
}



void
aDatabase::fillFeatures()
{
        featuresMySQL["autoincrement"] = "select last_insert_id()";
        featuresMySQL["encoding"] = "character set utf8";
        featuresMySQL["systemDatabase"] = "mysql";
        featuresPostgreSQL["autoincrement"] = "select currval('uniques_id_seq')";
        featuresPostgreSQL["encoding"] = "with encoding='UTF-8'";
        featuresPostgreSQL["systemDatabase"] = "template1";
        featuresSQLite["autoincrement"] = "select last_insert_rowid()";
        featuresSQLite["encoding"] = "";
}

/*!
 *      \~english
 *      Closes database connection.
 *      \~russian
 *      Закрывает соединение с базой данных и освобождает все занятые ресурсы.
 *      \~
 */
void
aDatabase::done()
{
        killTimer( v_timerId );
//        if ( db() )
//        {
//                db()->close();
//        }
        if ( qds )
        {
                delete qds;
                qds = 0;
        }
}



/**
 * \en
 *      \brief Returns database connection.
 *
 *      \param dbname (in) - database connection name. Default value used if dbname is empty.
 *      \return  database connection.
 * \_en \ru
 *      \brief Возвращает ссылку на соединение с базой данных.
 *
 *      \param dbname (in) - имя базы данных
 *      \return  соединение с базой данных. Возвращаемый объект удалять не следует.
 */
QSqlDatabase*
aDatabase::db ( const QString &dbname )
{
        if ( qds ) return qds->db();
        return 0;
}


/*!
 *      \~english
 *      Check database structure.
 *      \~russian
 *      проверяет структуру базы данных.
 *      \~
 *      \return  \~english true, if structure is valid. \~russian true, если база данных корректна \~
 */
bool
aDatabase::checkStructure()
{
        if ( qds )
        {
                if ( qds->verifyStructure() )
                {
                        return false;
                }
                else return true;
        }
        return false;
}



/*!
 *      \~english
 *      Helper method.
 *      \~russian
 *      Вспомогательный метод. Создает новую базу данных.
 *      \~
 *      \see createdb()
 */
bool
aDatabase::create()
{
        return createdb ( false );
}

bool
aDatabase::drop ( const QString& dbname )
{

        QString query = QString ( "drop database %1" ).arg ( dbname );
        if ( !dataBase ) return true;
        if ( dataBase->exec ( query ).lastError().type() ==QSqlError::None )
        {
                aLog::print ( aLog::Error,tr ( "aDatabase drop database %1" ).arg ( dbname ) );
        }
        query = QString ( "create database %1 %2" ).arg ( dbname ).arg ( feature ( "encoding" ) );
        //if ( driverName() == "QPSQL7" ) query.append( " with encoding='UTF-8'" );
//#ifdef MYSQL_UTF8
        //if ( driverName() == "QMYSQL3" ) query.append( " character set utf8" );
//#endif
        //printf("query = %s\n",query.ascii());
        QSqlQuery q = db()->exec ( query );
        if ( db()->lastError().type() !=QSqlError::None )
        {
                reportError ( db()->lastError(),query );
        }
        dataBase->setDatabaseName ( dbname );
        if ( !dataBase->open() )
        {
                cfg_message ( 3, ( const char * ) tr ( "Can't open database connection\n" ).utf8() );
                aLog::print ( aLog::Error,tr ( "aDatabase open connection to %1" ).arg ( dbname ) );
                return false;
        }
        else
        {
                aLog::print ( aLog::Info,tr ( "aDatabase open connection to %1" ).arg ( dbname ) );

        }
        return false;
}


/*!
 *      \~english
 *      Helper method.
 *      \~russian
 *      Вспомогательный метод. Обновляет структуру базы данных.
 *      \~
 *      \see createdb()
 */
bool
aDatabase::update()
{
        return createdb ( true );
}



/*!
 *      Creates aDataTable object for browsing and editing sql table in current database.
 *      \param name (in) - sql table name.
 *      \return newly created object.
 */
aDataTable *
aDatabase::table ( const QString & name )
{
        aDataTable *t = 0;

        if ( tableExists ( name ) )
        {
                t = new aDataTable ( qds->tableName ( name ), this );
        }
        else
        {
                t = new aDataTable ( QString::null, this );
        }
        return t;
}


/*!
 *      \~english
 *      Helper metod. Returns sql table name for metadata object.
 *      \~russian
 *      Вспомогательный метод. Возвращает имя sql таблицы для объекта метаданных.
 *      \~
 *      \see tableDbName( aCfg &md, aCfgItem context, long * tid )
 *      \param md (in) - \~english metadata tree object. \~russian дерево метаданных. \~
 *      \param context (in) - \~english node of metadata tree which represents metadata Class. \~russian объект метаданных \~
 *      \return \~english sql table name. \~russian имя таблицы в базе данных \~
 */
QString
aDatabase::tableDbName ( aCfg &md, aCfgItem context )
{
        long dummy;
        return tableDbName ( md,context,&dummy );
}


/*!
*       \~english
*       Returns sql table name for metadata object.
*       \~russian
*       Возвращает имя sql таблицы для объекта метаданных.
*       \~
*       \see tableDbName( aCfg &md, aCfgItem context, long * tid )
*       \param md (in) - \~english metadata tree object. \~russian дерево метаданных. \~
*       \param context (in) - \~english node of metadata tree which represents metadata Class. \~russian объект метаданных \~
*       \param tid (out) - \~english id of metadata Class. \~russian идентификатор объекта метаданных, которому принадлежит таблица \~
*       \return \~english sql table name. \~russian имя таблицы в базе данных \~
*/
QString
aDatabase::tableDbName ( aCfg &md, aCfgItem context, long * tid )
{
        aCfgItem parent = context;
        QString oclass, tname = "";
        long id, idp=0, temp=0;
        if ( context.isNull() ) return "";
        oclass = md.objClass ( context );
        id = md.id ( context );
        while ( idp == 0 && ! parent.isNull() )
        {
                parent = md.parent ( parent );
                idp = md.id ( parent );
        }
        if ( oclass == md_element )
        {
                tname = QString ( db_catalog_element ).arg ( idp );
                temp = idp;
        }
        if ( oclass == md_group )
        {
                tname =  QString ( db_catalog_group ).arg ( idp );
                temp = idp;
        }
        if ( oclass == md_header )
        {
                tname =  QString ( db_document_header ).arg ( idp );
                temp = idp;
        }
        if ( oclass == md_iregister )
        {
                tname = QString ( db_iregister ).arg ( id );
                temp = id;
        }
        if ( oclass == md_aregister )
        {
                tname = QString ( db_aregister ).arg ( id );
                temp = id;
        }
        if ( oclass == md_table )
        {
                if ( md.objClass ( parent ) == md_document )
                {
                        tname = QString ( db_document_table ).arg ( idp ).arg ( id );
                        temp = id;
                }
        }
        if ( oclass == md_resources )
        {
                if ( md.objClass ( parent ) == md_aregister )
                {
                        tname = QString ( db_areg_remainder ).arg ( idp );
                        temp = idp;
                }
        }
        if ( oclass == md_field )
        {
                if ( md.objClass ( parent ) == md_aregister )
                {
                        tname = QString ( db_areg_dim ).arg ( idp ).arg ( id );
                        temp = idp;
                }
        }
        *tid = temp;
//      printf("tname = %s\n",tname.ascii());
        return tname;
}



/*!
 *      \~english
 *      Generates new unique id for system object.
 *      \~russian
 *      Генерирует новый уникальный номер. Эти номера являются невосполнимым ресурсом системы.
 *      \~
 *      \param otype (in) - \~english object type (e.g. document, catalog, journal etc.) \~russian тип объекта \~
 *      \return \~english newly generated unique id. \~russian новый сгенерированный номер \~
 */
Q_ULLONG
aDatabase::uid ( int otype )
{
        Q_ULLONG uid = 0;
        QString query;
        QString drv = driverName();
//      printf("driver name =%s\n",drv.ascii());
        query.sprintf ( "insert into uniques (otype) values (%d)", otype );
        QSqlQuery q = db()->exec ( query );
        if ( db()->lastError().type() !=QSqlError::None )
        {
                reportError ( db()->lastError(),query );
        }
        query = feature ( "autoincrement" );
        q = db()->exec ( query );
        if ( db()->lastError().type() !=QSqlError::None )
        {
                reportError ( db()->lastError(),query );
        }
        if ( q.first() )
        {
                uid = q.value ( 0 ).toULongLong();
        }
        else
        {
                aLog::print ( aLog::Error,tr ( "aDatabase generate new unique number for object type %1" ).arg ( otype ) );
        }
//      printf("uid=%llu, otype=%d\n",uid,otype);
        aLog::print ( aLog::Debug,tr ( "aDatabase generate new unique number %1 for objecttype %2" ).arg ( uid ).arg ( otype ) );
        return uid;
}



/*!
 *      \~english
 *      Returns type of object identified by its uid.
 *      \~russian
 *      Возвращает тип объекта по его идентификатору.
 *      \~
 *      \see uid( int otype )
 *      \param uid (in) - \~english object's uid. \~russian уникальный идентификатор объекта \~
 *      \return \~english object's type \~russian тип объекта \~
 */
int
aDatabase::uidType ( Q_ULLONG uid )
{
        QSqlQuery q = db()->exec ( QString ( "SELECT otype FROM uniques WHERE id=%1" ).arg ( uid ) );
        if ( q.first() )
                return q.value ( 0 ).toInt();
        else
                aLog::print ( aLog::Error,tr ( "aDatabase get object type for unique id=%1" ).arg ( uid ) );
        return 0;
}



/*!
 *      \~english
 *      Checks table existence.
 *      \~russian
 *      Проверяет, существует ли таблица в базе данных.
 *      \~
 *      \param name (in) - \~english table name to check for. \~russian имя проверяемой таблицы \~
 *      \return \~english true, if table exists in current database \~russian true, если таблица существует \~
 */
bool
aDatabase::tableExists ( const QString & name )
{
        return qds->tableExists ( qds->tableName ( name ) );
}


/*!
 *
 */
bool
aDatabase::createdb ( bool update )
{
        bool rc = false;

        if ( qds )
        {
                qds_dd ( cfg );
                qds->setDataDictionary ( dd );
                if ( qds->verifyStructure() )
                {
                        // need to update
                        printf ( "verify log:\n%s\n", ( const char * ) qds->verifyLog().join ( "\n" ) );
                        printf ( "update structure query:\n%s\n", ( const char * ) qds->updateStructureQuery().join ( "\n" ) );
                        if ( qds->updateStructure() !=0 )
                        {
                                rc = false;
                                cfg_message ( 2, ( const char * ) tr ( "Data base update error\n" ) );
                        }
                        else
                        {
                                rc = true;
                                cfg_message ( 0, ( const char * ) tr ( "Data base update successfull\n" ) );
                        }
                }
        }
        return rc;
}



/*!
 *      \~english
 *      Sets delete flag to true in system table for \a uid.
 *      \~russian
 *      Устанавливает флаг удаления в true в системной таблице для заданного \a uid
 *      \~
 *      \see deleteMarked()
 *      \param uid - \~english id for mark deleted \~russian id для пометки на удаление \~
 */
void
aDatabase::markDeleted ( Q_ULLONG uid )
{
        db()->exec ( QString ( "UPDATE uniques SET df='1' WHERE id=%1" ).arg ( uid ) );
}


/*!
 *      \~english
 *      Delete all marked records.
 *      \~russian
 *      Удаляет все помеченные записи.
 *      \~
 *      \see markDeleted(Q_ULLONG uid)
 */
void
aDatabase::deleteMarked()
{
        db()->exec ( "DELETE FROM uniques WHERE df='1'" );
}


QStringList
aDatabase::supportedDriverList()
{
        QStringList l;
        l<<"internal";
        l<<"mysql";
        l<<"postgresql";
        return l;
}


bool
aDatabase::exchangeDataSystables ( QDomDocument &xml, bool import )
{
        bool res = false;
        if ( import )
        {
                qds->databaseImport ( "import.xml", false );
                res |= importTableData ( xml,"a_journ" );
                res |= importTableData ( xml,db_users );
                res |= importTableData ( xml,db_user_roles );
                res |= importTableData ( xml,db_roles );
                res |= importTableData ( xml,db_right_roles );
                res |= importTableData ( xml,db_indices );

        }
        else
        {
                qds->databaseExport ( "export.xml" );
                res |= exportTableData ( xml,"a_journ" );
                res |= exportTableData ( xml,db_users );
                res |= exportTableData ( xml,db_user_roles );
                res |= exportTableData ( xml,db_roles );
                res |= exportTableData ( xml,db_right_roles );
                res |= exportTableData ( xml,db_indices );
        }
        return res;
}

bool
aDatabase::exchangeDataUniques ( QDomDocument &xml, bool import )
{
        bool res = false;
        if ( import )
        {
                res |= importTableData ( xml,"uniques" );
        }
        else
        {
                res |= exportTableData ( xml,"uniques" );
        }
        return res;
}

bool
aDatabase::exchangeDataCatalogues ( QDomDocument &xml, bool import )
{
        aCfgItem gcont, cont, item;
        long id;
        int n, i;
        bool rc = false;

        gcont = cfg.find ( cfg.find ( mdc_metadata ), md_catalogues, 0 );
        n = cfg.count ( gcont, md_catalogue );
        for ( i = 0; i<n; i++ )
        {
                item = cfg.find ( gcont, md_catalogue, i );
                if ( !item.isNull() )
                {
//                      printf("cat = %s\n", (const char *) cfg.attr( item, mda_name ) );
                        id = cfg.id ( item );
                        cont = cfg.findChild ( item, md_element, 0 );
                        if ( !cont.isNull() )
                        {
                                if ( import )
                                {
                                        rc |= importTableData ( xml, tableDbName ( cfg, cont ) );
                                }
                                else
                                {
                                        rc|= exportTableData ( xml, tableDbName ( cfg, cont ) );
                                }
                        }
                        cont = cfg.findChild ( item, md_group, 0 );
                        if ( !cont.isNull() )
                        {
                                if ( import )
                                {
                                        rc |= importTableData ( xml, tableDbName ( cfg, cont ) );
                                }
                                else
                                {
                                        rc|= exportTableData ( xml, tableDbName ( cfg, cont ) );
                                }
                        }
                        cfg_message ( 0, ( const char * ) tr ( "Catalogue %s processed\n" ).utf8(), ( const char * ) cfg.attr ( item, mda_name ).utf8() );
                        if ( import )
                        {
                                aLog::print ( aLog::Info,tr ( "aDatabase catalogue %1 import" ).arg ( cfg.attr ( item, mda_name ) ) );
                        }
                        else
                        {
                                aLog::print ( aLog::Info,tr ( "aDatabase catalogue %1 export" ).arg ( cfg.attr ( item, mda_name ) ) );
                        }
                }
        }
        return rc;
//      return true;
}


bool
aDatabase::exchangeDataDocuments ( QDomDocument &xml, bool import )
{
        aCfgItem rcont, cont, item, tcont;
        int n, i, tn, ti;
        bool rc = false;

        rcont = cfg.find ( cfg.find ( mdc_metadata ), md_documents, 0 );
        n = cfg.count ( rcont, md_document );
        for ( i = 0; i<n; i++ )
        {
                item = cfg.find ( rcont, md_document, i );
                if ( !item.isNull() )
                {
                        //      printf("doc = %s\n", (const char *) cfg.attr( item, mda_name ) );
                        cont = cfg.findChild ( item, md_header, 0 );
                        if ( !cont.isNull() )
                        {
                                if ( import )
                                {
                                        rc |= importTableData ( xml, tableDbName ( cfg, cont ) );
                                }
                                else
                                {
                                        rc|= exportTableData ( xml, tableDbName ( cfg, cont ) );
                                }
                        }
                        tcont = cfg.find ( item, md_tables, 0 );
                        tn = cfg.count ( tcont, md_table );
                        for ( ti = 0; ti < tn; ti++ )
                        {
                                cont = cfg.findChild ( tcont, md_table, ti );
                                if ( !cont.isNull() )
                                {
                                        if ( import )
                                        {
                                                rc |= importTableData ( xml, tableDbName ( cfg, cont ) );
                                        }
                                        else
                                        {
                                                rc|= exportTableData ( xml, tableDbName ( cfg, cont ) );
                                        }
                                }
                        }
                }
                cfg_message ( 0, ( const char * ) tr ( "Document %s processed\n" ).utf8(), ( const char * ) cfg.attr ( item, mda_name ).utf8() );
                if ( import )
                {
                        aLog::print ( aLog::Info,tr ( "aDatabase document %1 import" ).arg ( cfg.attr ( item, mda_name ) ) );
                }
                else
                {
                        aLog::print ( aLog::Info,tr ( "aDatabase document %1 export" ).arg ( cfg.attr ( item, mda_name ) ) );
                }
        }
        return rc;
}


bool
aDatabase::exchangeDataJournals ( QDomDocument &xml, bool import )
{
        return false;
}


bool
aDatabase::exchangeDataInfoRegisters ( QDomDocument &xml, bool import )
{
        aCfgItem rcont, cont, item;
        int n, i;
        bool rc = false;
        QString filds;

        rcont = cfg.find ( cfg.find ( mdc_metadata ), md_iregisters, 0 );
        n = cfg.count ( rcont, md_iregister );
        for ( i = 0; i<n; i++ )
        {
                item = cfg.find ( rcont, md_iregister, i );
                if ( !item.isNull() )
                {

                        if ( import )
                        {
                                rc |= importTableData ( xml, tableDbName ( cfg, cont ) );
                        }
                        else
                        {
                                rc|= exportTableData ( xml, tableDbName ( cfg, cont ) );
                        }
                }
                cfg_message ( 0, ( const char * ) tr ( "Information registers %s processed\n" ).utf8(), ( const char * ) cfg.attr ( item, mda_name ).utf8() );
                if ( import )
                {
                        aLog::print ( aLog::Info,tr ( "aDatabase information register %1 import" ).arg ( cfg.attr ( item, mda_name ) ) );
                }
                else
                {
                        aLog::print ( aLog::Info,tr ( "aDatabase information register %1 export" ).arg ( cfg.attr ( item, mda_name ) ) );
                }
        }
        return rc;
}


bool
aDatabase::exchangeDataAccumulationRegisters ( QDomDocument &xml, bool import )
{
        aCfgItem rcont, cont, item, res, dim;
        int n, i;
        bool rc = false;
        QString filds;
        rcont = cfg.find ( cfg.find ( mdc_metadata ), md_aregisters, 0 );
        n = cfg.count ( rcont, md_aregister );
        for ( i = 0; i<n; i++ )
        {
                item = cfg.find ( rcont, md_aregister, i );
                if ( !item.isNull() )
                {
                        if ( import )
                        {
                                rc |= importTableData ( xml, tableDbName ( cfg, item ) );
                        }
                        else
                        {
                                rc|= exportTableData ( xml, tableDbName ( cfg, item ) );
                        }
                        res = cfg.find ( item, md_resources );
                        dim = cfg.find ( item, md_dimensions );
                        if ( !res.isNull() && !dim.isNull() )
                        {
                                aCfgItem d;
                                for ( uint k=0; k<cfg.count ( dim,md_field );k++ )
                                {
                                        d = cfg.findChild ( dim,md_field,k );
                                        if ( import )
                                        {
                                                rc |= importTableData ( xml, tableDbName ( cfg, d ) );
                                        }
                                        else
                                        {
                                                rc|= exportTableData ( xml, tableDbName ( cfg, d ) );
                                        }
                                }
                        }
                }
                cfg_message ( 0, ( const char * ) tr ( "Accumulation registers %s processed\n" ).utf8(), ( const char * ) cfg.attr ( item, mda_name ).utf8() );
                if ( import )
                {
                        aLog::print ( aLog::Info,tr ( "aDatabase accumulation register %1 import" ).arg ( cfg.attr ( item, mda_name ) ) );
                }
                else
                {
                        aLog::print ( aLog::Info,tr ( "aDatabase accumulation register %1 export" ).arg ( cfg.attr ( item, mda_name ) ) );
                }
        }
        return rc;
}


bool
aDatabase::exportTableData ( QDomDocument& xml, const QString &tableName )
{
        aDataTable *dataTable = table ( tableName );
        dataTable->select();
        QDomElement xml_root = xml.documentElement();
        if ( xml_root.isNull() )
        {
                aLog::print ( aLog::Error,tr ( "aDatabase export table: invalid XML file" ) );
//              printf("xml has no root element");
                return true;
        }
        QDomElement table = xml.createElement ( "table" );
        table.setAttribute ( "name",tableName );
        QDomElement row;
        QDomElement field;
        xml_root.appendChild ( table );
        if ( !dataTable->first() ) return false;
        do
        {
                uint countField = dataTable->count();
                QVariant val;
                row = xml.createElement ( "row" );
                table.appendChild ( row );
                for ( uint i=0; i<countField; i++ )
                {
                        val = dataTable->value ( i );
                        if ( val.isValid() )
                        {
                                field = xml.createElement ( "field" );
                                field.setAttribute ( "name",dataTable->fieldName ( i ) );
                                field.appendChild ( xml.createTextNode ( val.toString() ) );
                                row.appendChild ( field );
                        }
                }
        }
        while ( dataTable->next() );
        return false;
}


bool
aDatabase::importTableData ( QDomDocument &xml, const QString &tableName )
{
//      printf("import table data %s\n",tableName.ascii());
        QDomElement root = xml.documentElement();
        if ( root.nodeName() != "AnanasDump" )
        {

                aLog::print ( aLog::Error,tr ( "aDatabase import table: invalid XML file" ) );
                //printf("uncnown data format\n");
                return true;
        }
        QDomNodeList tables = root.childNodes();
        uint countTables = tables.count();
        for ( uint i=0; i<countTables; i++ )
        {
                //      printf("tableName==%s\n", tables.item(i).toElement().attribute("name").ascii());
                if ( tables.item ( i ).toElement().attribute ( "name" ) == tableName )
                {
                        // found table
                        aDataTable *tbl;
                        if ( !tableExists ( tableName ) )
                        {
                                aLog::print ( aLog::Error,tr ( "aDatabase import table: table %1 not exists" ).arg ( tableName ) );
                                //printf("table %s not exists\n",tableName.ascii());
                                return true;
                        }
                        //printf("table exists\n");
                        tbl = table ( tableName );
                        if ( !tbl )
                        {
                                aLog::print ( aLog::Error,tr ( "aDatabase import table: get table %1" ).arg ( tableName ) );
                                //printf("table is NULL\n");
                                return true;
                        }
                        QDomNodeList rows = tables.item ( i ).childNodes();
                        QDomNodeList fields;
                        uint countRows = rows.count();
                        uint countFields;
                        QSqlRecord *buffer;
                        QDomElement field;
                        QString query = "delete from uniques";
                        if ( tableName=="uniques" )
                        {
                                db()->exec ( query );
                        }
                        for ( uint j=0; j<countRows; j++ )
                        {
                                fields = rows.item ( j ).childNodes();
                                countFields = fields.count();
                                if ( tableName=="uniques" )
                                {
                                        // удаляем все строки в таблице
                                        if ( fields.item ( 0 ).toElement().hasChildNodes() && fields.item ( 1 ).toElement().hasChildNodes() )
                                        {
                                                query = QString ( "INSERT INTO %1 (id,otype) values(%2,%3)" )
                                                        .arg ( tableName )
                                                        .arg ( fields.item ( 0 ).toElement().toElement().text() )
                                                        .arg ( fields.item ( 1 ).toElement().toElement().text() );
                                                //              printf("query = %s\n",query.ascii());
                                                // добавляем записи с правильными идентификатораи
                                                db()->exec ( query );

                                        }
                                }
                                else
                                {
                                        buffer = tbl->primeInsert();
                                        for ( uint k=0; k<countFields; k++ )
                                        {
                                                field = fields.item ( k ).toElement();
                                                if ( field.hasChildNodes() )
                                                {
                                                        //      printf("node have child\n");

                                                        buffer->setValue ( field.attribute ( "name" ),field.toElement().text() );
                                                }
                                        }
                                        tbl->insert();
                                }
                        }
                        break;
                }
        }
        return false;
}

void
aDatabase::reportError ( QSqlError er, const QString &query )
{
        aLog::print ( aLog::Error, QObject::tr ( "aDatabase query: %1" ).arg ( query ) );
        aLog::print ( aLog::Error, QObject::tr ( "aDatabase text: %1" ).arg ( er.text() ) );
        aLog::print ( aLog::Error, QObject::tr ( "aDatabase database text: %1" ).arg ( er.databaseText() ) );
        aLog::print ( aLog::Error, QObject::tr ( "aDatabase driver text: %1" ).arg ( er.driverText() ) );
}

//start transaction
bool aDatabase::transaction()
{
        if ( dataBase )
                return dataBase->transaction();
        return false;
}

//commit transaction
bool aDatabase::commit()
{
        if ( dataBase )
                return dataBase->commit();
        return false;
}

//rollback transaction
bool aDatabase::rollback()
{
        if ( dataBase )
                return dataBase->rollback();
        return false;
}


QStringList
aDatabase::users()
{
        QStringList l;
        l << "user1";
        l << "user2";
        l << "user3";
        return l;
}


int
aDatabase::loginUsersCount()
{
        QSqlQuery q = db()->exec ( QString ( "SELECT users FROM netusers" ) );
        if ( q.first() ) return q.value ( 0 ).toInt();
        else aLog::print ( aLog::Error,tr ( "aDatabase get users count" ) );
        return 0;
}


bool
aDatabase::login ( const QString &username, const QString &password, int applicationId )
{
        QSqlQuery q, q1;
        int count = 0, role_id, md_id, perm, isActive = 0;
        bool userOk = false;

        v_user_id = 0;
        v_app_id = applicationId;

        if ( applicationId < 0 || applicationId > 3 ) applicationId = 0;
        q = db()->exec ( QString ( "SELECT count(*) FROM %1" ).arg ( qds->tableName ( db_users ) ) );
        if ( q.first() ) count = q.value ( 0 ).toInt();
        if ( !count )
        {
                userOk = true;
        }
        else
        {
                q = db()->exec ( QString ( "SELECT id,active%4 FROM %1 WHERE login='%2' AND password='%3'" ).arg ( qds->tableName ( db_users ) ).arg ( username ).arg ( password ).arg ( applicationId ) );
                if ( q.first() )
                {
                        v_user_id = q.value ( 0 ).toInt();
                        isActive = q.value ( 1 ).toInt();
                        userOk = true;
                }
                else aLog::print ( aLog::Error,tr ( "aDatabase get users count" ) );
        }
        if ( userOk )
        {
            if ( isActive !=0 && applicationId !=AApplication::Administrator ){
                QMessageBox::information ( 0, "Ananas",tr ( "User already logged in" ), 0, 0, 0 );
//                return false;
            }
                db()->exec ( QString ( "UPDATE %1 SET users=users+1" ).arg ( qds->tableName ( "netusers" ) ) );
                if ( count )
                {
                        db()->exec ( QString ( "UPDATE %1 SET active%2=1 WHERE id=%3" ).arg ( qds->tableName ( db_users ) ).arg ( applicationId ).arg ( v_user_id ) );
                }
                printf ( "login user id = %i OK\n", v_user_id );

// Считаем набор прав пользователя
                q = db()->exec ( QString ( "SELECT idr FROM %1 WHERE id=%2" ).arg ( qds->tableName ( db_user_roles ) ).arg ( v_user_id ) );
                while ( q.next() )
                {
                        role_id = q.value ( 0 ).toInt();
                        q1 = db()->exec ( QString ( "SELECT object,permission FROM %1 WHERE id=%2" ).arg ( qds->tableName ( db_right_roles ) ).arg ( role_id ) );
                        while ( q1.next() )
                        {
                                md_id = q1.value ( 0 ).toInt();
                                perm  = q1.value ( 1 ).toInt();
                                if ( accessRights.contains ( md_id ) )
                                {
                                        accessRights[ md_id ] |= perm;
                                }
                                else accessRights[ md_id ] = perm;
                        }
                }
                QMap<int,int>::Iterator it;
                for ( it = accessRights.begin(); it != accessRights.end(); ++it )
                {
                        printf ( "P %i:%04x\n",
                                 it.key(),
                                 it.data() );
                }

                return true;
        }
        return false;
}


void
aDatabase::logout()
{
        db()->exec ( QString ( "UPDATE %1 SET users=CASE WHEN users>0 THEN users-1 ELSE 0 END" ).arg ( qds->tableName ( "netusers" ) ) );
        objectUnlock ( 0 );
        db()->exec ( QString ( "UPDATE %1 SET active%2=0 WHERE id=%3" ).arg ( qds->tableName ( db_users ) ).arg ( v_app_id ).arg ( v_user_id ) );
        if ( !loginUsersCount() )
        {
                db()->exec ( QString ( "DELETE FROM %1" ).arg ( qds->tableName ( "locks" ) ) );
        }
}


void
aDatabase::netupdate()
{
        printf ( "UPDATE++\n" );
        db()->exec ( QString ( "UPDATE %1 SET updates=updates+1" ).arg ( qds->tableName ( "netusers" ) ) );
        v_updatesCount++;
}



bool
aDatabase::isObjectLocked ( Q_ULLONG id )
{
        if ( !id ) return false;
        QSqlQuery q = db()->exec ( QString ( "SELECT id FROM %1 WHERE userid=%2 AND id=%3" ).arg ( qds->tableName ( "locks" ) ).arg ( v_user_id ).arg ( id ) );
        if ( q.first() )
        {
                QMessageBox::information ( 0, "Ananas",tr ( "Object locked by another user" ), 0, 0, 0 );
                return true;
        }
        return false;
}


bool
aDatabase::objectLock ( Q_ULLONG id )
{
        if ( !id ) return true;
//    printf("LOCK user=%i obj=%Lu\n", v_user_id, id);
        db()->exec ( QString ( "INSERT INTO %1 (userid,id) VALUES (%2,%3)" ).arg ( qds->tableName ( "locks" ) ).arg ( v_user_id ).arg ( id ) );
        if ( db()->lastError().type() ==0 ) return true;
        QMessageBox::information ( 0, "Ananas",tr ( "Object locked by another user" ), 0, 0, 0 );
        return false;
}

void
aDatabase::objectUnlock ( Q_ULLONG id )
{
//    printf("UNLOCK user=%i obj=%Lu\n", v_user_id, id);
        QString query ( QString ( "DELETE FROM %1 WHERE userid=%2" ).arg ( qds->tableName ( "locks" ) ).arg ( v_user_id ) );
        if ( id ) query.append ( QString ( " AND id=%1" ).arg ( id ) );
        db()->exec ( QString ( query ) );
}


aDatabase *
aDatabase::database()
{
        return &aDatabase_default;
}


int
aDatabase::updatesCount()
{
        QSqlQuery q = db()->exec ( QString ( "SELECT updates FROM %1" ).arg ( qds->tableName ( "netusers" ) ) );
        if ( q.first() ) return q.value ( 0 ).toInt();
        return 0;
}



void
aDatabase::timerEvent ( QTimerEvent * )
{
        int uc;
	if ( !db()->isOpen() ) return;
        uc = updatesCount();
        if ( uc > v_updatesCount )
        {
                v_updatesCount = uc;
                emit refresh();
//        printf( "EMIT refresh\n" );
        }
}



bool
aDatabase::isAccessRights ( int md_type, Q_ULLONG obj_id, DBPermission req_operation )
{
        bool res = false;

        if ( accessRights.contains ( md_type ) )
        {
                res = ( accessRights[ md_type ] & req_operation ) != 0;
        }
        else res = true;
        if ( ! res )
        {
                QMessageBox::information ( 0, "Ananas",tr ( "No access rights" ), 0, 0, 0 );
        }
        return res;
}


int
aDatabase::rolePermission ( int role_id, int md_id )
{
        int perm = 0;

        QSqlQuery q = db()->exec ( QString ( "SELECT permission FROM %1 WHERE id=%2 AND object=%3" ).arg ( qds->tableName ( db_right_roles ) ).arg ( role_id ).arg ( md_id ) );
        if ( q.first() ) perm = q.value ( 0 ).toInt();
        else perm = DBP_All;
        return perm;
}


void
aDatabase::setRolePermission ( int role_id, int md_id, int new_permission )
{
        QString query;

        printf ( "SET PERMISSIONS role = %i, obj_id=%i perm=%08x\n", role_id, md_id, new_permission );

        QSqlQuery q = db()->exec ( QString ( "SELECT permission FROM %1 WHERE id=%2 AND object=%3" ).arg ( qds->tableName ( db_right_roles ) ).arg ( role_id ).arg ( md_id ) );
        if ( q.first() ) query = QString ( "UPDATE %1 SET permission=%4 WHERE id=%2 AND object=%3" ).arg ( qds->tableName ( db_right_roles ) ).arg ( role_id ).arg ( md_id ).arg ( new_permission );
        else query = QString ( "INSERT INTO %1 (id,object,permission) VALUES (%2,%3,%4)" ).arg ( qds->tableName ( db_right_roles ) ).arg ( role_id ).arg ( md_id ).arg ( new_permission );
        q = db()->exec ( query );
        if ( db()->lastError().type() !=0 ) printf ( "error query %s\n", ( const char* ) query );
}
