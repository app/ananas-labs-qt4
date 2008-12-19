/****************************************************************************
** $Id: messageswindow.cpp,v 1.1 2008/11/05 21:16:28 leader Exp $
**
** Header file of the Mesages window of Ananas
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

/*
*/
#include <q3textbrowser.h>
#include <qlayout.h>
#include <qworkspace.h>
#include <qimage.h>
#include <qpixmap.h>
//Added by qt3to4:
#include <QHideEvent>
#include "messageswindow.h"
#include "acfg.h"

/*
 *  Constructs a MessagesWindow as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
MessagesWindow *msgwindow = 0;


void
messageproc(int n, const char *msg){
	if (msgwindow) msgwindow->message( n, QObject::trUtf8( msg ) );
}


MessagesWindow::MessagesWindow( QWidget* parent, Qt::WFlags fl )
    : Q3DockWindow( parent, "MessagesWindow", fl )
{
	msgBrowser = new Q3TextBrowser( this, "msgBrowser" );
	boxLayout()->addWidget( msgBrowser );
	languageChange();
	setMinimumSize( QSize( 1, 1 ) );
	msgBrowser->setMinimumSize( QSize( 1, 1 ) );
	//--clearWState( WState_Polished );
	//--setWindowState( WState_Polished );
	setResizeEnabled( TRUE );
	setExpanded(false);
	setCloseMode( Q3DockWindow::Always );
	msgBrowser->show();
	msgwindow = this;
	connect(msgBrowser, SIGNAL(doubleClicked(int,int)), this, SLOT(on_click()));
	cfg_messageproc=messageproc;
}

/*
 *  Destroys the object and frees any allocated resources
 */
MessagesWindow::~MessagesWindow()
{
	msgwindow=NULL;
	cfg_messageproc = 0;
}

void
MessagesWindow::message( int msgtype, const QString &msg)
{
	char st[4][50]={
			"<img src=\"msg_info.png\" width=12 height=12>",
			"<img src=\"msg_warning.png\" width=12 height=12>",
			"<img src=\"msg_error.png\" width=12 height=12>",
			"<img src=\"msg_fatal.png\" width=12 height=12>"};
	if (msgtype<0) msgtype=0;
	if (msgtype>3) msgtype=3;
	msgBrowser->append( tr( st[ msgtype ] )+msg );
	show();
}


/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void MessagesWindow::languageChange()
{
    setCaption( tr( "Messages window" ) );
    msgBrowser->setText( QString::null );
}

void
MessagesWindow::hideEvent ( QHideEvent *e )
{
	msgBrowser->clear();
	Q3DockWindow::hideEvent( e );
}


void
MessagesWindow::on_click()
{
	setExpanded(!expanded);
}


void
MessagesWindow::setExpanded(bool exp)
{
	if(exp) setFixedExtentHeight ( 150 );
	else setFixedExtentHeight ( 30 );
	expanded = exp;
	updateGeometry();
}


bool
MessagesWindow::isExpanded()
{
	return expanded;
}


