/****************************************************************************
** $Id: ametareport.h,v 1.1 2008/12/24 16:19:38 leader Exp $
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
#ifndef AMETAREPORT_H
#define AMETAREPORT_H

#include "ametaobject.h"
#include "ametaobjectgroup.h"
#include "ametafield.h"
#include "ametaform.h"


class AMetaReport: public AMetaObject
{
Q_OBJECT
public:
    AMetaReport( AMetaObject *parent = 0 );

public slots:
    AMetaFields      *fields();
    AMetaTables      *tables();
    AMetaForms       *forms();

private:
    AMetaFields      v_fields;
    AMetaTables      v_tables;
    AMetaForms       v_forms;
};



class AMetaReports: public AMetaGroup
{
Q_OBJECT
public:
    AMetaReports();

    AMetaReport *newReport(){ return new AMetaReport( this );};
    AMetaReport *report( const QString &name ){ return (AMetaReport*) child( name );};
    AMetaReport *report( int idx ){ return (AMetaReport*) child( idx );};
    int reportCount(){ return childCount(); };

public slots:


};

#endif
