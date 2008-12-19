/****************************************************************************
** $Id: aextte.cpp,v 1.1 2008/11/05 21:16:28 leader Exp $
**
** Extension object implementation file of
** Ananas application library
**
** Created : 20031201
**
** Copyright (C) 2003-2006 Leader InfoTech.  All rights reserved.
** Copyright (C) 2003-2006 Valery Grazdankin, Orenburg.
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
#include <qlibrary.h>

#include "aextte.h"
#include "acfg.h"
#include "teglobal.h"
#include "tebase.h"

AExtTE::AExtTE()
    : AExtension("TradeEquipment")
{
    telib = 0;
    te_drivers = 0;
    te_driverExists = 0;
    te_createDriverObject = 0;
    telib = new QLibrary("tradeequip");
    if (telib) {
	    telib->load();
	    te_drivers = (p_te_drivers) telib->resolve( "te_drivers" );
	    te_driverExists = (p_te_driverExists) telib->resolve( "te_driverExists" );
	    te_createDriverObject = (p_te_createDriverObject) telib->resolve( "te_createDriverObject" );
    }
}



AExtTE::~AExtTE()
{
    if (telib) delete telib;
}


QStringList
AExtTE::drivers( const QString &baseclass )
{
    QStringList l;
    if (te_drivers) l = te_drivers( baseclass );
    return l;
}

bool 
AExtTE::driverExists( const QString &name )
{
    if (te_driverExists) return te_driverExists( name, QString::null );
    return false;
}


QObject * 
AExtTE::newDriver( const QString &name )
{
    TEBase *te = 0;
    if (te_createDriverObject) {
    	te =  te_createDriverObject( name );
	connect( te, SIGNAL( deviceEvent(int) ), this, SLOT( deviceEvent(int)));
    }
    return ( QObject *) te;
    
}

void 
AExtTE::deviceEvent( int eventCode )
{
    const QObject *obj = sender();
    emit event(QString(obj->name())+"::");
}

#include <aextensionplugin.h>
typedef AExtensionPlugin<AExtTE> ATEPlugin;
A_EXPORT_PLUGIN( ATEPlugin )
