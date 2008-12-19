/****************************************************************************
** $Id: acombobox.cpp,v 1.1 2008/11/05 21:16:29 leader Exp $
**
** Code file of the field plugin of Ananas
** Designer and Engine applications
**
** Copyright (C) 2006 Andrey Paskal.
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
**
**********************************************************************/

#include "acombobox.h"
//Added by qt3to4:
#include <QPixmap>
#include <Q3StrList>
#include <QEvent>

AComboBox::AComboBox( QWidget* parent, const char* name ):QComboBox( parent, name ) {
}

AComboBox::AComboBox(  bool rw, QWidget* parent, const char* name ):QComboBox( rw, parent, name ) {
}

AComboBox::~AComboBox(){
}

int
AComboBox::count() const {
	return QComboBox::count();
}

void
AComboBox::insertStringList( const QStringList &list, int index ) {
	QComboBox::insertStringList( list, index );
};

/*--void
AComboBox::insertStrList( const Q3StrList &list, int index ){
	QComboBox::insertStrList( list, index );
};

void
AComboBox::insertStrList( const Q3StrList *list, int index ){
	QComboBox::insertStrList( list, index );
};

void
AComboBox::insertStrList( const char **str, int numStrings, int index){
	QComboBox::insertStrList( str, numStrings, index );
};*/

void
AComboBox::insertItem( const QString &text, int index ){
	QComboBox::insertItem( text, index);
};

void
AComboBox::insertItem( const QPixmap &pixmap, int index ){
	QComboBox::insertItem( pixmap, index);
};

void
AComboBox::insertItem( const QPixmap &pixmap, const QString &text, int index ){
	QComboBox::insertItem(pixmap, text, index );
};


void
AComboBox::removeItem( int index ){
	QComboBox::removeItem( index );
};


int
AComboBox::currentItem() {
	return QComboBox::currentItem();
};

void
AComboBox::setCurrentItem( int index ){
	QComboBox::setCurrentItem( index );
};


QString
AComboBox::currentText() const {
	return QComboBox::currentText();
};

void
AComboBox::setCurrentText( const QString& string){
	QComboBox::setCurrentText( string );
};


QString
AComboBox::text( int index ) const {
	return QComboBox::text( index );
};

const QPixmap
AComboBox::pixmap( int index ) const {
	return QComboBox::pixmap( index );
};

void
AComboBox::changeItem( const QString &text, int index ){
	QComboBox::changeItem( text, index );
};

void
AComboBox::changeItem( const QPixmap &pixmap, int index ){
	QComboBox::changeItem( pixmap, index );
};

void
AComboBox::changeItem( const QPixmap &pixmap, const QString &text, int index ){
	QComboBox::changeItem( pixmap, text, index );
};


/*--bool
AComboBox::autoResize() const {
	return QComboBox::autoResize();
};

void
AComboBox::setAutoResize( bool val){
	QComboBox::setAutoResize( val );
};*/

QSize
AComboBox::sizeHint() const {
	return QComboBox::sizeHint();
};

void
AComboBox::setPalette( const QPalette &pal ){
	QComboBox::setPalette( pal );
};

void
AComboBox::setFont( const QFont & font) {
	QComboBox::setFont( font );
};

void
AComboBox::setEnabled( bool val){
	QComboBox::setEnabled( val);
};


/*--void
AComboBox::setSizeLimit( int size ){
	QComboBox::setSizeLimit( size );
};

int
AComboBox::sizeLimit() const {
	return QComboBox::sizeLimit();
};*/

void
AComboBox::setMaxCount( int cnt ){
	QComboBox::setMaxCount( cnt );
}

int
AComboBox::maxCount() const {
	return QComboBox::maxCount();
}

void
AComboBox::setInsertionPolicy(QComboBox::Policy policy ){
	QComboBox::setInsertionPolicy(policy );
}

QComboBox::Policy
AComboBox::insertionPolicy() const{
	return QComboBox::insertionPolicy() ;
}


void
AComboBox::setValidator( const QValidator * v){
	QComboBox::setValidator( v );
}

const QValidator *
AComboBox::validator() const{
	return QComboBox::validator();
}

/*--void
AComboBox::setListBox( Q3ListBox * lb){
	QComboBox::setListBox( lb );
}

Q3ListBox *
AComboBox::listBox() const {
	return QComboBox::listBox();
}*/

void
AComboBox::setLineEdit( QLineEdit *edit ){
	QComboBox::setLineEdit( edit );
}

QLineEdit*
AComboBox::lineEdit() const{
	return QComboBox::lineEdit();
}


void
AComboBox::setAutoCompletion( bool val ){
	QComboBox::setAutoCompletion( val );
}

bool
AComboBox::autoCompletion() const{
	return QComboBox::autoCompletion();
}

bool
AComboBox::eventFilter( QObject *object, QEvent *event ){
	return QComboBox::eventFilter( object, event );
}

void
AComboBox::setDuplicatesEnabled( bool enable ){
	QComboBox::setDuplicatesEnabled( enable );
}

bool
AComboBox::duplicatesEnabled() const{
	return QComboBox::duplicatesEnabled();
}

bool
AComboBox::editable() const{
	return QComboBox::editable();
}

void
AComboBox::setEditable( bool val ){
	QComboBox::setEditable( val );
}

void
AComboBox::popup(){
	QComboBox::popup();
}


void
AComboBox::hide(){
	QComboBox::hide();
}


