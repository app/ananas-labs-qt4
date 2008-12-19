/****************************************************************************
** $Id: acombobox.h,v 1.1 2008/11/05 21:16:29 leader Exp $
**
** Header file of the field plugin of Ananas
** Designer and Engine applications
**
** Copyright (C) 2006 Andrey Paskal
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
**********************************************************************/

#ifndef ACOMBOBOX_H
#define ACOMBOBOX_H


#include <qwidgetplugin.h>
#include "qcombobox.h"
//Added by qt3to4:
#include <QPixmap>
#include <Q3StrList>
#include <QEvent>
#include <Q3ListBox>


/*!
 * \en
 * 	\brief QComboBox wrapper for AnanasScript access.
 * \_en
 * \ru
 * 	\brief Плагин -- виджет обеспечивает доступ из Ананас.Скрипта ко всем публичным методам QComboBox, являясь врапером
 * 	последнего.
 * 	Наследует QComboBox.
 *
 * 	Смотрите документацию на QComboBox, поставляемую вместе с библиотекой QT компании TrollTech или на сайте
 * 	http://trolltech.com
 *
 * 	Все публичные методы QComboBox доступны как публичные слоты AComboBox.
 *
 *	\code
 *	var myBox=Widget("aComboBox1"); // Предполагается наличие в экранной форме комбобокса с именем aComboBox1
 *	myBox.clear();
 *	myBox.insertItem("Первый пункт");
 *	myBox.insertItem("Второй пункт");
 *	myBox.insertItem("Третий пункт");
 *	myBox.setCurrentItem(1); // Порядковые номера наченаются с нуля, так что текущим делаем второй пункт.
 *	\endcode
 *
 * \_ru
 */
class QT_WIDGET_PLUGIN_EXPORT AComboBox : public QComboBox
{
	Q_OBJECT
	public:
		AComboBox( QWidget* parent=0, const char* name=0 );
		AComboBox( bool rw, QWidget* parent=0, const char* name=0 );
		~AComboBox();
public slots:

    int		count() const;

    void	insertStringList( const QStringList &, int index=-1 );
    //--void	insertStrList( const Q3StrList &, int index=-1 );
    //--void	insertStrList( const Q3StrList *, int index=-1 );
    //--void	insertStrList( const char **, int numStrings=-1, int index=-1);

    void	insertItem( const QString &text, int index=-1 );
    void	insertItem( const QPixmap &pixmap, int index=-1 );
    void	insertItem( const QPixmap &pixmap, const QString &text, int index=-1 );

    void	removeItem( int index );

    int		currentItem();
    virtual void setCurrentItem( int index );

    QString 	currentText() const;
    virtual void setCurrentText( const QString& );

    QString 	text( int index ) const;
    const QPixmap pixmap( int index ) const;

    void	changeItem( const QString &text, int index );
    void	changeItem( const QPixmap &pixmap, int index );
    void	changeItem( const QPixmap &pixmap, const QString &text, int index );

    //--bool	autoResize()	const;
    //--virtual void setAutoResize( bool );
    QSize	sizeHint() const;

    void	setPalette( const QPalette & );
    void	setFont( const QFont & );
    void	setEnabled( bool );

    //--virtual void setSizeLimit( int );
    //--int		sizeLimit() const;


    virtual void setMaxCount( int );
    int		maxCount() const;

    virtual void setInsertionPolicy( Policy policy );
    QComboBox::Policy	insertionPolicy() const;

    virtual void setValidator( const QValidator * );
    const QValidator * validator() const;

    //--virtual void setListBox( Q3ListBox * );
    //--Q3ListBox *	listBox() const;

    virtual void setLineEdit( QLineEdit *edit );
    QLineEdit*	lineEdit() const;

    virtual void setAutoCompletion( bool );
    bool	autoCompletion() const;

    bool	eventFilter( QObject *object, QEvent *event );

    void	setDuplicatesEnabled( bool enable );
    bool	duplicatesEnabled() const;

    bool	editable() const;
    void	setEditable( bool );

    virtual void popup();

    void	hide();

};


#endif
