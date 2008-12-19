/****************************************************************************
** $Id: awidget.h,v 1.1 2008/11/05 21:16:29 leader Exp $
**
** Header file of the Ananas visual bject of Ananas
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

#ifndef AWIDGET_H
#define AWIDGET_H
#include <q3sqlform.h>
#include <q3toolbar.h>
#include <q3mainwindow.h>
//Added by qt3to4:
#include <QKeyEvent>
#include "ananas.h"

class QWidget;



/*!
 *\~english
 *	Base visual object.
 *\~russian
 *	\brief Базовый класс для визуальных объектов.
 *	Наследует QWidget.
 *
 *	От него наследуют свойства и методы такие визуальные объекты
 *	как контейнеры wDocument, wJournal, wCatalog, wDocument, wReport и виджеты wField, wDBField.\n
 *
 */
class  ANANAS_EXPORT aWidget : public QWidget
{
	Q_OBJECT
//	Q_PROPERTY( QString Name READ getName WRITE setName STORED true DESIGNABLE false )
	Q_PROPERTY( int Id READ getId WRITE setId STORED true DESIGNABLE false )
	Q_PROPERTY( bool openEditor READ getOpenEditor WRITE setOpenEditor STORED false )
	Q_PROPERTY( int FormMode READ formMode WRITE setFormMode STORED true )
public:
	aDatabase*	db;
	aCfg*		md;
	aEngine*	engine;

	aWidget( QWidget *parent = 0, const char *name = 0, Qt::WFlags fl = 0 );
	aWidget( const QString &oname, aDatabase *adb, QWidget *parent = 0, const char *name = 0, Qt::WFlags fl = 0 );
	aWidget( aCfgItem context, aDatabase *adb, QWidget *parent = 0, const char *name = 0, Qt::WFlags fl = 0 );
	virtual ~aWidget();

	virtual bool		checkStructure();
	virtual void		initObject( aDatabase *adb );
	virtual void		widgetEditor();
        static void		widgetEditor( QWidget *object, QDialog *editor );
	virtual QDialog*	createEditor( QWidget *parent );
	virtual Q3ToolBar*	createToolBar( Q3MainWindow *parent );
        virtual aObject*	createDBObject( aCfgItem obj, aDatabase * );
	virtual QString		displayString();

	void			init( aDatabase *adb );
	bool			getOpenEditor() const { return false; };
	void			setOpenEditor( bool );// { widgetEditor(); };
	bool			isInited() { return vInited; };
	virtual bool		isContainer();
	void			setInited( bool flag ){ vInited = flag; };
//	void			formInsert( QWidget *widget, const QString &field );
//	void			formRemove( const QString &field );
	static aWidget*		parentContainer( QWidget *w );
	static aForm*		parentForm( QWidget *w );
	aCfgItem*		getMDObject();
	void			setMDObject( aCfgItem object );
	aSQLTable*		table( const QString &name = "" );
//	bool tableInsert( const QString &name );
//	bool tableRemove( const QString &name );
	void			setObjectData( QWidget *object, aCfg *md );
	void			getObjectData( QWidget *object );
	aCfg*			getMd();
	virtual qulonglong	uid();
	virtual ERR_Code	New();
	virtual ERR_Code	Update();
	virtual ERR_Code	TurnOn();
	virtual ERR_Code	Select( qulonglong id );
	aObject*		dataObject(){ return dbobj; };

	virtual qulonglong	docId();
	int			formMode() const { return vFormMode; };
	virtual void		setFormMode( int mode ){ vFormMode = mode; };
	virtual QString		getFieldName()	const 	{ return "";	};

	static QString		widgetName( QWidget *w );
	static QWidget*		Widget( QWidget *owner, QString name );

//	QString			getName() const;
//	void			setName( QString fn );
	int 			getId() const { return vId ;};
	void			setId( int fn ){ vId = fn; };

public slots:
	virtual	QVariant 	value( const QString & name );
	virtual QString		value() const {return QString::null;};
	virtual	QString 	textValue() {return QString::null;};
	virtual void		setValue( const QString &);
	virtual int		setValue( const QString & name, const QVariant &value );
	virtual int 		setObjValue( const QString & nameWidget, aObject *value );
	virtual void		SetReadOnly(bool);
	virtual int		Refresh();
	QWidget*		Widget( QString name, bool onlyMyWidgets = true );

	virtual aDataField*	getAttribute( const QString & name );
	virtual int		setAttribute( const QString & name, const aDataField *value );

signals:
	void setData( QWidget *, aCfg * );
	void getData( QWidget * );
	void getMd( aCfg ** );
	void valueChanged( const QString &, const QVariant & );
	void valueChanged( const QString &, const QVariant &, const QString & );
	void changeObj(const QString &);
	void changeObjId(const qulonglong);
	void keyPressed(QKeyEvent *e);

protected:
	aObject *dbobj;
	virtual void updateProp() {};
	virtual void keyPressEvent ( QKeyEvent * e );
private:
	bool		vInited;
	QString		vName;
	int		vId, vFormMode;
	aCfgItem	obj;
	Q3SqlForm*	form;
//	QDict <aSQLTable> dbtables;
};


#endif
