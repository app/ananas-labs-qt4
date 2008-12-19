/****************************************************************************
** $Id: wreport.cpp,v 1.1 2008/11/05 21:16:30 leader Exp $
**
** Code file of the report plugin of Ananas
** Designer and Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
**
** This file is part of the Ananas Plugins of the Ananas
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

#include <qobject.h>
#include <q3sqlcursor.h>
#include <q3sqlpropertymap.h>
//--#include <qfocusdata.h>
#include <q3toolbar.h>
#include "adatabase.h"
#include "wreport.h"
#include "ereport.h"

//extern aCfg *plugins_aCfg;


wReport::wReport( QWidget *parent, Qt::WFlags fl )
:aWidget( parent, "wReport", fl )
{
	setInited( false );
}


wReport::~wReport()
{
}



void
wReport::initObject( aDatabase *adb )
{
	aWidget::initObject( adb );
//	focusData()->next()->setFocus();

//CHECK_POINT
}


bool
wReport::checkStructure()
{

	return false;
}



QDialog*
wReport::createEditor( QWidget *parent )
{
    return new eReport( parent );
}


int
wReport::select( qulonglong id )
{
    return 0;
}


qulonglong
wReport::insert()
{
    return 0;
}


int
wReport::update()
{
    return 0;
}


int
wReport::markDelete()
{
    return 0;
}

ERR_Code
wReport::New()
{
	return err_abstractobj;
}


ERR_Code
wReport::Update()
{
	return err_abstractobj;
}


ERR_Code
wReport::TurnOn()
{
	return err_abstractobj;
}


ERR_Code
wReport::Select( qulonglong id )
{
	return err_abstractobj;
}

QString
wReport::displayString()
{
    return QString("");
}

qulonglong
wReport::uid()
{
	return 0;
}
