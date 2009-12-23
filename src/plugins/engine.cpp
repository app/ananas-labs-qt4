/****************************************************************************
** $Id: engine.cpp,v 1.6 2008/12/24 17:47:41 leader Exp $
**
** Code file of the Ananas Engine of Ananas
** Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) 2003-2004 Grigory Panov, Yoshkar-Ola.
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
//#include <qsinterpreter.h>
//#include <qsinputdialogfactory.h>

#include <qobject.h>
#include <q3valuelist.h>
#include <qstringlist.h>
#include <qstring.h>
#include <stdlib.h>
#include <qfile.h>
//#include <qsproject.h>
//#include <qsscript.h>

#include <QScriptValue>
#include <QScriptEngine>
#include <QScriptContext>

#include <qdialog.h>
//--#include <qwidgetfactory.h>
#include <qobject.h>
//#include <qsutilfactory.h>
//#include <qsinputdialogfactory.h>


#include <qapplication.h>
//Added by qt3to4:
#include <QTimerEvent>
#include <Q3PopupMenu>

#include "ananas.h"
#include "wcatalogeditor.h"
#include "adatafield.h"
#include "alog.h"
#include "acombobox.h"

#include "ametaobject.h"

static QScriptValue
createObject(QScriptContext *context, QScriptEngine *engine) {
    QObject *parent = context->argument(0).toQObject();
    QObject *object = new QObject(parent);
    return engine->newQObject(object, QScriptEngine::ScriptOwnership);
}


/*!
 *	\~english
 *	Constructor
 *	\~russian
 *	Конструктор. Регистрирует следующие типу объектов для использования их в Ананас.Скрипте
 *	\arg \c PopupMenu всплывающее меню
 *	\arg \c Document документ
 *	\arg \c Catalogue каталог
 *	\arg \c CatalogueEditor редактор каталога
 *	\arg \c Report отчет
 *	\arg \c ARegister накопительный регистр
 *	\arg \c DataField поле (не работает)
 *	\arg \c Journal журнал
 *	\arg \c ATime объект для работы со временем
 *	\~
 */
aObjectsFactory::aObjectsFactory( aEngine *e )
{
	engine = e;
        if ( ! engine ) return;
        db = e->db;
        if ( ! db ) return;


        QScriptValue ctor = engine->code->newFunction( &createObject);

        engine->code->globalObject().setProperty("QObject",
                                          engine->code->newQMetaObject(&QObject::staticMetaObject, ctor));
        engine->code->globalObject().setProperty("MetaObject",
                                          engine->code->newQMetaObject(&AMetaObject::staticMetaObject, ctor));
        engine->code->globalObject().setProperty("MetaGroup",
                                          engine->code->newQMetaObject(&AMetaGroup::staticMetaObject, ctor));
        engine->code->globalObject().setProperty("Documents",
                                          engine->code->newQMetaObject(&AMetaDocuments::staticMetaObject, ctor));
        engine->code->globalObject().setProperty("Catalogues",
                                          engine->code->newQMetaObject(&AMetaCatalogues::staticMetaObject, ctor));
        engine->code->globalObject().setProperty("MetaInfo",
                                          engine->code->newQMetaObject(&AMetaInfo::staticMetaObject, ctor));
        engine->code->globalObject().setProperty("MetaGlobal",
                                          engine->code->newQMetaObject(&AMetaGlobal::staticMetaObject, ctor));
        engine->code->globalObject().setProperty("PopupMenu",
                                          engine->code->newQMetaObject(&Q3PopupMenu::staticMetaObject, ctor));
        engine->code->globalObject().setProperty("Document",
                                          engine->code->newQMetaObject(&aDocument::staticMetaObject, ctor));
        engine->code->globalObject().setProperty("Catalogue",
                                          engine->code->newQMetaObject(&aCatalogue::staticMetaObject, ctor));
        engine->code->globalObject().setProperty("CatalogEditor",
                                          engine->code->newQMetaObject(&wCatalogEditor::staticMetaObject, ctor));
        engine->code->globalObject().setProperty("Report",
                                          engine->code->newQMetaObject(&aReport::staticMetaObject, ctor));
        engine->code->globalObject().setProperty("ARegister",
                                          engine->code->newQMetaObject(&aARegister::staticMetaObject, ctor));
        engine->code->globalObject().setProperty("IRegister",
                                          engine->code->newQMetaObject(&aIRegister::staticMetaObject, ctor));
        engine->code->globalObject().setProperty("Time",
                                          engine->code->newQMetaObject(&aTime::staticMetaObject, ctor));
        engine->code->globalObject().setProperty("DataField",
                                          engine->code->newQMetaObject(&aDataField::staticMetaObject, ctor));
        engine->code->globalObject().setProperty("DocJournal",
                                          engine->code->newQMetaObject(&aDocJournal::staticMetaObject, ctor));
        engine->code->globalObject().setProperty("ComboBox",
                                          engine->code->newQMetaObject(&AComboBox::staticMetaObject, ctor));


//	registerClass("MetaObject",&AMetaObject::staticMetaObject);
//	registerClass("MetaGroup",&AMetaGroup::staticMetaObject);
//	registerClass("Documents",&AMetaDocuments::staticMetaObject);
//	registerClass("Catalogues",&AMetaCatalogues::staticMetaObject);
//	registerClass("MetaInfo",&AMetaInfo::staticMetaObject);
//	registerClass("MetaGlobal",&AMetaGlobal::staticMetaObject);
//
//
//	registerClass("PopupMenu",&Q3PopupMenu::staticMetaObject);
//	registerClass("Document",&aDocument::staticMetaObject);
//	registerClass("Catalogue",&aCatalogue::staticMetaObject);
//	registerClass("CatalogEditor",&wCatalogEditor::staticMetaObject);
//	registerClass("Report",&aReport::staticMetaObject);
//	registerClass("ARegister",&aARegister::staticMetaObject);
//	registerClass("IRegister",&aIRegister::staticMetaObject);
//	registerClass("ATime",&aTime::staticMetaObject);
//	registerClass("DataField",&aDataField::staticMetaObject);
//	registerClass("Journal",&aDocJournal::staticMetaObject);
//	registerClass("ComboBox",&AComboBox::staticMetaObject);

//Register extensions classes
	QStringList extlist = AExtensionFactory::keys();
	for ( int i=0; i<extlist.count(); i++){
//	    registerClass(extlist[i], AExtensionFactory::metaObject( extlist[i] ));
            engine->code->globalObject().setProperty(extlist[i],
                                              engine->code->newQMetaObject(AExtensionFactory::metaObject( extlist[i] ), ctor));
        }
}



/*!
 *	\~english
 *	Creates script object. Mapping Script name to real name.
 *	\~russian
 *	Создает объект. Отображает имена из скрипта в имена объектов библиотеки.
 *	\param className - \~english scripn object name \~russian имя объекта в скрипте \~
 *	\param arguments - \~english sarguments to constructor \~russian аргументы для конструктора \~
 *	\param context - \~english running context \~russian контекст, в котором выполняется скрипт \~
 *	\~
 */
QObject *
aObjectsFactory::create( const QString &className,
                     const QVariantList &arguments, QObject *context )
{
	QObject * res = 0;
//	context=context;
	if (className=="PopupMenu") {
//			return new QApopupmenu();
			res = new Q3PopupMenu();
	}else if (className=="Document") {
		if (arguments.size()>0) {
			res = new aDocument(arguments[0].toString(), db );
		}
	}else if (className=="Catalogue") {
		if (arguments.size()>0) {
			res = new aCatalogue( arguments[0].toString(), db );
		}
	}else if ( className=="Report" ) {
		if ( arguments.size()>0 )
		{
			if(arguments.size()==2) {
				res = new aReport( arguments[0].toString(),(aReport::RT_type)arguments[1].toInt(), engine );
			}else{
				res = new aReport( arguments[0].toString(),aReport::RT_text, engine );
			}
 		}
	}else if (className =="CatalogEditor") {
		if (arguments.size()>0) {
			aCfgItem it;
			it = db->cfg.find(QString("Catalogue.%1").arg(arguments[0].toString()));
			if(!it.isNull()) {
				wCatalogEditor * w = new wCatalogEditor(engine->ws,db->cfg.id(it));
				w->initCat(db);
				res = w;
			}
		}
	}else if (className == "ARegister") {
		if (arguments.size() > 0) {
			res = new aARegister(arguments[0].toString(), db);
		}

	}else if (className == "IRegister") {
		if (arguments.size() > 0) {
			res = new aIRegister(arguments[0].toString(), db);
		}

	} else if (className == "Journal") {
		if (arguments.size() > 0) {
			res = new aDocJournal(arguments[0].toString(), db);
		}

	} else if (className == "ATime") {
		  res = new aTime();
	} else if (className == "DataField") {
		  res = new aDataField();
	} else if ( AExtensionFactory::keys().contains( className )) {
//		printf("create extension object %s\n",(const char*) className);
		AExtension *obj = AExtensionFactory::create( className );
		// Insert check existence signal in future!!!!
		QObject::connect( obj, SIGNAL(event(const QString&)),
			 engine, SLOT( on_event(const QString&)));
		obj->init(db);
		res = obj;
	} else if ( className == "ComboBox" ) {
		if (arguments.size() == 1) {
			res = new AComboBox( 0, arguments[0].toString());
		} else {
			res = new AComboBox();
		}
	}
	if(!res) aLog::print(aLog::Error,QObject::tr("Unknown classname '%1' or metaobject '%2'").arg(className).arg(arguments[0].toString()));
	return res;
}



/*!
 *\en
 *	Constructor.
 *\_en
 *\ru
 *	Конструктор. Создает новый объект с именем "sys". Доступ к функциям этого объекта из Ананас.Скрипта возможен примерно так:
 *	\code
 *	sys.Date(); // текущая дата
 *	sys.OpenForm("DocJournal.Системный журнал.Form.Список документов"); // открывает форму "Список Документов" журнала "Системный журнал"
 *	\endcode
 *\_ru
 */
aEngine::aEngine():QObject(0,"sys")
{
	md = 0;
	next_obj_id = 1;
	db = aDatabase::database();
}



/*!
 *\en
 *	Destructor
 *\_en \ru
 *	Деструктор.
 *\_ru
 */
aEngine::~aEngine()
{
	values.clear();
//	delete project;
}



/*!
 * \~english
 * Init engine.
 * Init script subsystem, load global module.
 * \~russian
 *	Инициализирует объект. Инициализирует скриптовую систему, добавляет несколько глобальных функций.
 * \~
 */
bool
aEngine::init( const QString &rcfile )
{
//	QString mGlobal;
	aCfgItem gobj, obj, obj0;
	QString sysf =
//	"function Message( t, msg ){ sys.Message( t, msg );}"
//	"function StatusMessage( msg ){ sys.StatusMessage( msg );}"
//	"function Date(){ return sys.Date();}"
//	"function Time(){ return sys.Time();}"
//	"function Exit(){ return sys.Exit();}"
	"";

	bool ok = false;
        if ( ! db->init( rcfile ) ) {
            return false;
        }

        md = &db->cfg;
        code = new QScriptEngine();
        debugger = new QScriptEngineDebugger();
        debugger->attachTo(code); //окно дебагера появится при ошибке выполнении скрипта

        //code->addObjectFactory( new QSInputDialogFactory );
        //code->addObjectFactory( new aObjectsFactory( this ) );
        //code->addObjectFactory( new QSUtilFactory );

        //project.addObject( this );

        // Объект sys часто используется для вывода сообщений в окно сообщений
        QScriptValue ananasEngineObject = code->newQObject(this);
        code->globalObject().setProperty("sys", ananasEngineObject);


        //project.addObject( md );
        // Похоже не используется
        QScriptValue ananasMetadataObject = code->newQObject(md);
        code->globalObject().setProperty("Metadata", ananasMetadataObject);

//                project.addObject( AMetaData::metadata() );
        QScriptValue ananasMetadataData = code->newQObject(md);
        code->globalObject().setProperty("MetaData", ananasMetadataData);
        mGlobal = md->sText( md->find( md->find( mdc_metadata ), md_globals, 0 ), md_sourcecode );
        if ( ! mGlobal.isEmpty() ) {
                code->evaluate(sysf+sourcePreprocessor(mGlobal));
        } else {
//                        printf("Global module is empty\n");
        }
	return true;
}



/*!
 *\en
 *\_en \ru
 *	Заглушка.
 *\_ru
 */
void
aEngine::done()
{

}



/*!
 *\en
 *	Called on system start.
 *\_en \ru
 *	Автоматически вызывается при старте системы. Если в глобальном модуле определа функция on_systemstart()
 *	то она будет автоматически вызвана.
 *	Это можно использовать для открытия документов или журналов при старте системы, или каких-либо других действий.
 *	\see on_systemstop()
 *\_ru
 */
int
aEngine::on_systemstart(){

//	if (project.interpreter()->functions().findIndex("on_systemstart")!=-1) {
//		project.interpreter()->call("on_systemstart",QVariantList());
//	}
        if ( code->globalObject().property("on_systemstart").isValid() ){
            code->globalObject().property("on_systemstart").call();
        }

	return 0;
}


void
aEngine::on_event( const QString &data )
{
//	Q3ValueList<QVariant> lst;
//	lst <<  sender()->name();
//	lst << data;
//	if (project.interpreter()->functions().findIndex("on_event")!=-1) {
//		project.interpreter()->call("on_event", QVariantList(lst));
//	}
        if ( code->globalObject().property("on_event").isValid() ){
            QScriptValueList list;
            list.append(QScriptValue(sender()->name()));
//            list.append( engine->code->newVariant(val));
            list.append(QScriptValue(data));
            code->globalObject().property("on_event")
                    .call(QScriptValue(),list);
        }

	emit event( sender()->name(), data );
}



/*!
 *\en
 *	Called on system stop.
 *\_en \ru
 *	Автоматически вызывается при остановке системы.
 *	Если в глобальном модуле определа функция on_systemstop()
 *	то она будет автоматически вызвана.
 *	\see on_systemstart()
 *\_ru
 */
int
aEngine::on_systemstop(){
//	if (project.interpreter()->functions().findIndex("on_systemstop")!=-1) {
//		project.interpreter()->call("on_systemstop",QVariantList());
//	}
        if ( code->globalObject().property("on_systemstop").isValid() ){
            code->globalObject().property("on_systemstop").call();
        }

	return 0;
}



/*!
 *\en
 *	Stub.
 *\_en \ru
 *	Заглушка.
 *\_ru
 */
void
aEngine::timerEvent(QTimerEvent *e)
{
	e=e; //
//	project->interpreter()->evaluate(pr_timer, this);
}



/*!
 *\en
 *	Stub.
 *\_en \ru
 *	Заглушка. Возвращает всегда строку "???"
 *	В будущем возможно будет возвращать имя конфигурации.
 *\_ru
 */
QString
aEngine::cfgname() {
	return "???"; //cfg_info("name");
}



/*!
 *\en
 *\_en \ru
 *	Возвращает строковое представление текущего времени в формате "hh:mm:ss"
 *\_ru
 */
QString
aEngine::Time() {
	return QTime::currentTime().toString("hh:mm:ss");
}



/*!
 *\en
 *\_en \ru
 *	Возвращает строковое представление текущей даты в формате "dd.MM.yy"
 *\_ru
 */
QString
aEngine::Date() {
	return QDate::currentDate().toString("dd.MM.yy");
}


void
aEngine::Exit() {
	qApp->quit();
}

/*!
 *	\~english
 *	Outputs messages.
 *	\~russian
 *	Вывод сообщений.
 *	\param n - тип сообщения. 0 - уведомление, 1 - предупреждение, 2 - ошибка.
 *	\param msg - текст сообщения.
 *	\~
 *	\see cfg_message(int msgtype, const char *msgfmt,...)
 */
void
aEngine::Message(int n, const QString &msg)
{
	cfg_message(n, (const char *) msg.utf8());
}



/*!
 *\en
 *	Outputs status messages.
 *\_en \ru
 *	Вывод сообщений в строке состояния.
 *	\param msg - текст сообщения.
 *\_ru
 */
void
aEngine::StatusMessage( const QString &msg )
{
        emit statusMessage( msg );
}



/*!
 *\en
 *\_en \ru
 *\_ru
 */
void
aEngine::settimer(int sec, QString proc){
    // TODO Need id timer - get from startTimer
	//--killTimers();
	if (sec) {
		pr_timer=proc;
		startTimer(sec*1000);
	} else {
		pr_timer="";
	}
}



/*!
 *\en
 *\_en \ru
 *	Запускает действие
 *	\param id - идентификатор действия в метаданных.
 *\_ru
 */
void
aEngine::on_MenuBar( int id )
{
	aCfgItem obj, act;
	int actions, i;

//	printf("menu %i selected\n", id);
	if ( id>0 ) {
		obj = md->find( id );
		if ( !obj.isNull() ) {
			actions = md->count( obj, md_comaction );
//			printf("obj found %i actions\n", actions);
			for ( i=0; i<actions; i++) {
				act = md->findChild( obj, md_comaction, i );
				act = md->find( md->text( act ).toLong() );
				execAction( act );
			}
		}
	}
}



/*!
 *\en
 *\_en \ru
 *	Вспомогательный метод.
 *	Запускает действие в указанном контексте. Правильная работа не гарантируется (где-то там ошибки есть).
 *	\param act - ссылка на объект метаданных, описывающий действие
 *	\param context - контекст выполнения действия.
 *\_ru
 */
void aEngine::execAction( aCfgItem &act, QObject *context )
{
	QString aModule, arg;
	int atype, satype;
	long oid, foid;
	//aForm *form;
	aCfgItem gobj, obj;

	if ( ! act.isNull() ) {
		atype = md->attr( act, mda_type ).toInt();
//		printf("atype=%i\n",atype);
		switch ( atype ){
		case 0:
			satype = md->sText( act, md_actiontype ).toInt();
			oid = md->sText( act, md_objectid ).toLong();
			foid = md->sText( act, md_formid ).toLong();
			arg = md->sText( act, md_argument );
//			printf("satype=%d, oid=%d,foid=%d,arg=%s",satype,oid,foid,arg.ascii());
			gobj =  md->find( oid );
			if ( foid == 0 )
			{
				foid = md->getDefaultFormId( gobj, satype );
				break;
			}
			if ( foid == 1 )
			{
				openEmbedCatalogueEditor(oid, NULL, false); //open to edit
				break;
			}

			openForm(oid, foid, satype, 0, 0, 0 );
			break;
/*			if ( !arg.isEmpty() ) {
				obj = md->findName( md->find( gobj, md_forms ), md_form, arg );
				foid = md->id( obj );
//				if ( !obj.isNull() ) printf("form found\n");
//				else printf("! not found %s\n", (const char *) arg );
			}*/
			/*
			if ( wl->find( foid ) )
			{
				printf("form already exist,set focus \n");
				wl->get( foid )->setFocus();
			}
			else
			{
				printf("execute action\n");
			if ( oid )
				switch ( satype )
				{
				case md_action_new://create new
					printf("create new\n");
					form = new aForm( ws, this, foid );
//					form->init();
					form->New();
					form->show();
					break;
				case md_action_view://view
					printf("view\n");
					form = new aForm( ws, this, foid );
										//form->init();
					form->show();
					break;
				}
			}
			break;
			*/
		case 1:
			aModule = md->sText( act, md_sourcecode );
			if ( !aModule.isEmpty() ) {
//				code->evaluate( sourcePreprocessor(aModule),
//                                                context, md->attr( act, mda_name ) );
                                code->evaluate( sourcePreprocessor(aModule));
                        }
			break;
		case 2:
			break;
		}
//		printf("executed %s\n", ( const char *) md->attr( act, mda_name ).local8Bit() );
	}
}



/*!
 *\~english
 *	Create and open new form.
 *\~russian
 *	Создает и открывает новую форму.
 *	Форма создается с учетом её id в конфигураци и действия,
 *	режима работы (просмотр, редактирование, создание нового объекта).
 *	Форма должна поддерживать тот режим, для которого она открывается.
 *	Если форма открыта для просмотра она открывается в режиме отлько чтение.
 *	Если id формы равно 0, делается попытка найти первую форму с
 *	требуемым режимом.
 *\~
 *\param oid - \~english form owner md id,\~russian id владельца формы в конфигурации,\~
 *\param fid - \~english form md id, \~russian id формы в конфигурации\~
 *\param mode - \~english form use mode, \~russian режим работы формы\~
 *\param ido - \~english database id of opening object\~russian идентификатор объекта в базе данных, для которого открывается форма.\~
 *\return \~english link on new form or 0 if form not created.
 			\~russian ссылку на новую форму или 0, если форма не создана.\~
 */
aForm*
aEngine::OpenForm(QString fname, int mode, aObject* selecter)//Q_ULLONG ido)
{
	aCfgItem object, form;

	form = md->find(fname);
	if(!form.isNull())
	{
		object = md->parent(md->parent(form));
		if(object.isNull()) return 0;
		qulonglong ido =0;
		if(selecter) ido = selecter->sysValue("id").toULongLong();
		return openForm(atoi(md->attr(object,mda_id)), atoi(md->attr(form,mda_id)), mode, mode, ido);
	}
	else
	{
		return 0;
	}
}



/*!
 *\~english
 *	Create and open new form.
 *\~russian
 *	Создает и открывает новую форму.
 *	Форма создается с учетом её id в конфигураци и действия,
 *	режима работы (просмотр, редактирование, создание нового объекта).
 *	Форма должна поддерживать тот режим, для которого она открывается.
 *	Если форма открыта для просмотра она открывается в режиме отлько чтение.
 *	Если id формы равно 0, делается попытка найти первую форму с
 *	требуемым режимом.
 *	При необходимости поля формы заполняются в соответствии с параметром  \a id
 *\~
 *\param oid - \~english form owner md id,\~russian id владельца формы в конфигурации,\~
 *\param fid - \~english form md id, \~russian id формы в конфигурации,\~
 *\param defaultfor - \~english form use mode, \~russian режим работы формы,\~
 *\param mode - \~english not use yet\~russian пока не используется,\~
 *\param id - \~english database id of opening object\~russian идентификатор объекта в базе данных, для которого открывается форма. Используется только в режиме просмотра или редактирования.\~
 *\param caller - \~english caller widget\~russian Вызывающий данную функцию объект. Он будет обновлен автоматически при закрытии формы. \~
 *\return \~english link on new form or 0 if form not created.
 			\~russian ссылку на новую форму или 0, если форма не создана.\~
 */
aForm *
aEngine::openForm(int oid, int fid, int defaultfor, int mode, ANANAS_UID id, aWidget* caller )
{
	aForm *form = 0;
	if ( fid == 0 )
	{
		fid = md->getDefaultFormId( md->find( oid ), defaultfor, mode );
	}
	//printf("engine:try found %d %llu in wl\n", fid, id);
	if ( wl->find( fid, id ) )
	{
		//printf("foubd!, set focus\n");
		wl->get( fid, id )->setFocus();
	}
	else {
		//printf("not found!\n");
		aLog::print(aLog::Info,tr("aEngine open form %1 in mode %2, select %3").arg(fid).arg(mode).arg(id));
		if ( oid ) {
			switch ( defaultfor ) {
			case md_action_new://create new
				form = new aForm( ws, this, fid, caller );
				if(form)
				{
					form->setMode(0);
					form->New();
				}
				else
				{
					aLog::print(aLog::Error,tr("aEngine form create error"));
				}
				//printf("open new form\n");
				break;
			case md_action_view://view
				form = new aForm( ws, this, fid, caller );
				if(form)
				{
					form->setMode(1);
					form->SetReadOnly(true);
					form->Select( id );
				}
				else
				{
					aLog::print(aLog::Error,tr("aEngine form create error"));
				}
				//printf(" open form for view %llu\n",id);
				break;
			case md_action_edit:
				form = new aForm( ws, this, fid, caller );
				if(form)
				{
					form->setMode(2);
					form->Select( id );
				}
				else
				{
					aLog::print(aLog::Error,tr("aEngine form create error"));
				}
				//printf("open form for edit %llu\n ",id);
				break;
			default:
				aLog::print(aLog::Error, tr("aEngine open form mode %1 not supported").arg(defaultfor));
			}
			connect( this, SIGNAL( event(const QString &, const QString&)),
				 form, SLOT( on_event(const QString &, const QString&)));
		}
	}
	if(form && caller)
	{
		connect(form,SIGNAL(changedData()),caller,SLOT(Refresh()));
	}
//	printf("before clear\n");
//	this->project.interpreter()->clear();
	if ( form ) form->show();
//	printf("end open form\n");
	return form;
}



/*!
 *\~english
 *	Create and open new embedded catalog editor form.
 *\~russian
 *	Создает и открывает новую форму встроенного редактора каталога.
 *\~
 *\param oid - \~english catalogue md id,\~russian id каталога конфигурации,\~
 *\param parent - \~english parent wField object, \~russian родительский объект wField\~
 *\param toSelect - \~english Mode. true - for select, false - for edit\~russian Режим. true - выбор, false - редактирование\~
 */
void
aEngine::openEmbedCatalogueEditor(int oid, QWidget* parent,const bool toSelect)
{
	wCatalogEditor * w = new wCatalogEditor(ws,oid);
	if( parent )
	{
		connect(w,		SIGNAL(selected( qulonglong )),
		        (wField*)parent,SLOT(on_selected( qulonglong )));
		connect(w,		SIGNAL(destroyed_form()),
			(wField*)parent,SLOT(setFocus()));
	}

	w->initCat(db);
	if(toSelect)
	{
		w->select();
	}
	else
	{
		w->edit();
	}
}



/*!
 *\en
 *\_en \ru
 *	Печатает сообщение об ощибке.
 *\_ru
 */
void
aEngine::error ( const QString & message, QObject * context, const QString & scriptName, int lineNumber )
{
    if (code->uncaughtException().isValid()) {
        Message( 2, tr("Line:%1 Message:%2 Stack:(%3)")
                 .arg(lineNumber)
                 .arg(message)
                 .arg(code->uncaughtException().toString()) );
    } else {
        Message( 2, tr("Line:%1 Message:%2")
                 .arg(lineNumber)
                 .arg(message)
                 );
    }
}



/*!
 *\en
 *\_en \ru
 * Функция препроцессора исходного текста скрипта.
 * Для разработчиков:
 * В первую очередь может быть использована для замены коротких
 * имен из локального контекста на квалифицированные имена
 * глобальных функций и объектов, например Message() в контексте
 * формы может заменяться на global.Message() перед выполнением скрипта
 * и выполняться корректно из любого места.
 * В противном случае приходится дублировать глобальные
 * встроенные функции во всех контекстах.
 *\_ru
 */
QString
aEngine::sourcePreprocessor( const QString &src )
{
	QString psrc = src;
	// Global functions preprocessing
//	psrc.replace( "Message(", "global.Message(");
//	psrc.replace( "Statusglobal.Message(", "global.StatusMessage(");
//	psrc.replace( "Date(", "global.Date(");
//	psrc.replace( "Time(", "global.Time(");
	// Russian key words
//	psrc.replace( QString::fromUtf8("функция"), "function");
//	psrc.replace( QString::fromUtf8("если"), "if");
//	psrc.replace( QString::fromUtf8("то"), "then");
//	psrc.replace( QString::fromUtf8("иначе"), "else");
//	psrc.replace( QString::fromUtf8("начало"), "{");
//	psrc.replace( QString::fromUtf8("конец"), "}");
	return psrc;
}



/*!
 *\en
 *\_en \ru
 * Получение значения глобальной переменной.
 *\_ru
 */
QVariant
aEngine::value( const QString &name )
{
	QVariant v, *pv;
	pv = values.find( name );
	if ( pv ) return *pv;
	return v;
}



/*!
 *\en
 *\_en \ru
 * Установка значения глобальной переменной.
 *\_ru
 */
void
aEngine::setValue( const QString &name, QVariant value )
{
    values.remove( name );
    if ( !value.isValid() ) values.insert( name, new QVariant( value ) );
}



/*!
 *\en
 *\_en \ru
 * Установка значения глобальной переменной.
 *\_ru
 */
aDataField*
aEngine::enterValue( const QString &FieldType, const QString &title )
{
	aDataField *f = new aDataField( "", FieldType );
	QDialog *d = new QDialog( ws, title, true );
	wField *wf = new wField( d, "" );
	QPushButton *b_ok = new QPushButton( d, tr("OK") );
	QPushButton *b_cancel =new QPushButton( d, tr("Cancel") );
	connect( b_ok, SIGNAL( pressed() ), d, SLOT( accept() ) );
	connect( b_cancel, SIGNAL( pressed() ), d, SLOT( reject() ) );
	if ( d->exec() == QDialog::Accepted ) {
//		printf("accepted\n");
	}
	delete d;
	return f;
}

