/****************************************************************************
** $Id: awidget.cpp,v 1.2 2008/11/08 20:16:36 leader Exp $
**
** Code file of the Ananas visual bject of Ananas
** Designer and Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) 2003-2006 Grigory Panov <gr1313 at mail.ru>, Yoshkar-Ola.
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

#include <qobject.h>
#include <q3sqlcursor.h>
#include <q3sqlpropertymap.h>
#include <qdialog.h>
#include <qlineedit.h>
//Added by qt3to4:
#include <Q3SqlForm>
#include <QLabel>
#include <QKeyEvent>
#include "adatabase.h"
#include "awidget.h"
#include "wfield.h"
#include "wdbfield.h"
#include "wdbtable.h"
#include "aform.h"
#include "alog.h"


/*!
 *\en
 *	Abstract Ananas widget object.
 *\_en
 *\ru
 *	Конструктор, создающий абстрактный объект. Абстрактность в данном случае означает
 *	отстутствие привязки к базе данных. \n
 *	Вызывает конструктор базового класса QWidget.
 *	\param parent - указатель на родительский виджет.
 *	\param name - имя создаваемого объекта.
 *	\param fl - флаги используемые при создании виджета. Передаются в конструктор базового класса.
 *\_ru
 */
aWidget::aWidget( QWidget *parent, const char *name, Qt::WFlags fl )
:QWidget( parent, name, fl )
{
	vId = 0;
	db = 0;
        dbobj = 0;
	engine = 0;
	vFormMode = 0;
	vInited = false;
}

/*!
 * \ru
 * 	Конструктор, использующий для создания объекта имя элемента метаданных.\n
 * 	У класса есть также конструктор, использующий для создания объекта ссылку на объект с описанием элемента метаданных.
 * 	\param itemname - имя элемента метаданных.
 * 	\param adb - ссылка на объект доступа к базе данных.
 * 	\param parent - указатель на родительский виджет (передается в конструктор базового класса).
 * 	\param name - имя создаваемого объекта (передается в конструктор базового класса).
 * 	\param fl - флаги, используемые конструктором базового класса.
 * \_ru
 */
aWidget::aWidget( const QString &itemname, aDatabase *adb, QWidget *parent, const char *name, Qt::WFlags fl )
:QWidget( parent, name, fl )
{
	vId = 0;
        dbobj = 0;
	vInited = false;
	engine = 0;
	vFormMode = 0;
	if ( adb ) {
		obj = adb->cfg.find( itemname );
		init( adb );
	}
}


/*!
 * \ru
 * 	Конструктор, использующий для создания объекта ссылку на объект элемента метаданных.\n
 * 	У класса есть также конструктор, использующий для создания объекта имя элемента метаданных.
 * 	\param context - ссылка на объект элемента метаданных.
 * 	\param adb - ссылка на объект доступа к базе данных.
 * 	\param parent - указатель на родительский виджет (передается в конструктор базового класса).
 * 	\param name - имя создаваемого объекта (передается в конструктор базового класса).
 * 	\param fl - флаги, используемые конструктором базового класса.
 * \_ru
 */
aWidget::aWidget( aCfgItem context, aDatabase *adb, QWidget *parent, const char *name, Qt::WFlags fl )
:QWidget( parent, name, fl )
{
	vId = 0;
        dbobj = 0;
	vInited = false;
	engine = 0;
	obj = context;
	vFormMode = 0;
	init( adb );
}

aWidget::~aWidget()
{
	delete dbobj;
	dbobj = 0;
}



/*!
 *\en
 *	Create database object.
 *\_en
 *\ru
 *	Пока ничего не делает. Возвращает 0;
 *\_ru
 */
aObject *
aWidget::createDBObject(  aCfgItem , aDatabase * )
{
	return 0;
}




/*!
 *\en
 *	Create toolbar for QMainWindow owner object.
 *\_en
 *\ru
 *	Пока ничего не делает. Возвращает 0;
 *\_ru
 */
Q3ToolBar*
aWidget::createToolBar( Q3MainWindow * )
{
	return 0;
}


/*!
 *\ru
 *	Если объект не был проинициализирован, вызывает initObject(adb).
 *\_ru
 */

void
aWidget::init( aDatabase *adb )
{
	if ( isInited() ) return;
//	aWidget *pc = parentContainer( this );
//	if ( pc ) if ( !pc->isInited() ) pc->init( adb );
	initObject( adb );
}



/*!
 *\en
 *	Init object after create.
 *	Need setId( id ), where id - if of the metadata object of the adb->cfg loaded Configuration.
 *\_en
 *\ru
 *	Инизиализирует созданный объект.
 *	Необходимо выполнить setId(id), где id идентификатор обекта в конфигурации.
 *\_ru
 *\param adb - \en ananas database object. \_en \ru объект доступа к базе данных. \_ru
 */
void
aWidget::initObject( aDatabase *adb )
{
	aCfgItem fg, f;
	QString tname;

	aLog::print(aLog::Debug, tr("aWidget init widget %1 form mode %2").arg(name()).arg(formMode()) );
	// Init myself.
	setInited( true );

	//<для чего?
	Q3SqlPropertyMap *pm = new Q3SqlPropertyMap();
	//>


	db = adb;
	md = 0;
	if ( db )
	{
		md = &db->cfg;
	}
	else
	{
		aLog::print(aLog::Error, tr("aWidget init: invalid database") );
		return;
	}

	if ( obj.isNull() )
	{
		obj = md->find( getId() );
	}


	//<для чего?
	form = new Q3SqlForm( this );
	pm->insert("wDBField","value");
	form->installPropertyMap( pm );
	//>


	if ( obj.isNull() )
	{
		aLog::print(aLog::Error, tr("aWidget init: invalid meta object") );
		return;
	}
	dbobj = createDBObject( obj, adb );
	QObjectList l = this->queryList( "QWidget" );
	QListIterator<QObject*>  it( l );
	QObject *obj;
	while ( it.hasNext() )
	{
		obj = it.next();
		if ( parentContainer( ( QWidget *) obj ) != this ) continue;

		//debug_message("SCAN: Widget class name=%s\n", obj->className() );

		if ( obj->className()==QString("wDBTable") )
		{

			aLog::print(aLog::Debug, tr("aWidget init: connect signals wDBTable") );
			connect( this, SIGNAL( changeObj(const QString &) ),
				//(wDBTable*)
				obj, SLOT( newFilter(const QString &) ));
			connect( this, SIGNAL( changeObjId(const qulonglong) ),
				//(wDBTable*)
				obj, SLOT( newDataId(const qulonglong) ));
		}
	//	if ( obj->className()==QString("wDBField") )
	//	{
//			connect( this, SIGNAL( changeObj(const QString &) ),
//				//(wDBTable*)
//				obj, SLOT( newFilter(const QString &) ));
	//	}
	}
	//--delete l; // delete the list, not the objects
	//--l=0;
}



/*!
 *\~english
 *\~russian
 *\~
 */
 /*
void
aWidget::formInsert( QWidget *widget, const QString &field )
{
	form->insert( widget, field );
}
*/


/*!
 *\~english
 *\~russian
 *\~
 */
/*
void
aWidget::formRemove( const QString &field )
{
	form->remove( field );
}
*/


/*!
 *\en
 *\_en
 *\ru
 *	Ничего не делает. Возвращает false.
 *\_ru
 */
bool
aWidget::checkStructure()
{
	return false;
}



/*!
 *\en
 *\_en
 *\ru
 *	Возвращает значение свойства vName.
 *\_ru
 */
/*QString
aWidget::getName() const	{
//	if (vName.isEmpty()) return name();
	return vName;
}
*/


/*!
 *\en
 *\_en
 *\ru
 *	Устанавливает значение свойства vName.
 *\_ru
 */
 /*
void
aWidget::setName(QString fn){
	vName = fn;
}
*/


/*!
 *\en
 *\_en
 *\ru
 *	Ничего не делает. Возвращает 0.
 *\_ru
 */
QDialog*
aWidget::createEditor( QWidget * )
{
	return 0;
}



/*!
 *\en
 *\_en
 *\ru
 *	Вызывает редактор свойств, если значение передаваемого параметра равно true.
 *	Редактор свойств предоставляет пользователю диалог задания значений свойств виджета.
 *	Например, для такого виджета как wDBField редактор свойств позволяет выбрать элемент метаданных
 *	описывающий поле базы данных, значение которого будет редактироваться с использованием виджета.
 *	То есть осуществить привязку данных (data bindings).
 *\param fn - параметр, значение которого должно быть равно true, для того чтобы был вызван
 *	редактор свойств.
 *\_ru
 */
void
aWidget::setOpenEditor( bool fn )
{
	if(fn) widgetEditor();
}



/*!
 *\en
 *	Emit signal setData( object, md ).
 *\_en
 *\ru
 *	Посылает сигнал setData( object, md ).
 *\_ru
 */
void
aWidget::setObjectData( QWidget *object, aCfg *md )
{
        emit( setData( object, md ) );
}



/*!
 *\en
 *	Emit signal getData( object ).
 *\_en
 *\ru
 *	Посылает сигнал getData( object ).
 *\_ru
 */
void
aWidget::getObjectData( QWidget *object )
{
        emit( getData( object ) );
}



/*!
 *\en
 *	Return toplevel metadata configuration.
 *\_en
 *\ru
 *	Возвращает объект конфигурации для виджета верхнего уровня.
 *\_ru
 */
aCfg*
aWidget::getMd()
{
	aCfg *md = 0;
	QWidget *mw = topLevelWidget();
	if (mw->name() == QString("ananas-designer_mainwindow") )
	{
		connect( this, SIGNAL( getMd( aCfg ** ) ), mw, SLOT( getMd( aCfg ** ) ));
		emit ( getMd( &md ) );
	}
	return md;
}



/*!
 *\~english
 *	Get id from database.
 *	Get id for current database record or 0 if database object not created.
 *\~russian
 *	Возвращает id объекта базы данных.
 *	Возвращает id текущей записи в базе данных или 0 если обект
 *	для работы с базой не создан. Id является уникальным и идентифицирует конкретный
 *	экземпляр обекта.
 *\~
 *\return \~english current database record id\~russian id текущей записи в базе данных.\~
 */
qulonglong
aWidget::uid()
{
	if ( dbobj )
	{
		return dbobj->getUid();
	}
	else
	{
		aLog::print(aLog::Error, tr("aWidget invalid data source object") );
	}
	return 0;
}



/*!
 *\~english
 *\~russian
 *\~
 */
void
aWidget::widgetEditor()
{
	aWidget::widgetEditor( this, createEditor( topLevelWidget() ) );
}



/*!
 *\~english
 *\~russian
 *\~
 */
void
aWidget::widgetEditor(  QWidget *object, QDialog *editor )
{
        aCfg *md = 0;
	QWidget *mw = object->topLevelWidget();
        aWidget o( mw );
	if ( mw->name() == QString( "ananas-designer_mainwindow" ) )
	{
                md = o.getMd();
		if ( !md )
		{
			aLog::print(aLog::Error, tr("aWidget widgetEditor: invalid metadata object") );
			return;
		}
		if ( editor ) {
			connect( &o, SIGNAL( setData( QWidget *, aCfg* ) ), editor, SLOT( setData( QWidget *, aCfg* ) ) );
			connect( &o, SIGNAL( getData( QWidget * ) ), editor, SLOT( getData( QWidget * ) ) );
			o.setObjectData( object, md );
			if ( editor->exec() == QDialog::Accepted )
			{
				o.getObjectData( object );
				//o.updateProp();
			}
			delete editor;
			editor = 0;
		}
	}
}



/*!
 *\~english
 *\~russian
 *\~
 */
aWidget *
aWidget::parentContainer( QWidget *w )
{
        QWidget* prev;
        while ( w )
	{
                prev = w;
                w = w->parentWidget();
                if ( w )
		{
			if ( w->inherits("aWidget") )
			{
				if ( ( ( aWidget *) w )->isContainer()) break;
			}
		}
        }
        if (!w) w = prev;
        return ( aWidget *) w;
}



/*!
 *\~english
 *\~russian
 *\~
 */
aForm *
aWidget::parentForm( QWidget *w )
{
        while ( w )
	{
                if ( w->inherits("aForm") ) break;
                w = w->parentWidget(true);
        }
        return ( aForm *) w;
}



/*!
 *\~english
 *\~russian
 *\~
 */
aCfgItem *
aWidget::getMDObject()
{
    return &obj;
}



/*!
 *\~english
 *\~russian
 *\~
 */
void
aWidget::setMDObject( aCfgItem object )
{
    obj = object;
}



/*!
 *\~english
 *\~russian
 *\~
 */
aSQLTable *
aWidget::table( const QString &name )
{
        if ( dbobj )
	{
		return dbobj->table( name );
	}
	aLog::print(aLog::Error, tr("aWidget table: invalid data source object") );
        return 0;
}

/*
bool
aWidget::tableInsert( const QString &name )
{
	if ( db ) {
		aSQLTable *t = db->table( name );
		if ( !t ) return false;
		dbtables.insert( name, t );
		return true;
	}
	return false;
}


bool
aWidget::tableRemove( const QString &name )
{
	dbtables.remove( name );
	return true;
}
*/



/*!
 *\~english
 *\~russian
 *\~
 */
QString
aWidget::displayString()
{
	return "";
}




/*!
 *\~english
 *	Create new database record object.
 *	New object create with using wirtual function aObject::New() of aObject
 *	and it's childs.
 *\~russian
 *	Создает новый объект для работы с записями базы данных.
 *	Объект создается испоьлзуя виртуальную функцию aObject::New() объекта aObject
 *	и его потомков.
 *\~
 *\return \~engilsh error code. \~russian код ошибки.\~
 */
ERR_Code
aWidget::New()
{
	if ( dbobj )
	{
		return dbobj->New();
	}
	aLog::print(aLog::Error, tr("aWidget new: invalid data source object") );
	return err_abstractobj;

}



/*!
 *\~english
 *	Select object in database.
 *\~russian
 *	Выбирает объект в базе данных.
 *\~
 *\param id -\~english database record id.\~russian id объекта в базе данных.\~
 *\return \~english error code.\~russian код ошибки.\~
 */
ERR_Code
aWidget::Select( qulonglong id )
{
	int rc = 0;
	if ( dbobj )
	{
		bool res;
		res = dbobj->select( id );
		if(res) rc = err_noerror;
		else rc = err_selecterror;
		//Refresh();
		return rc;
	}
	aLog::print(aLog::Error, tr("aWidget select: invalid data source object") );
	return err_abstractobj;
}



/*!
 *\~english Update data
 *\~russian Обновляет данные по данному виджету в базе
 *\~
 */
ERR_Code
aWidget::Update()
{
	QString fname;
	if ( dbobj )
	{

		QObjectList l = this->queryList( "wDBField" );
		QListIterator<QObject*> it( l );
		aWidget *obj;
		while ( it.hasNext() )
		{
			obj = qobject_cast<aWidget*>(it.next());
			fname= obj->getFieldName();
			dbobj->SetValue( fname,( ( aWidget *) obj )->value() );
		}
		//--delete l; // delete the list, not the objects
		//--l=0;
/*
		while ( (obj = (aWidget*) tit.current()) != 0 ){
			++tit;
			((QDataTable*)obj)->refresh();
		}
		delete tl; // delete the list, not the objects

		return err_noerror;
*/
		return dbobj->Update();
	}
	aLog::print(aLog::Error, tr("aWidget update: invalid data source object") );
	return err_incorrecttype;
}

/*!
 *\~english
 *\~russian
 *\~
 */
ERR_Code
aWidget::Refresh()
{
	QString fname;
	QObjectList l = this->queryList( "wDBField" );
	QListIterator<QObject*> it( l );
	QObjectList tl = this->queryList( "wDBTable" );
	QListIterator<QObject*> tit( tl );
	aWidget *obj;
	while ( it.hasNext() ){
		obj = qobject_cast<aWidget*>(it.next());
		if ( parentContainer( obj ) != this ) continue;
		fname= obj->getFieldName();
		obj->setValue(dbobj->Value(fname).toString());
	}
	//--delete l; // delete the list, not the objects
	//--l=0;

    Q3DataTable* obj2;
	while ( tit.hasNext() ){
		obj2 = qobject_cast<Q3DataTable*>(tit.next());
		obj2->refresh();
	}
	//--delete tl; // delete the list, not the objects

	return err_noerror;
}


/*!
 *\~english
 *\~russian
 *\~
 */
ERR_Code
aWidget::TurnOn()
{
	return err_incorrecttype;
}



/*!
 *\~english
 *	Get field value.
 *	get value for field with such name.
 *\~russian
 *	Возвращает значение поля.
 *	Возвращает значение поля с именем name.
 *\~
 *\param name - \~english field md name. \~russian имя поля в конфигурации.\~
 *\return \~english field value. \~russian значение поля.\~
 */
QVariant
aWidget::value( const QString & nameWidget )
{
	QObject *w;
	QVariant res;
/*
	aObject *o = 0;
        if ( mainWidget ) o = mainWidget->dataObject();
        if ( o ) {
                res = o->Value( name );
        }
        if ( res.type() != QVariant::Invalid ) return res;
        else printf("no field %s in data object\n", ( const char *) name );
*/
	w = Widget( nameWidget );
	if ( w ) {
		if (w->inherits("aWidget")){
			res=( (aWidget*)w )->textValue();//value();
//		} else if (w->inherits("wField")){
//			res=( (wField*)w )->text();//value();
		} else if (!strcmp(w->className(),"QPushButton")){
			res=((QPushButton *)w)->text();
		} else if (!strcmp(w->className(),"QLabel")){
			res=((QLabel*)w)->text();
		} else if (!strcmp(w->className(),"QLineEdit")){
			res=((QLineEdit*)w)->text();
		} else if (!strcmp(w->className(),"QCheckBox")){
			res=((QCheckBox*)w)->text();
		} else if (!strcmp(w->className(),"QDateEdit")){
			res=((Q3DateEdit*)w)->date().toString(Qt::ISODate);
		}
	} else {
	//	debug_message(tr("Error! Can't find widget by name==`%s`\n"),(const char*) name.local8Bit());

		aLog::print(aLog::Error, tr("aWidget value not fount widget with name %1").arg(nameWidget) );
	}
	return res;
//	if ( dbobj ) return dbobj->Value(name);
//CHECK_POINT
//	return QVariant("");
}



/*!
 *\~english
 *	Setting form widgets value.
 *	Support type of widgets: wField, wDBField, QPushButton, QLabel, QLineEdit, QCheckBox.
 *\~russian
 * 	Установка значения виджетов формы.
 *	Поддерживаемые типы виджетов: wField, wDBField, QPushButton, QLabel, QLineEdit, QCheckBox.
 *\~
 *\param nameWidget -\~english Widget name.
 		\~russian Имя виджета.
		\~
 *\param value -\~english value.
 		\~russian Значение виджета.
		\~
 */
int
aWidget::setValue( const QString & nameWidget, const QVariant &value )
{
	QObject *w;
	int	rc= err_noerror;

	w = Widget( nameWidget );
	if ( w ) {
		if (w->inherits("wField")){
			((aWidget*)w)->setValue(value.toString());
//		} else if (w->inherits("wField")){
//			((wField*)w)->setValue(value.toString());
		} else if (!strcmp(w->className(),"QPushButton")){
			((QPushButton *)w)->setText( value.toString() );
		} else if (!strcmp(w->className(),"QLabel")){
			((QLabel*)w)->setText( value.toString() );
		} else if (!strcmp(w->className(),"QLineEdit")){
			((QLineEdit*)w)->setText( value.toString() );
		} else if (!strcmp(w->className(),"QCheckBox")){
			((QCheckBox*)w)->setText(value.toString() );
		} else if (!strcmp(w->className(),"QDateEdit")){
			((Q3DateEdit*)w)->setDate( value.toDate() );
		}
	} else {
		//debug_message(tr("aForm::SetValue() Error! Can't find widget by name==`%s`\n"),(const char*) name.local8Bit());
		aLog::print(aLog::Error, tr("aWidget set value: not fount widget with name %1").arg(nameWidget) );
		rc = err_nowidgetfound;
	}
	return rc;
}



/*!
 *\~english
 *	Setting form widgets value.
 *	Must be implemented in inherited class.
 *\~russian
 * 	Функция установки значения виджетов формы.
 *	Должна быть реализована в наследующем классе.
 *\~
 *\param value -\~english value.
 		\~russian Значение виджета. Имя у виджета уже есть в момент вызова этой функции.
		\~
 */
void
aWidget::setValue( const QString & value)
{
}
/*!
 *\~english
 *	Setting form widgets value of object type.
 *	Support type of widgets: wField, wDBField.
 *\~russian
 * 	Установка значения виджетов формы сложного типа.
 *	Поддерживаемые типы виджетов: wField, wDBField.
 *\~
 *\param nameWidget -\~english Widget name.
 		\~russian Имя виджета.
		\~
 *\param value -\~english value.
 		\~russian Значение виджета.
		\~
 */
int
aWidget::setObjValue( const QString & nameWidget, aObject *value )
{
	QObject *w;
	int	rc= err_noerror;
	w = Widget( nameWidget );
	if ( w ) {
		if (w->inherits("wField") && value){
			((aWidget*)w)->setValue(QString("%1").arg(value->getUid()));
		}
	} else {
		//debug_message(tr("aForm::SetValue() Error! Can't find widget by name==`%s`\n"),(const char*) name.local8Bit());
		aLog::print(aLog::Error, tr("aWidget set value: not fount widget with name %1").arg(nameWidget) );
		rc = err_nowidgetfound;
	}
	return rc;
}


/*!
 *\~english
 *\~russian
 *\~
 */
bool
aWidget::isContainer()
{
	return false;
}



/*!
 *\~english
 *	Seting widget readonly flag.
 *	When widget readonly it's fields can't be edit, and make updatе action
 *\~russian
 *	Устанавливает флаг толко чтение.
 *	Когда установлен этот флаг, запрещается редактирование полей виджета,
 *	и сохранение изменений в базу.
 *\~
 *\param status - \~english New readonly flag.\~russian Новое значение флага только чтение.\~
 */
void
aWidget::SetReadOnly ( bool status )
{
//CHECK_POINT
	QObjectList l;// = this->queryList( "wField" );
	QObject *obj;
	l = this->queryList( "aWidget" );
	QListIterator<QObject*> itl( l );
	while ( itl.hasNext() )
	{
//	CHECK_POINT
		obj = itl.next();
		(( aWidget *)obj)->SetReadOnly( status );
	}
	//--delete l; // delete the list, not the objects
	l = this->queryList( "QFrame" );
	QListIterator<QObject*> tl( l );
	while ( tl.hasNext() )
	{
		//printf("QFrame classname '%s'\n", (const char*) obj->className() );
		obj = tl.next();
		if (obj->inherits("QTable")) (( Q3Table * )obj)->setReadOnly(true);
		//else (( QFrame *)obj)->setDisabled( status );
	}
	//--delete l; // delete the list, not the objects
	//--l=0;
}


/*!
 *
 */
qulonglong
aWidget::docId()
{
	if ( dbobj )
	{
		return dbobj->docId();
	}
	aLog::print(aLog::Error, tr("aWidget doc Id: invalid data source object") );
	return 0;
}



QString
aWidget::widgetName(QWidget *obj)
{
	QString res="";
	if (obj)
	{
		if (obj->inherits("wDBField"))
		{
			res= ((aWidget*)obj)->getFieldName();
		}
		else
		{
			res=obj->name();
		}
	}
	return res;
}




/*!
 *\~english
 *	Return form widget pointer by name.
 *\~russian
 *\~
*/
QWidget*
aWidget::Widget( QWidget *owner, QString name )
{
	QWidget *res = 0;
	QWidget* obj;
	QObjectList list = owner->queryList("QWidget");

	QListIterator<QObject*> it(list);
	while ( it.hasNext() ) {
			obj = qobject_cast<QWidget*>(it.next());
			if (!obj) continue;
			if ( name == aWidget::widgetName( obj ) ) {
				res = obj;
				break;
			}
	}
	return res;
}



/*!
 *\~english
 *	Return form widget pointer by name.
 *\~russian
 *\~
*/
QWidget *
aWidget::Widget( QString name, bool onlyMyWidgets )
{
	QWidget *res = aWidget::Widget( this, name );
	if ( onlyMyWidgets && parentContainer( res ) != this ) res = 0;
	return res;
}



/*!
 *\~english
 *	Return form widget pointer by name.
 *\~russian
 *\~
*/
aDataField*
aWidget::getAttribute( const QString & name )
{
	aDataField *f = 0;

	return f;
}



/*!
 *\~english
 *	Return form widget pointer by name.
 *\~russian
 *\~
*/
int
aWidget::setAttribute( const QString & name, const aDataField *value )
{
	return 0;
}


/*!
 *\~english
 *
 *\~russian
 * Обрабатывает события при нажатии кнопок клавиатуры.
 *\~
 */
void
aWidget::keyPressEvent ( QKeyEvent *e )
{
//	switch ( e->key() ){
//	case Qt::Key_Escape:
//		printf("ESC\n");
//		break;
//	default:
//		e->ignore();
//		break;
//	}

	QWidget::keyPressEvent( e );
}

