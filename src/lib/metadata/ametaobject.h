/****************************************************************************
** $Id: ametaobject.h,v 1.7 2008/12/24 20:06:51 leader Exp $
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
#ifndef AMETAOBJECT_H
#define AMETAOBJECT_H

#include <QObject>
#include <QDate>

#ifdef __BORLANDC__
#define CHECK_POINT 	printf("%s:%i %s()\n",__FILE__,__LINE__,__FUNC__);
#else
#define CHECK_POINT 	printf("%s:%i %s()\n",__FILE__,__LINE__,__FUNCTION__);
#endif

/*!
 * \~english Metadata object methods return codes. \~russian
 * Коды возвратов методов объекта метаданных. \~
 */
#define RC_OK		0	/**< All right - no errors */
#define RC_ERROR	1	/**< Error while executing method */

//#include <QHash>
#include <QMap>
#include <QVariant>
class AMetaObjectGroup;

class AMetaObject: public QObject
{
Q_OBJECT
// system properties
Q_PROPERTY( Q_ULONG id READ id SCRIPTABLE true )
Q_PROPERTY( QString name READ name SCRIPTABLE true )
Q_PROPERTY( QString className READ className SCRIPTABLE true )
Q_PROPERTY( QString description READ description SCRIPTABLE true )

public:
    AMetaObject( const QString &objectclass  = QString::null, 
                 const QString &objectname = QString::null, 
                 AMetaObject *parent = 0 );

    ~AMetaObject();
    void setClassName( const QString &name );

    Q_ULONG id();
    void setId( Q_ULONG );
    QString className();
    QString name();
    AMetaObject *rootObject();
    AMetaObject *parentMetaObject();
    void setParentMetaObject( AMetaObject * parent );

    QString description();
    void setDescription( const QString &descr );

    void setAttr( const QString &name, const QVariant &value );
    QVariant attr( const QString &name ) const;
    QVariant attr( int idx ) const;
    QString attrName( int idx ) const;
    bool attrExists( const QString &name );
    int attrCount() const;


    int childCount();
    AMetaObject *child( int idx );
    AMetaObject *child( const QString &name );
    void addChild( AMetaObject * c );
    void removeChild( AMetaObject * c );

    static int lastId();
    static void setLastId( int id );
    static QString  varToStr( QVariant v );
    static QVariant strToVar( const QString &str, QVariant::Type t );
 
public slots:

private:
    static int v_lastId;
    AMetaObject *v_parent;
    Q_ULONG v_id;
    QString v_className;
    QString v_description;
    QMap <QString, QVariant> v_attr;
    QMap <QString, AMetaObject *> v_child;
};


class AMetaGroup: public AMetaObject
{
Q_OBJECT
public:
    AMetaGroup( const QString &groupname, AMetaObject *parent = 0 )
    :AMetaObject( groupname, QString::null, parent ){};
};



template<class T>
class  AMetaGroupA : public AMetaGroup
{
//    Q_OBJECT
public:

    AMetaGroupA( AMetaObject * parent = 0 )
    :AMetaGroup( "", parent )
    { 
    };
};

typedef AMetaGroupA<AMetaObject> AMetaGroupAXXX;

#endif
