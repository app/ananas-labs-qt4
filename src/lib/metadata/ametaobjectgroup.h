/****************************************************************************
** $Id: ametaobjectgroup.h,v 1.4 2008/12/20 21:17:49 leader Exp $
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


class ACataloguesGroup: public AMetaGroup
{
Q_OBJECT
public:
    ACataloguesGroup();

public slots:


};



class AReportsGroup: public AMetaGroup
{
Q_OBJECT
public:
    AReportsGroup();

public slots:


};



class AJournalsGroup: public AMetaGroup
{
Q_OBJECT
public:
    AJournalsGroup();

public slots:


};



class AInfoRegistersGroup: public AMetaGroup
{
Q_OBJECT
public:
    AInfoRegistersGroup();

public slots:


};



class AAccRegistersGroup: public AMetaGroup
{
Q_OBJECT
public:
    AAccRegistersGroup();

public slots:


};



class AMetaTables: public AMetaGroup
{
Q_OBJECT
public:
    AMetaTables();

public slots:


};




#endif
