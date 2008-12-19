/****************************************************************************
** $Id: wjournal.cpp,v 1.2 2008/11/08 20:16:36 leader Exp $
**
** Code file of the journal plugin of Ananas
** Designer and Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
**
** This file is part of the Ananas Plugins of the Ananas
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

#include <qobject.h>
#include <q3sqlcursor.h>
#include <q3sqlpropertymap.h>
#include <qmessagebox.h>
#include <qaction.h>
#include <q3datetimeedit.h>
//Added by qt3to4:
#include <q3mimefactory.h>
#include "adatabase.h"
#include "wjournal.h"
#include "ejournal.h"
#include "eselectdoctype.h"
#include "adocjournal.h"
#include "eselectdoctype.h"
#include "wdbtable.h"
#include "alog.h"



wJournal::wJournal( QWidget *parent, Qt::WFlags fl )
:aWidget( parent, "wJournal", fl )
{
	dSelectType = new eSelectDocType();
	setInited( false );
}


wJournal::~wJournal()
{
	delete dSelectType;
	dSelectType = 0;
}



void
wJournal::initObject( aDatabase *adb )
{
	eSelectDocType *d = ( eSelectDocType *)dSelectType;
	aWidget::initObject( adb );
	d->setJournal( &adb->cfg, getId() );


	if(((aDocJournal*)dbobj)->type()==0 && toolbar)
	{
		date_from = new Q3DateEdit(toolbar);
		date_to = new Q3DateEdit(toolbar);
		QDate current = QDate::currentDate();
		date_to->setMinimumWidth(100);
		date_from->setMinimumWidth(100);
		date_to->setDate(current);
		date_from->setDate(current.addMonths(-1));
		connect(date_from, SIGNAL(valueChanged(const QDate &)), this, SLOT(setFilterByDate()));
		connect(date_to, SIGNAL(valueChanged(const QDate &)), this, SLOT(setFilterByDate()));
	}
	else
	{
		date_from = date_to = 0;
	}
	setFilterByDate();
	QObject *obj;
	uint i = 0;
	QObjectList lb = this->queryList( "wDBTable", 0, false, false );
	QListIterator<QObject*> itb( lb ); // iterate over the buttons
	i = 0;
	while ( itb.hasNext() )
	{
		obj = itb.next();
		//printf("wDBTable #%u found\n",++i);
		aLog::print(aLog::Info, tr("wDBTable #%1 found ").arg(++i));
		connect( (wDBTable *)obj, SIGNAL(selectRecord( qulonglong )),
			 this, SLOT(select( qulonglong )) );
		connect( (wDBTable *)obj, SIGNAL( insertRequest()),
			 this, SLOT(insert()) );
		connect( (wDBTable *)obj, SIGNAL(updateRequest()),
			 this, SLOT(update()) );
		connect( (wDBTable *)obj, SIGNAL(viewRequest()),
			 this, SLOT(view()) );
		connect( (wDBTable *)obj, SIGNAL(deleteRequest()),
			 this, SLOT(markDelete()) );
//<<<<<<< wjournal.cpp
//		((QWidget *)obj)->setFocus(); // set focus to wDBTable

//=======
		//((QWidget *)obj)->setFocus(); // set focus to wDBTable

//>>>>>>> 1.15.2.6
	}
	//--delete lb; // delete the list, not the objects
	//--lb=0;

}


bool
wJournal::checkStructure()
{

	return false;
}


/*!
 * Create toolbar for Journal.
 */
Q3ToolBar*
wJournal::createToolBar( Q3MainWindow * owner )
{
	QAction *a,*b,*c,*d, *e;
	toolbar = new Q3ToolBar( owner, "JournalTools" );
	a = new QAction(
	rcIcon("doc_new.png"),
	tr("New"),
	QKeySequence(QString("Insert")),
	toolbar,
	tr("New document")
	);
	a->setToolTip(tr("New document <Ins>"));
	a->addTo( toolbar );
	connect( a, SIGNAL( activated() ), this, SLOT( insert() ) );
	b = new QAction(
	rcIcon("doc_edit.png"),
	tr("Edit"),
	QKeySequence(Qt::Key_Return),
	toolbar,
	tr("Edit document")
	);
	b->setToolTip(tr("Edit document <Enter>"));
	b->addTo( toolbar );
	connect( b, SIGNAL( activated() ), this, SLOT( update() ) );
	c = new QAction(
	rcIcon("doc_view.png"),
	tr("View"),
	QKeySequence(Qt::SHIFT + Qt::Key_Return),
	toolbar,
	tr("View document")
	);
	c->setToolTip(tr("View document <Shift+Enter>"));
	c->addTo( toolbar );
	connect( c, SIGNAL( activated() ), this, SLOT( view() ) );
	d = new QAction(
	rcIcon("doc_delete.png"),
	tr("Delete"),
	QKeySequence(QString("Del")),
	toolbar,
	tr("Delete document")
	);
	d->setToolTip(tr("Delete document <Delete>"));
	d->addTo( toolbar );
	connect( d, SIGNAL( activated() ), this, SLOT( markDelete() ) );

	e = new QAction(
	rcIcon("doc_copy.png"),
	tr("Copy"),
	QKeySequence(Qt::CTRL+Qt::Key_D),
	toolbar,
	tr("Copy document")
	);
	e->setToolTip(tr("Duplicate document <Ctrl+D>"));
	e->addTo( toolbar );
	connect( e, SIGNAL( activated() ), this, SLOT( copy() ) );

	return toolbar;
}



QDialog*
wJournal::createEditor( QWidget *parent )
{
    return new eJournal( parent );
}


int
wJournal::select( qulonglong id )
{
	if ( !dbobj ) return err_abstractobj;
	docUid = dbobj->docId();
//	docUid = 0;
	docId = db->uidType(docUid);
//	printf("selected record uid = %llu, doc uid = %llu doc md id %i\n", id, docUid, docId );
    return 0;
}



qulonglong
wJournal::insert()
{
	aForm * f = 0;
	int md_id = 0;
	eSelectDocType *d = ( eSelectDocType *)dSelectType;

	if ( dSelectType->exec() == QDialog::Accepted ) {
		md_id = d->docId;
		if (  md_id > 0 ){
			if ( engine ) {
				f = engine->openForm( md_id, 0, md_action_new, 0, 0, (aWidget*)this );
				if ( f ) {
//					connect(f, SIGNAL(selected( Q_ULLONG )), this, SLOT(on_selected( Q_ULLONG )));
//					f->closeAfterSelect = true;
				}
			} else printf("engine = NULL\n");

		}
	}
    return 0;
}


int
wJournal::update()
{
	if ( !dbobj ) return err_abstractobj;
	aDocument *doc = ((aDocJournal*)dbobj)->CurrentDocument();
	//doc->select(docUid);
	int res=0;
	if(doc->IsConducted())
	{
		res = QMessageBox::question(this,
						tr("Can not edit document"),
						tr("To edit document you have to unconduct it. Or you can view it. Unconduct?"),
						tr("&Unconduct"), tr("&View"),
						QString::null, 0, 1 );
	}
	else
	{
		delete doc;
		engine->openForm( docId, 0, md_action_edit, 0, docUid,  (aWidget*)this );
		return 0;
	}
	if(res==0)
	{
		doc->UnConduct();
		engine->openForm( docId, 0, md_action_edit, 0, docUid,  (aWidget*)this );
	}
	else
	{
		view();
	}
	delete doc;
	return 0;
}

int
wJournal::copy()
{
	aDocument *doc = ((aDocJournal*)dbobj)->CurrentDocument();
	if(doc)
	{
		int res = doc->Copy();
		Refresh();
		return res;
	}
	return err_copyerror;
}
int
wJournal::view()
{
//CHECK_POINT
	engine->openForm( docId, 0, md_action_view, 0, docUid,  (aWidget*)this );
    return 0;
}



int
wJournal::markDelete()
{
	if ( !dbobj ) return err_abstractobj;
	int res = QMessageBox::question(this,
					tr("Confirm"),
					tr("Do you really want to delete document?"),
					tr("&Yes"), tr("&No"),
					QString::null, 0, 1 );
	if(res) return res;
	aDocument *doc = ((aDocJournal*)dbobj)->CurrentDocument();
	if(doc->IsConducted()) doc->UnConduct();
	res = doc->Delete();
	//invalidate selection
	docId = docUid = 0;
	Refresh();
	delete doc;
	doc = 0;
	return res;
}


QString
wJournal::displayString()
{
	return "***";
}


/*!
 * Create aDocJournal database object.
 */
aObject *
wJournal::createDBObject(  aCfgItem obj, aDatabase *adb )
{
	return new aDocJournal( obj, adb );
}

void
wJournal::setFilterByDate()
{
	if(date_from && date_to)
	{
		QObject *obj;
		QObjectList lb = this->queryList( "wDBTable" );
		QListIterator<QObject*> itb( lb ); // iterate over the buttons
		while ( itb.hasNext() )
		{
			obj = itb.next();
			((wDBTable *)obj)->setFilter(QString("ddate>='%1T00:00:00' AND ddate<='%2T23:59:59'").arg(date_from->date().toString(Qt::ISODate)).arg(date_to->date().toString(Qt::ISODate)));
		}
		//--delete lb; // delete the list, not the objects
	}
	Refresh();
}
