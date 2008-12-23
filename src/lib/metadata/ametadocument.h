/****************************************************************************
** $Id: ametadocument.h,v 1.2 2008/12/20 21:17:49 leader Exp $
**
** Header file of the Ananas configuration objects of Ananas
** Designer and Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) Andrey Paskal <app at lrn dot ru>, Yoshkar-Ola
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
#ifndef AMETADOCUMENT_H
#define AMETADOCUMENT_H

#include "ametaobject.h"
#include "ametaobjectgroup.h"
#include "ametafield.h"
#include "ametaform.h"


class AMetaDocument: public AMetaObject
{
Q_OBJECT
public:
    AMetaDocument( AMetaObject *parent = 0 );

public slots:
    AMetaFields      *fields();
    AMetaTables      *tables();
    AMetaForms       *forms();

private:
    AMetaFields      v_fields;
    AMetaTables      v_tables;
    AMetaForms       v_forms;
};



class ADocumentsGroup: public AMetaGroup
{
Q_OBJECT
public:
    ADocumentsGroup();

    AMetaDocument *newDocument(){ return new AMetaDocument( this );};
    AMetaDocument *document( const QString &name ){ return (AMetaDocument*) child( name );};
    AMetaDocument *document( int idx ){ return (AMetaDocument*) child( idx );};
    int documentCount(){ return childCount(); };

public slots:
    QString test(){ return QString("DOCUMENTS TEST STRING");};


};

#endif
