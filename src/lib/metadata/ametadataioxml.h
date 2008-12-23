/****************************************************************************
** $Id: ametadataioxml.h,v 1.4 2008/12/20 21:17:49 leader Exp $
**
** Header file of the Ananas configuration objects of Ananas
** Designer and Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2008 Grazhdankin Valeriy.  All rights reserved.
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

#ifndef AMETADATAOIXML_H
#define AMETADATAIOXML_H
#include "ametadataio.h"
#include <QDomDocument>
#include <QDomElement>

class AMetaDataIOXML: public AMetaDataIO
{
Q_OBJECT
public:
    AMetaDataIOXML();
    int read(const QString &name, AMetaData *md = 0);
    int write( const QString &name, AMetaData *md = 0);
    QString text();
private:
    void XMLToAMetaObject( QDomElement e, AMetaObject *o );
    QDomElement AMetaObjectToXML( AMetaObject *o );
    QDomDocument xml;
};

#endif
