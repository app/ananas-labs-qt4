/****************************************************************************
** $Id: wcatalogeditor.h,v 1.1 2008/11/05 21:16:30 leader Exp $
**
** Header file of the field plugin of Ananas
** Designer and Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) 2003-2004 Grigory Panov, Yoshkar-Ola.
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

#ifndef WCATALOGEDITOR_H
#define WCATALOGEDITOR_H

#include "ananasglobal.h"

#include <qwidget.h>
#include <qwidgetplugin.h>
#include <qlabel.h>
#include <qworkspace.h>
//#include <qobjectlist.h>
//#include <qlayout.h>
//#include <qsocket.h>
//#include <qeventloop.h>
//#include <qdatetimeedit.h>
#include "wfield.h"
#include "adatabase.h"
#include "ananas.h"

/*!
 * \en
 * 	\brief Ananas catalogue type field editor.
 * \_en
 * \ru
 * 	\brief Класс предоставляет доступ к методам встроенного редактора справочников (каталогов)
 * 	Наследует QWidget.
 * \_ru
 */
class QT_WIDGET_PLUGIN_EXPORT wCatalogEditor : public QWidget
{
	Q_OBJECT
	Q_PROPERTY( QString value READ value WRITE setValue)
public:
	QLabel* label;
    	wCatalogEditor( wField* parent = 0, const char *name = 0, const char* catname=0);
    	wCatalogEditor( QWidget* w, int cat);
    	~wCatalogEditor();
    	QString value() const;
    	void setValue(QString newvalue);
    	void initCat(aDatabase *adb);
    	QString displayValue(qulonglong id_element) const;
private slots:
	void on_selected( qulonglong );
	void on_destroyed_form();

protected slots:
    	void  openForm(const bool toSelect);
public slots:
	void  checkUserFields( QStringList &lst);
	void 	select();
	void 	edit();

signals:
	void valueChanged(const QString &);
	void selected( qulonglong );
	void destroyed_form();
private:
	QString vValue;
	aCfg* md;
	int catId;
	QWidget *ws;
	aDatabase* db;
};
#endif
