/****************************************************************************
** $Id: aextxml.h,v 1.1 2008/11/05 21:16:28 leader Exp $
**
** Extension object header file of
** Ananas application library
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
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

#ifndef AEXTXML_H
#define AEXTXML_H

#include <qdom.h>
#include "aextension.h"

/**
 * \en
 * Class for work with extensions
 * \_en
 * \ru
 * \_ru
 */
class  ANANAS_EXPORT AExtXML : public AExtension
{
    Q_OBJECT

public:

    AExtXML();
    ~AExtXML();
public:

public slots:
	bool read(const QString &fname);
	bool write(const QString &fname);
	void createNew( const QString &rootnodename );

	bool isRoot();
	bool parent();
	bool newChild( const QString &childname );
	QString nodeName();
	bool setText( const QString &text );
	QString text() const;

	bool setAttr(  const QString &name, const QString &value );
	QString attr(   const QString &name ) const;
	bool removeAttr(  const QString &name );

	bool firstChild();
	bool lastChild();
	bool next();
	bool prev();
	bool childExists();

private:
	QDomDocument xml;
	QDomElement rootnode, current;


};

#endif //AEXTTEXT_H
