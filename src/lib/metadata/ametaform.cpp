/****************************************************************************
** $Id: ametaform.cpp,v 1.3 2008/12/24 20:06:51 leader Exp $
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
#include "ametaform.h"


/*!
 * \class AMetaField
 *
 */
AMetaForm::AMetaForm( AMetaObject *parent )
:AMetaObject("MetaForm", "", parent )
{
    setSourceCode(
    "function on_formstart()\n{\n}\n\n"
    "function on_formstop()\n{\n}\n\n"
    "function on_button( name )\n{\n}\n\n"
    "function on_valuechanged( name, value )\n{\n}\n\n"
    "function on_tabupdate( row, col, tablename )\n{\n}\n\n"
    "function on_tablerow( tablename )\n{\n}\n\n"
    "function on_event( source, data )\n{\n}\n\n"
    "function on_tabrowselected( tablename, uid )\n{\n}\n\n"
    "function on_conduct()\n{\n}\n\n"
    );
    setDefaultMode( 0 );
}


QString 
AMetaForm::sourceCode()
{
    return attr("sourcecode").toString();
}


void 
AMetaForm::setSourceCode( const QString &src )
{
    setAttr( "sourcecode", src );
}


int  
AMetaForm::defaultMode()
{
    return attr("defaultmode").toInt();
}


void 
AMetaForm::setDefaultMode( int mode )
{
    setAttr( "defaultmode", mode );
}


QByteArray 
AMetaForm::dialogue()
{
    return attr("dialogue").toByteArray();
}


void 
AMetaForm::setDialogue( const QByteArray &d )
{
    setAttr( "dialogue", d );
}


/*!
 * \class AMetaForms
 *
 */
AMetaForms::AMetaForms()
:AMetaGroup("MetaForms")
{

}

