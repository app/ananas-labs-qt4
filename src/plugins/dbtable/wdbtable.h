/****************************************************************************
** $Id: wdbtable.h,v 1.1 2008/11/05 21:16:30 leader Exp $
**
** Header file of the database table plugin of Ananas
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

#ifndef WDBTABLE_H
#define WDBTABLE_H

#include <q3sqlpropertymap.h>
#include <q3datatable.h>
//Added by qt3to4:
#include <Q3SqlEditorFactory>
#include <QContextMenuEvent>
#include <Q3Frame>
#include <Q3ValueList>
#include <QPixmap>
#include <Q3SqlCursor>
#include <QKeyEvent>
#include <QEvent>
#include "acfg.h"
#include "aobject.h"
#include "awidget.h"
#include "wfield.h"

class aDatabase;
class wDBTable;
class Q3DataTable;

class aSearchWidget : public Q3Frame
{
Q_OBJECT
public:
	aSearchWidget( QWidget *parent, wDBTable *table );
	~aSearchWidget();
	void search( const QString &t ="");
public slots:
	void setText( const QString &t );
protected:
//	virtual void keyPressEvent ( QKeyEvent *e );
        bool eventFilter( QObject *obj, QEvent *ev );
private:
	QString ftext;
	QLineEdit *l;
	wDBTable *t;
};



/*!
 * \en	Class for editing documents tables. \_en
 * \ru
 * 	\brief Визуальный класс для редактирования табличных частей документа.
 * 	Наследует QDataTable.
 *
 * 	Поддерживает редактирование журнала документов
 * 	и таблицы элементов каталога.
 * \_ru
 *
 */
class QT_WIDGET_PLUGIN_EXPORT wDBTable : public Q3DataTable
{
	friend class aForm;

	Q_OBJECT
	Q_PROPERTY( qulonglong	Id READ getId WRITE setId DESIGNABLE true )
	Q_PROPERTY( int		DefaultColWidth READ getDefColWidth WRITE setDefColWidth DESIGNABLE true )
	Q_PROPERTY( int		TableInd READ getTblInd WRITE setTblInd DESIGNABLE true )
	Q_PROPERTY( QStringList	DefFields READ getDefFields WRITE setDefFields DESIGNABLE true )
	Q_PROPERTY( QStringList	DefHeaders READ getDefHeaders WRITE setDefHeaders DESIGNABLE true )
	Q_PROPERTY( QStringList	ColWidth READ getColWidth WRITE setColWidth DESIGNABLE true )
	Q_PROPERTY( QStringList	DefIdList READ getDefIdList WRITE setDefIdList DESIGNABLE true )
//	Q_PROPERTY( bool	openEditor READ getOpenEditor WRITE setOpenEditor STORED true )
	Q_PROPERTY( QString	editFormName READ getEditFormName WRITE setEditFormName DESIGNABLE true )
public:
	aCfg*		md;
	aDatabase*	db;
	aEngine*	engine;
	//char vLoaded;
	QStringList	list_available_tables;
	QStringList	fname, hname, colWidth,idList;
	int		defColWidth;
	int		tableInd;
	aCfgItem	tables;
	Q3SqlCursor*	cur;
	bool		searchMode;
	QString		searchString;
	aSearchWidget	*searchWidget;

	wDBTable( QString objtype, QWidget *parent = 0, const char *name = 0);
	virtual ~wDBTable();
	void		checkFields();
	void		setFields(int numTable); //set propertis default value for table with num=numTable
	QStringList	getFields(int numTable, bool GetId=false); // return list field names or id's, defined in metadata
	QString		getFieldName(long idField); // return attribute `name' value
	QString		getFieldType(long idField); // return attribute `type' value
	int		getTableInd(int id);
	int		getTableId(int ind);

	//	void 	init( aDatabase *adb );
	void		init();
	void		init(aDatabase *adb, aEngine *e = 0 ); // call in ananas engine
	void		OpenEditor();
	//QWidget*	createEditor();

	//int 	getDefaultWidth() const {return defColWidth;};

	QStringList	getDefFields()	const;
	QStringList	getDefHeaders()	const;
	QStringList	getColWidth() 	const;
	QStringList	getDefIdList()	const;
	qulonglong	getId();
	int 		getTblInd() 	const;
	bool		getOpenEditor() const;
	int		getDefColWidth() const;

	void 	setDefFields(QStringList lst);
	void 	setDefHeaders(QStringList lst);
	void 	setColWidth(QStringList lst);
	void 	setDefIdList(QStringList lst);
	void 	setId( qulonglong fn );
	void	setTblInd( int ind );
	void 	setOpenEditor( bool fn );
	void 	setDefColWidth( int fn );

	QString	getEditFormName() const	{ return vEditFormName;};
	void 	setEditFormName( QString name)	{ vEditFormName = name;	};

	QString	containerType()	const { return container_type;};
	void 	setContainerType( QString name)	{ container_type = name; };
	QPixmap systemIcon();
	virtual int Select( ANANAS_UID db_uid );
	aCfg*	getMd();
public slots:
	Q3ValueList<int> getBindList();
	void 	setWFieldEditor();
	void	setAvailableTables();
	void 	lineUpdate(QSql::Op mode);
	void	newFilter(const QString & );
	void	newDataId(const qulonglong );
	QVariant Value( const QString &colname );

	bool searchColumn( const QString &text, bool FromCurrent = FALSE, bool Forward = TRUE );
	void searchOpen( const QString &text = "" );
	void searchClose();

protected slots:
	void doubleClickEventHandler(int , int , int, const QPoint& ); //parametrs not used
	virtual void updateTableCellHandler(int, int);
signals:

/*!
 *	\~english
 *	Signal emitted after table line update.
 *	\~russian
 *	Сигнал испускается после обновлении строки таблицы.
 *	\~
 */
	void saveLine(QSqlRecord *rec);

/*!
 *	\~english
 *	Signal emitted after table line delete.
 *	\~russian
 *	Сигнал испускается после удаления строки таблицы.
 *	\~
 */
	void deleteLine(QSqlRecord *rec);
/*!
 *	\~english
 *	Signal emitted after select another document.
 *	\~russian
 *	Сигнал испускается после выбора документа.
 *	\~
 */
	void selected( qulonglong uid );
/*!
 *	\~english
 *	Signal emitted after change line.
 *	\~russian
 *	Сигнал испускается после изменения строки.
 *	\~
 */
	void selectRecord ( qulonglong );
/*!
 *	\~english
 *	Signal emitted after update cell.
 *	\~russian
 *	Сигнал испускается после обновления ячейки с номером \a row, \a col.
 *	\~
 */
	void updateCurr(int row, int col);
//<<<<<<< wdbtable.h

	//signals from context menu for connecting to wJournal
	//if container has another type, then do QDataTable context menu
	//and this signal no emitted
//=======

/*!
 *	\~english
 *	signals from context menu for connecting to wJournal
 *	if container has another type, then do QDataTable context menu
 *	and this signal no emitted
 *	\~russian
 *	Сигналы от контекстного меню для присоединения к wJournal
 *	Если контейнер другого типа, то вызывается стандартное меню,
 *	и эти сигналы не испускаются.
 *	\~
 *	\see updateRequest(); deleteRequest(); viewRequest();
 *
*/
//>>>>>>> 1.45.2.4
	void insertRequest();
/*!
 *	\see insertRequest();
*/
	void updateRequest();
/*!
 *	\see insertRequest();
*/
	void deleteRequest();
/*!
 *	\see insertRequest();
*/
	void viewRequest();
	// end
	void getMd( aCfg ** );
	void getId( qulonglong * );

private slots:
	//void select( Q_ULLONG group );
	void lineChange(int, int);
	void lineInsert(QSqlRecord*);
	void updateItem( ANANAS_UID db_uid );

protected:
	virtual void paintField ( QPainter * p, const QSqlField * field, const QRect & cr, bool selected );
	QWidget * beginUpdate ( int row, int col, bool replace );
	virtual bool updateCurrent();
	void  contentsContextMenuEvent ( QContextMenuEvent * e );
//	void propertyUpdate(const QString &propName);
	virtual bool deleteCurrent();
	//virtual QSql::Confirm confirmEdit ( QSql::Op m );
	virtual void keyPressEvent ( QKeyEvent *e );
	void EditElement();
	virtual void activateNextCell();
	virtual bool beginInsert ();
	virtual QSql::Confirm confirmEdit( QSql::Op m );

private:

	QPixmap t_doc;
	QPixmap t_doc_d;
	QPixmap t_doc_t;
	QPixmap t_doc_m;
	QPixmap t_doc_tm;
	QPixmap t_cat_e;
	QPixmap t_cat_ed;
	QPixmap t_cat_g;
	QPixmap t_cat_gd;

	int lastEditedRow;
	int lastEditedCol;

	QString container_type;
	qulonglong	doc_id;
	qulonglong	cat_group_id;
	long journalFieldId(long);
	QString journalFieldName(long);
	bool inEditMode;
	//aDBTablePrivate impl;
//	void updateProp(void);
	Q3ValueList<int> listBindings;
	aCfgItem obj;
        QString vName, vEditFormName;
	QString	vDefineCols;
	qulonglong oid;
};


/*!
 * \en	Class for support custom editor in wDBTable. \_en
 * \ru
 * 	\brief Класс для поддержки собственного редактора поля в wDBTable.
 * 	Наследует QSqlEditorFactory.
 * \_ru
 */
class aEditorFactory: public Q3SqlEditorFactory
{
public:
/*!
 * \~english	Constructor
 * \~russian 	Конструктор \~
 */
	aEditorFactory(QObject * parent = 0, const char * name = 0):Q3SqlEditorFactory(parent/*--,name*/) {};
	QWidget * createEditor (QWidget * parent, const QSqlField * field);
	void setMd(aCfg *md);
private:
	aCfg * md;
};
#endif
