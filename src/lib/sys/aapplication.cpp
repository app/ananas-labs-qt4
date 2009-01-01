/****************************************************************************
** $Id: aapplication.cpp,v 1.3 2008/12/25 19:10:21 leader Exp $
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
#include "QTextCodec"


AApplication::AApplication(  int & argc, char ** argv, AApplicationType aat )
    :QApplication( argc, argv, true )
{
    v_aat = aat;
    printf("langDir = %s\n", langDir().toUtf8().data() );
}

int
AApplication::applicationType()
{
    return v_aat;
}


QString
AApplication::applicationName()
{
    char *appN[] = {"unknown","ananas","designer","administrator","srv"}; 
    return QString(appN[v_aat]);
}


QString
AApplication::langDir()
{
#ifdef _Windows
	return applicationDirPath()+"/";
#else
	return "/usr/share/ananas4/translations/";
#endif
}


QString
AApplication::lang()
{
    char *s, locale[50]="en";

    strncpy( locale, QTextCodec::locale(), sizeof( locale ) );
    s = strchr( locale, '_' );
    if ( s ) *s = 0;
    return QString( locale );
}
