/****************************************************************************
** $Id: aform.cpp,v 1.3 2008/12/10 21:04:56 leader Exp $
**
** Code file of Ananas forms of Ananas
** Designer and Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2006 Leader InfoTech.  All rights reserved.
** Copyright (C) Andrey Paskal <app at lrn dot ru>, Yoshkar-Ola
** Copyright (C) Grigory Panov <gr1313 at mail dot ru>, Yoshkar-Ola
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

//--#include <qwidgetfactory.h>
#include <QFormBuilder>
#include <QDesignerCustomWidgetInterface>
#include <QCoreApplication>
#include <qdialog.h>
#include <qobject.h>
#include <qdialog.h>
#include <qbuffer.h>
#include <q3cstring.h>
#include <qiodevice.h>
#include <qstring.h>
#include <qfile.h>
#include <qdir.h>
//#include <qsinterpreter.h>
#include <QScriptValue>
#include <QScriptEngine>

//--#include <qbutton.h>
#include <qpushbutton.h>
//--#include <qfocusdata.h>
#include <qlabel.h>
#include <q3mainwindow.h>
#include <q3datetimeedit.h>
#include <qstatusbar.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qbuffer.h>
#include <qiodevice.h>
#include <qmessagebox.h>
#include <qstatusbar.h>
#include <q3sqlcursor.h>
#include <q3sqlpropertymap.h>
#include <q3sqlform.h>
#include <qwidget.h>
//Added by qt3to4:
#include <QKeyEvent>
#include <Q3ValueList>
#include <q3mimefactory.h>

#include "aform.h"
#include "wcatalogue.h"
#include "wdocument.h"
#include "wjournal.h"
#include "wreport.h"
#include "wfield.h"
#include "wdbfield.h"
#include "wtable.h"
#include "wdbtable.h"
#include "wactionbutton.h"
#include "alog.h"
/*!
Base form object.
*/
aForm::aForm( QWidget *parent,  aEngine *eng )
:QObject( eng, (QString("aForm%1").arg(eng->next_obj_id++)).ascii())
{
	engine = eng;
	db = eng->db;
	md = &db->cfg;
	parentWidget = parent;
	form = 0;
	closeAfterSelect = false;
	RO=false;
	callerWidget = NULL;
	mode = 0;
	db_uid=0;
	objid=0;
	init();
}



aForm::aForm( QWidget *parent,  aEngine *eng,  Q_ULONG form_mid, aWidget* caller )
:QObject( eng, (QString("aForm%1").arg(eng->next_obj_id++)).ascii() )
{
	aCfgItem obj;

	engine = eng;
	db = eng->db;
	md = engine->md;
	parentWidget = parent;
	form = 0;
	closeAfterSelect = false;
	mdObj = md->find( form_mid );
	objid = form_mid;
	db_uid=0;
	RO=false;
	callerWidget = caller;
	mode = 0;
	init();
}

aForm::aForm( QWidget *parent,  aEngine *eng, QString oftype, QObject *aobj )
:QObject( eng, (QString("aForm%1").arg(eng->next_obj_id++)).ascii())
{
	engine = eng;
	db = eng->db;
	md = &db->cfg;
	parentWidget = parent;
	db_uid=0;
	objid=0;
	form = 0;
	closeAfterSelect = false;
	RO=false;
	mode = 0;
	init();

}



aForm::~aForm()
{
}

aWidget *
aForm::parentContainer( QWidget *w )
{
        w = w->parentWidget();
        while ( w ) {
                if ( w->inherits("aWidget") ) break;
                w = w->parentWidget();
        }
	return ( aWidget *) w;
}


/*!
 *\~english
 *	Init container object, that not inherits from aWidget.
 *\~russian
 *	Инициализирует объекты, которые не унаследованы от aWidget, но
 *	требуют инициализации, например wDBTable.
 *\~
 */
void
aForm::initWidget( QWidget *widget, aDatabase *adb )
{
	QString oclass = widget->className();
	if (oclass == "wDBTable") ( ( wDBTable* ) widget )->init( adb, engine );
}


/*!
 *\~english
 *	Recursive init all form containers and widgets.
 *\~russian
 *	Инициализирует все виджеты, унаследованные от aWidget
 *\~
 */
void
aForm::initContainer( aWidget *widget, aDatabase *adb ){

	// Init subobjects.
	if ( !widget->isContainer() ) return;

	widget->engine = engine;
	widget->init( adb );
	QObjectList l = widget->queryList( "QWidget" );
	QListIterator<QObject*> it( l ); // iterate over all subwidgets
	QObject *obj;
	QString oclass;
	aWidget *aw;
//	QWidget *w;
	// Init all container owned widgets
	while ( it.hasNext()  ) {
		// for each found object...
		obj = it.next();
		if ( !obj ) continue;
		if ( parentContainer( ( QWidget *) obj ) != widget ) continue;
 		if ( obj->inherits("aWidget") ) {
			aw = ( aWidget* ) obj;
			aw->engine = engine;
			if ( !aw->isContainer() )
			{
				aw->init( adb );
			}
			if ( form->inherits( "QMainWindow" ) ) aw->createToolBar( ( Q3MainWindow *) form );
		} else initWidget( ( QWidget *) obj, adb );
	}
	it.toFront();
	// Init all container subcontainers
	while ( it.hasNext() ) {
		obj = it.next();
		if ( !obj ) continue;
                if ( obj == widget ) continue;
		if ( parentContainer( ( QWidget*) obj ) != widget ) continue;
		if ( obj->inherits("aWidget") ) {
			aw = ( aWidget* ) obj;
			aw->engine = engine;
			if ( aw->isContainer() ) initContainer( aw, adb );
		}
	}
	//delete l;
}


/*!
 *\~english
 *	Init form, reparent central widget of form, if it needed. Evaluate script module.
 *\~russian
 *	Инициализирует форму, меняет родителя у центрального виджера формы, если это необходимо.
 *	Обрабатывает также модуль скрипта, благодаря этому впоследствии можно вызывать
 *	функции, определенные в этом модуле.
 *\~
 */
void
aForm::init()
{
	QString ui, sModule;
	aCfgItem obj;
	static QMutex mutex;
	mainWidget = 0;
	if ( !mdObj.isNull() && md ) {
		ui = md->sText( mdObj, md_formdesign );
		sModule = md->sText( mdObj, md_sourcecode );
		if ( !ui.isEmpty() ) {
		    //aLog::print(aLog::Debug, ui);
		    QByteArray buf = ui.toUtf8();
			QBuffer b(&buf);
			//--b.open(QIODevice::WriteOnly);

			//--b.writeBlock( ( const char *) ui, strlen( ( const char *) ui) );
			//--b.close();
			b.open(QIODevice::ReadOnly );
			aLog::print(aLog::Info, tr("aForm creating form from ui"));
			//--form = QWidgetFactory::create( &b );
			QFormBuilder fb;
			fb.addPluginPath(QCoreApplication::applicationDirPath() /*"/plugins/designer"*/);
                        fb.addPluginPath("/usr/lib/ananas/");
                        /*QList<QDesignerCustomWidgetInterface*> l = fb.customWidgets();
			int i;
			for (i=0; i<l.size(); i++)
			{
			    aLog::print(aLog::Info, l[i]->name());
			}*/
                        form = fb.load(&b);
			aLog::print(aLog::Info, tr("aForm form create from ui ok"));
			b.close();
		}
	}
	if (form) {
		Q3MainWindow *mw = new Q3MainWindow( parentWidget, "main form", Qt::WDestructiveClose );
  		mw->statusBar()->hide();
		mw->setCaption( form->caption() );

		printf("try find in windowslist %d, %llu\n ", objid, db_uid);
		if ( engine->wl->find( objid, db_uid ) )
		{
			printf("found, remove\n");
			engine->wl->remove( objid, db_uid );
		}
		else
		{
			printf("not found\n");
		}
		engine->wl->insert( objid, mw, db_uid );
		printf("insert new in wl ok\n");
		if ( form->inherits("QMainWindow" )){
			((Q3MainWindow *) form)->statusBar()->hide();
		}
	    	mainWidget = 0;
		aCfgItem par = md->parent( md->parent( mdObj ) );
		if ( !form->inherits("aWidget"))
		{
			aLog::print(aLog::Error, tr("aForm do not inherits aWidget!"));
			QMessageBox::critical( 0, tr("Error"), tr("Error while open dialog form. Init hasn't been completed.") );
			return;
		}
                // Create main widgets container.
	    	if ( md->objClass( par ) == md_catalogue ) {
                        mainWidget = ( wCatalogue *) form; //new wCatalogue( mw );
                        mw->setIcon( rcIcon("wcatalogue.png"));
                }
	    	if ( md->objClass( par ) == md_document ) {
                        mainWidget = ( wDocument *) form; //new wDocument( mw );
                        mw->setIcon( rcIcon("wdocument.png"));
                }
	    	if ( md->objClass( par ) == md_journal ) {
                        mainWidget = ( wJournal *) form; //new wJournal( mw );
                        mw->setIcon( rcIcon("wjournal.png"));
			mw->statusBar()->show();
			mw->statusBar()->message(tr("New document - <Ins>, Edit - <Enter>, View - <Shift+Enter>"));
		}
	    	if ( md->objClass( par ) == md_report ) {
                        mainWidget = ( wReport *) form;
                        mw->setIcon( rcIcon("wreport.png"));
                }

		if ( !mainWidget ) {
        		form->reparent( parentWidget, Qt::WDestructiveClose, QPoint(0, 0));
                        delete mw;
                } else {
			form = mw;
			mainWidget->setFormMode( md->attr( mdObj, mda_type ).toInt());
			mainWidget->createToolBar( mw );
			mw->resize( mainWidget->size() );
			mainWidget->reparent( mw, 0, QPoint(0, 0));
			mw->setCentralWidget( mainWidget );
			initContainer( mainWidget, db );
			mainWidget->show();
			form = mw;
			mw->setFocusPolicy( Qt::NoFocus );
                }
            //--
            ((QWorkspace*)engine->ws)->addWindow(form);
            form->show();

		connectSlots();
		if ( !sModule.isEmpty() )
		{
//                        engine->project.interpreter()->evaluate(sModule,this );
                        engine->code->evaluate(sModule);
			aLog::print(aLog::Debug, tr("aForm load form module script"));
//			QStringList lst = engine->project.interpreter()->functions(this);
//			for(uint i=0; i<lst.count();i++)
//			{
//				aLog::print(aLog::Debug, tr("aForm defined function %1").arg(lst[i]));
//			}
		}
		else
		{
			aLog::print(aLog::Debug, tr("aForm form module script is empty"));
		}
		if ( md->attr( mdObj, mda_readonly ).toInt()) SetReadOnly(true);
	}
	else
	{
		aLog::print(aLog::Error, tr("aForm form not found"));
		QMessageBox::critical( 0, tr("Error"), tr("Error open dialog form. Form not found.") );

	}
}

/*!
 *\~english
 *	Return ananas contaner object for widget.
 *	Use for widget->init() function.
 *\~russian
 *	Возвращает объект-контейнер для заданного виджета.
 *	\param widget - виджет, для которого необходимо найти объект-контейнер.
 *	\return объект контейнер.
 *\~
*/
QWidget *
aForm::aParent( QWidget *widget )
{
	QWidget *p = widget->parentWidget();
	QString pc;
	while ( p ){
		pc = p->className();
		if ( pc == "wCatalogue") break;
		if ( pc == "wDocument") break;
		p = p->parentWidget();
	}
	return p;
}


/*!
 *\~english
 *	Show form. Move it in left top corner of workspace.
 *\~russian
 *	\brief ScriptAPI. Показывает форму на экране.
 *
 *	Перемещает ее в левый верхний угол рабочего пространства. Вызывает функцию on_formstart() скрипта модуля
 *	экранной формы, если функция определена в модуле.
 *
 *	\see close
 *\~
*/
void
aForm::Show()
{
	if ( form )
	{
//		if ( engine->project.interpreter()->functions( this ).findIndex("on_formstart")!=-1)
//		{
//			engine->project.interpreter()->call("on_formstart", QVariantList(), this);
//		}
                if ( engine->code->globalObject().property("on_formstart").isValid() ){
                    engine->code->globalObject().property("on_formstart").call();
                }

		form->show();
		((QWidget*)form->parent())->move(0,0);
		connect( form, SIGNAL(destroyed()), this, SLOT(close()) );
    }
}

/**
 * \brief Deprecated. See Show()
 */
void
aForm::show() {
	aLog::print(aLog::Debug,tr("Deprecated method call: aForm::show()"));
	this->Show();
}


/*!
 *\~english
 *	Close form. Delete this pointer after close.
 *\~russian
 *	\brief ScriptAPI. Закрывает и уничтожает форму.
 *
 *	Вызывает функцию on_formstop()
 *	скрипта модуля экранной формы, если функция определена в модуле.
 *
 *	\see show
 *\~
*/
void
aForm::Close() {
        // don't call function name() in this place in Win32 - crash
	emit(closeForm(selectedCatId()));
	on_form_close(); //to run ananas-script
	
	if( form ) {
		if( form->isShown() ) {
			aLog::print(aLog::Debug,tr("aForm::Close() hides form"));
			disconnect( form );
			form->hide();
		}
	}
	if( engine && engine->wl ) {
		engine->wl->remove( objid, db_uid );
	}
    deleteLater();
}

/**
 * \brief Deprecated
 */
void
aForm::close() {
	aLog::print(aLog::Debug,tr("Deprecated method call: aForm::close(). Use aForm::Close() instead"));
	this->Close();
}

/**
 * 	\brief Deprecated
 */
int
aForm::turn_on(){
	aLog::print(aLog::Debug,tr("Deprecated method call: aForm::turn_on()"));
	return SignIn();
}

/*!
 *\~english
 *	Turn on document.
 *\~russian
 *	\brief ScriptAPI. Проводит (регистрирует) документ.
 *
 *	Вызывает функцю on_conduct() кода Ананас.скрипта модуля экранной формы.
 *	В скрипте вы можете проверить условия проведения документа,
 *	и, если они не выполняются, вернуть в функции false.
 *	Если функция возвращает false, то отметка о проведении не ставится.
 *\~
 */
int
aForm::SignIn(){
        QScriptValue res;
	if ( form && !mainWidget->dataObject()->IsConducted())
	{
//		if ( engine->project.interpreter()->functions( this ).findIndex("on_conduct")!=-1)
//		{
//			res  = engine->project.interpreter()->call("on_conduct",QVariantList(), this);
//		}
                if ( engine->code->globalObject().property("on_conduct").isValid() ){
                    res = engine->code->globalObject().property("on_conduct").call();
                }

	}
        // if return false
        if(res.isValid() && res.toBool() == false )
        {
                aLog::print(aLog::Info, tr("aForm conduct: function on_conduct() return false, document not conducted"));
                return 0;
        }
	return mainWidget->TurnOn();
}

/**
 * 	\brief Deprecated
 */
int
aForm::turn_off(){
	aLog::print(aLog::Debug,tr("Deprecated method call: aForm::turn_off()"));
	return SignOut();
}

/*!
 *\~english
 *	Turn off document. Do nothing.
 *\~russian
 *	\brief ScriptAPI. Отменяет проведение (регистрацию) документа.
 *
 *	На самом деле ничего не делает. Отмена проведения производится при открытии
 *	документа.
 *\~
 */
int
aForm::SignOut(){
	return 0;
}

/**
 * 	\brief Depticated
 */
ERR_Code
aForm::update(){
	aLog::print(aLog::Debug,tr("Deprecated method call: aForm::update()"));
	return this->UpdateDB();
}

/*!
 *\~english
 *	Update object.
 *\~russian
 *	\brief ScriptAPI. Обновляет данные в базе.
 *\~
 */
ERR_Code
aForm::UpdateDB()
{
	ERR_Code	rc = err_noerror;
	aWidget*	aw = NULL;

	if ( RO ) return err_readonly;
	rc = mainWidget->Update();
	if (!rc && callerWidget!=NULL)
	{
		emit( changedData());
		emit( update( db_uid ) );
	}
	return rc;
}

/*!
 *\~english
 *	do nothing.
 *\~russian
 *	Ничего не делает.
 *\~
char*
aForm::formMetaObjectId(QString filename){
//	if (filename==""){
//	}
//	return aot_doc;
    return "";
}
 */



//QString
//aForm::widgetName(QObject *obj){
//	return aWidget::widgetName( ( QWidget *) obj );
/*
	QString res="";

	if (obj) {
*/
/*
		if (!strcmp(obj->className(),"QAnanasField")) res=((QAnanasField *)obj)->getName();
		else
		if (!strcmp(obj->className(),"QAnanasDBField")) res=((QAnanasDBField *)obj)->getName();
		else
		if (!strcmp(obj->className(),"QAnanasTable")) res=((QAnanasTable *)obj)->getName();
		else
*/
/*		if (!strcmp(obj->className(),"wDBField")) {
			res= ((wDBField*)obj)->getMdFieldName();
//			printf("wdbfield name=%s\n",(const char*)((wDBField*)obj)->getMdFieldName().local8Bit());
		} else {
			res=obj->name();
		}
	}
	return res;
*/
//}



/*!
 *\~english
 *	Return form widget pointer by name.
 *\~russian
 *	\brief ScriptAPI. Возвращает указатель на виджет по его имени.
 *
 *	Очень полезная функция для работы с QT виджетами, находящимися в экранной форме. Используя этот метод,
 *	можно получить доступ к методам любого виджета, лежащего в форме.
 *	ВАЖНО! Из Ананас скрипта вы сможете обратиться только к публичным слотам и свойствам виджета. Публичные методы
 *	виджета из Ананас скрипта недоступны.
 *	Документацию по свойствам и методам QT виджетов смотрите на сайте http://trolltech.com
 *
 * 	\code
 *	// допустим в экранной форме есть виджет myCheckBox, являющийся экземпляром класса QCheckBox
 *
 *	var myBox = Widget("myCheckBox"); //возвращает указатель на объект
 *	if ( myBox.checked ) { 		// Проверяем значение свойства checked
 *		sys.Message(0, "Yes" );
 *	} else {
 *		sys.Message(0, "No" );
 *	}
 *
 *	// К сожалению мы не имеем доступа к методам  setChecked() и isChecked(), так как они не являются публичными слотами.
 *
 * 	\endcode
 *	\see aWidget::Widget()
 *\~
*/
QWidget *
aForm::Widget( QString name )
{
	return mainWidget->Widget( name );
}

//QObject *
//aForm::findWidget( QString name ){

//	return mainWidget->Widget( name );
/*
	QObject *res=NULL;
	QObject* obj;
	QObjectList* list = form->queryList("QWidget");

	QObjectListIt it(*list);
	while ( ( obj = it.current() ) !=0 ) {
			++it;
			if (!obj) continue;
			if (name==widgetName(obj)) {
				res=obj;
				break;
			}
	}
	delete list;
	return res;
*/
//}


/*!
 *\~english
 *	Connecting widget slots to form.
 *\~russian
 *	Коннектит некоторые слоты объектов формы.
 *	Обеспечивает работу функций ананас скрипта
 *	on_valuechanged(), on_button(), on_tabvaluechanged(), etc.
 *\~
 */
void
aForm::connectSlots()
{
	QObject* obj;
	if ( !form ) return;
	QObjectList list = form->queryList("QWidget");
	if ( mainWidget->inherits("aWidget") )
		connect( mainWidget, SIGNAL( valueChanged( const QString &, const QVariant & ) ),\
				 this, SLOT( on_valueChanged( const QString &, const QVariant & ) ) );

	QListIterator<QObject*> it(list);
	while ( it.hasNext() ) {
			obj = it.next();
			if (!obj) continue;
			if ( obj->inherits("wActionButton" )){
				connect( obj, SIGNAL( clicked() ), this, SLOT( on_actionbutton() ) );
				connect( mainWidget, SIGNAL( keyPressed(QKeyEvent*) ), obj, SLOT( keyPressHandler(QKeyEvent*) ) );
                        //        continue;
			}
			if ( obj->inherits("QPushButton" )){
				connect( obj, SIGNAL( clicked() ), this, SLOT( on_button() ) );
                                continue;
			}
			if ( obj->inherits("wDBTable" )){
				connect( obj, SIGNAL( currentChanged(  QSqlRecord * ) ), this, SLOT( on_dbtablerow( QSqlRecord * ) ) );
				connect( obj, SIGNAL( deleteLine( QSqlRecord * ) ), this, SLOT( on_tabupdate( QSqlRecord * ) ) );

				connect(obj,	SIGNAL(updateCurr(int,int) ),
					this,	SLOT( on_tabvalueChanged(int, int) ) );
//				connect( obj, SIGNAL( primeUpdate(  QSqlRecord * ) ), this, SLOT( on_tabupdate( QSqlRecord * ) ) );
				connect( obj, SIGNAL( selected( qulonglong ) ), this, SLOT( on_tabselected( qulonglong ) ) );
				connect( obj, SIGNAL( selectRecord( qulonglong ) ), this, SLOT( on_tablerow( qulonglong ) ) );
                                continue;
			}
	}
	//--delete list;
}

void
aForm::on_tabupdate( QSqlRecord *rec )
{
}

/*!
 * \ru
 * 		\brief ScriptAPI. Возвращает значение поля экранной формы.
 *
 * 		Например, значение поля шапки "Номер" приходной накладной.
 * 		Слот предназначен для использования Ананас-Скриптом.
 * 		\param name - имя поля, как оно определено в метаданных. Например, "Номер" или "Покупатель".
 * 		\return значение поля. Необходимо понимать, что если поле имеет сложный тип данных, например
 * 		его значение задается выбором элемента из справочника товаров, то будет возвращено текстовое
 * 		представление товара (его название), а не ссылка на составной объект, хранящий все атрибуты товара.
 * 		Для того что бы получить доступ к составному объекту, для обращения ко всем атрибутам товара
 * 		необходимо воспользоваться функцией DBValue();
 *\see aForm::DBValue()

Пример кода для размещения в модуле экранной формы. Экранная форма содержит одну кнопку с именем Button1 и
поле ввода LineEdit1.
После ввода значения, следует нажать на кнопку и в окно сообщений будет выведено только что введенное значение.


\code
function on_button(buttonName) // обработчик нажатия кнопки
{
	if(buttonName=="Button1") // Button1 - имя кнопки, данное ей в дизайнере
 	{
 		// Получаем значение поля LineEdit1 экранной формы
 		str = Value("LineEdit1");

		// Выводим в окно сообщений, полученное значение
 	        StatusMessage(str);
 	}
}
\endcode
 * \_ru
 */
QVariant
aForm::Value(const QString &name)
{
	QVariant res= mainWidget->value( name ); //value();
	if(res.type()==QVariant::ULongLong || res.type()==QVariant::LongLong)
	{
		res = res.toString();
	}
	//if(res.type()==QVariant::DateTime)
	//{
	//	res = res.toDateTime().toString(ISODate);
	//}
	return res;

/*
	aObject *o = 0;
        if ( mainWidget ) o = mainWidget->dataObject();
        if ( o ) {
                res = o->Value( name );
        }
        if ( res.type() != QVariant::Invalid ) return res;
        else printf("no field %s in data object\n", ( const char *) name );
*/
/*
	w = findWidget( name );
	if ( w ) {
		if (!strcmp(w->className(),"wDBField")){
			res=( (wDBField*)w )->text(); //value();
		} else if (w->inherits("wField")){
			res=( (wField*)w )->text(); //value();
		} else if (!strcmp(w->className(),"QPushButton")){
			res=((QPushButton *)w)->text();
		} else if (!strcmp(w->className(),"QLabel")){
			res=((QLabel*)w)->text();
		} else if (!strcmp(w->className(),"QLineEdit")){
			res=((QLineEdit*)w)->text();
		} else if (!strcmp(w->className(),"QDateEdit")){
			res=((QDateEdit*)w)->date().toString(Qt::ISODate);
		}
	} else {
		debug_message(tr("Error! Can't find widget by name==`%s`\n"),(const char*) name.local8Bit());
	}
	return res;
*/
}


/*!
 *\~english
 *	Getting widgets value, inherited aWidget.
 *\~russian
 *	\brief ScriptAPI. Возвращает значение атрибута <name> бизнес объекта Ананаса.
 *
 * 	Предназначен для использования в контексте экранной формы редактирования бизнес объекта.
 * 	Или, говоря по другому, в модуле экранной формы.
 * 	Тип возвращаемого значения такой же, как задан в метаданных у соответствующего атрибута бизнес объекта.
 * 	Для поля типа Каталог или Документ функция вернет число (id).
 * 	Функция необходима, если нужно заполнять поля сложных типов.
 * 	Для остальных случаев можно использовать aForm::Value(), обеспечивающее доступ к значению виджета экранной формы,
 * 	в контексте которой исполняется Скрипт.
 *\~
 *\param name -\~english Widget name. For wDBField this parameter must be name field of metadata.
 		\~russian Имя атрибута бизнес объекта, как задано в метаданных.
		\~

Пример кода для размещения в модуле экранной формы. Экранная форма содержит одну кнопку с именем Button1.
При нажатии на кнопку в окно сообщений будет выведено значение атрибута "Покупатель" редактируемого бизнес объекта.


\code
function on_button(buttonName) // обработчик нажатия кнопки
{
	if(buttonName=="Button1") // Button1 - имя кнопки, данное ей в дизайнере
 	{
 		// Получаем значение атрибута "Покупатель" редактируемого бизнес объекта
 		contragent = DBValue("Покупатель");

		// Выводим в окно сообщений, полученное значение
 	        StatusMessage(contragent);
 	}
}
\endcode
\~
 */
QVariant
aForm::DBValue(const QString &name)
{
	QObject *w;
	QVariant res;
	w = Widget( name );
	if ( w )
	{
		if (!strcmp(w->className(),"wDBField"))
		{
			res=( (wDBField*)w )->value(); //value();
			if(res.type()==QVariant::ULongLong || res.type()==QVariant::LongLong)
			{
				res = res.toString();
			}
			//if(res.type()==QVariant::DateTime)
			//{
			//	res = res.toDateTime().toString(ISODate);
			//}
		}
		else
		{
			if (w->inherits("wField"))
			{
				res=( (wField*)w )->value(); //value();
			}
			if(res.type()==QVariant::ULongLong || res.type()==QVariant::LongLong)
			{
				res = res.toString();
			}
			//if(res.type()==QVariant::DateTime)
			//{
			//	res = res.toDateTime().toString(ISODate);
			//}
		}
	}
	else
	{
		aLog::print(aLog::Error, tr("aForm not found widget with name %1").arg(name));
	}
	return res;
}


/*!
 *\~english
 *	Setting form widgets value of object type.
 *\~russian
 * 	\brief ScriptAPI. Устанавливает значения виджетов формы сложных типов (aDocument, aCatalogue)
 *
 * 	Обычно для установки значения используют SetValue(...)
 *\~
 *	\see SetValue(...)
 *\param tname -\~english Widget name.
 		\~russian Имя виджета.
		\~
 */
int
aForm::SetObjValue(const QString &name, aObject *value)
{
	return mainWidget->setObjValue( name, value );
}

/*!
 *\~english
 *	Setting form widgets value.
 *\~russian
 * 	\brief ScriptAPI. Устанавливает значения виджетов экранной формы.
 *\~
 *	\see aWidget::setValue()
 *\param name -\~english Widget name.
 		\~russian Имя виджета.
		\~
 *\param value -\~english Widget value.
 		\~russian Значение виджета. Для сложных типов (Документ,Каталог) должен использоваться идентификатор.
 *		Если идентификатор недоступен, используйте функция SetObjValue(), которая принимает в качестве
 *		параметра ссылку на объект.
		\~
 */
int
aForm::SetValue(const QString &name, QVariant value)
{
	return mainWidget->setValue( name, value );
/*
	QObject *w;
	int	rc= err_noerror;

	w = findWidget( name );
	if ( w ) {
		if (!strcmp(w->className(),"wDBField")){
			((wDBField*)w)->setValue(value.toString());
		} else if (w->inherits("wField")){
			((wField*)w)->setValue(value.toString());
		} else if (!strcmp(w->className(),"QPushButton")){
			((QPushButton *)w)->setText( value.toString() );
		} else if (!strcmp(w->className(),"QLabel")){
			((QLabel*)w)->setText( value.toString() );
		} else if (!strcmp(w->className(),"QLineEdit")){
			((QLineEdit*)w)->setText( value.toString() );
		}
	} else {
		debug_message(tr("aForm::SetValue() Error! Can't find widget by name==`%s`\n"),(const char*) name.local8Bit());
		rc = err_nowidgetfound;
	}
	return rc;
*/
}

/*!
 *\~english
 *	Setting column readonly flag.
 *	When column readonly, user can't edit value of this column.
 *\~russian
 *	\brief ScriptAPI. Устанавливает для столбца таблицы флаг только чтение.
 *
 *	Когда установлен этот флаг, пользователь не может редактировать значения в этом столбце.
 *\~
 *\param tname - \~english Table name.\~russian Имя таблицы.\~
 *\param numCol - \~english Column number.\~russian Номер столбца.\~
 *\param ro - \~english `Readonly' flag value.\~russian Значение флага `только чтение'.\~
 */
void
aForm::SetColumnReadOnly(const QString &tname, int numCol, bool ro)
{
	QObject *w;
	w = Widget( tname );
	if ( w )
	{
		if (!strcmp(w->className(),"wDBTable"))
		{
			((wDBTable*)w)->setColumnReadOnly(numCol,ro);
		}
	}
	else
	{
		aLog::print(aLog::Error, tr("aForm not found widget with name %1").arg(tname));
	}

	return;
}


/*!
 *\~english
 *	Count table row.
 *\~russian
 *	\brief ScriptAPI. Возвращает количество строк в табличном виджете wDBTable.
 *
 *	Если табличный виджет не является объектом класса wDBTable,
 *	метод запишет в лог сообщение об ошибке и вернет 0.
 *\~
 *\param tname - \~english Table name.\~russian Имя таблицы.\~
 *\return - \~english Number rows of table.\~russian Количество строк в таблице.\~
 */
int
aForm::TabCount(const QString &tname)
{
	int res=0;
	QObject *w;
	w = Widget( tname );
	if ( w )
	{
		if (!strcmp(w->className(),"wDBTable"))
		{
			res=( (wDBTable*)w )->numRows() ; //value();
		}
		else
		{
			aLog::print(aLog::Error, tr("Expected wDBTable widget but found %1").arg(w->className()));
		}
	}
	else
	{
		aLog::print(aLog::Error, tr("aForm not found widget with name %1").arg(tname));
	}

        return res;

}

/**
 * \ru
 * 	\brief ScriptAPI. Добавляет новую строку в табличный виджет wDBTable.
 *
 * 	\param tname - имя виджета
 * \_ru
 */
void
aForm::TabNewLine(const QString &tname)
{
       QObject *w;

       w = Widget( tname );
       if ( w  && !strcmp(w->className(),"wDBTable"))
       {
               wDBTable *wdb = (wDBTable*)w;
               Q3SqlCursor *cur = wdb->sqlCursor();
               QSqlRecord *buffer = cur->primeInsert();
               wdb->lineInsert(buffer);
               cur->insert();
       }
       else
       {
               aLog::print(aLog::Error, tr("aForm not found wDBTabe widget with name %1").arg(tname));
       }
}

void
aForm::TabUpdate(const QString &tname)
{
       QObject *w;

       w = Widget( tname );
       if ( w  && !strcmp(w->className(),"wDBTable"))
       {
               wDBTable *wdb = (wDBTable*)w;
               Q3SqlCursor *cur = wdb->sqlCursor();
               //QSqlRecord *buffer = cur->primeInsert();
             //  wdb->lineInsert(buffer);
               cur->primeUpdate();
	       cur->update();
       }
       else
       {
               aLog::print(aLog::Error, tr("aForm not found wDBTabe widget with name %1").arg(tname));
       }
}
/**
 * \ru
 * 	\brief ScriptAPI. Возвращает значение ячейки табличного виджета wDBTable.
 *
 *
 * 	Если табличный виджет не является объектом класса wDBTable, метод запишет сообщение
 * 	об ошибке в лог и вернет строку "Unknown".
 * 	То есть он предназначен только для работы с таблицами привязанными (binded) к
 * 	табличным частям бизнес объектов Ананаса. Если в ячейке хранится сложный тип данных (Документ, Элемент справочника)
 * 	будет возвращен уникальный идентификатор объекта, а не его текстовое представление, которое видит пользователь.
 * 	Для получения текстового представления сложного объекта воспользуйтесь методом aForm::TabDBValue.
 *
 * 	\param tname - имя виджета
 * 	\param row - индекс строки таблицы
 * 	\param col - индекс столбца таблицы
 * 	\return значение ячейки табличного виджета
 * 	\see aForm::TabDBValue(...)
 * \_ru
 *
 */
QVariant
aForm::TabValue(const QString &tname, int row, int col)
{
	return tabValue(tname,row,col,true);
}

/**
 * \ru
 * 	\brief ScriptAPI. Возвращает значение ячейки табличного виджета wDBTable для сложного типа данных.
 *
 *
 * 	Если табличный виджет не является объектом класса wDBTable, метод запишет сообщение
 * 	об ошибке в лог и вернет строку "Unknown".
 * 	То есть он предназначен только для работы с таблицами привязанными (binded) к
 * 	табличным частям бизнес объектов Ананаса.
 * 	В отличие от aForm::TabValue, который вернет числовой идентификатор, в случае, если ячейка имеет сложный тип данных,
 * 	aForm::TabDBValue вернет текстовое представление, которое видит в таблице пользователь.
 *
 * 	\param tname - имя виджета
 * 	\param row - индекс строки таблицы
 * 	\param col - индекс столбца таблицы
 * 	\return значение ячейки табличного виджета
 * 	\see aForm::TabValue(...)
 * \_ru
 *
 */
QVariant
aForm::TabDBValue(const QString &tname, int row, int col)
{
	return tabValue(tname,row,col,false);
}


QVariant
aForm::tabValue(const QString &tname, int row, int col, bool dbval)
{
	QVariant res=QString("Unknown");
	QObject *w;
	QStringList l;
	w = Widget( tname );
	if ( w  && !strcmp(w->className(),"wDBTable"))
	{
		wDBTable *wdb = (wDBTable*)w;
			if(wdb->getDefIdList().count()>col && col>=0)
			{
				long f_oid = wdb->getDefIdList()[col].toLong();
				QString type = wdb->getFieldType(f_oid);
				if(row==-1)
				{
					row = wdb->currentRow();
				}
				if(type.left(1) == "O" && dbval)
				{
					res = ananas_objectstr(db, wdb->value(row,col).toULongLong(), type.section(' ',1,1).toInt());
				}
				else
				{
					res = wdb->value(row,col);
					//if( res.type() == QVariant::DateTime || res.type() == QVariant::Date )
					//{
					//	res = QString("%1").arg(res.toDateTime().toString(Qt::ISODate));
					//}
				}
			}
			else
			{
				aLog::print(aLog::Error, tr("aForm table value: column out of range"));
			}
	}
	else
	{
		aLog::print(aLog::Error, tr("aForm not found widget with name %1").arg(tname));
	}
        return res;
}


/*!
 *\~english
 *	Return column index in table \param tname.
 *\~russian
 *	\brief ScriptAPI. Возвращает индекс столбца таблицы tname по его имени в метаданных.
 *\~
 * \param tname - \~english Table name.\~russian Имя таблицы.\~
 * \param colname - \~english Column name.\~russian Имя столбца.\~
 * \return  \~english Column index.\~russian Индекс столбца.\~
 */
int
aForm::ColIndex(const QString &tname, const QString &colname)
{
	int res;
	QObject *w;
	int col=-1;
	w = Widget( tname );
	if ( w  && !strcmp(w->className(),"wDBTable"))
	{
			wDBTable *wdb = ((wDBTable*)w );
			col= wdb->getDefFields().findIndex(colname);
	}
	else
	{
		aLog::print(aLog::Error, tr("aForm not found widget with name %1").arg(tname));
	}
	return col;
}


/**
 * \ru
 * 	\brief ScriptAPI. Задает значение ячейки табличного виджета wDBTable.
 *
 * 	\param tname - имя таблицы
 * 	\param colname - имя столбца
 * 	\param row - номер строки
 * 	\param value - значение
 * \_ru
 */
void
aForm::SetTabValue(const QString &tname, const QString &colname, int row, QVariant value)
{
	QObject *w;
	w = Widget( tname );
	if ( w  && !strcmp(w->className(),"wDBTable"))
	{
	//	printf(">>>>>>>>>>>>>>Set tab value %s, for %s, rov = %d, value = %s\n",(const char*)tname.local8Bit(), (const char *)colname.local8Bit(), row, (const char*)value.toString().local8Bit() );
		wDBTable *wdb = (wDBTable*)w;
		aSQLTable *t = ( aSQLTable *) wdb->sqlCursor();
		t->select(QString("idd=%1").arg(mainWidget->uid()));
		t->first();
		if(t->seek(row))
		{
			t->setValue(colname,value);
			t->Update();
			//t->update();
		}
		wdb->refresh();//updateCurrent();
			//	wdb->setValue(row, col, value);
				//res = wdb->text(row,col);
	}
	else
	{
		aLog::print(aLog::Error, tr("aForm not found wDBTable widget with name %1").arg(tname));
	}

        return;

}



/**
 * 	\brief Deprecated
 */
void
aForm::done( int rc ){
	aLog::print(aLog::Debug,tr("Deprecated method call: aForm::done(...)"));
	rc=rc;
//	if (form) form->done(rc);
}

/**
 * 	\brief Deprecated
 */
void
aForm::setfocus(QString fname){
	aLog::print(aLog::Debug,tr("Deprecated method call: aForm::setfocus()"));
//	QWidget *w;
//	w=(QWidget *) form->child(fname,"QWidget");
//	if (w) w->setFocus();
}

/**
 *
 */
void
aForm::SetFocus(){
//	aLog::print(aLog::Debug,tr("Deprecated method call: aForm::setfocus()"));
//	QWidget *w;
//	w=(QWidget *) form->child(fname,"QWidget");
	if(form) form->setFocus();
}

void
aForm::on_button(){
//	if ( engine->project.interpreter()->functions(this).findIndex("on_button")!=-1)
//	{
//		engine->project.interpreter()->call("on_button",QVariantList()<<sender()->name(),this);
//	}
        if ( engine->code->globalObject().property("on_button").isValid() ){
            engine->code->globalObject().property("on_button").call();
        }

}



/*!
 *\~english
 *	Action button pressed.
 *\~russian
 *\~
 */
void
aForm::on_actionbutton()
{
        wActionButton *b = ( wActionButton *) sender();
        if ( b ) {
                if ( b->isAction() ) {
                        if ( b->isActionUpdate() ) update();
                        if ( b->isActionTurnOn() ) turn_on();
                        if ( b->isActionClose() )
                        {
	                       	if(form)
				if( form->isShown() )
				{
//#ifndef _Windows
					form->disconnect();
					form->hide();
//#endif
				}
	                        close();
                         }
                } else {
			if(b->getActionId())
			{
                		aCfgItem item = md->find(b->getActionId());
				engine->execAction( item, this);
                        }
                }
        }
}



void
aForm::on_return(){
}


void
aForm::on_lostfocus(){
}


void
aForm::on_form_close(){
	if(!engine) return;
//	if ( engine->project.interpreter()->functions(this).findIndex("on_formstop")!=-1) {
//		engine->project.interpreter()->call("on_formstop", QVariantList(),this);
//	}
        if ( engine->code->globalObject().property("on_formstop").isValid() ){
            engine->code->globalObject().property("on_formstop").call();
        }
}


void
aForm::on_valueChanged(const QString &s){
//	char buf[200];
/*
	const QObject *sobj=sender();
	QString objName=widgetName((QObject *) sobj);
	cfg_message(0, "value changed %s (%s)\n", (const char *) objName.utf8(), (const char *) dbobject->className());
	cfg_message(0, "classname %s\n", (const char *) sobj->className());
	if (strcmp(sobj->className(),"QAnanasDBField")==0) {
//	if (sobj->className()=="QAnanasDBField") {
		cfg_message(0,"OK");
		if (dbobject->className()=="QAdoc"){
			((QAdoc *)dbobject)->setfield(objName, s);
		}
		if (dbobject->className()=="QAcat"){
			cfg_message(0,"Поле справочника %s=%s\n",(const char *) objName.utf8(), (const char *) s.utf8());
			((QAcat *)dbobject)->setfield(objName, s);
		}
	}
*/
}



/*!
 *\~english
 *	Process field value changed.
 *	Connect on main widget signal, and work only with it fields.
 *	Call afta widget process change field value signal.
 *\~russian
 *	Обрабатывает изменение значения поля
 *	Присоединяется на сигнал главнов виджета формы
 *	и обрабатывает только изменение его полей.
 *	Сигнал вызывается виджетом, после того, как он сам обработает новое значение.
 *\~
 *\param name - \~english field metedata name \~russian имя поля в конфигурации.\~
 *\param value - \~english new field value \~russian новое значение поля. \~
 */
void
aForm::on_valueChanged( const QString & name, const QVariant & val )
{

//	if ( engine->project.interpreter()->functions(this).findIndex("on_valuechanged")!=-1)
//	{
//		Q3ValueList<QVariant> lst;
//		lst << name;
//		lst << val;
//		engine->project.interpreter()->call("on_valuechanged",QVariantList(lst), this);
//	}
        if ( engine->code->globalObject().property("on_valuechanged").isValid() ){
            QScriptValueList list;
            list.append(QScriptValue(name));
            list.append( engine->code->newVariant(val));
            engine->code->globalObject().property("on_valuechanged")
                    .call(QScriptValue(),list);
        }

}


void
aForm::on_tabvalueChanged(int row, int col)
{

//	if ( engine->project.interpreter()->functions(this).findIndex("on_tabupdate")!=-1)
//	{
//		Q3ValueList<QVariant> lst;
//		lst << row;
//		lst << col;
//		lst << sender()->name();
//
//		engine->project.interpreter()->call("on_tabupdate",QVariantList(lst), this);
//	}
        if ( engine->code->globalObject().property("on_tabupdate").isValid() ){
            QScriptValueList list;
            list << row;
            list << col;
            list << sender()->name();
            engine->code->globalObject().property("on_tabupdate")
                    .call(QScriptValue(),list);
        }
}


void
aForm::on_dbtablerow( QSqlRecord *r )
{
        unsigned int i;
        aObject *o = 0;
        QVariant tv;
        if ( mainWidget ) o = mainWidget->dataObject();
        if ( o )
	{
		aSQLTable *t = o->table();
		id = t->value(0).toULongLong();
        }
//	if ( engine->project.interpreter()->functions(this).findIndex("on_tablerow")!=-1) {
//		engine->project.interpreter()->call("on_tablerow", QVariantList()<<sender()->name(), this);
//	}
        if ( engine->code->globalObject().property("on_tablerow").isValid() ){
            QScriptValueList list;
            list << sender()->name();
            engine->code->globalObject().property("on_tablerow")
                    .call(QScriptValue(),list);
        }
}

void
aForm::on_event( const QString &source, const QString &data )
{
//	Q3ValueList<QVariant> lst;
//	lst << source;
//	lst << data;
//	if ( engine->project.interpreter()->functions(this).findIndex("on_event")!=-1) {
//		engine->project.interpreter()->call("on_event", QVariantList(lst), this);
//	}
        if ( engine->code->globalObject().property("on_event").isValid() ){
            QScriptValueList list;
            list << source;
            list << data;
            engine->code->globalObject().property("on_event")
                    .call(QScriptValue(),list);
        }
}



void
aForm::on_tabselected( qulonglong uid )
{
	emit selected( uid );
	if ( closeAfterSelect ) {
		aLog::print(aLog::Debug, QObject::tr("aForm::on_tabselected(...),   closeAfterSelect is true"));
		close();
	} else {
		aLog::print(aLog::Debug, QObject::tr("aForm::on_tabselected(...),   closeAfterSelect is false"));
	}
}

void
aForm::on_tablerow( qulonglong uid )
{
//	Q3ValueList<QVariant> lst;
//	lst << sender()->name();
//	lst << QString("%1").arg(uid);
//	if ( engine->project.interpreter()->functions(this).findIndex("on_tabrowselected")!=-1) {
//		engine->project.interpreter()->call("on_tabrowselected", QVariantList(lst), this);
//	}
        if ( engine->code->globalObject().property("on_tabrowselected").isValid() ){
            QScriptValueList list;
            list << sender()->name();
            list << QString("%1").arg(uid);
            engine->code->globalObject().property("on_tabrowselected")
                    .call(QScriptValue(),list);
        }
}

int
aForm::New()
{
        if ( mainWidget ) {
//		printf("try find in windowslist %d, %llu\n ", objid, db_uid);
		if ( engine->wl->find( objid, db_uid ) )
		{
//			printf("found, remove\n");
			engine->wl->remove( objid, db_uid );
		}
		else
		{
//			printf("not found\n");
		}
                mainWidget->New();
		db_uid = mainWidget->uid();
		printf(">>>>>db_uid = %llu\n",db_uid);
		if(engine->wl->find( objid, db_uid ))
		{
			engine->wl->get(objid, db_uid )->setFocus();
		}
		else
		{
			engine->wl->insert( objid, form , db_uid );
		}
        }
        return 0;
}


int
aForm::Select( qulonglong id )
{
        if ( mainWidget ) {
//		printf("try find in windowslist %d, %llu\n ", objid, db_uid);
		if ( engine->wl->find( objid, db_uid ) )
		{
//			printf("found, remove\n");
			engine->wl->remove( objid, db_uid );
		}
		else
		{
//			printf("not found\n");
		}
//		printf("insert new in wl ok\n");
//		printf("form->select()\n");
		mainWidget->Select( id );
		mainWidget->Refresh();
		db_uid = id;
		if(engine->wl->find( objid, db_uid ))
		{
			engine->wl->get(objid, db_uid )->setFocus();
		}
		else
		{
			engine->wl->insert( objid, form, db_uid );
		}
        }
        return 0;
}


int
aForm::SelectGroup( qulonglong id )
{
	if ( mainWidget && !strcmp(mainWidget->className(),"wCatalogue"))
	 {
		if ( engine->wl->find( objid, db_uid ) )
		{
//			printf("found, remove\n");
			engine->wl->remove( objid, db_uid );
		}
		((wCatalogue*)mainWidget)->SelectGroup ( id );
		db_uid = id;
		if(engine->wl->find( objid, db_uid ))
		{
			engine->wl->get(objid, db_uid )->setFocus();
		}
		else
		{
			engine->wl->insert( objid, form, db_uid );
		}
        }
        return 0;
}

/*
int
aForm::select( Q_ULLONG id )
{
CHECK_POINT
//  Select( id );
	QObject *res=NULL;
	QObject* obj;
	QObjectList* list = form->queryList("wDBField");
	aCfgItem o;
	QString fieldName;
	QObjectListIt it(*list);
	while ( ( obj = it.current() ) !=0 )
	{
		++it;
		if (!obj) continue;
		((aWidget*)obj)->Select(id);
		//	if (name==widgetName(obj)) {
		//o = md->find(((wDBField*)obj)->getId());
		//if(o.isNull()) continue;
		fieldName = ((wDBField*)obj)->getMdFieldName();
		((wDBField*)obj)->setValue(mainWidget->table()->value(fieldName).toString());

  //debug_message("mainWidget class = %s\n", mainWidget->className() );
			//	break;
		//	}
	}
	//return res;

        return 0;
}
*/


/**
 * \ru
 * 	\brief ScriptAPI. Обновляет экранную форму.
 * \_ru
 */
int
aForm::Update()
{
        if ( mainWidget ) {
                mainWidget->Update();
        }
        return err_noerror;
}



/*!
 *\~english
 *	Seting form readonly flag.
 *	When form readonly we can't edit database fields, and make updatе action
 *\~russian
 *	\brief ScriptAPI. Устанавливает флаг "только чтение".
 *
 *	Когда установлен этот флаг, запрещается редактирование полей формы,
 *	и сохранение изменений в базу.
 *\~
 *\param status - \~english New readonly flag.\~russian Новое значение флага только чтение.\~
 */
void
aForm::SetReadOnly ( bool status )
{
	RO = status;
	if ( mainWidget )
	{
		if(status)
		{
			aLog::print(aLog::Info, tr("aForm set read only"));
		}
		else
		{
			aLog::print(aLog::Info, tr("aForm set read/write"));
		}
		mainWidget->SetReadOnly( status );
	}
/*	QObjectList *l = mainWidget->queryList( "wDBField" );
	QObjectListIt it( *l );
	QObject *obj;
	while ( (obj = it.current()) != 0 )
	{
CHECK_POINT
		++it;
		(( wDBField *)obj)->setDisabled( status );
	}
	delete l; // delete the list, not the objects*/
}



/*!
 *\~english
 *	Geting form readonly flag.
 *\~russian
 *	\brief ScriptAPI. Возвращает значение флага "только чтение".
 *\~
 *\return \~english true if form open in read only mode.
 *		\~russian true если форма открыта в режиме только чтение.\~
 */
bool
aForm::IsReadOnly()
{
	return RO;
}

/*!
 *\~english
 *	Convert number to russian language text format.
 *\~russian
 *	\brief Конвертирует число в его текстовое представление c указанием единиц измерения.
 *
 *	Параметры позволяют настроить вид результата. По умолчанию валюта - рубли, копейки выводятся.
 *
 *	примеры использования:
 *	\li Propis("20301.34") = Двадцать тысяч триста один рубль 34 копейки
 *	\li Propis("20301.34", false) = Двадцать тысяч триста один рубль
 *	\li Propis("20301.34", false, false, "штук", "штука", "штуки") = Двадцать тысяч триста одна штука
 *	\li Propis("2", false, true, "слонёнков", "слонёнок", "слонёнка") = Два слонёнка
 *	\li Propis("5", false, false, "мартышек", "мартышка", "мартышки") = Пять мартышек
 *	\li Propis("38.5", false, true, "попугаев", "попугай", "попугая") = Тридцать восемь попугаев
 *
 *	пример неправильного использования:
 *	\li Propis("38.5", true, true, "попугаев", "попугай", "попугая") = Тридцать восемь попугаев 50 копеек
 *
 *\~
 * \param val - \~english number to convert \~russian число для конвертирования\~
 * \param need_kopeyki - \~english wtite to output decimal part \~russian записывать в результат копейки (по умолчанию - да)\~
 * \param male - \~english  male \~russian конвертируемая единица - мужского рода (по умолчанию - да)\~
 * \param end1 - \~english end1 \~russian окончание для нуля конвертируемых единиц (по умолчанию - `рублей')\~
 * \param end2 - \~english end2 \~russian окончание для одной конвертируемой единицы (по умолчанию - `рубль')\~
 * \param end3 - \~english end3 \~russian окончание для двуx конвертируемых единиц (по умолчанию - `рубля')\~
 * \return - \~english text representetion of number \~russian текстовое представление числа \~
 * \see Propis()
 */
QString
aForm::Propis( QString val, bool need_kopeyki, bool male, const QString &end1, const QString &end2, const QString &end3)
{
	return aService::number2money(QVariant(val).toDouble(),need_kopeyki,male, end1, end2, end3);
}


/**
 * \ru
 * 		\brief	Конвертирует числовое представление денег в пропись с учетом указанной валюты.
 * 		\param amount - сумма денег для преобразование в пропись.
 * 		\param currency - строка с обозначением валюты. Допустимые значения: "RUR" - рубль, "USD" - доллар, "EUR" - евро.
 * 		\returns значение суммы прописью.
 * \_ru
 */
QString
aForm::MoneyToText( QString amount , QString currency ) {
	return aService::number2money( currency, QVariant(amount).toDouble());
}


/*!
 * \ru
 * 	\brief ScriptAPI. Используя идентификатор редактируемого виджетами формы документа, настраивает объект aDocument на доступ к
 * 	документу с таким же идентификатором.
 *
 * 	То есть к тому же документу. Используется в Ананас.Скрипте для передачи
 * 	ссылки на документ в другие объекты. Например в регистры. Для примера смотри код на Ананас.Cкрипте,
 * 	отрабатываемый при проведении Приходной накладной в бизнес схеме Inventory.
 * 	\param doc - документ, который необходимо настроить.
 * 	\return void.
 * \_ru
 */
void
aForm::SelectByCurrent(aObject *doc)
{
	if(mainWidget)
	{
		int res = doc->select(mainWidget->uid());
		aLog::print(aLog::Debug, QObject::tr("aForm SelectByCurrent %1 ended with %1").arg(mainWidget->uid()).arg(res));
	}
}

/**
 * \ru
 * 	\brief Устаревшая. Смотри SelectByCurrent()
 * \_ru
 * \en
 * 	\brief Deprecated. See SelectByCurrent()
 * \_en
 */
void
aForm::SetCurrent( aObject *doc) {
	SelectByCurrent( doc );
}

/*!
 *\~english
 *	Return current data object of form.
 *\~russian
 *	\brief ScriptAPI. Возвращает ссылку на бизнес объект, редактируемый данной экранной формой.
 *
 *	По свему назначению метод аналогичен SelectByCurrent()
 *\~
 * \return - \~english current data object \~russian текущий объект формы \~
 * \see SelectByCurrent()
 */
aObject*
aForm::Current()
{
	if(mainWidget)
	{
		return mainWidget->dataObject();
	}
	else return 0;
}

/*!
 *\~english
 *	Get value of attribute
 *\~russian
 *	\brief Возвращает значение атрибута формы.
 *\~
 *\param name - \~english Name of attribute.\~russian Имя атрибута.\~
aDataField*
aForm::getAttribute(const QString &name)
{
	QString oname, aname;
	aWidget *w = mainWidget;
	QVariant v;
	aDataField *f = 0;

	oname = name.section(".",0,0);
	aname = name.section(".",1);
	if ( oname.isEmpty() ) aname = oname;
	else {
	}
	v = w->value( aname );
//	f  = new aDataField( this, aname );
	return f;
}
 */


/*!
 *\~english
 *	Sets attribute value
 *\~russian
 *	Устанавливает значение атрибута формы.
 *\~
 *	*Not implemented*
 *\param name - \~english Name of attribute.\~russian Имя атрибута.\~
int
aForm::setAttribute(const QString &name, aDataField *value)
{
	return 1;
}
 */

/*!
 *\~english
 *	Convert number to currensy format.
 *\~russian
 *	\brief ScriptAPI. Конвертирует число  в денежный формат (#0.00 :) ). Используется для вывода на печать.
 *
 *	Для конвертирования числа в текстовое представление используйте Propis()
 *\~
 * \param number - \~english number to convert \~russian число для конвертирования\~
 * \return - \~english rounded number \~russian округленное до 2-х цифр после запятой число \~
 * \see Propis()
 */
QString
aForm::ConvertNumber2MoneyFormat(double number)
{
	return aService::convertNumber2MoneyFormat(number);
}

/*!
 *\~english
 *	Convert date to local format.
 *\~russian
 *	\brief ScriptAPI. Конвертирует дату в локальный формат. Используется для вывода на печать.
 *
 *	Все функции работы с датой оперируют датой в ISO формате. (yyyy-MM-ddThh:mm:ss)
 *	Данная функция предназначена для конвертации такой даты в читаемый вид.
 *\~
 * \param ISODate - \~english date in ISO format \~russian дата в ISO формате (yyyy-MM-ddThh:mm:ss)\~
 * \return - \~english date in local format \~russian дату в локальном формате \~
 */
QString
aForm::ConvertDateFromIso(const QString &ISODate)
{
	return aService::Date2Print(ISODate);
}


/*!
 *\~english
 *	Date of end day.
 *\~russian
 *	\brief ScriptAPI. Возвращает дату-время на конец дня.
 *	\param ISODate - строка, содержащая дату в формате ISO (YYYY-MM-DD).
 *\~
 * \return - \~english date \~russian дату вида yyyy-mm-ddT23:59:59 \~
 */
QString
aForm::EndOfDay(const QString& ISODate)
{
	return QString("%1T%2").arg(ISODate.section('T',0,0)).arg("23:59:59");
}

/*!
 *\~english
 *	Sets form mode.
 *\~russian
 *	\brief ScriptAPI. Устанавливает режим формы.
 *	\param m - Допустим один из трех режимов открытия формы: 0 - новая, 1 - редактирование, 2- просмотр.
 *\~
 * \see GetMode()
 */
void
aForm::setMode(int m)
{
	this->mode = m;
}

/*!
 *\~english
 *	Get form mode. 0 - new, 1- edit, 2- browse
 *\~russian
 * 	\brief ScriptAPI. Получение режима открытия формы.
 * 	\return 0 - новая, 1 - редактирование, 2- просмотр.
 *\~
 * \return - \~english mode \~russian режим открытия формы \~
 */
int
aForm::GetMode()
{
	return this->mode;
}
