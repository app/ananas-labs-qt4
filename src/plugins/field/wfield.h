/****************************************************************************
** $Id: wfield.h,v 1.1 2008/11/05 21:16:30 leader Exp $
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

#ifndef QAFIELD_H
#define QAFIELD_H

#include <qwidget.h>
#include <qwidgetplugin.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <q3datetimeedit.h>
#include <qcheckbox.h>
#include "qlineedit.h"
//Added by qt3to4:
#include <QKeyEvent>
#include "wdateedit.h"
#include "adatabase.h"
#include "ananas.h"

class wCatButton;
class wCheckBox;

/*!
 * \en
 * 	\brief Ananas Field widget.
 * \_en
 * \ru
 * 	\brief Плагин -- виджет предназначенный для редактирования значений любых типов, определенных в бизнес схеме.
 * 	Наследует aWidget.
 *
 * 	В отличие от wDBField класс wField не биндится к атрибутам persistent бизнес объектов. То есть время жизни
 * 	хранимых в нем значений равно времени жизни родительской экранной формы. Прикладной программист сам должен
 * 	решить вопрос обработки и хранения значений задаваемых пользователем.
 * \_ru
 */
#include <QtDesigner/QDesignerExportWidget>

//class QT_WIDGET_PLUGIN_EXPORT wField : public aWidget
class QDESIGNER_WIDGET_EXPORT wField : public aWidget
{
	Q_OBJECT

	Q_PROPERTY( QString value READ value WRITE setValue)
	Q_PROPERTY( QString FieldType READ getFieldType WRITE setFieldType )
	Q_PROPERTY( tEditorType EditorType READ getEditorType WRITE setEditorType )
	Q_ENUMS(tEditorType)
	Q_ENUMS(FieldMode)
public:
	enum tEditorType { Unknown, Numberic, String, Date, DateTime, Boolean, Catalogue=101, Document};

	wField( QWidget *parent, const char *name, Qt::WFlags fl = 0 );
//	wField( QWidget *parent, const char *name, WFlags fl, bool dbf = TRUE );
	virtual ~wField();

	virtual QDialog*	createEditor( QWidget *parent );

signals:
	void valueChanged( const QString & );
	void valueChanged( const QVariant & );
	void lostFocus();

public slots:
	bool			getOpt() const { return false; };
	void			setOpt( bool fn ) { if(fn) widgetEditor(); };
	virtual QString		value() const;
	QString			text() const;
	virtual void		setValue( const QString &fn );
	void			setValue( const QDate& d);
	virtual	QString 	textValue() {return text();};
	void			setFieldType( QString n );
	QString			getFieldType() const {return vFieldType;};
	void			setEditorType( tEditorType n );
	tEditorType		getEditorType() const { return vEditorType; };
	void			fieldSelect();
//	void			propertyUpdate(QString propName);
	virtual void		focusInEvent();
	virtual void		focusOutEvent();
	void 			selectAll();
	virtual void		SetReadOnly(bool);

private slots:
	void on_selected( qulonglong uid );

protected:
	QLineEdit 	*lineEdit;
	wDateEdit	*dateEdit;
	QLabel		*objLabel;
	wCatButton	*objButton;
	wCheckBox	*checkBox;
	tEditorType	vEditorType;
	QString		vFieldType;
	QString		vValue;
//	virtual 	void updateProp();
	void widgetInit();
private:
	int loaded;
	int md_oid, md_fid;
};

class  wCatButton: public QPushButton
{
	Q_OBJECT
	public:
		wCatButton(const char * caption, QWidget *parent);
		virtual ~wCatButton();
	protected:
		void keyPressEvent ( QKeyEvent * e );
};
class  wCheckBox: public QCheckBox
{
	Q_OBJECT
	public:
		wCheckBox(QWidget *parent, const char * name =0);
		virtual ~wCheckBox();

	public slots:
		void on_toggled();
	signals:
		void valueChanged(const QString &);
};

#endif
