/****************************************************************************
** $Id: wactionbutton.cpp,v 1.1 2008/11/05 21:16:29 leader Exp $
**
** Header file of the table plugin of Ananas
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

#include "wactionbutton.h"
#include "eactionbutton.h"
#include <q3mainwindow.h>
#include <q3header.h>
#include <qvariant.h>
#include "qobject.h"
#include "qobject.h"
//Added by qt3to4:
#include <QKeyEvent>

//--#include "command.h"
//--#include "formwindow.h"
//--#include "mainwindow.h"
//--#include "mainform.h"

#include "adatabase.h"

wActionButton::wActionButton( QWidget *parent, const char *name )
    : QPushButton( parent, "wActionButton")
{
    action = true;
    actionId=0;
    actionUpdate = actionTurnOn = actionClose = false;
    connect( this, SIGNAL(clicked()), this, SLOT(onClick()) );

}


wActionButton::~wActionButton()
{
}


void
wActionButton::openEditor()
{
	eActionButton *e = new eActionButton(this->topLevelWidget());
	aCfg *md;
	e->setData(this);
	if ( e->exec()==QDialog::Accepted )
	{
		e->getData(this);
		//updateProp();
	}
	delete e;
	e=0;
}



void
wActionButton::onClick()
{
//CHECK_POINT
}

void
wActionButton::keyPressHandler ( QKeyEvent * e )
{
	CHECK_POINT
	switch(e->key())
	{
		case Qt::Key_Return:
		break;
		case Qt::Key_Escape:
			if(accel() == QKeySequence(Qt::Key_Escape))
			{
				emit(clicked());
				e->accept();
			}
		break;

	}
	//printf("key pressed %c",e->ascii());
	//QWidget::keyPressEvent ( e );
	e->ignore();
}
