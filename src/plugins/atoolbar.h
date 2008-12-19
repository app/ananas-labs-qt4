/****************************************************************************
** $Id: atoolbar.h,v 1.1 2008/11/05 21:16:29 leader Exp $
**
** Header file of the Ananas Tool bar of Ananas
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

/****************************************************************************
****************************************************************************/

#ifndef ATOOLBAR_H
#define ATOOLBAR_H

#include <q3toolbar.h>
#include <qaction.h>

#include "acfg.h"
#include "ananas.h"

class  QT_WIDGET_PLUGIN_EXPORT aToolBar : public Q3ToolBar
{
    Q_OBJECT
private:
    aCfg	*md;
    aEngine *en;
    Q3IntDict<QAction> actions;

public:
	aToolBar( aCfg *cfg, aCfgItem &obj, aEngine *e, Q3MainWindow* parent = 0, const char* name = 0 );
	~aToolBar();

	void ReadTool( aCfgItem &obj );

public slots:
	void on_Item();
};


#endif // ATOOLBAR_H
