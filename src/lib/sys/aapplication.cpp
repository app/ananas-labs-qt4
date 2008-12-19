/****************************************************************************
** $Id: aapplication.cpp,v 1.1 2008/11/09 21:08:09 leader Exp $
**
** Code file of the Ananas Library of Ananas
** Designer and Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
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

#include "aapplication.h"
#include "dselectdb.h"
#include "dlogin.h"


AApplication::AApplication(  int & argc, char ** argv, AApplicationType aat )
    :QApplication( argc, argv, true )
{
    v_aat = aat;
}

int
AApplication::applicationType()
{
    return v_aat;
}

