/****************************************************************************
** $Id: wreport.h,v 1.1 2008/11/05 21:16:30 leader Exp $
**
** Header file of the report plugin of Ananas
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

#ifndef WREPORT_H
#define WREPORT_H
#include <qwidgetplugin.h>
#include "awidget.h"

class QWidget;

class QT_WIDGET_PLUGIN_EXPORT wReport : public aWidget
{
	Q_OBJECT
public:
	wReport( QWidget *parent = 0, Qt::WFlags fl = 0 );
	virtual ~wReport();
	bool checkStructure();
	virtual void initObject( aDatabase *adb );
	virtual QDialog* createEditor( QWidget *parent );
	virtual QString displayString();
	virtual bool isContainer() { return true; };

	virtual qulonglong	uid();
	virtual ERR_Code	New();
	virtual ERR_Code	Update();
	virtual ERR_Code	TurnOn();
	virtual ERR_Code	Select( qulonglong id );

public slots:
//	QVariant value( const QString &name );
//	void setValue( const QString &name, QVariant &value );
	int select( qulonglong id );
	qulonglong insert();
	int update();
	int markDelete();

private:

};


#endif // WREPORT_H
