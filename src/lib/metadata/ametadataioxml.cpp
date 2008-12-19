/****************************************************************************
** $Id: ametadataioxml.cpp,v 1.2 2008/12/13 22:19:15 leader Exp $
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

#include <qdatetime.h>
#include <qobject.h>
#include <qfile.h>
#include <qstringlist.h>
#include <QTextStream>
#include <QDomDocument>

#include "ametadataioxml.h"


/*!
Metadata tags.
*/
#define md_root 		"ananas_configuration"
#define md_interface		"interface"

#define md_info 		"info"
#define md_info_name 		"name"
#define md_info_author 		"author"
#define md_info_date 		"date"
#define md_info_mdate 		"mdate"
#define md_info_remark 		"remark"
#define md_info_lastid 		"lastid"
//#define md_info_dir 		"dir"

#define md_metadata 		"metadata"	/**< Metadata tag */
#define md_globals		"globals"
#define md_languages		"languages"
#define md_language		"language"
#define md_aliases		"aliases"
#define md_alias		"alias"
#define md_roles		"roles"
#define md_role 		"role"
#define md_description		"description"
#define md_sourcecode		"sourcecode"
#define md_servermodule	        "server_module"
#define md_clientmodule		"client_module"
#define md_formsource		"form_source"
#define md_menutext		"menutext"
#define md_argument		"argument"

#define md_documents		"documents"
#define md_document		"document"

#define md_catalogues		"catalogues"
#define md_catalogue		"catalogue"
#define md_element		"element"
#define md_group		"group"

#define md_string_view		"string_view"
#define md_svfunction		"svfunction"

#define md_registers		"registers"
#define md_iregisters		"iregisters"
#define md_iregister		"iregister"
#define md_aregisters		"aregisters"
#define md_aregister		"aregister"

#define md_reports		"reports"
#define md_report		"report"

#define md_journals		"journals"
#define md_journal		"journal"
#define md_columns		"columns"
#define md_column		"column"
#define md_used_doc		"used_doc"
#define md_fieldid		"fieldid"

#define md_field		"field"
#define md_forms		"forms"
#define md_form			"form"
#define md_defaultmod		"defaultmod"
#define md_webforms		"webforms"
#define md_webform		"webform"
#define md_formdesign		"dialogform"

#define md_tables		"tables"
#define md_table		"table"
#define md_header		"header"

#define md_resources		"resources"
#define md_dimensions		"dimensions"
#define md_information		"information"

/*!
Action metadata object items.
*/
#define md_actions		"actions"
#define md_actiongroup		"actiongroup"
#define md_action		"action"
#define md_objectid		"objectid"
#define md_formid		"formid"
#define md_actiontype		"actiontype"
#define md_active_picture	"active_picture"
#define md_passive_picture	"passive_picture"

/*!
Interface metadata object items.
*/
#define md_interface		"interface"
#define md_toolbars		"toolbars"
#define md_toolbar		"toolbar"
#define md_popupmenus		"popupmenus"
#define md_popupmenu		"popupmenu"
#define md_mainmenu		"mainmenu"
#define md_submenu		"submenu"
#define md_command		"command"
#define md_comaction		"comaction"
#define md_separator		"separator"
#define md_key			"key"
#define md_menutext		"menutext"
//key modifiers
#define md_km_ctrl		"Ctrl+"
#define md_km_alt		"Alt+"
#define md_km_shift		"Shift+"

#define md_image_collection	"image_collection"
#define md_image		"image"


/*!
Metadata attributes.
*/
#define mda_name		"name"
#define mda_id		        "id"
#define mda_type		"type"
#define mda_readonly		"readonly"
#define mda_lang		"lang"
#define mda_nz		        "nz"
#define mda_sort		"sort"
#define mda_no_unconduct	"no_unconduct"
#define mda_plus		"plus"
#define mda_sum			"sum"
#define mda_tag			"tag"
#define mda_trfile		"trfile"
#define mda_length		"length"
#define mda_format		"format"
#define mda_stdf		"stdf"
#define mda_rights		"rights"

/*!
Actions types
*/

#define md_action_new		0
#define md_action_edit		1
#define md_action_view		2
#define md_action_execute	3

/*!
 *	Default forms types.
 *	If bit with some number set in 1 form can use for this action.
 */

#define md_form_nouse		0
#define md_form_new		1
#define md_form_view		2
#define md_form_edit		3
#define md_form_select		4


/*!
 *	Catalogue forms types.
 */

#define md_form_elem_list	0
#define md_form_elem		1
#define md_form_group		2

/*!
System object id
*/

#define	md_systemjournal	1
#define md_aregarhive		2

//class aCfg;
//extern aCfg cfg;

#define ERR_Code int

#define md_user_roles		"userroles"
#define md_users		"users"
#define md_user			"user"
#define md_usr_role		"u_role"
#define md_roles		"roles"
#define md_role			"role"
#define md_rl_users		"r_users"
#define md_rl_user		"r_user"
#define md_right_roles		"rroles"

#define perm_read		0
#define perm_execute		1
#define perm_create		2
#define perm_delete		3
#define perm_turn_on		4
#define perm_turn_off		5
#define perm_admin		6

/*!
 * \class AMetaDataIOXML
 *
 */
AMetaDataIOXML::AMetaDataIOXML()
:AMetaDataIO()
{
}


int
AMetaDataIOXML::read(  const QString &name, AMetaData *md )
{
    QFile file( name );
    QByteArray buf;
    QString err;
    int errLine = 0, errColumn = 0;
//    QDomDocument xml;
    QDomNode cur, sub;
    QDomElement node, rootnode, cfginfo, mdelement, iface, actions;
    long id;

    if ( !md ) md = AMetaData::metadata();
    if ( !file.open( QIODevice::ReadOnly ) ) return 1;
    buf = file.readAll();
    file.close();
    xml.setContent( QString("") );
    if ( !xml.setContent( buf, false, &err, &errLine, &errColumn ) ) {
	emit message( 2, QObject::tr(
		     "Error read configuration line:%1 col:%2 %3"
		     ).arg( errLine ).arg( errColumn ).arg( err ));
	return 1;
    }
    if ( !xml.isDocument() ) {
//            aLog::print(aLog::Error, tr("Bad configuration"));
        return 1;
    }
    md->clear();
    rootnode = xml.documentElement();

    node = rootnode.namedItem( md->info()->className() ).toElement();
    XMLToAMetaObject( node, md->info() );
    node = rootnode.namedItem( md->global()->className() ).toElement();
    XMLToAMetaObject( node, md->global() );

    mdelement = rootnode.namedItem( md_metadata ).toElement();
    iface = rootnode.namedItem( md_interface ).toElement();
    actions = rootnode.namedItem( md_actions ).toElement();
    return 0;
}


int
AMetaDataIOXML::write(  const QString &name, AMetaData *md )
{
    QFile file( name );

    if ( !md ) md = AMetaData::metadata();
//    QDomDocument xml("ananas4_configuration");
    QDomNode cur, sub;
    QDomElement node, rootnode, cfginfo, mdelement, iface, actions;

    xml.setContent(QString("<?xml version = '1.0' encoding = 'UTF-8'?>\n"));
    xml.appendChild( xml.implementation().createDocumentType("ananas_configuration",
                     QString::null,"ananas-cfg.dtd"));
    rootnode = xml.createElement( md_root );
    xml.appendChild( rootnode );
    rootnode = xml.documentElement();

    rootnode.appendChild( AMetaObjectToXML( md->info() ) );
    rootnode.appendChild( AMetaObjectToXML( md->global() ) );


    node     = xml.createElement( md_info );
    rootnode.appendChild( node );


    node     = xml.createElement( md_interface );
    rootnode.appendChild( node );

    node     = xml.createElement( md_metadata );
    rootnode.appendChild( node );

    node     = xml.createElement( md_actions );
    rootnode.appendChild( node );


    if ( !file.open( QIODevice::WriteOnly ) ) return 1;
    QTextStream ts( &file );
    xml.save( ts, 4 );
    file.close();
    return 0;
}

#include <QtDebug>

void 
AMetaDataIOXML::XMLToAMetaObject( QDomElement e, AMetaObject *o )
{
    int i;
    QDomElement se;
    QDomNode cur;
    QString s, t;
    int id;

	cur = e.firstChild();
	while (!cur.isNull()) {
            se = cur.toElement();
            if ( se.hasAttribute( mda_id ) ) {
                id = 0;
                id = se.attribute( mda_id ).toInt();
                o->setId( id );
            }
            s = se.text();
// qDebug() << se.tagName() << ":" << s;
            if ( !s.isNull() ) o->setText( se.tagName(), s );
            cur = cur.nextSibling();
	}

}



QDomElement 
AMetaDataIOXML::AMetaObjectToXML( AMetaObject *o )
{
    int i;
    QDomElement e, se;

    e = xml.createElement( o->className() );
    for ( i=0; i< o->textCount(); i++ ){
       se = xml.createElement( o->textName( i ) );
       se.appendChild( xml.createTextNode( o->text( i ) ) );
       e.appendChild( se );
    }
    return e;
}


QString 
AMetaDataIOXML::text()
{
    return xml.toString( 4 );
}
