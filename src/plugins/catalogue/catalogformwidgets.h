/****************************************************************************
** $Id: catalogformwidgets.h,v 1.1 2008/11/05 21:16:29 leader Exp $
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
#ifndef CATALOGFORMWIDGET_H
#define CATALOGFORMWIDGET_H

#include <stdlib.h>
#include <qwidget.h>
#include <qtimer.h>
#include <q3listbox.h>
#include <qlineedit.h>
#include <q3listview.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qevent.h>
#include <q3popupmenu.h>
//Added by qt3to4:
#include <Q3Frame>
#include <QKeyEvent>
#include <QFocusEvent>
//class QVBoxLayout;

class aListBox:public Q3ListBox
{
	Q_OBJECT
public:
	aListBox(QWidget *parent = 0, const char* name = 0, Qt::WFlags f = 0);
	~aListBox();
	void insertItem(const QString &s, long idx, int index = -1);
	void show(QWidget *w, Q3Frame *fr);//, QLabel *lb);

	long getId(int ind) { return atol(listPrivate->text(ind).ascii()); };

	void clear();
	//void setId(long idx) {id = idx;};
public slots:
	virtual void setFocus();

protected:

	virtual void keyPressEvent ( QKeyEvent *e );
	void focusOutEvent ( QFocusEvent *e );
protected slots:
	void doubleClickHandler(Q3ListBoxItem *i);
	//	QVBoxLayout* layout1;
//	QLabel * statusBar;
signals:
	void keyArrowLRPressed();
	void keyArrowLRPressed(const QString&);
	void keyEnterPressed();
	void lostFocus();
	void sendMessage(const QString &);
private:
	Q3ListBox* listPrivate;
};


class aLineEdit:public QLineEdit
{
	Q_OBJECT
public:
	aLineEdit( QWidget* parent, const char* name = 0 );
	~aLineEdit();

protected:
	virtual void keyPressEvent ( QKeyEvent *e );

private:
	QTimer *timer;

public slots:
	void	stopTimer();
	virtual void 	setFocus();
protected slots:
	void	timerDone();
	void	timerRestart(const QString & s);

signals:
	void 	keyArrowPressed();
	void 	delayTextChanged(const QString &);
	void 	keyEnterPressed();
	void	sendMessage(const QString &);
};

class aListView : public Q3ListView
{
	Q_OBJECT
public:
	aListView(QWidget* parent = 0, const char* name = 0, Qt::WFlags f = 0);
	~aListView();

	Q3PopupMenu *menu;
	void setDestination(const bool dest);
	bool getDestination();
protected:
	virtual void keyPressEvent ( QKeyEvent *e );

public slots:

	void 	showMenu( Q3ListViewItem* item, const QPoint& p, int);
	void 	newItem();
	void 	newGroup();
	void	delItem();
	void	markDeleted();
	void 	undoMarkDeleted();
	void	edit();
	void	select();
	virtual void 	setFocus();

protected slots:
	void 	doubleClickHandler( Q3ListViewItem *, const QPoint&, int col);

signals:

	void	sendMessage(const QString &);
 	void	newItemRequest(Q3ListViewItem* parentItem);
	void	newGroupRequest(Q3ListViewItem* parentItem);
	void	delItemRequest(Q3ListViewItem* item);
	void	markDeletedRequest(Q3ListViewItem* item);
	void 	undoMarkDeletedRequest(Q3ListViewItem* item);
	void	editRequest(Q3ListViewItem* item, int columnClicked);
	void	selectRequest(Q3ListViewItem* item);
private:
	Q3ListViewItem* parentItem;
	int columnClicked;
	bool toSelect;
};
#endif

