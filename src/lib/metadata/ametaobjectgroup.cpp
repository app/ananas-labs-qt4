/****************************************************************************
** $Id: ametaobjectgroup.cpp,v 1.4 2008/12/20 21:17:49 leader Exp $
**
** Code file of the Ananas configuration objects of Ananas
** Designer and Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) Grigory Panov <gr1313 at mail.ru>, Yoshkar-Ola.
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

#include <qdatetime.h>
#include <qobject.h>
#include <qfile.h>
#include <qstringlist.h>
//--#include <q3textstream.h>
#include <QTextStream>
#include <QDomDocument>

#include "ametaobject.h"
#include "ametaobjectgroup.h"

#define context_startid	100


/*!
 * \class AMetaGroup
 *
 */



/*!
 * \class ACataloguesGroup
 *
 */
ACataloguesGroup::ACataloguesGroup()
:AMetaGroup("Catalogues")
{

}



/*!
 * \class AReportsGroup
 *
 */
AReportsGroup::AReportsGroup()
:AMetaGroup("Reports")
{

}



/*!
 * \class AReportsGroup
 *
 */
AJournalsGroup::AJournalsGroup()
:AMetaGroup("Journals")
{

}



/*!
 * \class AInformationRegistersGroup
 *
 */
AInfoRegistersGroup::AInfoRegistersGroup()
:AMetaGroup("InfoRegisters")
{

}



/*!
 * \class AInformationRegistersGroup
 *
 */
AAccRegistersGroup::AAccRegistersGroup()
:AMetaGroup("AccRegisters")
{

}



/*!
 * \class AMetaTables
 *
 */
AMetaTables::AMetaTables()
:AMetaGroup("MetaTables")
{

}



