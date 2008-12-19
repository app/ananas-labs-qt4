/****************************************************************************
** $Id: wactionbutton.h,v 1.1 2008/11/05 21:16:29 leader Exp $
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

#ifndef WACTIONBUTTON_H
#define WACTIONBUTTON_H

#include <qpushbutton.h>
#include <qwidgetplugin.h>
//Added by qt3to4:
#include <QKeyEvent>
#include "acfg.h"

class QWidget;

class QT_WIDGET_PLUGIN_EXPORT wActionButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY( bool openEditor READ getOpenEditor WRITE setOpenEditor STORED false )
    Q_PROPERTY( bool action READ isAction WRITE setAction DESIGNABLE true )
    Q_PROPERTY( bool actionUpdate READ isActionUpdate WRITE setActionUpdate DESIGNABLE true )
    Q_PROPERTY( bool actionTurnOn READ isActionTurnOn WRITE setActionTurnOn DESIGNABLE true)
    Q_PROPERTY( bool actionClose READ isActionClose WRITE setActionClose DESIGNABLE true)
    Q_PROPERTY( int actionId READ getActionId WRITE setActionId DESIGNABLE true)
    Q_PROPERTY( bool script READ isScript WRITE setScript DESIGNABLE true)
//    Q_PROPERTY( QString scriptCode READ getScriptCode WRITE setScriptCode DESIGNABLE false)
public:

    wActionButton( QWidget *parent = 0, const char *name = 0 );
    virtual ~wActionButton();

    void openEditor();


public slots:

    void onClick();
    void keyPressHandler ( QKeyEvent * e );

    bool getOpenEditor() const { return false; };
    void setOpenEditor( bool b ) { if( b ) openEditor(); };
    bool isAction() 		const 	{ return action; 	};
    bool isActionUpdate() 	const 	{ return actionUpdate; 	};
    bool isActionTurnOn() 	const 	{ return actionTurnOn; 	};
    bool isActionClose() 	const 	{ return actionClose; 	};
    int	 getActionId()	 	const 	{ return actionId; 	};
    bool isScript() 		const 	{ return !action; 	};
    QString getScriptCode() 	const 	{ return scriptCode; 	};

    void setAction( bool a ) 		{ action = a; 		};
    void setActionUpdate( bool a ) 	{ actionUpdate = a; 	};
    void setActionTurnOn( bool a ) 	{ actionTurnOn = a; 	};
    void setActionClose( bool a ) 	{ actionClose = a; 	};
    void setActionId( int a )	 	{ actionId = a; 	};
    void setScript( bool a ) 		{ action = !a;		};
    void setScriptCode( QString a ) 	{ scriptCode = a; 	};
private:

    bool action, actionUpdate, actionTurnOn, actionClose;
    int actionId;
    QString scriptCode;
};

#endif //WACTIONBUTTON_H

