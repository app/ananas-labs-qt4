/****************************************************************************
** $Id: acalctemplate.cpp,v 1.2 2008/11/08 20:16:36 leader Exp $
**
** Report metadata object header file of
** Ananas application library
**
** Created : 20050202
**
** Copyright (C) 2003 by HaPK, email : theHaPK@yandex.ru
** Copyright (C) 2003-2006 Grigory Panov <gr1313 at mail.ru>, Yoshkar-Ola.
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

#include "acalctemplate.h"
#include "alog.h"

#include <qdom.h>
#include <qregexp.h>


aCalcTemplate::aCalcTemplate() : aOOTemplate()
{

}

aCalcTemplate::~aCalcTemplate()
{
}


/**
 *	\~english
 *	Return true, if node contain tag
 *	\~russian
 *	Возвращает истину, когда текст ноды содержит тег с заданным именем.
 *	\~
 *	\param node - \~english context for searching \~russian узел, с которого осуществляется поиск. \~
 *	\param sname - \~english tag name \~russian имя тега для поиска \~
 *	\param params - \~english true, if find simple tag and false, if section
 *			\~russian true, если ищется обычный тег и false, если ищется тег секции \~
 */
bool
aCalcTemplate::getNodeTags(QDomNode node, const QString &tagname, bool params )
{
  	if(node.isText())
	{
		QString str = node.nodeValue();
		QRegExp re;
	//	printf("n->text=%s\n",str.ascii());
		if(params)
		{
			re.setPattern(QString("%1.*%2").arg(open_token).arg(close_token));
		}
		else
		{
			re.setPattern(QString("%1.*%2").arg(open_token_section).arg(close_token_section));
		}
		re.setMinimal(true);
		int pos = re.search(str,0);

		while(pos != -1)
		{
	//		printf("find string =%s\n",str.mid(pos+2, re.matchedLength()-4).ascii());
			if(tagname == str.mid(pos+2, re.matchedLength()-4))
			{
	//			printf(">>>>>>>>>ok!\n");
				return true;
			}
			pos+= re.matchedLength();
			pos = re.search(str,pos);
		}

	}
 return false;
}



QDomNode
aCalcTemplate::getCellNode(QDomNode node)
{
	while(!node.parentNode().isNull())
	{
		QDomNode parent = node.parentNode();
		if(parent.nodeName() == "table:table-cell")
		{
			return parent;
		}
		else
		{
			return getCellNode(parent);
		}
	}
}

 /**
 *	\~english
 *	Addes to tag value of parametr \a tagName
 *	\~russian
 *	Добавляет к тегу значение параметра \a tagName. После вызова этой функции тег не исчезает,
 *	и к нему можно добавить еще значения, которые добавятся к концу текста, содержащего тег.
 *	\~
 *	\param node - \~english context \~russian узел к которому добавляется значение \~
 *	\param sname - \~english tag name \~russian имя тега \~
 */
void
aCalcTemplate::insertTagsValues(QDomNode node, const QString &tagName)
{
	QDomNode n = node;
	QString val = getValue(tagName);
	bool ok;
	val.toDouble(&ok);
	if(ok)
	{
		aLog::print(aLog::Debug, QString("aCalcTemplate  append  double value %1 to tag").arg(val));
		QDomNode cellNode = getCellNode(n);
		if(!cellNode.isNull())
		{
			cellNode.toElement().setAttribute("office:value-type", "float");
			cellNode.toElement().setAttribute("office:value", val);
		}
		else
		{
			aLog::print(aLog::Error, "aCalcTemplate cell not found\n");
		}
	}
	else
	{
		aLog::print(aLog::Debug, "aCalcTemplate append string value to tag\n");
	}
	n.setNodeValue(n.nodeValue()+getValue(tagName));
}


