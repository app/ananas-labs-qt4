/****************************************************************************
** $Id: wdbfield.h,v 1.1 2008/11/05 21:16:29 leader Exp $
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

#ifndef QADBFIELD_H
#define QADBFIELD_H

#include <qwidget.h>
#include <qwidgetplugin.h>
#include <qlabel.h>
#include <qobject.h>
#include <qlayout.h>
#include <q3socket.h>
#include <qeventloop.h>
//Added by qt3to4:
#include <Q3ValueList>
#include "adatabase.h"
#include "acfg.h"
#include "wfield.h"
//#include "addfdialog.h"

/*!
 * \en
 * 	\brief Ananas DBField. widget.
 * \_en
 * \ru
 * 	\brief Универсальный Плагин -- виджет, предназначен для редактирования атрибутов бизнес объектов Ананаса.
 * 	Наследует wField.
 *
 * 	Основным полезным свойством этого визуального класса является возможность его привязки к атрибуту persistence бизнес объекта
 * 	Ананаса. Как только привязка задана - логика заполнения объекта значением соответствующего атрибута бизнес объекта, логика
 * 	сохранения заданного пользователем значения в соответствующем атрибуте бизнес объекта выполняется автоматически runtime
 * 	подсистемой Ананаса. То есть мы используем прием, который имеет общепринятое название data bindings.
 *
 * \_ru
 */
class QT_WIDGET_PLUGIN_EXPORT wDBField : public wField
{
	Q_OBJECT

	Q_PROPERTY( bool openEditor READ getOpenEditor WRITE setOpenEditor STORED false)
	Q_PROPERTY( QString FieldName READ getFieldName WRITE setFieldName )

friend class addfdialog;


public:

	wDBField( QWidget *parent, Qt::WFlags fl );
	wDBField( QWidget *parent, const char *name, Qt::WFlags fl );
	~wDBField();

	virtual QDialog* createEditor( QWidget *parent );
	virtual void initObject( aDatabase *adb );
	void		openEditor();
	bool		getOpenEditor()	const	{ return false;	 	};
	virtual QString	getFieldName()	const 	{ return FieldName;	};
	void		setOpenEditor(bool b)  	{ if(b) openEditor();	};
	void		setFieldName(QString n);
protected:
	QStringList	getFields();
	void		init();
	Q3ValueList<qulonglong> getBindList();
	void		setEditorType(void);

	QString		FieldName;
	QStringList 	defDisplayFields,defFields,defId;

private:
	aCfgItem head;
};
#endif
