/****************************************************************************
** $Id: ametaobject.h,v 1.2 2008/12/15 22:22:48 leader Exp $
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

#include <QHash>
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
                 AMetaObjectGroup *parent = 0 );

    void setClassName( const QString &name );

    Q_ULONG id();
    void setId( Q_ULONG );
    QString className();
    QString name();

    QString description();
    void setDescription( const QString &descr );

    void setAttr( const QString &name, const QVariant &value );
    void setText( const QString &name, const QString &text );
    void setRawdata( const QString &name, const QByteArray &data );

    QVariant attr( const QString &name ) const;
    QString text( const QString &name ) const;
    QByteArray rawdata( const QString &name ) const;

    QVariant attr( int idx ) const;
    QString text( int idx ) const;
    QByteArray rawdata( int idx ) const;

    QString attrName( int idx ) const;
    QString textName( int idx ) const;
    QString rawdataName( int idx ) const;

    bool attrExists( const QString &name );
    bool textExists( const QString &name );
    bool rawdataExists( const QString &name );

    int attrCount() const;
    int textCount() const;
    int rawdataCount() const;

public slots:

private:
    Q_ULONG v_id;
    QString v_className;
    QHash <QString, QVariant> v_attr;
    QHash <QString, QString> v_text;
    QHash <QString, QByteArray> v_rawdata;
};



class AMetaCatalogue: public AMetaObject
{
Q_OBJECT
public:
    AMetaCatalogue();
};



class AMetaDataInfo: public AMetaObject
{
Q_OBJECT
Q_PROPERTY( QString appname READ appName SCRIPTABLE true )
Q_PROPERTY( QString author  READ author  SCRIPTABLE true )
Q_PROPERTY( QDate   date    READ date    SCRIPTABLE true )
public:
    AMetaDataInfo();
    AMetaDataInfo( const AMetaDataInfo &info );
    ~AMetaDataInfo();
    AMetaDataInfo& operator=(const AMetaDataInfo&);

    QString appName();
    void setAppName( const QString &name );
    long lastId();
    void setLastId( long id );
    QString author();
    void setAuthor( const QString &name );
    QDate date();
    void setDate( QDate d );
};


class AMetaDataGlobal: public AMetaObject
{
Q_OBJECT
public:
    AMetaDataGlobal();

    QString sourceCode();
    void setSourceCode( const QString &src );

};

Q_DECLARE_METATYPE ( AMetaDataInfo )

//qRegisterMetaType<AMetaDataInfo>("MetaDataInfo");
//Q_DECLARE_METATYPE ( AMetaDataGlobal )

#endif
