/****************************************************************************
** $Id: aexttext.cpp,v 1.1 2008/11/05 21:16:28 leader Exp $
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

#include <qtextcodec.h>
#include "aexttext.h"
#include "acfg.h"
#include <QTextStream>
#include <QTextCodec>

/*!
\en
    \class AExtText aexttext.h
    \brief The AExtText is Ananas extension class for work with text files.
\_en \ru
    \class AExtText aexttext.h
    \brief Класс расширения Ананас AExtText предназначен для работы с текстовыми файлами.

    Поддерживает работу с файлами в различных кодировках,
    режимах:
    RO - Только чтение
    WO - Только запись
    RW - чтение/запись
    A  - добавление к концу файла.
\_ru
*/


// \param cmd (in) command text.
// \return - result.


/*!
 *\en
 *
 *\_en \ru
 *
 *\_ru
 */
AExtText::AExtText()
    : AExtension("Text")
{
	text = new QTextStream( &file );
	setCodec("UTF-8");
}



/*!
 *\en
 *
 *\_en \ru
 *
 *\_ru
 */
AExtText::~AExtText()
{
	delete text;
}


/*!
 *\en
 *
 *\_en \ru
 *
 *\_ru
 */
int
AExtText::open( const QString &filename, const QString &mode )
{
	QIODevice::OpenMode m = QIODevice::ReadOnly;
	file.close();
	file.setName( filename );
	if ( mode == "WO" ) m = QIODevice::WriteOnly;
	if ( mode == "RW" ) m = QIODevice::ReadWrite;
	if ( mode == "A" ) m = QIODevice::Append;
	if ( file.open( m ) ) return 0;
	return 1;
}


/*!
 *\en
 *
 *\_en \ru
 *
 *\_ru
 */
void
AExtText::close()
{
	file.close();
}


/*!
 *\en
 *
 *\_en \ru
 *
 *\_ru
 */
QString
AExtText::readLine()
{
	return text->readLine();
}



/*!
 *\en
 *
 *\_en \ru
 *
 *\_ru
 */
void
AExtText::writeLine( const QString &l )
{
	*text << l << "\n";
}



/*!
 *\en
 *
 *\_en \ru
 *
 *\_ru
 */
bool
AExtText::atEnd()
{
	return text->atEnd();
}



/*!
 *\en
 *
 *\_en \ru
 *
 *\_ru
 */
QString
AExtText::getCodec() const
{
	QTextCodec *codec = text->codec();
	if ( codec ) return codec->name();
	return "";
}



/*!
 *\en
 *
 *\_en \ru
 *
 *\_ru
 */
void
AExtText::setCodec( const QString &codecname )
{
	QTextCodec *codec = QTextCodec::codecForName( ( const char *) codecname );
	if ( codec ) text->setCodec( codec );
}


#include <aextensionplugin.h>
typedef AExtensionPlugin<AExtText> AExtTextPlugin;
A_EXPORT_PLUGIN( AExtTextPlugin )
