/****************************************************************************
** $Id: acfg.cpp,v 1.2 2008/11/08 20:16:35 leader Exp $
**
** Code file of the Ananas configuration objects of Ananas
** Designer and Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) Grigory Panov <gr1313 at mail.ru>, Yoshkar-Ola.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#include <qdatetime.h>
#include <qobject.h>
#include <qdom.h>
#include <qfile.h>
#include <qstringlist.h>
//--#include <q3textstream.h>
#include <QTextStream>

#include "acfg.h"
#include "alog.h"

#ifdef _MSC_VER
#define vsnprintf _vsnprintf
#endif

#define context_startid	100

/*!
 * 	Global configuration variable.
 */
//aCfg * ANANAS_EXPORT globalCfg = 0;


/*
 */
void (*cfg_messageproc)(int , const char *)=NULL;

/*!
 *\en
 *	Outputs error messages.
 *\_en \ru
 *	Вывод сообщений об ошибках.
 *\_ru
 */
void
cfg_message(int msgtype, const char *msgfmt,...){
	char msg[2048];
	char *ts=(char *)"";
	va_list args;

	if (msgfmt) {
		va_start(args, msgfmt);
		vsnprintf(msg, sizeof(msg)-1, msgfmt, args);
		va_end(args);
		if (cfg_messageproc) cfg_messageproc(msgtype, msg);
		else {
			if (msgtype==1) ts=(char *)"- ";
			if (msgtype==2) ts=(char *)"! ";
			if (msgtype==2) ts=(char *)"!!! ";
			printf("%s%s", ts, (const char *) QString::fromUtf8(msg).local8Bit());
		}
	}
}

/*!
 *\en
 *\_en \ru
 *	Выводит отладочные сообщения, если определен макрос ANANAS_DEBUG
 *\_ru
 */
void
debug_message(const char *msgfmt,...){
#ifdef ANANAS_DEBUG
	char msg[2048];
	va_list args;

	if (msgfmt) {
		va_start(args, msgfmt);
		vsnprintf(msg, sizeof(msg)-1, msgfmt, args);
		va_end(args);
	 	printf("debug");
		printf("> %s", (const char *) QString::fromUtf8(msg).local8Bit());
	}
#endif
}


/*!
 *\~english
 *	Ananas configuation object contaner constructor.
 *\~russian
 *	Конструктор контейнера объекта конфигурации Ананас.
 *\~
 *	\param newid (in) - \~english container's ID.\~russian
 *		ID контейнера. \~
 *	\param newitem (in) - \~english container's item.\~russian
 *		Элемент контейнера. \~
 */
aCfgItemContaner::aCfgItemContaner(long newid, aCfgItem newitem)
{
	id = newid;
	item = newitem;
}



/*!\~english
 *	Ananas configuation object constructor.
 *\~russian
 *	Конструктор объекта конфигурации АНАНАС.
 *\~
 */
aCfg::aCfg() : QObject(0, "Metadata"), xml( md_root )
{
	setCompressed( false );
	setModified( false );
	idcache.setAutoDelete( TRUE );
	createNew();
}



/*!\en
 *	Ananas configuation object destructor.
 *\_en \ru
 *	Деструктор объекта конфигурации АНАНАС.
 *\_ru
*/
aCfg::~aCfg()
{
	xml.setContent( QString("") );
}



/*!
 *	Reads configuration information (metadata) from file.
 *	\param fname (in) - name of configuration file.
 *	\return error code.
 */
int
aCfg::read(const QString &fname){
    QFile file( fname );
    QByteArray buf;
    QString err;
    int errLine = 0, errColumn = 0;
    if ( !file.open( QIODevice::ReadOnly ) ) return RC_ERROR;
    buf = file.readAll();
    file.close();
    xml.setContent( QString("") );
    if ( !xml.setContent( buf, false, &err, &errLine, &errColumn ) ) {
	emit message( 2, QObject::tr(
		     "Error read configuration line:%1 col:%2 %3"
		     ).arg( errLine ).arg( errColumn ).arg( err ));
//	cfg_message( 2, QObject::tr(
//		     ( const char *) QObject::tr(
//		     "Error read configuration line:%d col:%s %s"),
//		     errLine, errColumn, ( const char *) err );
	return RC_ERROR;
    }
    init();
//CHECK_POINT
	setModified( false );
//CHECK_POINT
    return RC_OK;
}



/*!\~english
 *	helper method. Save current configuration.
 *\~russian
 *	Вспомогательный метод. Сохраняет текущую конфирацию.
 *\~
 */
int
aCfg::write(const QString &fname)
{
	return write(xml, fname);
}

/*!\~english
 *	Writes configuration information (metadata) to file.
 *\~russian
 *	Записывает информацию о бизнес схеме ( метаданные ) в файл.
 *\~
 *	\param doc (in) - \~english xml document to save. \~russian xml для сохранения \~
 *	\param fname (in) - \~english name of configuration file. \~russian имя файла для сохранения \~
 *	\return \~english error code. \~russian код ошибки \~
 */
int
aCfg::write(QDomDocument doc, const QString &fname)
{
    QFile file( fname );
    QByteArray buf( xml.toString(4).utf8() );
    if ( file.open( QIODevice::WriteOnly ) ) {
	QTextStream ts( &file );
	//--ts.setEncoding(Q3TextStream::UnicodeUTF8);
	doc.save(ts, 4);
	//ts <<  toString(4);
	file.close();
	return RC_OK;
    }
    return RC_ERROR;
}

/*!
 *	Returns configuration file compression mode.
 *	\return boolean
 */
bool
aCfg::compressed()
{
	return fCompressed;
}


/*!
 *	Sets compression mode of configuration file.
 *	\param mode (in) - compression mode.
 *	\return void
 */
void
aCfg::setCompressed( bool mode )
{
	fCompressed = mode;
}


/*!
 *	Returns modification status.
 *	\return boolean
 */
bool
aCfg::modified()
{
	return fModified;
}


/*!
 *	Set modification status.
 *	\param mode (in) - new value of modification status
 *	\return void
 */
void
aCfg::setModified( bool mode )
{
	fModified = mode;
}


/*!
 *	Data structures initialization. Should be run after load metadata from configuration file.
 *	\return void
 */
void
aCfg::init(){
	QDomNode cur, sub;
	long id;

//	QString s = xml.toString(4).local8Bit(); // fake call for trap troble
	if ( !xml.isDocument() ) {
		aLog::print(aLog::Error, tr("Bad configuration"));
		return;
	}
//CHECK_POINT
	QDomElement rootnode = xml.documentElement();
//CHECK_POINT
//	QDomElement root = rootnode;
//CHECK_POINT
	cfginfo = rootnode.namedItem( md_info ).toElement();
	md = rootnode.namedItem( md_metadata ).toElement();
	iface = rootnode.namedItem( md_interface ).toElement();
	actions = rootnode.namedItem( md_actions ).toElement();
	idcache.clear();
	cur = rootnode.firstChild();
	while (!cur.isNull()) {
		if ( cur.toElement().hasAttribute( mda_id ) ) {
			id = 0;
			id = attr(cur.toElement(), mda_id).toInt();
			if (id) {
				idcache.insert(id, new aCfgItemContaner( id, cur.toElement() ) );
			}
//			printf("cache name=%s id=%ld\n",(const char *)cur.nodeName(), id);
		}
		sub = cur.firstChild();
		if (!sub.isNull()) cur = sub;
		else
		{
			if (!cur.nextSibling().isNull()) cur = cur.nextSibling();
			else
			{
				sub = cur;
				cur.clear();
				while (cur.isNull()) {
					sub = sub.parentNode();
					if (sub.isNull()) break;
					else cur = sub.nextSibling();
				}
			}
		};
	}
//CHECK_POINT
//	printf("init ok\n");
}


/*!
 *	Reads resource file.
 *	\param fname (in) - rc file name.
 *	\return error code.
 */
int
aCfg::readrc(const QString &fname)
{
	int r = RC_ERROR;
	QString configfile;

	r = rc.read( fname );
	if ( r ) return r;
	configfile = rc.value( "configfile" );
	if ( !configfile.isEmpty() ) {
		r = read( configfile );
		if ( r ) createNew();//return r;
//		init();
		r = RC_OK;
	} else r = RC_ERROR;
	return r;
}


/*!
 *	Writes resource file.
 *	\param (in) - rc file name to write to.
 *	\return int.
 */
int
aCfg::writerc(const QString &fname)
{
	int r = RC_ERROR;
	QString configfile;
	r = rc.write( fname );
	if ( r ) return r;
	configfile = rc.value( "configfile" );
	if ( !configfile.isEmpty() ) {
		r = write( configfile );
	}
	if ( !r ) setModified( false );
	return r;
}



/*!
 * 	Creates new empty configuration.
 * 	\return void.
 */
void
aCfg::createNew(){
	aCfgItem i;
	xml.setContent(QString("<?xml version = '1.0' encoding = 'UTF-8'?>\n"));
	setModified( true );

	xml.appendChild( xml.implementation().createDocumentType("ananas_configuration",QString::null,"ananas-cfg.dtd"));
	rootnode = xml.createElement( md_root );
	xml.appendChild( rootnode );
	rootnode = xml.documentElement();
	cfginfo 	= insert( rootnode, md_info, QString::null, -1 );
	iface 		= insert( rootnode, md_interface, QString::null, -1 );
	md 		= insert( rootnode, md_metadata, QString::null, -1 );
	actions	= insert( rootnode, md_actions, QString::null, -1 );
	init();
	setInfo( md_info_name, "New configuration" );
	setInfo( md_info_lastid, "0" );
	setInfo( md_info_author, "unnamed" );
	setInfo( md_info_date, QDateTime::currentDateTime().date().toString(Qt::ISODate) );
	//setInfo( md_info_dir, "newConfig" );
	insert( md, md_globals, QString::null, -1 );
	insert( md, md_catalogues, QString::null, -1 );
	insert( md, md_documents, QString::null, -1 );
	insert( md, md_journals, QString::null, -1 );
	i = insert( md, md_registers, QString::null, -1 );
	insert( i, md_iregisters, QString::null, -1 );
	insert( i, md_aregisters, QString::null, -1 );
}


/*!
 * 	Returns value of info section field of configuration.
 * 	\param name (in) - name of field to fetch value from.
 * 	\return info field's value.
 */
QString
aCfg::info(const QString &name)
{
	return text(cfginfo.namedItem( name ).toElement()); //.firstChild().nodeValue();
}

/*!
 * 	Sets new value to specified field of info section of configuration.
 * 	\param name (in) - field's name.
 * 	\param value (in) - field's value.
 * 	\return void.
 */
void
aCfg::setInfo(const QString &name, const QString &value)
{
	aCfgItem i;
	i = cfginfo.namedItem( name ).toElement();
	if (i.isNull()) {
		i = xml.createElement( name );
		cfginfo.appendChild( i );
	}
	setText(i, value);
	setModified( true );
}


/*!
 * 	Generates new unique ID for metadata tree object.
 * 	\return new unique ID.
 */
long
aCfg::nextID()
{
	long id = 0;
	id = info( md_info_lastid ).toLong();
	if (id == 0 ) id = context_startid;
	++id;
	setInfo( md_info_lastid, QString("%1").arg(id) );
	return id;
}



/*!
 * 	Counts specified objects in metadata tree.
 * 	\param context (in) - parent of objects to count. May be a root
 * 	of metadata tree.
 * 	\param classname (in) - class of objects to count.
 * 	\return number of objects.
 */
int
aCfg::count(aCfgItem context, const QString &classname){
	return context.elementsByTagName( classname ).count();
}


/*!
 * 	Counts specified child objects of object in metadata tree.
 * 	Only child objects will be counted, not subchild objects.
 * 	\param context (in) - parent of objects to count. May be a root
 * 	of metadata tree.
 * 	\param classname (in) - class of objects to count.
 * 	\return number of objects.
 */
int
aCfg::countChild(aCfgItem context, const QString &classname){
	int n = 0;
	aCfgItem i = firstChild( context );
	while ( !i.isNull() ) {
		if ( classname.isEmpty() ) n++;
		else
		if ( objClass( i ) == classname ) n++;
		i = nextSibling( i );
	}
	return n;
}


/*!
 * \en
 * 	Finds object by its name and number.
 * 	\param context (in) - context to seach in.
 * 	\param name (in) - name of object to search for.
 * 	\param n (in) - number of object in found context.
 * 	\return founded object.
 * \_en
 * \ru
 * 	Находит элемент метаданных в заданном контексте (поддереве) метаданных по имени тэга
 * 	и порядковому номеру. Порядковый номер имеет смысл, если в контексте может находиться
 * 	более одного элемента с одним и тем же тэгом. Так, например, внутри документа может быть несколько
 * 	полей. Поэтому, делая поиск элемента метаданных, описывающего поле,
 * 	внутри контекста (поддерева), описывающего документ, необходимо задавать порядковый номер поля.
 * 	\param context (in) - контекст (поддерево) метаданных,для сужения пространства поиска.
 * 	\param name (in) - имя тэга, с помощью которого описывается элемент метаданных, который мы ищем.
 * 	Возможные значения для этого параметра смотрите в файле acfg.h под заголовком Metadata tags.
 * 	\param n (in) - порядковый номер объекта, на случай, если таких объектов несколько.
 * 	\return ссылку на найденный элемент метаданных или NULL, если элемент не найден.
 * \_ru
 */
aCfgItem
aCfg::find(aCfgItem context, const QString &name, int n)
{
	QDomNodeList l = context.elementsByTagName( name );
	return l.item( n ).toElement();
}


/*!
 * \en
 * 	\brief Finds object by unique ID in metadata tree.
 *	\param id (in) - id to search for.
 *	\return founded object.
 * \_en
 * \ru
 * 	\brief Находит описание объекта в метаданных по его уникальному в рамках файла метаданных идентификатору.
 *
 * 	\param id - идетнификатор объекта в метаданных
 * 	\return ссылку на элемент метаданных
 * \_ru
 */
aCfgItem
aCfg::find(long id)
{
	long idl = id;
	aCfgItem i;
	aCfgItemContaner *ic;

	if ( id == 0 || id == mdc_metadata ) return md;
	if ( id == mdc_root ) return rootnode;
	if ( id == mdc_info ) return cfginfo;
	if ( id == mdc_interface ) return iface;
	if ( id == mdc_actions ) return actions;

	ic = idcache.find( idl );
	if (ic) i = ic->item;
//	if (!i.isNull()) printf("!founded id=%li\n", id);
//	else printf("!NOT founded id=%li\n", id);
	return i;
}

/*!
 *\en
 * 	Finds object by its full name.
 * 	\param name (in) - object's full name.
 * 	\return founded object.
 *\_en \ru
 *	\brief Находит объект в текущей (открытой) конфигурации по его полному имени.
 *
 *	\n Полное имя имеет вид "<OType>.<OName>[.<extName>]"
 *	\n OType может принимать только одно из следующих значений:
 *	- Document - для объектов типа Документ
 *	- Catalog - для объектов типа Каталог (примером каталога является Справочник товаров)
 *	- DocJournal - для объектов типа журнал документов
 *	- InfoRegister - для объектов типа Информационный Регистр
 *	- AccumulationRegister - для объектов типа Накопительный регистр.
 *	  Накопительный регистр отличается от информационного тем, что по его записям выполняется автоматический расчет остатков.
 *	- Form - для объектов типа Экранная форма.
 *
 *	OName задает имя объекта, которое было присвоено объекту во время редактирования конфигурации в программе Анананс-Дизайнер.
 *	Для поиска документа "Приходная накладная" нужно вызвать find("Document.Приходная накладная");
 *	Для поиска формы с именем "Основная форма" в этом документе надо вызвать find("Document.Приходная накладная.Form.Основная форма");
 * 	\param name (in) - полное имя объекта.
 * 	\return ссылка на найденный объект в случае успеха или NULL в случае, если объект не найден.
 *\_ru
 */
aCfgItem
aCfg::find(const QString &name)
{
	QString oType, oName, omType, extName;
	aCfgItem gobj, obj, sobj, item;

	oType = name.section( ".", 0, 0 );
	oName = name.section( ".", 1, 1 );
	extName = name.section( ".", 2 );
	gobj = md;
	while ( !oType.isEmpty() && !oName.isEmpty() ) {
		if (oType == "Document" || oType == tr("Document") ) {
			omType = md_document;
			gobj = findChild( gobj, md_documents );
		}
		if (oType == "Catalogue" || oType == tr("Catalogue") ) {
			omType = md_catalogue;
			gobj = findChild( gobj, md_catalogues );
		}
		if (oType == "DocJournal" || oType == tr("DocJournal")) {
			omType = md_journal;
			gobj = findChild( gobj, md_journals );
		}
		if (oType == "Report" || oType == tr("Report")) {
			omType = md_report;
			gobj = findChild( gobj, md_reports );
		}
		if (oType == "InfoRegister" || oType == tr("InfoRegister") ) {
			omType = md_iregister;
			gobj = findChild( find(gobj,md_registers,0),md_iregisters );
		}
		if (oType == "AccumulationRegister" || oType == tr("AccumulationRegister") )
		{
			omType = md_aregister;
			gobj = findChild( find(gobj,md_registers,0),md_aregisters );
		}
		if (oType == "Form" || oType == tr("Form")) {
			omType = md_form;
			gobj = findChild( gobj, md_forms );
		}
		if ( !gobj.isNull() ) item = findName( gobj, omType, oName );
		if ( item.isNull() ) break;
		gobj = item;
		oType = extName.section( ".", 0, 0 );
		oName = extName.section( ".", 1, 1 );
		extName = extName.section( ".", 2 );
	}
	return item;
}

/*
 * ??? Parameter classname is what for ???
 */
aCfgItem
aCfg::findName(aCfgItem context, const QString &classname, const QString &name )
{
	aCfgItem item;

	if ( !context.isNull() && !classname.isEmpty() ){
		item = firstChild( context );
		while ( !item.isNull() ) {
			if ( attr( item, mda_name ) == name ){
				break;
			}
			item = nextSibling( item );
		}
	}
	return item;
}

/*
 *	Finds child object by its class name and number.
 *	\param context (in) - parent object.
 *	\param classname (in) - class name of child to search for.
 *	\param n (in) - number of child (0..countChild( context, classname )-1).
 *      \return child number n or Null object.
 */
aCfgItem
aCfg::findChild(aCfgItem context, const QString &classname, int n)
{
	int in = 0;
	aCfgItem i = firstChild( context );
	while ( !i.isNull() ) {
		if ( classname.isEmpty() ) {
			if ( in == n ) break;
			in++;
		}
		else {
			if ( objClass( i ) == classname ) {
				if ( in == n ) break;
				in++;
			}
		}
		i = nextSibling( i );
	}
	return i;
}



/*!
 * 	Swaps two items in metadata tree.
 * 	\param item1 (in) - first item to swap.
 * 	\param item2 (in) - second item to swap.
 * 	\return true or false.
 */
bool
aCfg::swap(aCfgItem &item1, aCfgItem &item2)
{
	aCfgItem item, ic1, ic2, p1, p2;

	if ( item1.isNull() || item2.isNull() || ( item1 == item2 )  )
	{
		if(item1.isNull()) aLog::print(aLog::Debug, tr("aCfg swap: first element is null"));
		else
		if(item2.isNull()) aLog::print(aLog::Debug, tr("aCfg swap: second element is null"));
		else
		aLog::print(aLog::Error, tr("aCfg swap element himself"));
		return false;
	}
	p1 = parent( item1 );
	p2 = parent( item2 );
	if ( p1.isNull() || p2.isNull() || ( p1 != p2 ) )
	{
		if(p1.isNull()) aLog::print(aLog::Debug, tr("aCfg swap: first parent null"));
		else
		if(p2.isNull()) aLog::print(aLog::Debug, tr("aCfg swap: second parent null"));
		else
		aLog::print(aLog::Error, tr("aCfg swap elements with differernt parents"));
		return false;
	}
	p1.insertBefore( item1, item2 );
	setModified( true );
	return true;
}

/*!
 *	Returns onject's order number among siblings.
 *	\param item (in) - object.
 *	\return object's order number.
 */
int
aCfg::order( aCfgItem item )
{
	int i = 0;
	aCfgItem current = item;
	while ( !current.isNull() )
	{
		++i;
		current = previousSibling( current );
	}
	return i;
}


/*!
 * 	Returns parent object.
 * 	\param context (in) - object.
 * 	\return paren object.
*/
aCfgItem
aCfg::parent( aCfgItem context )
{
	return context.parentNode().toElement();
}


/*!
 *	Returns the first child item of the context.
 *	If there is no child, a null context is returned.
 *	Changing the returned item will also change the item
 *	in the metadata tree.
 *	\param context (in) - node in metadata tree.
 *	\return aCfgItem - node in metafata tree if found, NULL if not found.
 */
aCfgItem
aCfg::firstChild( aCfgItem context )
{
	return context.firstChild().toElement();
}


/*!
 *	Returns the last child item of the context.
 *	If there is no child, a null context is returned.
 *	Changing the returned item will also change the item
 *	in the metadata tree.
 */
aCfgItem
aCfg::lastChild( aCfgItem context )
{
	return context.lastChild().toElement();
}


/*!
 *	Returns the next sibling in the netadata tree.
 *	Changing the returned item will also change the item in the metadata tree.
 */
aCfgItem
aCfg::nextSibling( aCfgItem context )
{
	return context.nextSibling().toElement();
}


/*!
 *	Returns the previous sibling in the netadata tree.
 *	Changing the returned item will also change the item in the metadata tree.
 */
aCfgItem
aCfg::previousSibling( aCfgItem context )
{
	return context.previousSibling().toElement();
}


/*!
 *	Returns text of object.
 *	\param context (in) - object.
 *	\return string.
 */
QString
aCfg::text(aCfgItem context)
{
	if (context.isNull()) return "";
	return context.firstChild().nodeValue();
}


/*!
 *	Sets text for object.
 *	\param context (in) - object.
 *	\param value (in) - new value.
 *	\return void.
 */
void
aCfg::setText(aCfgItem context, const QString &value)
{
	QDomText t;
	if (context.isNull()) return;
	while (!context.firstChild().isNull()) {
		context.removeChild( context.firstChild() );
	}
	t = xml.createTextNode( value );
	context.appendChild( t );
	setModified( true );
}


/*!
 * 	Returns binary value of object.
 * 	\param context (in) - object to get value of.
 * 	\return array of bytes.
 */
QByteArray
aCfg::binary( aCfgItem context )
{
	unsigned int i, d, blen;
	bool ok;
	QString vs = text( context );
	blen = attr( context, mda_length ).toInt();
	QByteArray b( blen );
	for ( i=0; i < blen; i++ ){
		d = 0xff & vs.mid( i*2, 2 ).toInt( &ok, 16 );
		if ( ok ) b.data()[ i ] = d;
		else b.data()[ i ] = 0;
	}
	return b;
}


/*!
 *	Sets binary value of metadata tree node.
 *	\param context (in) - metadata tree node.
 *	\param value (in) - new binary value.
 *	\param format (in) - format of binary data.
 *	\return void.
 */
void
aCfg::setBinary( aCfgItem context, const QByteArray &value, const QString &format )
{
	QString vs ="", s;
	unsigned int i, d;
	for ( i=0; i<value.count(); i++) {
		d = ( unsigned char ) value.data()[i];
		s = QString("00")+QString::number( d, 16 ).upper();
		vs += s.right(2);
	}
	setText( context, vs );
	setAttr( context, mda_length, value.count() );
	if ( !format.isNull() ) setAttr( context, mda_format, format );
}

/*!
 *	Returns format of metadata tree node binary data.
 *	\param context (in) - node of metadata tree.
 *	\return format of binary data.
 */
QString
aCfg::binaryFormat( aCfgItem context )
{
	return attr( context, mda_format );
}


/*!
 *	Returns text value of child node of metadata tree.
 *	\param context (in) - parent node.
 *	\param subname (in) - name of child node.
 *	\return text value.
 */
QString
aCfg::sText(aCfgItem context, const QString & subname )
{
	aCfgItem i;
	i = findChild( context, subname, 0 );
	if ( i.isNull() ) return ""; else return text( i );
}


/*!
 * 	Sets text value of child node.
 * 	\param context (in) - parent node.
 * 	\param subname (in) - name of child node.
 * 	\param value (in) - new value of child node.
 * 	\return void.
 */
void
aCfg::setSText(aCfgItem context, const QString & subname, const QString &value)
{
	aCfgItem i;
	i = findChild( context, subname, 0 );
	if ( i.isNull() ) {
		i = insert( context, subname );
	}
	setText( i, value );
}


/*!
 * 	Returns class name of object.
 * 	\param id (in) - object id.
 * 	\return object's class name.
 */
QString
aCfg::objClass( long id )
{
	return objClass( find( id ) );
}


/*!
 * 	Returns class name of object.
 * 	\param context (in) - object.
 * 	\return object's class name.
 */
QString
aCfg::objClass( aCfgItem context )
{
	if (context.isNull()) return "";
	else return context.tagName();
}


/*!
 * 	Returns object's attribute value.
 * 	\param context (in) - object.
 * 	\param name (in) - attribute name.
 * 	\return attribute value.
 */
QString
aCfg::attr(aCfgItem context, const QString &name)
{
	return context.attribute(name);
}

/*!
 * 	Sets object's attribute value.
 * 	\param context (in) - object.
 * 	\param name (in) - attribute's name.
 * 	\param value (in) - new value.
 * 	\return void.
 */
void
aCfg::setAttr(aCfgItem context, const QString &name, const QString &value)
{
	QString v = value;
	if ( objClass( context ) == md_field && name == mda_type ) {
		if ( v.section(" ", 1).isEmpty() ) v.append(" 0 0 *");
		if ( v.section(" ", 2).isEmpty() ) v.append(" 0 *");
		if ( v.section(" ", 3).isEmpty() ) v.append(" *");
	}
 	context.setAttribute( name, v );
	setModified( true );
}


/*!
 * 	Changes value of object's attribute.
 * 	\param context (in) - object.
 * 	\param name (in) - attribute's name.
 * 	\param value (in) - new attribute's vakue.
 * 	\return nothing.
 */
void
aCfg::setAttr(aCfgItem context, const QString &name, int value)
{
	context.setAttribute(name, value);
	setModified( true );
}


/*!
 *	Inserts a new object into metadata tree.
 *	\param context (in) - metadata object context.
 *	\param otype (in) - metadata object type.
 *	\param name (in) - metadata object name. If name = QString::null,
 *	name attribute will not be setted.
 *	\param id (in) - metadata object id. If id = 0, setting next avalable id for
 *	the metadata tree.
 *	\return newly created metadata object.
 *	To check if created object is valid use "if (CreatedObject.isNull()) ..." construction.
 */
aCfgItem
aCfg::insert(aCfgItem context, const QString &otype, const QString &name, long id)
{
	aCfgItem i;

	if ( id==0 ) id = nextID();
	i = xml.createElement( otype );
	if ( id >= 100 ) setAttr(i, mda_id, id );
	if ( !name.isNull()) setAttr(i, mda_name, name );
	context.appendChild( i );
	if ( id > 0 ) idcache.insert(id, new aCfgItemContaner( id, i ) );
	setModified( true );
	return i;
}

/*!
 *\~english
 *	Saves configuration object to file.
 *\~russian
 *	Сохраняет объект бизнес схемы в файл.
 *\~
 *	\param context - \~english object to save \~russian объект для сохранения \~
 *	\param fileName - \~english file name \~russian имя файла \~
 *	\return - \~english error code \~russian код ошибки \~
 */
int
aCfg::saveOneObject(aCfgItem context, const QString &fileName)
{
	int res=RC_ERROR;
	if(!context.isNull())
	{
		QDomDocument doc;
		if(context.isDocument())
		{
			doc = context.toDocument();
		}
		else
		{
			doc.setContent(QString("<?xml version = '1.0' encoding = 'UTF-8'?>\n"));
			doc.appendChild(context.cloneNode(true));
		}
		res = write(doc,fileName);
		if(res==RC_OK)
		{
			aLog::print(aLog::Info, tr("aCfg saveOneObject: context save to file `%1'").arg(fileName));
		}
	}
	else
	{
		aLog::print(aLog::Error, tr("aCfg saveOneObject: context is null"));
	}
	return res;
}

/*!
 *\~english
 *	Load configuration object from file.
 *\~russian
 *	Загружает объект бизнес схемы из файла.
 *\~
 *	\param fileName - \~english file name \~russian имя файла \~
 *	\return - \~english object, stored in xml file \~russian объект, хранящийся в xml файле \~
 */
aCfgItem
aCfg::loadOneObject( const QString &fileName )
{
	aCfgItem res;
	QDomDocument doc;

	if(fileName!="")
	{
		QFile file( fileName );
		QByteArray buf;
		QString err;
//		int errLine = 0, errColumn = 0;
		if ( file.open( QIODevice::ReadOnly ) )
		{
			buf = file.readAll();
			file.close();
			if(doc.setContent( buf, false ))
			{
				res = doc.documentElement();
			}
			else
			{
				aLog::print(aLog::Error, tr("aCfg loadOneObject: invalid cfg file `%1'").arg(fileName));
			}

		}
		else
		{
			aLog::print(aLog::Error, tr("aCfg loadOneObject: invalid file name or not permission to file `%1'").arg(fileName));
		}
    	}
	return res;
}

/*!
 *	Removes metadata object from metadata tree.
 *	All children objects also removed.
 *	\param context (in) - metadata object.
 *	\return nothing.
 */
void
aCfg::remove(aCfgItem context)
{
	long id;
	aCfgItem child;

	QDomNode own = context.parentNode();
	if (!own.isNull()) {
		id = attr( context, mda_id ).toLong();
		child = firstChild(context);
		remove(child);
		idcache.remove( id );
		own.removeChild(context);
		setModified( true );
	}
}


/*!
 *\~english
 *	Import configuration object from another configuration.
 *\~russian
 *	Импортирует объект конфигурации из другой конфигурации в текущую.
 *	Устанавливает новые Id для всех потомков объекта.
 *\~
 *	\param context - \~english parent for imported object \~russian родитель для импортируемого объекта \~
 *	\param objFromAnotherCfg - \~english imported object \~russian импортируемый объект \~
 */
aCfgItem
aCfg::importCfgItem(aCfgItem context, aCfgItem objFromAnotherCfg )
{
	aCfgItem item = xml.importNode(objFromAnotherCfg,true).toElement();
	if(!item.isNull())
	{
		context.appendChild( item );
		setupNewId(item);
	}
	return item;

}
/*!
 *\~english
 *	Sets new id for all children of object \a obj
 *\~russian
 *	Устанавивает новый id для всех потомков объекта \a obj
 *\~
 */
void
aCfg::setupNewId( aCfgItem obj )
{
	if(!obj.isNull())
	{
		if(attr(obj, mda_id)!="")
		{
			long id;
			aLog::print(aLog::Debug, tr("aLog setup new id for %1").arg(attr(obj, mda_name)));
			id = nextID();
			aLog::print(aLog::Debug, tr("aLog next id is %1").arg( id ) );
			setAttr(obj,mda_id,id);
			idcache.insert(id, new aCfgItemContaner( id, obj ) );
		}
		aCfgItem i = firstChild(obj);
		while(!i.isNull())
		{
			setupNewId(i);
			i = nextSibling(i);
		}
	}
}

/*!
 *	Gets ID for metadata object by complete name.
 *	Name like "Document.Invoice", "Report.Total", etc.
 *	\param name (in) - full object's name.
 *	\return object's ID.
 */
long
aCfg::getId( QString &name )
{
	QString p1, p2, p3;
	p1 = name.section( ".", 0, 0 );
	p2 = name.section( ".", 1, 1 );
	p3 = name.section( ".", 2, 2 );

	return 0;
}


/*!
 *	Returns configuration object ID.
 *	\param context (in) - object which ID we looking for.
 *	\return object's ID.
 */
long
aCfg::id(aCfgItem context)
{
	long i = 0;
	QString sid;

	if ( !context.isNull() ) {
		sid = attr( context, mda_id );
		if ( !sid.isEmpty() ) i=sid.toLong();
	}
	return i;
}

/*
 * ???
 */
void
aCfg::addTypes( QStringList &list, aCfgItem context, const QString &tobject, const QString &tname)
{
	aCfgItem obj;
	int i, n;
	long oid;

	if ( context.isNull() ) return;
	n = count( context, tobject );
	for (i=0; i<n; i++) {
		obj = find( context, tobject, i);
		if ( !obj.isNull() ) {
			oid = id( obj );
			if ( oid ) {
				list << QString("O ")+QString::number( oid )+QString("\t")+tname+QString(".")+attr( obj, mda_name );
			}
		}
	}
}

/*!
 *	Returns list of the configuration types.
 *	Every string in string list stored in format, consist of type's
 *	format string and type's human name fields, separated
 *	by tabulate char. For example:
 *	N %d %d\tNumberic
 *	C %d\tChar
 *	O 221\tDocument.Invoice
 *	\param filter (in) - used to limit returned types. If this parameter is empty
 *	all types will be returned.
 *	\return list of types.
 */
QStringList
aCfg::types( const QString &filter, aCfgItem *obj )
{
	QStringList l, lg;
	aCfgItem group, context;

	if ( filter.isEmpty() ) {
		l << QString("\t")+QObject::tr("Unknown");
		l << QString("N %d %d\t")+QObject::tr("Numberic");
		l << QString("C %d\t")+QObject::tr("Char");
		l << QString("D\t")+QObject::tr("Date");
		l << QString("B\t")+QObject::tr("Boolean");
		addTypes( l, find( md, md_catalogues, 0), md_catalogue, QObject::tr("Catalogue") );
		addTypes( l, find( md, md_documents, 0), md_document, QObject::tr("Document") );
	} else {
		QString fg, fd;
		if ( filter == md_catalogue ){
			fg = md_catalogues;
			fd = QObject::tr("Catalogue");
		}
		if ( filter == md_document ){
			fg = md_documents;
			fd = QObject::tr("Document");
		}
		if ( filter == md_journal ){
			fg = md_journals;
			fd = QObject::tr("Journal");
		}
		if ( filter == md_field ){
			fg = md_field;
			fd = QObject::tr("Field");
			if ( obj ) context = *obj;
			else return l;
		} else context = find( md, fg, 0);
		l << QString("\t")+QObject::tr("Unknown");
		if ( !fg.isEmpty() ) addTypes( l, context, filter, fd );
	}
	return l;
}

/*!
 * 	Returns object types names.
 * 	\return list of types names.
 */
QStringList
aCfg::ObjectTypes()
{
	QStringList l, lg;
	aCfgItem group, obj;

	addTypes( l, find( md, md_catalogues, 0), md_catalogue, QObject::tr("Catalogue") );
	addTypes( l, find( md, md_documents, 0), md_document, QObject::tr("Document") );
	addTypes( l, find( md, md_journals, 0), md_journal, QObject::tr("Journal") );
	addTypes( l, find( md, md_iregisters, 0), md_iregister, QObject::tr("Information Register") );
	addTypes( l, find( md, md_aregisters, 0), md_aregister, QObject::tr("Accumuliation Register") );
	addTypes( l, find( md, md_reports, 0), md_report, QObject::tr("Report"));
return l;
}


/*!
 *	Inserts new document in metadata tree.
 *	\param name (in) - document's name.
 *	\return newly created document.
 */
aCfgItem
aCfg::insertDocument( const QString &name )
{
	aCfgItem r, i;
	r = find( md, md_documents, 0 );
	if ( ! r.isNull() ) {
		i = insert( r, md_document, name );
		insert( i, md_header, QString::null, -1 );
		insert( i, md_tables, QString::null, -1 );
		insert( i, md_forms, QString::null, -1 );
		insert( i, md_webforms, QString::null, -1 );
	}
	return i;
}


/*!
 *	Inserts document's table in metadata tree.
 *	\param owner (in) - document to insert table.
 *	\param name (in) - table's name.
 *	\return newly created table.
 */
aCfgItem
aCfg::insertDocumentTable(aCfgItem owner, const QString &name )
{
	aCfgItem sec, t;
	sec = find( owner, md_tables, 0 );
	if ( !sec.isNull() ) {
		t = insert( sec, md_table, name );
	}
	return t;
}


/*!
 *	Inserts new catalogue in metadata tree.
 *	\param name (in) - name of catalogue.
 *	\return newly created catalogue.
 */
aCfgItem
aCfg::insertCatalogue( const QString &name )
{
	aCfgItem r, i;
	r = find( md, md_catalogues, 0 );
	if ( ! r.isNull() ) {
		i = insert( r, md_catalogue, name );
		insert( i, md_element, QString::null, -1 );
		insert( i, md_group, QString::null, -1 );
		insert( i, md_forms, QString::null, -1 );
		insert( i, md_webforms, QString::null, -1 );
	}
	return i;
}

/*!
 *	Inserts new information register in metadata tree.
 *	\param name (in) - name of catalogue.
 *	\return newly created catalogue.
 */
aCfgItem
aCfg::insertIRegister ( const QString &name )
{
	aCfgItem r, i;
	r = find(find( md, md_registers, 0 ),md_iregisters, 0);
	if ( ! r.isNull() ) {
		i = insert( r, md_iregister, name );
		insert( i, md_resources, QString::null, -1 );
		insert( i, md_dimensions, QString::null, -1 );
		insert( i, md_information, QString::null, -1 );
	}
	return i;
}

/*!
 *	Inserts new accumalation register in metadata tree.
 *	\param name (in) - name of catalogue.
 *	\return newly created catalogue.
 */
aCfgItem
aCfg::insertARegister ( const QString &name )
{
	aCfgItem r, i;
	r = find(find( md, md_registers, 0 ),md_aregisters, 0);
	if ( ! r.isNull() ) {
		i = insert( r, md_aregister, name );
		insert( i, md_resources, QString::null, -1 );
		insert( i, md_dimensions, QString::null, -1 );
		insert( i, md_information, QString::null, -1 );
	}
	return i;
}

/*!
 *	Inserts new journal in metadata tree.
 *	\param name (in) - name of catalogue.
 *	\return newly created catalogue.
 */
aCfgItem
aCfg::insertJournal ( const QString &name )
{
	aCfgItem r, i;
	r = find( md, md_journals, 0 );
	if ( ! r.isNull() ) {
		i = insert( r, md_journal, name );
		insert( i, md_columns, QString::null, -1 );
		insert( i, md_forms, QString::null, -1 );
		insert( i, md_webforms, QString::null, -1 );
	}
	return i;
}

/*!
 *	Inserts new report in metadata tree.
 *	\param name (in) - name of catalogue.
 *	\return newly created catalogue.
 */
aCfgItem
aCfg::insertReport ( const QString &name )
{
	aCfgItem r, i;
	r = find( md, md_reports, 0 );
	if ( ! r.isNull() ) {
		i = insert( r, md_report, name );
		insert( i, md_forms, QString::null, -1 );
		insert( i, md_webforms, QString::null, -1 );
	}
	return i;
}


/*!
 *	Creates a new field's definition in owner's section.
 *	If section is empty, field will be created in owner's context.
 *	\param owner (in) - owner of a new field.
 *	\param section (in) - section of owner. For example Document
 *	may consists of one or more table sections. All of them contains set of fields.
 *	\param name (in) - field's name.
 *	\param ftype (in) - field's type.
 *	\return newly created field.
 *
 */
aCfgItem
aCfg::insertField( aCfgItem owner, const QString &section, const QString &name,const QString &ftype )
{
	aCfgItem sec, f;
	if ( section.isEmpty() ) sec = owner;
	else sec = find( owner, section, 0 );
	if ( !sec.isNull() ) {
		f = insert( sec, md_field, name );
		setAttr( f, mda_type, ftype );
	}
	return f;
}

/*!
 * 	Creates new screen form for object like Document and writes it to metadata tree.
 * 	\param owner (in) - owner of new form.
 * 	\param name (in) - name of new form.
 * 	\return node of metadata tree which represents a newly created form.
 */
aCfgItem
aCfg::insertForm( aCfgItem owner, const QString &name )
{
	aCfgItem sec, f;
	sec = find( owner, md_forms, 0 );
	if ( !sec.isNull() ) {
		f = insert( sec, md_form, name );
	}
	return f;
}

/*!
 * 	Creates new Web form for object like Document and writes it to metadata tree.
 * 	\param owner (in) - owner of new form.
 * 	\param name (in) - name of new form.
 * 	\return node of metadata tree which represents a newly created form.
 */
aCfgItem
aCfg::insertWebForm( aCfgItem owner, const QString &name )
{
	aCfgItem sec, f;
	sec = find( owner, md_webforms, 0 );
	if ( !sec.isNull() ) {
		f = insert( sec, md_webform, name );
	}
	return f;
}

/*!
 * 	Returns ID of default screen form of object like Document.
 * 	\param owner (in) - form's owner.
 * 	\param actiontype (in) - action to which form is a default form.
 * 	\param mode (in) - action to which form is a mode.
 * 	\return form's ID.
 */
long
aCfg::getDefaultFormId( aCfgItem owner, int actiontype, int mode )
{
	aCfgItem	fobj, cobj;
	cobj = find( owner, md_forms, 0 );
	if ( cobj.isNull() )
		return 0;
	int formtype, fa;
	switch ( actiontype )
	{
		case md_action_new :
			formtype = md_form_new;
			break;
		case md_action_view :
			formtype = md_form_view;
			break;
		case md_action_edit :
			formtype = md_form_edit;
			break;
		default :
			return 0;
	}
	uint n = count( cobj, md_form );
	for ( uint i = 0; i < n; i++ )
	{
		fobj = find( cobj, md_form, i);
		fa = sText( fobj, md_defaultmod).toInt();
		if (  (fa>>formtype)%2 && attr( fobj, mda_type ).toInt() == mode )
			return id(fobj);
	}
	return 0;
}

/*!
 * 	Gets journal's documents list.
 * 	\param journal (in) - journal to get list from.
 * 	\return list of documents IDs.
 */
QStringList
aCfg::getJournalDocuments( aCfgItem journal )
{
	QStringList l;
	l.clear();
	if ( objClass(journal) != md_journal )
		return l;
	aCfgItem	dobj, pobj, jobj;
	uint n, m;
	int type = attr(journal,mda_type).toInt();
	switch ( type )
	{
		case 0:
			pobj = find( find( mdc_metadata ), md_documents, 0 );
			n = count(pobj,md_document);
			for ( uint i = 0; i < n; i++ )
			{
				dobj = find ( pobj, md_document, i );
				if ( !dobj.isNull())
					l.append( attr( dobj, mda_id ));
			}
			break;
		case 1:
			n = count(journal,md_used_doc);
			for ( uint j = 0; j < n; j++ )
			{
				dobj = find( journal, md_used_doc, j );
				QString id = text(dobj);
				if ( !find(id.toLong()).isNull() )
					l.append( text(dobj) );
				else
					remove(dobj);
			}
			break;
		case 2 :
			pobj = find( find( mdc_metadata ), md_documents, 0 );
			n = count(pobj,md_document);
			for ( uint i = 0; i < n; i++ )
			{
				jobj = find ( pobj, md_document, i );
				if ( !jobj.isNull())
					l.append ( attr( jobj, mda_id ));
			}
			pobj = find( find( mdc_metadata ), md_journals, 0 );
			n = count( pobj, md_journal );
			for ( uint i = 0; i < n; i++ )
			{
				jobj = find ( pobj, md_journal, i );
				if ( !jobj.isNull())
				{
					m = count ( jobj, md_used_doc );
					for ( uint j = 0; j < m; j++ )
					{
						dobj = find( jobj, md_used_doc, j );
						QString str = text(dobj);
						l.remove ( str );
					}
				}
			}
			break;
	}
	return l;
}

/*!
 *
 */
aCfgItem
aCfg::findJournal(int jtype, aCfgItem doc)
{
	aCfgItem res;
	uint n,m;
	aCfgItem js = find( find( mdc_metadata ), md_journals, 0 );
	if(js.isNull()) return res;

	n = count( js, md_journal );
	switch(jtype)
	{
		case 0:
		for ( uint i = 0; i < n; i++ )
		{
			aCfgItem jobj = find ( js, md_journal, i );
			if ( !jobj.isNull())
			{
				if(attr(jobj, mda_type).toInt()==0) return jobj;
			}
		}
		break;

		case 1:
		for ( uint i = 0; i < n; i++ )
		{
			aCfgItem jobj = find ( js, md_journal, i );
			if ( !jobj.isNull())
			{
				if(attr(jobj, mda_type).toInt()==1)
				{
					m = count ( jobj, md_used_doc );
					for ( uint j = 0; j < m; j++ )
					{
						QString dobj = text(find( jobj, md_used_doc, j ));
						if(dobj==attr(doc, mda_id)) return jobj;

					}
				}
			}
		}
		break;

		default:
		break;
	}
	return res;
}


/*!
 * Return list of object database tables.
 *	Every string in string list stored in format,
 *	consist of table id, table's human name fields,
 *	separated by tabulate char.
 *	This function used by Database tables plugins.
 *	For example:
 *	0\tHeader
 *	123\tTable1
 *	id = 0 for document header, catalogue element;
 *	id = 1 for catalogue group;
 *	id = metadata object id for document table.
 *	\param obj (in) - metadata object.
 *	\return list of tables.
 */
QStringList
aCfg::objTables( aCfgItem obj )
{
	QStringList list;
	QString t;
	int i, n;
	long oid, tid;
	aCfgItem objt;

	if ( obj.isNull() ) return list;
	t = objClass( obj );
	oid = id( obj );
	if ( t == md_journal ) {
		list << QString("0\t")+QObject::tr("Documents");
	}
	if ( t == md_catalogue ) {
		list << QString("0\t")+QObject::tr("Element");
		list << QString("1\t")+QObject::tr("Group");
	}
	if ( t == md_document ) {
		list << QString("0\t")+QObject::tr("Header");
		n = count( obj, md_table );
		for (i=0; i<n; i++) {
			objt = find( obj, md_table, i);
			if ( !objt.isNull() ) {
				tid = id( objt );
				if ( tid ) {
					list << QString::number( tid )+QString("\t")+attr( objt, mda_name );
				}
			}
		}
	}
	return list;
}


/*!
 * Return list of object database tables.
 *	Every string in string list stored in format,
 *	consist of table id, table's human name fields,
 *	separated by tabulate char.
 *	This function used by Database tables plugins.
 *	For example:
 *	0\tHeader
 *	123\tTable1
 *	id = 0 for document header, catalogue element;
 *	id = 1 for catalogue group;
 *	id = metadata object id for document table.
 *	\param objid (in) - metadata object id.
 *	\return list of tables.
 */
QStringList
aCfg::objTables( int objid )
{
	return objTables( find( objid ) );
}


/*!
 * Return context of the object table by id.
 *	This function used by Database tables plugins.
 *	\param obj (in) - metadata object.
 *	\param id (in) - metadata object table id.
 *	id = 0 for document header, catalogue element;
 *	id = 1 for catalogue group;
 *	id = metadata object id for document table.
 *	\return table context.
 */
aCfgItem
aCfg::objTable( aCfgItem obj, int id )
{
	aCfgItem item;
	QString t;

	if ( obj.isNull() ) return obj;
	t = objClass( obj );
	if ( t == md_catalogue ) {
		if ( id == 0 ) item = findChild( obj, md_element );
		if ( id == 1 ) item = findChild( obj, md_group );
	}
	if ( t == md_document ) {
		if ( id == 0 ) item = findChild( obj, md_header );
		else item = find( id );
	}
	if ( t == md_journal ) {
		if ( id == 0 ) item = findChild( obj, md_columns );
	}
	return item;
}


/*!
 * Return context of the object table by object id and table id.
 *	This function used by Database tables plugins.
 *	\param objid (in) - metadata object id.
 *	\param id (in) - metadata object table id.
 *	id = 0 for document header, catalogue element;
 *	id = 1 for catalogue group;
 *	id = metadata object id for document table.
 *	\return table context.
 */
aCfgItem
aCfg::objTable( int objid, int id )
{
	return objTable( find( objid ), id );
}


/************ P U B L I C   S L O T S ******************************/
/*!
 *
 */
QString
aCfg::Info( QString partname )
{
        return info( partname );
}



/*!
 * AMetadataObject
 *
 */
AMetadataObject::AMetadataObject( aCfgItem newitem )
{
        item = newitem;
}


QString
AMetadataObject::Class()
{
        return "Unknown";
}

