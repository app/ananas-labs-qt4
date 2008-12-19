/****************************************************************************
** $Id: wjournal.h,v 1.1 2008/11/05 21:16:30 leader Exp $
**
** Header file of the catlogue plugin of Ananas
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

#ifndef WJOURNAL_H
#define WJOURNAL_H
#include <qwidgetplugin.h>
#include <q3datetimeedit.h>
#include "awidget.h"

class QWidget;
/*!
 *\en
 *	 Visual object for work with Journals of Documents.
 *\_en
 *\ru
 *	\brief Визуальный объект - контейнер для работы с Журналами документов.
 *	Наследует aWidget.
 *
 *	При создании диалогов - экранных форм журналов все элементы диалога
 *	(в том числе wDBField, wDBTable) располагаются в этом контейнере.
 *	В свойстве aObject *dbobj, унаследованном от aWidget, этого класса хранится объект доступа к данным
 *	журнала в sql таблице базы данных, используемый элементами диалога для выборки и записи данных в базу данных.
 *	Смотрите метод wJournal::dataObject().
 *	В свойстве aCfgItem obj, унаследованном от aWidget, этого класса хранится элемент конфигурации (метаданные),
 *	описывающий структуру объекта данных (как правило таблицу).
 *	Смотрите метод wJournal::getMDObject().
 *\_ru
 */

class QT_WIDGET_PLUGIN_EXPORT wJournal : public aWidget
{
	Q_OBJECT
public:
	wJournal( QWidget *parent = 0, Qt::WFlags fl = 0 );
	virtual ~wJournal();
	bool			checkStructure();
	virtual void		initObject( aDatabase *adb );
	virtual Q3ToolBar*	createToolBar( Q3MainWindow *parent );
	virtual QDialog*	createEditor( QWidget *parent );
	virtual QString		displayString();
	virtual bool		isContainer() { return true; };
	virtual aObject*	createDBObject(  aCfgItem obj, aDatabase *adb );
//	virtual ERR_Code Select( Q_ULLONG id );


public slots:
	int		select( qulonglong id );
	qulonglong	insert();
	int		update();
	int		view();
	int		markDelete();
	int		copy();

protected slots:
	void		setFilterByDate();

private:
	QDialog *dSelectType;
	Q3ToolBar *toolbar;
	Q3DateEdit *date_from, *date_to;
	qulonglong docUid;
	int docId;
};


#endif // WJOURNAL_H
