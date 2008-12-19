/****************************************************************************
** $Id: wtable.h,v 1.1 2008/11/05 21:16:30 leader Exp $
**
** Header file of the table plugin of Ananas
** Designer and Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) 2006 Grigory Panov <gr1313 at mail.ru>, Yoshkar-Ola.
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

#ifndef WTABLE_H
#define WTABLE_H

#include <qwidget.h>
#include <qwidgetplugin.h>
#include <qobject.h>
#include <q3scrollview.h>
#include <q3table.h>
#include <qlabel.h>
#include <qevent.h>
#include <q3datatable.h>
#include "acfg.h"
#include "aobject.h"
#include "awidget.h"

/*!
 * \en
 * 	\brief Object for view and edit tabel in aTable object.
 * \_en
 * \ru
 * 	\brief Плагин - виджет для редактирования табличных данных, не хранящихся в бизнес объектах Ананаса.
 *	Наследует QTable.
 *
 * 	В отличие от wDBTable wTable не сохряняет редактируемые в нем данные автоматически
 * 	в persistent бизнес объектах Ананаса. Программист самостоятельно
 * 	должен закодировать на Ананас.Скрипте логику заполнения виджета данными и использования данных, вводимых
 * 	пользователем.
 *
 * \_ru
 */


class QT_WIDGET_PLUGIN_EXPORT wTable : public Q3Table
{
	Q_OBJECT

public:

	wTable(QWidget* parent, const char * name);
	virtual ~wTable();
public slots:
	virtual void setDocument(int row, aObject *object);
	void setText(int row, int col, const QString& text);
	QString text(int col, int row);
	void setHeaderText(int col, const QString& text);
	QString textHeader(int col);
	virtual void sortColumn ( int col, bool ascending = TRUE, bool wholeRows = TRUE );
	virtual void setColumnWidth(int col, int width);
	virtual int columnWidth(int col);
	virtual void setNumCols ( int r );
	virtual int numCols () const ;
	virtual void hideColumn ( int col );
	virtual void showColumn ( int col );
//	bool isColumnHidden ( int col ) const;
	virtual void adjustColumn ( int col );
	virtual void setColumnStretchable ( int col, bool stretch );
	bool isColumnStretchable ( int col ) const;
	virtual void swapColumns ( int col1, int col2, bool swapHeader = FALSE );
	virtual void swapCells ( int row1, int col1, int row2, int col2 );
	virtual void setCurrentCell ( int row, int col );
	virtual void setColumnReadOnly ( int col, bool ro );
	virtual void insertColumns ( int col, int count = 1 );
	virtual void removeColumn ( int col );
	virtual void editCell ( int row, int col, bool replace = FALSE );
	virtual void swapRows ( int row1, int row2, bool swapHeader = FALSE );
signals:
//	void pressed ( int row, int col, int button, const QPoint & mousePos );
//	void currentChanged ( int row, int col );
//	void clicked ( int row, int col, int button, const QPoint & mousePos );
//	void valueChanged ( int row, int col );

protected slots:
	virtual void columnClicked ( int col );

private slots:
//	void on_pressed ( int row, int col, int button, const QPoint & mousePos );
//	void on_currentChanged ( int row, int col );
//	void on_clicked ( int row, int col, int button, const QPoint & mousePos );
//	void on_valueChanged ( int row, int col );
protected:
	virtual void activateNextCell () {} ;

};

#endif
