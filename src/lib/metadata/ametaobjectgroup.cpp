/****************************************************************************
** $Id: ametaobjectgroup.cpp,v 1.2 2008/12/15 22:22:48 leader Exp $
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
//--#include <q3textstream.h>
#include <QTextStream>
#include <QDomDocument>

#include "ametaobject.h"
#include "ametaobjectgroup.h"

#define context_startid	100


/*!
 * \class AMetaObjectGroup
 *
 */
AMetaObjectGroup::AMetaObjectGroup( const QString &groupname )
:QObject()
{
    setObjectName( groupname );
}




AMetaObjectGroup::AMetaObjectGroup( const QString &groupname, AMetaObject *parent )
:QObject( parent )
{
    setObjectName( groupname );
}


AMetaObjectGroup::AMetaObjectGroup( const QString &groupname, AMetaObjectGroup *parent )
:QObject( parent )
{
    setObjectName( groupname );
}


QString
AMetaObjectGroup::name()
{
    return objectName();
}



AMetaObject *
AMetaObjectGroup::metaObject( const QString &name )
{
    return findChild<AMetaObject *>( name );
}



AMetaObject *
AMetaObjectGroup::metaObject( int idx )
{
    return v_metaObjects.at( idx );
}


void 
AMetaObjectGroup::append( AMetaObject *obj )
{
    obj->setParent( this );
    v_metaObjects.append( obj );
    connect( obj, SIGNAL(destroyed(QObject*)), this, SLOT(destroyed(QObject *)));
}


int 
AMetaObjectGroup::count() const
{
    return v_metaObjects.count();
}


void 
AMetaObjectGroup::clear()
{
    v_metaObjects.clear();
}


void 
AMetaObjectGroup::destroyed( QObject *obj )
{
  if ( obj->inherits("AMetaObject") ){
      v_metaObjects.removeAll( ( AMetaObject *) obj );
  }
}



/*!
 * \class ADocumentsGroup
 *
 */
ADocumentsGroup::ADocumentsGroup()
:AMetaObjectGroup("Documents")
{

}



/*!
 * \class ACataloguesGroup
 *
 */
ACataloguesGroup::ACataloguesGroup()
:AMetaObjectGroup("Catalogues")
{

}



/*!
 * \class AReportsGroup
 *
 */
AReportsGroup::AReportsGroup()
:AMetaObjectGroup("Reports")
{

}



/*!
 * \class AReportsGroup
 *
 */
AJournalsGroup::AJournalsGroup()
:AMetaObjectGroup("Journals")
{

}



/*!
 * \class AInformationRegistersGroup
 *
 */
AInfoRegistersGroup::AInfoRegistersGroup()
:AMetaObjectGroup("InfoRegisters")
{

}



/*!
 * \class AInformationRegistersGroup
 *
 */
AAccRegistersGroup::AAccRegistersGroup()
:AMetaObjectGroup("AccRegisters")
{

}



/*!
 * \class AMetaTables
 *
 */
AMetaTables::AMetaTables()
:AMetaObjectGroup("MetaTables")
{

}



/*!
 * \class AMetaForms
 *
 */
AMetaForms::AMetaForms()
:AMetaObjectGroup("MetaForms")
{

}
