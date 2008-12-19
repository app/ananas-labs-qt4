/****************************************************************************
** $Id: atemplate.h,v 1.1 2008/11/05 21:16:29 leader Exp $
**
** Report metadata object header file of
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

#ifndef ATEMPLATE_H
#define ATEMPLATE_H

#include "ananasglobal.h"
#include "itemplate.h"

#define token_open "<#"
#define token_close "#>"

#include <qobject.h>
#include <q3dict.h>
#include <qstringlist.h>

/**
 * \en
 * Class for work with pattern in HTML format
 * \_en
 * \ru
 * 	\brief Класс для работы с шаблонами в HTML формате.
 * 	Наследует iTemplate.
 *
 * Приведенный пример показывает создание таблицы размеров 10х10.
 *	\code
 *	function printreport()
 *	{
 *		p = new Report("Report1");
 *		p.setTable("report.tpl");
 *		p.setValue("title","Заголовок таблицы :"+Value("eHeader") + Value("fNDS"));
 *		p.exec("main");
 *		for ( i = 1; i<10; i++)
 *		{
 *			p.setValue("value", String(i) );
 *			p.exec("trow");
 *			for ( j=1; j<10; j++)
 *			{
 *				p.setValue("value", String(i)+":"+String(j) );
 *				p.exec("col");
 *			}
 *			p.exec("endrow");
 *		}
 *		p.exec("endmain");
 *		p.show();
 *		delete p;
 *	}
 *	\endcode
 *	Шаблон:
 *	\code
<#section main#>
<html>
<header>
<title>Ananas HTTP Server</title>
</header>
<body bgcolor=gray>
<img src="a-system.png"><#title#>
<table border=1 cellspacing=0 cellpadding=0 spacing=0>
<#endsection#>

<#section endmain#>
</table>
</body></html>
<#endsection#>

<#section trow#><tr><td width=100 bgcolor=lightblue><#value#>)</td><#endsection#>

<#section endrow#></tr>
<#endsection#>

<#section col#><td align=center><#value#></td><#endsection#>
 *	\endcode
 * \_ru
 */
class  ANANAS_EXPORT aTemplate : public iTemplate
{
    Q_OBJECT
public:

    aTemplate();
    virtual ~aTemplate();

    bool open( const QString &fname );
    void close();
    void clear(){ buf="";};

    QString getValue( const QString &name );
    void setValue( const QString &name, const QString &value );
    QString exec( const QString &sname );
    QString result(){ return buf;};
    bool save( const QString & fname );
    void cleanUpTags();
	virtual void setDir(const QString &dir);
	virtual QString getDir();

protected:
		QString templateDir;

private:
    QString buf;
    QString tpl;
    Q3Dict<QString> values;

    void freeValues();
};

#endif //ATEMPLATE_H
