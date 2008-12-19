/****************************************************************************
** $Id: atemplate.cpp,v 1.2 2008/11/08 20:16:36 leader Exp $
**
** Report metadata object header file of
** Ananas application library
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
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

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <qfile.h>
#include <qdir.h>
#include <QTextStream>

#include "atemplate.h"
#include "alog.h"

aTemplate::aTemplate()
    : iTemplate()
{
    clear();
}



aTemplate::~aTemplate()
{
}



bool
aTemplate::open( const QString &fname )
{
	QFile file( QDir::convertSeparators(QDir::currentDirPath()+"/"+templateDir+"/"+fname) );
    if ( file.open( QIODevice::ReadOnly ) )
    {
        QTextStream stream( &file );
        tpl = stream.read();
        file.close();
	return true;
    }
    else
    {
	    return false;
    }
}



void
aTemplate::close()
{
    freeValues();
}



QString
aTemplate::getValue( const QString &name )
{
	QString * pStr=values.find( name );
	return pStr==0?(QString::null):*pStr;
}



void
aTemplate::setValue( const QString &name, const QString &value )
{
    values.remove( name );
    if ( !value.isEmpty() ) values.insert( name, new QString( value ) );
}



void
aTemplate::freeValues()
{
    values.clear();
}


QString
aTemplate::exec( const QString &sname )
{
    int c = 0, c1 = 0, sec_start = 0, sec_end = 0, sec_len = 0, sfound = 0, l;
    QString token, tname, tparam, sec_buf = "", v;

    if ( tpl.isEmpty() ) return "";

    while ( !sfound ) {
	sec_len = 0;
	c = tpl.find( token_open, c );
	if ( c >= 0 ) {
	    c += strlen( token_open );
	    c1 = tpl.find( token_close, c );
	    if ( c1 >= 0 ) {
		l = c1 - c;
		token = tpl.mid( c, l );
		tname = token.section( " ", 0, 0 );
		tparam = token.section( " ", 1, 1 );
		if ( tname == "section" && tparam == sname ) {
		    sec_end = tpl.find( QString(token_open"endsection"token_close), c1 );
		    if ( sec_end > 0 ) {
			sfound = 1;
			sec_start = c1 + QString( token_close ).length();
			sec_len = sec_end - sec_start;
		    } else break;
		}
	    } else break;
	} else break;
    }

    if ( sfound && sec_len ) {
	c = sec_start;
	while ( c < sec_end ) {
	    c1 = tpl.find( token_open, c );
	    l = c1 - c;
	    if ( l ) sec_buf.append( tpl.mid( c, l ) );
	    if ( c1 < sec_end ) {
		c = c1 + QString( token_open ).length();
		c1 = tpl.find( token_close, c );
		if ( c1 ) {
		    l = c1 - c;
		    token = tpl.mid( c, l );
    		    tname = token.section( " ", 0, 0 );
		    tparam = token.section( " ", 1, 1 );
		    c1 += QString( token_close ).length();
		    v = getValue( tname );
		    if ( !v.isEmpty() ) sec_buf.append( v );
		} else break;
	    }
	    c = c1;
	}
    }
    buf.append( sec_buf );
//	printf("%s",( const char *) sec_buf.local8Bit() );
    return sec_buf;
}



bool
aTemplate::save( const QString & fname)
{
	QFile file( QDir::convertSeparators(QDir::currentDirPath()+"/"+templateDir+"/"+fname) );
	if ( file.open( QIODevice::WriteOnly ) )
	{
		QTextStream stream( &file );
		stream << result();
		file.close();
		aLog::print(aLog::Info, tr("aTemplate save file %1").arg(file.name()));
		return true;
	}
	else
	{
		aLog::print(aLog::Error, tr("aTemplate save file %1").arg(file.name()));
		return false;
	}
}



void
aTemplate::cleanUpTags()
{
}



void
aTemplate::setDir(const QString &dir)
{
	templateDir = dir;
}



QString
aTemplate::getDir()
{
	return templateDir;
}

