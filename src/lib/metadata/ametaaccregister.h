/****************************************************************************
** $Id: ametaaccregister.h,v 1.1 2008/12/24 16:19:38 leader Exp $
**
** Header file of the Ananas configuration objects of Ananas
** Designer and Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader AccTech.  All rights reserved.
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
#ifndef AMETAACCREGISTER_H
#define AMETAACCREGISTER_H

#include "ametaobject.h"
#include "ametaobjectgroup.h"
#include "ametafield.h"
#include "ametaform.h"


class AMetaAccRegister: public AMetaObject
{
Q_OBJECT
public:
    AMetaAccRegister( AMetaObject *parent = 0 );

public slots:
    AMetaFields      *fields();
    AMetaFields      *gfields();
    AMetaForms       *forms();

private:
    AMetaFields      v_fields;
    AMetaFields      v_gfields;
    AMetaForms       v_forms;
};



class AMetaAccRegisters: public AMetaGroup
{
Q_OBJECT
public:
    AMetaAccRegisters();

    AMetaAccRegister *newAccRegister(){ return new AMetaAccRegister( this );};
    AMetaAccRegister *accReister( const QString &name ){ return (AMetaAccRegister*) child( name );};
    AMetaAccRegister *accRegister( int idx ){ return (AMetaAccRegister*) child( idx );};
    int accRegisterCount(){ return childCount(); };

public slots:


};

#endif
