/****************************************************************************
** $Id: ametadataio.cpp,v 1.2 2008/12/13 22:19:15 leader Exp $
**
** Code file of the Ananas configuration objects of Ananas
** Designer and Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) Grigory Panov <gr1313 at mail.ru>, Yoshkar-Ola.
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

#include <qdatetime.h>
#include <qobject.h>
#include <qfile.h>
#include <qstringlist.h>
#include <QTextStream>
#include <QDomDocument>

#include "ametadataio.h"

/*!
 * \class AMetaDataIO
 *
 */
AMetaDataIO::AMetaDataIO()
:QObject()
{
}


AMetaDataIO::~AMetaDataIO()
{
}


int
AMetaDataIO::read(  const QString &name, AMetaData *md )
{
    if ( !md ) md = AMetaData::metadata();
}


int
AMetaDataIO::write(  const QString &name, AMetaData *md )
{
    if ( !md ) md = AMetaData::metadata();
}


QObject *
AMetaDataIO::createObject( const QString &name )
{
   QObject *o = 0;
   if ( name == "AMetaObject" ) return new AMetaObject();
//   if ( name == "AMetaObjectGroup" ) return new AMetaObjectGroup();

   return o;
} 
