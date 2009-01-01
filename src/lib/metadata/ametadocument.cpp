/****************************************************************************
** $Id: ametadocument.cpp,v 1.3 2008/12/24 16:19:38 leader Exp $
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
#include "ametadocument.h"

/*!
 * \class AMetaDocument
 *
 */
AMetaDocument::AMetaDocument( AMetaObject *parent )
:AMetaObject("Document", "", parent )
{
    setId( lastId() );
    setName( QString("%1_%2").arg( tr("Document") ).arg( id() ) );
    addChild( &v_fields );
    addChild( &v_tables );
    addChild( &v_forms );

}


AMetaFields *
AMetaDocument::fields()
{
    return &v_fields;
}



AMetaTables *
AMetaDocument::tables()
{
    return &v_tables;
}


AMetaForms *
AMetaDocument::forms()
{
    return &v_forms;
}



/*!
 * \class ADocumentsGroup
 *
 */
AMetaDocuments::AMetaDocuments()
:AMetaGroup("Documents")
{

}

