/****************************************************************************
** $Id: ametadata.cpp,v 1.3 2008/12/14 10:53:02 leader Exp $
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

#include "ametadata.h"

#define context_startid	100

AMetaData AMetaData_default;

/*!
 * \class AMetaData
 *
 */
AMetaData::AMetaData()
:AMetaObjectGroup( "MetaData", (AMetaObjectGroup *) 0 )
{
/*
    int i = qRegisterMetaType<AMetaDataInfo>("AMetaDataInfo");
printf("metatype=%i\n", i);

int id = QMetaType::type("AMetaDataInfo");
 if (id != -1) {
     void *myClassPtr = QMetaType::construct(id);
     if (myClassPtr) printf("OK\n");

     QMetaType::destroy(id, myClassPtr);
     myClassPtr = 0;
 }
*/
}


AMetaData* 
AMetaData::metadata()
{
    return &AMetaData_default;
}


void
AMetaData::clear()
{

}



ACataloguesGroup *
AMetaData::catalogues()
{
    return &v_catalogues;
}



ADocumentsGroup *
AMetaData::documents()
{
    return &v_documents;
}


AReportsGroup *
AMetaData::reports()
{
    return &v_reports;
}


AJournalsGroup *
AMetaData::journals()
{
    return &v_journals;
}


AInfoRegistersGroup *
AMetaData::inforegisters()
{
    return &v_inforegisters;
}



AAccRegistersGroup *
AMetaData::accregisters()
{
    return &v_accregisters;
}

