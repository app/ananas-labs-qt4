/****************************************************************************
** $Id: mdtree.cpp,v 1.3 2008/12/05 21:11:54 leader Exp $
**
** Code file of the Metadata Tree of Ananas Administrator applications
**
** Created : 20050609
**
** Copyright (C) 2003-2005 Leader InfoTech.  All rights reserved.
** Copyright (C) 2003-2005 Grigory Panov <gr1313 at mail dot ru>, Yoshkar-Ola.
**
** This file is part of the Designer application  of the Ananas
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

#include <qlabel.h>
#include <qapplication.h>
//#include <qpixmap.h>
//#include <qpopupmenu.h>
//#include <qstatusbar.h>
//#include <qlineedit.h>
//#include <qmessagebox.h>
//#include <qvaluelist.h>
//#include <qimage.h>
//#include <qbitmap.h>
//#include <qdragobject.h>
//#include <qheader.h>
//#include <qfont.h>
#include <qcursor.h>
//#include <qlayout.h>

#include "adatabase.h"
#include "mainform.h"
#include "deditpermissions.h"
#include "dselectuser.h"
#include "deditrole.h"
#include "dedituser.h"
#include "dselectrole.h"
//#include "deditcat.h"
//#include "deditireg.h"
//#include "deditareg.h"
//#include "deditjournal.h"
//#include "deditwebform.h"
//#include "deditcolumns.h"
#include "awindowslist.h"

//#include "acfg.h"
#include "acfgrc.h"

#include "mdtree.h"
#include "usersform.h"
#include "rolesform.h"

//extern MainForm *mainform;
//extern QPixmap rcIcon(const char *name);
//extern void set_Icon(QListViewItem *item, const char *name);



aListViewItem::aListViewItem(	ananasListViewItem *parent,
				ananasListViewItem *after,
				aCfg *cfgmd,
				aCfgItem cfgobj,
				const QString &name,
				aUser *usr,
				aRole *rl
				): ananasListViewItem( parent, after, cfgmd, cfgobj, name )
{
        adb = aDatabase::database();
	if(usr)
	{
//		printf("new user\n");
		user = new aUser(usr->db);
		user->Select(usr->sysValue("id").toULongLong());
	}
	else
	{
		user=NULL;
	}
//	user = usr;
	if(rl)
	{
//		printf("new role\n");
		role = new aRole(rl->db);
		role->Select(rl->sysValue("id").toULongLong());
	}
	else
	{
		role=NULL;
	}
	loadTree();
}



aListViewItem::aListViewItem(QListView *parent, aCfg *cfgmd,  aCfgItem cfgobj, const QString &name )
: ananasListViewItem( parent, cfgmd, cfgobj, name )
{
        adb = aDatabase::database();
	role=NULL;
	user=NULL;
	loadTree();
}


aListViewItem::~aListViewItem()
{

	if(role)
	{
//		printf("delete role\n");
		delete role;
		role = NULL;
	}
	if(user)
	{
//		printf("delete user\n");
		delete user;
		user = NULL;
	}
}


QString
aListViewItem::text( int column ) const
{
	return Q3ListViewItem::text( column );
//	if ( obj.isNull() ) return QObject::tr("Unknown object context");
	if ( !id ) return Q3ListViewItem::text( column );
	switch ( column ) {
	case 0: return md->attr( obj, mda_name );
	default:
		return "";
	}
}



int
aListViewItem::compare( Q3ListViewItem *i, int col, bool accending ) const
{
	int id1 = 0, id2 = 0;
	aListViewItem *ai = (aListViewItem *) i;
	id1 = md->order( obj );
	id2 = ai->md->order( ai->obj );
	if ( md == ai->md && md->parent( obj ) == ai->md->parent( ai->obj ) && col ==0 && accending ) {
		if ( id1 < id2 ) return -1;
		if ( id1 == id2 ) return 0;
		return 1;
	}
	return 1;
}



void
aListViewItem::setup()
{
	Q3ListViewItem::setup();
}



/*!
Update metadata tree for the object context.
*/
void
aListViewItem::update()
{
	QString oclass = md->objClass( obj );
	printf("remove !\n");	
	if( oclass == md_user )
	{
		if(user->First())
		{
			setText(0,user->sysValue("login").toString());
		}
	}
	if( oclass == md_role)
	{
		if(role->First())
		{
			setText(0,role->sysValue("name").toString());
		}
		
	}
	if( oclass == md_rl_user)
	{
		
	}
	if( oclass == md_rl_user)
	{
		
	}
	if( oclass == md_rl_user)
	{
		
	}
	if( oclass == md_rl_user)
	{
		
	}

	
}


void
aListViewItem::loadTree()
{
//printf("load tree\n");
	aListViewItem *item, *nextitem;
	QString oclass, ldclass="";
	int i, n;
	aCfgItem cobj;

	// clear tree
//       item = (aListViewItem *) firstChild();
//        while( item ) {
//		nextitem = (aListViewItem *) item->nextSibling();
//		delete item;
//		item = nextitem;
 //       }
	if ( !md ) return;
	if (id) setRenameEnabled(0, true);
	oclass = md->objClass( obj );
//	printf("oclass=`%s'\n",oclass.ascii()); 
	if ( oclass == md_roles )
	{
		loadRoles(this);	
		
	}
	if ( oclass == md_role )
	{	
		cobj = md->find ( mdc_metadata );
//		cobj.setTagName("eqwe");
		new aListViewItem(this, getLastChild(), md, cobj, QObject::tr("Metadata Objects"),NULL,role );
	//	cobj.clear();
		cobj = cobj.cloneNode(false).toElement();
		cobj.setTagName( md_rl_users ); // change type to users 
//		cobj.setTagName("eqwe");
					
		aUser *usr = new aUser(adb);
		new aListViewItem(this, getLastChild(), md, cobj, QObject::tr("Users"),usr,role);
	}
	if ( oclass == md_users )
	{
		loadUsers(this);
	}
	if ( oclass == md_user )
	{
		loadUsrRoles(this);
	}
	if ( oclass == md_metadata )
	{
		cobj = md->find ( mdc_metadata );
		setPixmap(0, QPixmap(":/images/metadata.png"));
	//	setOpen(true);
		cobj = md->find ( obj, md_catalogues, 0 );
		new aListViewItem( this, getLastChild(), md, cobj, QObject::tr("Catalogues"),NULL,role );
		cobj = md->find ( obj, md_documents, 0 );
		new aListViewItem( this, getLastChild(), md, cobj, QObject::tr("Documents"),NULL,role );
		cobj = md->find ( obj, md_reports, 0 );
		new aListViewItem( this, getLastChild(), md, cobj, QObject::tr("Reports"),NULL,role );
		cobj = md->find ( obj, md_journals, 0 );
		new aListViewItem( this, getLastChild(), md, cobj, QObject::tr("Journals"),NULL,role );
		cobj = md->find ( obj, md_iregisters, 0 );
		new aListViewItem( this, getLastChild(), md, cobj, QObject::tr("Information registers"),NULL,role );
		cobj = md->find ( obj, md_aregisters, 0 );
		new aListViewItem( this, getLastChild(), md, cobj, QObject::tr("Accumulation registers"),NULL,role );
		
	}
	if ( oclass == md_catalogues ){
		QPixmap  pixmap = QPixmap(":/images/cat_g.png");
		if(pixmap.isNull()) printf("image not loaded\n");
		setPixmap(0, pixmap );
		ldclass = md_catalogue;
	}
	if ( oclass == md_documents ){
		setPixmap(0, QPixmap(":/images/doc_g.png"));
		ldclass = md_document;
	}
	if ( oclass == md_reports ){
		setPixmap(0, QPixmap(":/images/report_g.png"));
		ldclass = md_report;
	}
	if ( oclass == md_journals ){
		setPixmap(0, QPixmap(":/images/journ_g.png"));
		ldclass = md_journal;
	}
	if ( oclass == md_iregisters ){
		setPixmap(0, QPixmap(":/images/reg_g.png"));
		ldclass = md_iregister;
	}
	if ( oclass == md_aregisters ){
		setPixmap(0, QPixmap(":/images/regs_g.png"));
		ldclass = md_aregister;
	}
	if ( oclass == md_rl_users ){
		setPixmap(0, QPixmap(":/images/users.png"));
		loadRlUsers(this);
	//	ldclass = md_rl_user;
	}
	if ( oclass == md_users ){
		setPixmap(0, QPixmap(":/images/users.png"));
	//	ldclass = md_rl_user;
	}
	if ( oclass == md_roles ){
		setPixmap(0, QPixmap(":/images/roles.png"));
	//	ldclass = md_role;
	}
	if ( oclass == md_user || oclass == md_rl_user){
		setPixmap(0, QPixmap(":/images/user.png"));
	//	ldclass = md_document;
	}
	if ( oclass == md_role || oclass == md_usr_role){
		setPixmap(0, QPixmap(":/images/role.png"));
	//	ldclass = md_document;
	}
	if ( !ldclass.isEmpty() ) {
		
		if(ldclass==md_rl_user)
		{
		}
//		printf("%s\n",(const char *) ldclass.local8Bit() );
		n = md->count( obj, ldclass );
//		printf(" objclass=%s ldclass = %s count = %d\n", (const char *) oclass, (const char *) ldclass, n);
		for ( i = 0; i<n; i++ ) {
			cobj = md->find( obj, ldclass, i );
			if ( !cobj.isNull() ) {
//				printf("%d ok\n", i);
//				new aListViewItem( this, md, md->find( obj, ldclass, i ), QString::null );
				new aListViewItem( this, getLastChild(), md,  cobj, QString::null, NULL ,role );
			}
		}
	}


// elements
}


void
aListViewItem::loadUsrRoles ( aListViewItem *parent )
{
	QValueList<aRole*> list = user->getRoles(true);
	aCfgItem cobj;
	//listId.clear();
//	QValueList<aRole*> list = user->getRoles(false);
	QValueList<aRole*>::iterator it;
	for ( it = list.begin(); it != list.end(); ++it )
	{
		cobj = md->find ( mdc_metadata );
		cobj = cobj.cloneNode(false).toElement();
		cobj.setTagName( md_usr_role ); // change type to users 
		new aListViewItem(parent, getLastChild(), md, cobj, (*it)->sysValue("name").toString(),user,(*it));
//		listBox1->insertItem( (*it)->sysValue("name").toString() );
		delete (*it);
	}
}



void
aListViewItem::loadRoles (aListViewItem *parent)
{
	aCfgItem cobj;
	if( adb )
	{
		role = new aRole( adb );
		
		cobj = md->find ( mdc_metadata );
		cobj = cobj.cloneNode(false).toElement();
		cobj.setTagName( md_role ); // change type to users 
		role->Select();
		if(role->First())
		{
//			printf("role first!!!\n");
			do
			{
//				printf("role next\n");
				new aListViewItem(parent, getLastChild(), md, cobj, role->sysValue("name").toString(),NULL,role);
			}while(role->Next());
		}
	}
}



void 
aListViewItem::loadUsers (aListViewItem *parent)
{
	aCfgItem cobj;
		if( adb )
		{
			user = new aUser(adb);
			
			cobj = md->find ( mdc_metadata );
			cobj = cobj.cloneNode(false).toElement();
			cobj.setTagName( md_user ); // change type to role 
			user->Select();
			if(user->First())
			{
//				printf("user first!!!\n");
				do
				{
//					printf("user next\n");
				 	new aListViewItem(parent, getLastChild(), md, cobj, user->sysValue("login").toString(),user);
					
				}while(user->Next());
			}
		}
}



void 
aListViewItem::loadRlUsers ( aListViewItem *parent ) 
{
//printf("load users!\n");	
//	aUser *usr = new aUser(adb);
	if(!user)
	{
		printf("user=NULL\n");
		return;
	}
		
	//user->Select();
	aCfgItem l;
	l = md->find( mdc_metadata );
	l = l.cloneNode(false).toElement();
	l.setTagName(md_rl_user);
	user->Select();
	if(user->First())
	{
//		printf("user first\n");
		do
		{
			if(role->hasUser(user->sysValue("id").toULongLong()))
			{
				new aListViewItem(parent, getLastChild(),md, l, user->sysValue("login").toString(),user,role);
			}
//			printf("user next\n");
		}while (user->Next()); 
	}
//	printf("end\n");
}



void 
aListViewItem::deleteRole ()
{
	printf("delete role!\n");	
	role->Delete();
//	removeItem(this);
}



void 
aListViewItem::deleteUser ()
{
	printf("delete user!\n");	
	user->Delete();
//	removeItem(this);
}



void 
aListViewItem::remove ()
{
	QString oclass = md->objClass( obj );
	printf("remove !\n");	
	if( oclass == md_usr_role )
	{
		user->delRole(role->sysValue("id").toULongLong());
	}
	if( oclass == md_rl_user)
	{
		role->delUser(user->sysValue("id").toULongLong());	
	}
//	removeItem(this);
}


/*
*edit selected object
*/



void
aListViewItem::editRole( bool isNew)
{
	QString oclass = md->objClass( obj );
	if(oclass == md_role || oclass == md_roles )
	{
		dEditRole *e = new dEditRole( qApp->mainWidget());
	//	wl->insert( objid, e );
		editor = e;
	//	QObject::connect( mainform, SIGNAL( tosave() ), editor, SLOT( updateMD() ) );
		if(isNew)
		{
			printf("new role\n");
			e->setData("New Role");
		}
		else
		{
			printf("edit role `%s'\n",role->sysValue("name").toString().ascii());
			e->setData(role->sysValue("name").toString());
		}
		if(e->exec() == QDialog::Accepted)
		{
			if(isNew)
			{
				role->New(e->getData());
				aCfgItem l;
				l = md->find( mdc_metadata );
				l = l.cloneNode(false).toElement();
				l.setTagName(md_role);
				new aListViewItem(this,getLastChild(),md,l,role->sysValue("name").toString(),NULL,role);
				role->Select();
			}
			else
			{
				role->setSysValue("name",e->getData());
				role->Update();
				setText(0,e->getData());
			}
		}
		delete e;
	}
}



void 
aListViewItem::editPermission()
{
	QString oclass = md->objClass( obj );
	dEditPermissions *e = new dEditPermissions( qApp->mainWidget());
//	wl->insert( objid, e );
	editor = e;
	
        int role_id, obj_id;
        role_id = role->sysValue ( "id" ).toULongLong();
        obj_id = md->id( obj );
        printf("role = %i, obj=%i id=%i\n", role_id, obj_id, id );
        int perm = adb->rolePermission( role_id, obj_id );
        
        e->setData ( perm );
        if ( e->exec() == QDialog::Accepted )
        {
                perm = e->getData ();
                adb->setRolePermission( role_id, obj_id, perm );
        }
        delete e;
	
//	QObject::connect( mainform, SIGNAL( tosave() ), editor, SLOT( updateMD() ) );
/*	QMap<aPermission,bool> m;
	m [perm_read] = true; 
	m [perm_execute] = false;
	m [perm_turn_on] = false; 
	m [perm_turn_off] = false; 
	m [perm_create] = false; 
	m [perm_delete] = false; 
	
	e->setData( m );
	if(e->exec() == QDialog::Accepted)
	{
		e->getData( m );
		//TODO: add logic here!
	}
	delete e;
*/
//	mainform->addTab(++mainform->lastTabId,e->name());
//	e->parentWidget()->setGeometry(0,0,e->parentWidget()->frameSize().width(),
//	e->parentWidget()->frameSize().height());
	
//	printf("oclass = %s\n",( const char *) oclass );
	//if ( oclass == md_documents ) newDocument();
//	if ( oclass == md_columns ) newColumn();
}



void 
aListViewItem::editUser(bool isNew)
{
	QString oclass = md->objClass( obj );
	if ( oclass == md_users || oclass == md_user)
	{
		
	    dEditUser *e = new dEditUser( qApp->mainWidget() );
	    editor = e;
	    e->setData( user, isNew );
	    if(e->exec()==QDialog::Accepted)
	    {
			if(isNew)
			{
				aCfgItem l;
				l = md->find( mdc_metadata );
				l = l.cloneNode(false).toElement();
				l.setTagName(md_user);
				new aListViewItem(this,getLastChild(),md,l,user->sysValue("login").toString(),user,NULL);
				user->Select();
			}
			else
			{
				update();
			}
	    }
	    delete e;
		
	}
}



void 
aListViewItem::addUser()
{
	QString oclass = md->objClass( obj );
	if ( oclass == md_rl_users)
	{
		
	    dSelectUser *e = new dSelectUser( qApp->mainWidget() );
	    editor = e;
	    e->setData( role );
	    if(e->exec() == QDialog::Accepted)
	    {
		aCfgItem l;
		l = md->find( mdc_metadata );
		l = l.cloneNode(false).toElement();
		l.setTagName(md_rl_user);
		aUser *usr = new aUser(e->getData(),role->db); 
		new aListViewItem(this,getLastChild(),md,l,usr->sysValue("login").toString(),usr,role);
		delete usr;
		usr = NULL;
	    }
	    delete e;
		
	}
}



void 
aListViewItem::addRole()
{
	QString oclass = md->objClass( obj );
	if ( oclass == md_user)
	{
		
	    dSelectRole *e = new dSelectRole( qApp->mainWidget() );
	    editor = e;
	    e->setData( user );
	    if(e->exec() == QDialog::Accepted)
	    {
		aCfgItem l;
		l = md->find( mdc_metadata );
		l = l.cloneNode(false).toElement();
		l.setTagName(md_usr_role);
		aRole *rl = new aRole(e->getData(),user->db); 
		new aListViewItem(this,getLastChild(),md,l,rl->sysValue("name").toString(),user,rl);
		delete rl;
		rl = NULL;
	    }
	    delete e;
		
	}
}



aMetadataTreeView::aMetadataTreeView(  QWidget *parent, aCfg *cfgmd )
:ananasTreeView( parent, cfgmd )
{
	aListViewItem *conf;

	if ( !md ) return;
	
//	aCfgItem it = md->find( mdc_metadata );
//	it = it.cloneNode(false).toElement();
//	it.setTagName(md_roles);
	
//	conf = new aListViewItem( this, md, it, tr(QString("Roles")));
//	conf->setOpen( TRUE );
	connect( this, SIGNAL( contextMenuRequested( Q3ListViewItem*, const QPoint&, int) ), this, SLOT(ContextMenu() ) );
//	connect( this, SIGNAL( returnPressed( QListViewItem* ) ), this, SLOT( itemEdit() ) );
//	connect( this, SIGNAL( doubleClicked( QListViewItem* ) ), this, SLOT( itemEdit() ) );
//	connect( this, SIGNAL( collapsed( QListViewItem* ) ), this, SLOT( on_collapsed( QListViewItem* ) ) );
}


aMetadataTreeView::aMetadataTreeView( UsersForm  *parent, aCfg *cfgmd )
:ananasTreeView( parent, cfgmd )
{
	aListViewItem *conf;

	if ( !md ) return;
	
	aCfgItem it = md->find( mdc_metadata );
	it = it.cloneNode(false).toElement();
	it.setTagName(md_users);
	
	conf = new aListViewItem( this, md, it, tr(QString("Users")));
	conf->setOpen( TRUE );
	connect( this, SIGNAL( contextMenuRequested( Q3ListViewItem*, const QPoint&, int) ), this, SLOT(ContextMenu() ) );
//	connect( this, SIGNAL( doubleClicked( QListViewItem* ) ), this, SLOT( itemEdit() ) );
	connect( this, SIGNAL( collapsed( QListViewItem* ) ), this, SLOT( on_collapsed( QListViewItem* ) ) );
}


aMetadataTreeView::aMetadataTreeView( RolesForm *parent, aCfg *cfgmd )
:ananasTreeView( parent, cfgmd )
{
	aListViewItem *conf;

	if ( !md ) return;
	
	aCfgItem it = md->find( mdc_metadata );
	it = it.cloneNode(false).toElement();
	it.setTagName(md_roles);
	
	conf = new aListViewItem( this, md, it, tr(QString("Roles")));
	conf->setOpen( TRUE );
	connect( this, SIGNAL( contextMenuRequested( Q3ListViewItem*, const QPoint&, int) ), this, SLOT(ContextMenu() ) );
//	connect( this, SIGNAL( returnPressed( QListViewItem* ) ), this, SLOT( itemEdit() ) );
//	connect( this, SIGNAL( doubleClicked( QListViewItem* ) ), this, SLOT( itemEdit() ) );
	connect( this, SIGNAL( collapsed( Q3ListViewItem* ) ), this, SLOT( on_collapsed( Q3ListViewItem* ) ) );
}



void 
aMetadataTreeView::on_collapsed( Q3ListViewItem * item )
{
	aListViewItem *i = (aListViewItem *) item;
	if ( i )
	{
		QString oclass = md->objClass( i->obj );
		if ( oclass == md_roles ) setOpen( i, true );
	}
}


void
aMetadataTreeView::ContextMenu()
{

	QPopupMenu *m=new QPopupMenu( this, "PopupMenu" );
	Q_CHECK_PTR(m);

/*    	QLabel *caption = new QLabel( "<font color=darkblue><u><b>"
		"Context Menu</b></u></font>", this );
		caption->setAlignment( Qt::AlignCenter );
    	m->insertItem( caption );
    	m->insertItem( "&New",  this, SLOT( itemNew() ), CTRL+Key_N );
		m->insertItem( "&Rename", this, SLOT( itemRename() ), CTRL+Key_O);
		m->insertItem( "&Edit",  this, SLOT( itemEdit() ), CTRL+Key_N );
    	m->insertItem( "&Delete", this, SLOT( itemDelete() ), CTRL+Key_O );*/
	
	aListViewItem *i = (aListViewItem *) selectedItem();
	if(!i) return;
		
	QLabel *caption = new QLabel( tr("<font color=darkblue><u><b>" "Context Menu</b></u></font>"), this );
	caption->setAlignment( Qt::AlignCenter );
//	m->insertItem( caption );
	
	QString oclass = md->objClass(i->obj);
	printf("popup oclass==`%s'\n",oclass.ascii());
	
	if(oclass == md_user)
	{
	
		m->insertItem( tr("&Add Role"),  this, SLOT( addRole() ), Qt::CTRL+Qt::Key_A );
	}
	if(oclass == md_roles)
	{
		m->insertItem( tr("&New Role"),  this, SLOT( newRole() ), Qt::CTRL+Qt::Key_N );
	}
	if( oclass == md_role)
	{
		
		m->insertItem( tr("&Edit Role"),  this, SLOT( editRole() ), Qt::CTRL+Qt::Key_E );
		m->insertItem( tr("&Delete Role"),  this, SLOT( delRole() ), Qt::CTRL+Qt::Key_D );
	}
	
	if(oclass==md_rl_users)
	{
		m->insertItem( tr("&Add User"),  this, SLOT( addUser() ), Qt::CTRL+Qt::Key_A );
	}
	if(oclass==md_users )
	{
		m->insertItem( tr("&New User"),  this, SLOT( newUser() ), Qt::CTRL+Qt::Key_N );
	}
	
	if(oclass==md_user)
	{
		m->insertItem( tr("&Edit User "), this, SLOT( editUser() ), Qt::CTRL+Qt::Key_E );
		m->insertItem( tr("&Delete User"), this, SLOT( delUser() ), Qt::CTRL+Qt::Key_D );
	}
	if(oclass==md_usr_role || oclass==md_rl_user)
	{
		m->insertItem( tr("&Remove"), this, SLOT( itemDelete() ), Qt::CTRL+Qt::Key_R );
	}
	if(	oclass	  == md_catalogue 
		|| oclass == md_document
		|| oclass == md_journal
		|| oclass == md_aregister
		|| oclass == md_iregister
		|| oclass == md_report )
	{
		m->insertItem( tr("&Set permissions"), this, SLOT( itemSetPermission() ), Qt::CTRL+Qt::Key_S );
	}

	ContextMenuAdd(m);
//	m->insertItem( tr("&New"),  this, SLOT( itemNew() ), CTRL+Key_N );
	m->exec( QCursor::pos() );
	delete m;
}


/*void
aMetadataTreeView::itemRename()
{
	renameItem();
}
*/
void
aMetadataTreeView::newUser()
{
	aListViewItem *i = (aListViewItem *) selectedItem();
	if ( i )
	{
		 i->editUser(true);
	}
}

void
aMetadataTreeView::newRole()
{
	aListViewItem *i = (aListViewItem *) selectedItem();
	if ( i )
	{
		i->editRole(true);
	}
}

void
aMetadataTreeView::editUser()
{
	aListViewItem *i = (aListViewItem *) selectedItem();
	if ( i )
	{
		i->editUser(false);
//		emit (updateRoles());
	}
}

void
aMetadataTreeView::editRole()
{
	aListViewItem *i = (aListViewItem *) selectedItem();
	if ( i )
	{
		i->editRole(false);
		
		//emit(updateUsers());
	}
}

void
aMetadataTreeView::delRole()
{
	aListViewItem *i = (aListViewItem *) selectedItem();
	if ( i )
	{
		i->deleteRole();
		delete i;
		//emit(updateUsers());
	}
}
void
aMetadataTreeView::delUser()
{
	aListViewItem *i = (aListViewItem *) selectedItem();
	if ( i )
	{
		i->deleteUser();
		delete i;
		//emit (updateRoles());
	}
}

void
aMetadataTreeView::addRole()
{
	aListViewItem *i = (aListViewItem *) selectedItem();
	if ( i )
	{
		i->addRole();
		//i->update();
		//emit(updateUsers());
		//emit (updateRoles());
	}
}
void
aMetadataTreeView::addUser()
{
	aListViewItem *i = (aListViewItem *) selectedItem();
	if ( i )
	{
		i->addUser();
		//i->update();
		//emit (updateRoles());
	}
}

void
aMetadataTreeView::itemSetPermission()
{
	aListViewItem *i = (aListViewItem *) selectedItem();
	if ( i )
	{
		//if ( i->id == 0 )
			 i->editPermission();
	}
}


void
aMetadataTreeView::itemDelete()
{
	aListViewItem *i = (aListViewItem *) selectedItem();
	if ( i )
	{
		i->remove();
		delete i;
	}
//	deleteItem();
}



