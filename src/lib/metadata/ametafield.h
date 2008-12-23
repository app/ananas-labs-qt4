/****************************************************************************
** $Id: ametafield.h,v 1.2 2008/12/20 21:17:49 leader Exp $
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
#ifndef AMETAFIELD_H
#define AMETAFIELD_H

#include "ametaobject.h"
#include "ametaobjectgroup.h"


class AMetaField: public AMetaObject
{
Q_OBJECT
Q_ENUMS( FieldTypes )
Q_PROPERTY( int  fieldtype    READ fieldType    SCRIPTABLE true )
Q_PROPERTY( int  fieldsubtype READ fieldSubType SCRIPTABLE true )
Q_PROPERTY( int  width        READ width        SCRIPTABLE true )
Q_PROPERTY( int  decimals     READ decimals     SCRIPTABLE true )
Q_PROPERTY( bool notnull      READ notNull      SCRIPTABLE true )
Q_PROPERTY( bool notnegative  READ notNegative  SCRIPTABLE true )
Q_PROPERTY( bool calcsumm     READ calcSumm     SCRIPTABLE true )

public:
    enum FieldTypes { Unknown, Char, String, Number, Date, Object, Document, Catalogue };
    AMetaField( AMetaObject *parent = 0 );

/*    AMetaField( FieldTypes t, int st = 0, int w = 0, 
                int d = 0, bool notnul = false, bool notneg = false, 
                bool calcsum = false );
*/
    int fieldType();
    void setFieldType( FieldTypes t );

    int fieldSubType();
    void setFieldSubType( int st );

    int width();
    void setWidth( int w );

    int decimals();
    void setDecimals( int d );

    bool notNull();
    void setNotNull( bool f );

    bool notNegative();
    void setNotNegative( bool f );

    bool calcSumm();
    void setCalcSumm( bool f );

private:

};


class AMetaFields: public AMetaGroup
{
Q_OBJECT
public:
    AMetaFields();
    AMetaField * newField(){ return new AMetaField( this );};
    AMetaField *field( const QString &name ){ return (AMetaField*) child( name );};
    AMetaField *field( int idx ){ return (AMetaField*) child( idx );};
    int fieldCount(){ return childCount(); };

public slots:


};

#endif
