/****************************************************************************
** $Id: ametaobjectgroup.h,v 1.2 2008/12/15 22:22:48 leader Exp $
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
#ifndef AMETAOBJECTGROUP_H
#define AMETAOBJECTGROUP_H

#include <QObject>
#include <QDate>
#include <QPointer>

#include "ametaobject.h"

#include <QHash>
#include <QVariant>


/*!
 *\en
 *	Metadata object for metadata script access.
 *\_en
 *\ru
 *	\brief Объект метаданных для обеспечения доступа из скриптов.
 *	Наследует QObject.
 *\_ru
 */
class AMetaObjectGroup: public QObject
{
Q_OBJECT
Q_PROPERTY( QString name READ name SCRIPTABLE true )
public:
    AMetaObjectGroup( const QString &groupname );
    AMetaObjectGroup( const QString &groupname, AMetaObject *parent );
    AMetaObjectGroup( const QString &groupname, AMetaObjectGroup *parent );

    QString name();
//        QMetaObject *metaObject(const QString &objClass, const QString &objName);
//    QMetaObject *metaObject(const QString &objClass, const QString &objName);
    AMetaObject *metaObject( const QString &name );
    AMetaObject *metaObject( int idx );
    void append( AMetaObject *object );
    int count() const;
    void clear();

public slots:
    QString test(){ return QString("TEST STRING");};

private slots:
    void destroyed( QObject *obj );

private:
    QList <AMetaObject *> v_metaObjects;
};


class ACataloguesGroup: public AMetaObjectGroup
{
Q_OBJECT
public:
    ACataloguesGroup();

public slots:


};



class ADocumentsGroup: public AMetaObjectGroup
{
Q_OBJECT
public:
    ADocumentsGroup();

public slots:
    QString test(){ return QString("DOCUMENTS TEST STRING");};


};



class AReportsGroup: public AMetaObjectGroup
{
Q_OBJECT
public:
    AReportsGroup();

public slots:


};



class AJournalsGroup: public AMetaObjectGroup
{
Q_OBJECT
public:
    AJournalsGroup();

public slots:


};



class AInfoRegistersGroup: public AMetaObjectGroup
{
Q_OBJECT
public:
    AInfoRegistersGroup();

public slots:


};



class AAccRegistersGroup: public AMetaObjectGroup
{
Q_OBJECT
public:
    AAccRegistersGroup();

public slots:


};



class AMetaTables: public AMetaObjectGroup
{
Q_OBJECT
public:
    AMetaTables();

public slots:


};



class AMetaForms: public AMetaObjectGroup
{
Q_OBJECT
public:
    AMetaForms();

public slots:


};

#endif
