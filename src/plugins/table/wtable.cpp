/****************************************************************************
** $Id: wtable.cpp,v 1.1 2008/11/05 21:16:30 leader Exp $
**
** Code file of the table plugin of Ananas
** Designer and Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) 2006 Grigory Panov <gr1313 at mail.ru>, Yoshkar-Ola.
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

#include <q3table.h>
#include <qlayout.h>
#include <qstring.h>
#include <q3header.h>
#include <qvariant.h>
#include <qobject.h>
#include <qobject.h>

#include "acfg.h"
#include "aobject.h"
#include "awidget.h"
#include "wtable.h"



wTable::wTable(QWidget *parent, const char * name):Q3Table(parent, name)
{

}

wTable::~wTable()
{
}


void
wTable::setDocument(int row, aObject *object)
{
}

void
wTable::setText(int row, int col, const QString& text)
{
	return Q3Table::setText(row, col,text);
}


QString
wTable::text(int col, int row)
{
	return Q3Table::text(col,row);
}

void
wTable::setHeaderText(int col, const QString& text)
{
	Q3Table::horizontalHeader()->setLabel( col, text );
}


QString
wTable::textHeader(int col)
{
	return Q3Table::horizontalHeader()->label(col);
}

void
wTable::sortColumn ( int col, bool ascending, bool wholeRows )
{
	printf("do sort column\n");
	Q3Table::sortColumn( col, ascending, true );
}


void
wTable::setColumnWidth(int col, int width)
{
	Q3Table::setColumnWidth(col,width);
}


int
wTable::columnWidth(int col)
{
	return Q3Table::columnWidth(col);
}

void
wTable::setNumCols ( int r )
{
	Q3Table::setNumCols(r);
}

int
wTable::numCols () const
{
	int res = Q3Table::numCols();// - 1;
	return res;
}
void
wTable::hideColumn ( int col )
{
	Q3Table::hideColumn ( col );
}

void
wTable::showColumn ( int col )
{
	Q3Table::showColumn ( col );
}
/*
bool
wTable::isColumnHidden ( int col ) const
{
//	int i = -2;
//	if(col>=0) i=col;
	return QTable::isColumnHidden ( col );
}
*/
void
wTable::adjustColumn ( int col )
{
	return Q3Table::adjustColumn  ( col );
}

void
wTable::setColumnStretchable ( int col, bool stretch )
{
	return Q3Table::setColumnStretchable ( col, stretch );
}

bool
wTable::isColumnStretchable ( int col ) const
{
	return Q3Table::isColumnStretchable ( col );
}


void
wTable::swapColumns ( int col1, int col2, bool swapHeader )
{
	Q3Table::swapColumns( col1, col2, swapHeader );

}

void
wTable::swapCells ( int row1, int col1, int row2, int col2 )
{
	Q3Table::swapCells( row1, col1, row2, col2 );
}

void
wTable::setCurrentCell ( int row, int col )
{
	Q3Table::setCurrentCell( row, col );
}

void
wTable::setColumnReadOnly ( int col, bool ro )
{
	Q3Table::setColumnReadOnly( col, ro );
}

void
wTable::insertColumns ( int col, int count )
{
	Q3Table::insertColumns( col, count );
}

void
wTable::removeColumn ( int col )
{
	Q3Table::removeColumn( col );
}

void
wTable::editCell ( int row, int col, bool replace )
{
	Q3Table::editCell( row, col, replace );
}

void
wTable::columnClicked ( int col )
{
//	int i = -2;
//	if(col>=0) i=col;
	Q3Table::columnClicked ( col );//i-1 );
}
void
wTable::swapRows ( int row1, int row2, bool swapHeader )
{
	Q3Table::swapRows (  row1, row2,swapHeader );
}



