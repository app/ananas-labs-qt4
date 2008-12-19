/****************************************************************************
** $Id: aextension.h,v 1.1 2008/11/05 21:16:28 leader Exp $
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

#ifndef AEXTENSION_H
#define AEXTENSION_H

#include "ananasglobal.h"
#include "adatabase.h"
#include <qobject.h>

/**
 * \en
 * Class for work with extensions
 * \_en
 * \ru
 * \_ru
 */
class  ANANAS_EXPORT AExtension : public QObject
{
    Q_OBJECT

public:
    aDatabase* db;

    AExtension( const char *name = 0 );
    ~AExtension();
    int init(aDatabase* dbo);

signals:
    void event( const QString &data );

};

#endif //AEXTENSION_H
