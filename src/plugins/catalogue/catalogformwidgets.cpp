/****************************************************************************
** $Id: catalogformwidgets.cpp,v 1.1 2008/11/05 21:16:29 leader Exp $
**
** Catalogue metadata object implementation file of
** Ananas application library
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Grigory Panov, Yoshkar-Ola.
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

#include <q3popupmenu.h>
#include <qtimer.h>
#include <qmessagebox.h>
//Added by qt3to4:
#include <Q3Frame>
#include <QKeyEvent>
#include <QFocusEvent>

#include "catalogformwidgets.h"
/*!
 * Constructor
 */
aListBox::aListBox(QWidget* parent, const char * name, Qt::WFlags f):Q3ListBox(parent,name,f)
{

	listPrivate = new Q3ListBox(parent);
	listPrivate->hide();
	setFocusPolicy (Qt::StrongFocus);
	connect(this, 	SIGNAL	( doubleClicked( Q3ListBoxItem * ) ),
		this,	SLOT	( doubleClickHandler( Q3ListBoxItem * ) ));
//	layout1 = new QVBoxLayout();
//	statusBar = new QLabel(this,"statusBar");
//	statusBar->
//	layout1->addWidget(this,0);
//	layout1->addWidget(statusBar,1);

}
/*!
 * Destructor
 */
aListBox::~aListBox()
{
//	delete statusBar;
	delete listPrivate;
}
/*!
 * Function move and resize frame and list box.
 *\param wd (in) - widget for pattern geometry.
 * \param fr (in, out) - status frame.
 */
void
aListBox::show(QWidget *wd, Q3Frame *fr)//, QLabel *lb)
{
 	setGeometry(wd->x(),
		    wd->y()+wd->size().height()+1,
		    wd->size().width()+wd->x(),
		    wd->size().height()+wd->y()+100);
	QWidget::show();
//	statusBar->setGeometry(	size().higth(),
//				size()+10
//	statusBar->setWidth(wd->size().width() );
//	fr->resize(size());
//	fr->setPos(pos());
	fr->setGeometry(x(),
			size().height()+y(),
			size().width(),
			20);
	emit(sendMessage(tr("Press arrow down for go to drop down list")));
//	lb->move(fr->x()+2,fr->y()+2);
}
/*!
 * Key press event handler.
 * Emit signals keyArrowLRPressed if press arrow left or arrow rigth.
 * \param e (in) - key press event.
 */
void
aListBox::keyPressEvent(QKeyEvent *e)
{
	if(e->key()==Qt::Key_Left || e->key()==Qt::Key_Right)
	{
		emit(keyArrowLRPressed());
		emit(keyArrowLRPressed(currentText()));
		//printf("emit key pressed\n");
	}
	if(e->key() == Qt::Key_Return)
	{
		//printf("enter !!!\n");// return;
		emit(keyArrowLRPressed());
		emit(keyArrowLRPressed(currentText()));
		emit(keyEnterPressed());
		//printf("enter !!!\n");// return;
	}
	if(e->key() == Qt::Key_Escape)
	{
		this->clearFocus();

//		emit(lostFocus());
//		printf("Esc!\n");
		e->accept();
		return;

	}
	e->accept();
	Q3ListBox::keyPressEvent(e);
}
/*!
 * Inserts s in list and idx in list of id.
 * \param s (in) - string for insert.
 * \param idx (in) - id for insert.
 * \param index (in) - position for insert.
 */
void
aListBox::insertItem(const QString &s, long idx, int index)
{
	listPrivate->insertItem(QString("%1").arg(idx),index);
	Q3ListBox::insertItem(s,index);
}
/*!
 * Clears list and list of id.
 */
void
aListBox::clear()
{
	listPrivate->clear();
	Q3ListBox::clear();
}
/*!
 * Focus out event handler. Emit signal lost focus.
 * \param e - focus out event.
 */
void
aListBox::focusOutEvent ( QFocusEvent *e )
{
	QWidget::focusOutEvent ( e );
//	printf("focus is out\n");
	emit(lostFocus());
}

void
aListBox::setFocus()
{
	Q3ListBox::setFocus();
	emit(sendMessage(tr("Use Enter for go to item and arrow for navigation")));
}

void
aListBox::doubleClickHandler(Q3ListBoxItem *i)
{
//	printf("double click handler!\n");
	emit(keyArrowLRPressed());
	emit(keyArrowLRPressed(i->text()));
	emit(keyEnterPressed());

}
aLineEdit::aLineEdit ( QWidget* parent, const char* name) : QLineEdit(parent,name)
{
	timer = new QTimer(this);
	setFocusPolicy(Qt::StrongFocus);
	connect(timer, 	SIGNAL	(timeout()),
		this,	SLOT	(timerDone()));
	connect(this,	SIGNAL	(textChanged ( const QString & )),
		this,  	SLOT	(timerRestart(const QString &)));
	connect(this,	SIGNAL	(lostFocus()),
			SLOT	(stopTimer()));
}

aLineEdit::~aLineEdit()
{
	if(timer)
	{
		delete timer;
		timer=0;
	}
}

/*! Key press event handler. Emit signal keyArrowPressed().
 * \param e (in) - key press event.
 */
void
aLineEdit::keyPressEvent(QKeyEvent *e)
{
//	timer->start(500,TRUE);
	if(e->key()==Qt::Key_Return)//Key_Enter)
	{
		e->accept();
		emit(keyEnterPressed());
		//e->ignore();
	}
	if(e->key()==Qt::Key_Down)
	{
		emit(keyArrowPressed());
	}
	e->ignore();
	QLineEdit::keyPressEvent(e);
}
void
aLineEdit::timerDone()
{
	emit(delayTextChanged(text()));

}

void
aLineEdit::timerRestart(const QString &)
{
	timer->start(500, false);
}

void
aLineEdit::stopTimer()
{
	timer->stop();
}

void
aLineEdit::setFocus()
{
	QLineEdit::setFocus();
	emit(sendMessage(tr("Enter string for search")));
}
/*!
 <* Constructor create list view and popup menu.
 */
aListView::aListView(QWidget* parent,
		     const char* name,
		     Qt::WFlags f) : Q3ListView(parent,name,f)
{
	menu = 0;
	toSelect = true;

	connect(this, SIGNAL( doubleClicked( Q3ListViewItem *, const QPoint&, int)),
		this, SLOT( doubleClickHandler( Q3ListViewItem *, const QPoint&, int)));

	connect(this, SIGNAL( contextMenuRequested (Q3ListViewItem*, const QPoint&, int)),
		this, SLOT( showMenu( Q3ListViewItem*, const QPoint&, int)));

}

aListView::~aListView()
{
	delete menu;
	menu = 0;
}
/*!
 * Shows popup menu.
 * \param item (in) - current item.
 * \param p (in) - mouse pointer position.
 * \param col (in) - current column.
 */
void
aListView::showMenu( Q3ListViewItem* item, const QPoint& p, int col)
{
	delete menu;
	menu = new Q3PopupMenu();
	if(toSelect)
	{
		menu->insertItem(tr("Select"),	this,	SLOT(select()));
		menu->insertSeparator();
	}
	menu->insertItem(tr("Edit"), 	this, 	SLOT(edit()));
	menu->insertSeparator();
	menu->insertItem(tr("New element"),	this, 	SLOT(newItem()));
	menu->insertItem(tr("New group"),	this, 	SLOT(newGroup()));
	menu->insertSeparator();
	//menu->setItemEnabled(menu->idAt(3),false);
	menu->insertItem(tr("Undo mark delete"),	this,	SLOT(undoMarkDeleted()));
	menu->insertItem(tr("Delete (mark deleted)"),this, 	SLOT(markDeleted()));
	menu->insertItem(tr("Delete (phisical)"),	this, 	SLOT(delItem()));
	parentItem = item;
	if(col==-1) col = 0;
	columnClicked = col;
	menu->popup( p );
}

void
aListView::newItem()
{
	emit (newItemRequest(parentItem));
}
void
aListView::newGroup()
{
	emit (newGroupRequest(parentItem));
}
void
aListView::delItem()
{
	emit (delItemRequest(parentItem));
}
void
aListView::markDeleted()
{
	emit (markDeletedRequest(parentItem));
}
void
aListView::undoMarkDeleted()
{
	emit (undoMarkDeletedRequest(parentItem));
}

void
aListView::edit()
{
	emit (editRequest(parentItem,columnClicked));
}
void
aListView::select()
{
	emit (selectRequest(parentItem));
}
void
aListView::setDestination(const bool dest)
{
	toSelect = dest;
}
bool
aListView::getDestination()
{
	return toSelect;
}
/*! Key press event handler. Emit signal keyEnterPressed().
 * \param e (in) - key press event.
 */
void
aListView::keyPressEvent(QKeyEvent *e)
{

	switch(e->key())
	{
	case Qt::Key_Return:
		e->accept();
		parentItem = selectedItem();
		columnClicked = 0;
		if(toSelect)
		{
			select();
		}
		else
		{
			edit();
		}
		break;
	case Qt::Key_Insert:
		parentItem = selectedItem();
		columnClicked = 0;
		switch(QMessageBox::question(this,
				tr("Type of new item"),
				tr("<p align=\"center\">Select type of new item</p>"),
				tr("&Elemnt"), tr("&Group")))
		{
		case 0:
			newItem();
			break;
		case 1:
			newGroup();
			break;
		default:
			break;
		}
		break;
	case Qt::Key_Delete:
		parentItem = selectedItem();
		columnClicked = 0;
		switch(QMessageBox::question(this,
				tr("Delete item"),
				tr("<p align=\"center\">Mark deleted or delete?</p>"),
				tr("&Mark deleted"), tr("&Delete")))
		{
		case 0:
			markDeleted();
			break;
		case 1:
			delItem();
			break;
		default:
			break;
		}
		break;
	default:
		e->ignore();
		break;
	}
	Q3ListView::keyPressEvent(e);

}

void
aListView::setFocus()
{
	Q3ListView::setFocus();
	if(toSelect)
	{
		emit(sendMessage(tr("<Enter> - select, <Ins> - add, <Del> - delete items")));
	}
	else
	{
		emit(sendMessage(tr("<Enter> - edit, <Ins> - add, <Del> - delete items")));
	}
}


void
aListView::doubleClickHandler( Q3ListViewItem *item, const QPoint& p, int col)
{
	parentItem = item;
	columnClicked = col;
	if(toSelect)
	{
		select();
	}
	else
	{
		edit();
	}
}

