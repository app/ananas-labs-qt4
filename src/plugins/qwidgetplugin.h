/****************************************************************************
** $Id: qwidgetplugin.h,v 1.2 2008/11/27 20:20:55 leader Exp $
**
** Definition of QWidgetPlugin class
**
** Created : 010920
**
** Copyright (C) 2001-2005 Trolltech AS.  All rights reserved.
**
** This file is part of the widgets module of the Qt GUI Toolkit.
**
** This file may be distributed under the terms of the Q Public License
** as defined by Trolltech AS of Norway and appearing in the file
** LICENSE.QPL included in the packaging of this file.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid Qt Enterprise Edition or Qt Professional Edition
** licenses may use this file in accordance with the Qt Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.trolltech.com/pricing.html or email sales@trolltech.com for
**   information about Qt Commercial License Agreements.
** See http://www.trolltech.com/qpl/ for QPL licensing information.
** See http://www.trolltech.com/gpl/ for GPL licensing information.
**
** Contact info@trolltech.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#ifndef QWIDGETPLUGIN_H
#define QWIDGETPLUGIN_H

#ifndef QT_NO_WIDGETPLUGIN

#ifdef Q_WS_WIN
#ifdef QT_PLUGIN
#define QT_WIDGET_PLUGIN_EXPORT __declspec(dllexport)
#else
#define QT_WIDGET_PLUGIN_EXPORT __declspec(dllimport)
#endif
#else
#define QT_WIDGET_PLUGIN_EXPORT
#endif

#endif // QT_NO_WIDGETPLUGIN
#endif // QWIDGETPLUGIN_H
