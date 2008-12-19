/****************************************************************************
** $Id: aextxml.cpp,v 1.1 2008/11/05 21:16:28 leader Exp $
**
** Extension object implementation file of
** Ananas application library
**
** Created : 20031201
**
** Copyright (C) 2003-2006 Leader InfoTech.  All rights reserved.
** Copyright (C) 2003-2006 Valery Grazdankin, Orenburg.
** Copyright (C) 2003-2005 Grigory Panov, Yoshkar-Ola.
**
** This file is part of the Designer application of the Ananas
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

#include <qfile.h>
#include <qtextstream.h>

#include "aextxml.h"
#include "acfg.h"
/*!
\en
    \class AExtXML aextxml.h
    \brief The AExtXML is Ananas extension class for work with XML files.
\_en \ru
    \class AExtXML aextxml.h
    \brief Класс расширения Ананас AExtXML предназначен для работы с XML файлами.
\_ru
*/



/*!
 *\en
 *	Create object.
 *\_en \ru
 *	Создает объект.
 *\_ru
 */
AExtXML::AExtXML()
    : AExtension("XML")
{
}



/*!
 *\en
 *	Destroy object.
 *\_en \ru
 *	Уничтожает объект.
 *\_ru
 */
AExtXML::~AExtXML()
{
}



/*!
 *\en
 *	Reads XML from file.
 *	\param fname (in) - name of XML file.
 *	\return true if OK.
 *\_en \ru
 *	Читает файл XML.
 *	\param fname (in) - имя файла XML.
 *	\return true в случае успеха.
 *\_ru
 */
bool
AExtXML::read(const QString &fname)
{
    QFile file( fname );
    QByteArray buf;
    QString err;
    int errLine = 0, errColumn = 0;
    if ( !file.open( IO_ReadOnly ) ) return RC_ERROR;
    buf = file.readAll();
    file.close();
    xml.setContent( QString("") );
    if ( !xml.setContent( buf, false, &err, &errLine, &errColumn ) ) {
	cfg_message( 2,
		     ( const char *) QObject::tr(
		     "Error read XML line:%d col:%s %s"),
		     errLine, errColumn, ( const char *) err );
	return false;
    }
    rootnode = xml.documentElement();
    current = rootnode;
    return true;
}



/*!
 *\en
 *	Writes XML file.
 *	\param fname (in) - name of XML file.
 *	\return error code.
 *\_en \ru
 *	Записывает файл XML.
 *	\param fname (in) - имя файла XML.
 *	\return Код ошибки.
 *\_ru
 */
bool
AExtXML::write(const QString &fname)
{
    QFile file( fname );
    QByteArray buf( xml.toString(4).utf8() );
    if ( file.open( IO_WriteOnly ) ) {
	QTextStream ts( &file );
	ts.setEncoding(QTextStream::UnicodeUTF8);
	xml.save(ts, 4);
	file.close();
	return true;
    }
    return false;
}



/*!
 *\en
 *	Create new XML document.
 *	Old document content will be destroyed.
 *	\param rootnodename (in) - new XML docemrnt root node name.
 *	\return void.
 *\_en \ru
 *	Создает новый XML документ.
 *	Старое содержимое документа уничтожается.
 *	\param rootnodename (in) - имя корневого узла документа XML.
 *	\return ничего.
 *\_ru
 */
void
AExtXML::createNew( const QString &rootnodename )
{
	xml.setContent(QString("<?xml version = '1.0' encoding = 'UTF-8'?>\n"));
	rootnode = xml.createElement( rootnodename );
	xml.appendChild( rootnode );
 	current = rootnode;
}



/*!
 *\en
 *	Check current node is root node.
 *	\return true if current node is root.
 *\_en \ru
 *	Проверяет, является ли текущий узел корневым.
 *	\return true - если текущий узел корневой.
 *\_ru
 */
bool
AExtXML::isRoot()
{
	return current == rootnode;
}



/*!
 *\en
 *
 *\_en \ru
 *
 *\_ru
 */
bool
AExtXML::parent()
{
	QDomNode pn = current.parentNode();
	if ( pn.isNull() ) return false;
	current = pn.toElement();
	return true;
}



/*!
 *\en
 *
 *\_en \ru
 *
 *\_ru
 */
bool
AExtXML::newChild( const QString &childname )
{
	QDomElement ne = xml.createElement( childname );
	if ( ne.isNull() ) return false;
	current.appendChild( ne );
	current = ne;
	return true;
}



/*!
 *\en
 *
 *\_en \ru
 *
 *\_ru
 */
QString
AExtXML::nodeName()
{
	if (current.isNull()) return "";
	else return current.tagName();
}



/*!
 *\en
 *	Sets text for current node.
 *	\param text (in) - new text value.
 *	\return void.
 *\_en \ru
 *	Устанавливает текст текущего узла.
 *	\param text (in) - Значение нового текста.
 *	\return ничего.
 *\_ru
 */
bool
AExtXML::setText( const QString &text )
{
	QDomText t;
	if (current.isNull()) return false;
	while (!current.firstChild().isNull()) {
		current.removeChild( current.firstChild() );
	}
	t = xml.createTextNode( text );
	if ( t.isNull() ) return false;
	current.appendChild( t );
	return true;
}



/*!
 *\en
 *	Returns text of current node.
 *	\return string.
 *\_en \ru
 *	Возвращает текст текущего узла.
 *	\return Строка.
 *\_ru
 */
QString
AExtXML::text() const
{
	if (current.isNull()) return "";
	return current.firstChild().nodeValue();
}



/*!
 *\en
 * 	Sets current node's attribute value.
 * 	\param name (in) - attribute's name.
 * 	\param value (in) - new value.
 * 	\return void.
 *\_en \ru
 *	Устанавливает значение атрибута текущего узла.
 * 	\param name (in) - иям атрибута.
 * 	\param value (in) - новое значение.
 * 	\return ничего.
 *\_ru
 */
bool
AExtXML::setAttr(  const QString &name, const QString &value )
{
 	current.setAttribute( name, value );
	return true;
}



/*!
 *\en
 * 	Returns current node's attribute value.
 * 	\param name (in) - attribute name.
 * 	\return attribute value.
 *\_en \ru
 *	Возвращает значение атрибута текущего узла.
 * 	\param name (in) - иям атрибута.
 * 	\return значение атрибута.
 *\_ru
 */
QString
AExtXML::attr(   const QString &name ) const
{
	return current.attribute(name);
}



/*!
 *\en
 *
 *\_en \ru
 *
 *\_ru
 */
bool
AExtXML::removeAttr(  const QString &name )
{
	current.removeAttribute( name );
	return true;
}



/*!
 *\en
 *
 *\_en \ru
 *
 *\_ru
 */
bool
AExtXML::firstChild()
{
	QDomNode n = current.firstChild();
	if ( n.isNull() ) return false;
	current = n.toElement();
	return true;
}



/*!
 *\en
 *
 *\_en \ru
 *
 *\_ru
 */
bool
AExtXML::lastChild()
{
	QDomNode n = current.lastChild();
	if ( n.isNull() ) return false;
	current = n.toElement();
	return true;
}



/*!
 *\en
 *
 *\_en \ru
 *
 *\_ru
 */
bool
AExtXML::next()
{
	QDomNode n = current.nextSibling();
	if ( n.isNull() ) return false;
	current = n.toElement();
	return true;
}



/*!
 *\en
 *
 *\_en \ru
 *
 *\_ru
 */
bool
AExtXML::prev()
{
	QDomNode n = current.previousSibling();
	if ( n.isNull() ) return false;
	current = n.toElement();
	return true;
}



/*!
 *\en
 *
 *\_en \ru
 *
 *\_ru
 */
bool
AExtXML::childExists()
{
	QDomNode n = current.firstChild();
	if ( n.isNull() ) return false;
	return true;
}

#include <aextensionplugin.h>
typedef AExtensionPlugin<AExtXML> AExtXMLPlugin;
A_EXPORT_PLUGIN( AExtXMLPlugin )
