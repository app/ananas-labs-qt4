/****************************************************************************
** $Id: aextte.h,v 1.1 2008/11/05 21:16:28 leader Exp $
**
** Extension object header file of
** Ananas application library
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) 2003-2005 Grigory Panov, Yoshkar-Ola.
**
** This file is part of the Designer application of the Ananas
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

#ifndef AEXTTE_H
#define AEXTTE_H

#include "aextension.h"
class TEBase;
class QLibrary;

typedef QStringList (*p_te_drivers)( const QString &  );
typedef bool (*p_te_driverExists)( const QString &,  const QString & );
typedef TEBase *(*p_te_createDriverObject)( const QString &className );    

/**
 * \en
 * Class for work with Trade Equipment drivers
 * \_en
 * \ru
 * \_ru
 */
class  ANANAS_EXPORT AExtTE : public AExtension
{
    Q_OBJECT

public:

    AExtTE();
    ~AExtTE();

public slots:
    QStringList drivers( const QString &baseclass );
    bool driverExists( const QString &name );
    QObject *newDriver( const QString &name );
protected slots:
    void deviceEvent( int eventCode );
private:
    QLibrary *telib;
    p_te_drivers te_drivers;
    p_te_driverExists te_driverExists;
    p_te_createDriverObject te_createDriverObject;
};

#endif //AEXTTE_H
