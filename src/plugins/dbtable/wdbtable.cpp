/****************************************************************************
** $Id: wdbtable.cpp,v 1.2 2008/11/08 20:16:36 leader Exp $
**
** Code file of the database table plugin of Ananas
** Designer and Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) 2003-2006 Grigory Panov, <gr1313 at mail.ru>, Yoshkar-Ola.
** Copyright (C) 2003-2004 Andrey Paskal <app at lrn dot ru>, Yoshkar-Ola.
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

//--#include <qfocusdata.h>
#include <stdlib.h>
#include <qpainter.h>
#include <qmessagebox.h>
//Added by qt3to4:
#include <QContextMenuEvent>
#include <Q3HBoxLayout>
#include <QKeyEvent>
#include <Q3SqlPropertyMap>
#include <Q3ValueList>
#include <Q3SqlCursor>
#include <QPixmap>
#include <Q3Frame>
#include <q3mimefactory.h>
#include <Q3PopupMenu>
#include <QEvent>
#include <Q3SqlFieldInfo>
#include <QPointer>
//--#include "propertyeditor.h"
//--#include "command.h"
//--#include "formwindow.h"
//--#include "mainwindow.h"
//--#include "mainform.h"
#include "adocjournal.h"
#include "alog.h"

#include "wdbtable.h"
#include "edbtable.h"


/*!
 * \en	Constructor. \_en
 * \ru	Конструктор.
 * 	Задает значение ширины по умолчанию для столбца = 100. \_ru
 */
wDBTable::wDBTable( QString objtype,  QWidget *parent, const char *name )
    : Q3DataTable( parent, name )
{
//	vId = 0;
//	verticalHeader()->hide();
	setLeftMargin(0);
	setNullText("");
	objtype = "";
	defColWidth = 100; //default column width
	tableInd = -1;
	inEditMode = false;
	searchWidget = 0;
	searchMode = false;
	searchString = "";
	connect( this, SIGNAL(cursorChanged ( QSql::Op ) ), this, SLOT(lineUpdate( QSql::Op ) ) );
	connect( this, SIGNAL(currentChanged ( int, int ) ), this, SLOT(lineChange( int, int ) ) );
	connect( this, SIGNAL(beforeInsert ( QSqlRecord* ) ), this, SLOT(lineInsert( QSqlRecord* ) ) );
	connect( this, SIGNAL(valueChanged ( int, int )  ), this, SLOT(updateTableCellHandler(int, int ) ) );
	connect( this, SIGNAL(doubleClicked ( int, int, int, const QPoint&) ), this, SLOT(doubleClickEventHandler(int,int,int, const QPoint&)));
	init();


	aLog::print(aLog::Debug, tr("wDBTable init ok"));
//	printf("ok init wdbtable\n");

}



/*!
 * \en	Opens property editor. \_en
 * \ru	Открывает окно редактора свойсв, если fn == true \_ru
 */
void
wDBTable::setOpenEditor( bool fn )
{
	if(fn)	OpenEditor();
}


/*!
 *	\~english
 *	Gets list of defined fields.
 *	\~russian
 *	Получение списка заданных полей.
 *	\~
 */
QStringList
wDBTable::getDefFields() const
{
	return fname;
}


/*!
 *	\~english
 *	Gets list of defined headers.
 *	\~russian
 *	Получение списка заданных заголовков.
 *	\~
 */
QStringList
wDBTable::getDefHeaders() const
{
	return hname;
}


/*!
 *	\~english
 *	Gets list of defined column width.
 *	\~russian
 *	Получение списка заданных размеров столбцов.
 *	\~
 */
QStringList
wDBTable::getColWidth() const
{
	return colWidth;
}


/*!
 *	\~english
 *	Gets list of defined column id.
 *	\~russian
 *	Получение списка заданных идентификаторов столбцов.
 *	\~
 */
QStringList
wDBTable::getDefIdList() const
{
	return idList;
}

/*!
 *	\~english
 *	Gets metadata document id.
 *	\~russian
 *	Получение идентификатора документа метаданных, которому принадлежит виджет.
 *	\~
 */
qulonglong
wDBTable::getId() const
{
	return doc_id;
}

/*!
 *	\~english
 *	Gets current table id.
 *	\~russian
 *	Получение идентификатора текущей таблицы.
 *	\~
 */
int
wDBTable::getTblInd() const
{
	return tableInd;
}

/*!
 *	\~english
 *	Olways return false.
 *	\~russian
 *	Всегда возвращает false.
 *	\~
 *	\return false
 */
bool
wDBTable::getOpenEditor() const
{
	return false;
}

/*!
 *	\~english
 *	Gets default column width.
 *	\~russian
 *	Получение размера столбца по умолчанию.
 *	\~
 */
int
wDBTable::getDefColWidth() const
{
	return defColWidth;
}

/*!
 *	\~english
 *	Sets list of field names.
 *	\~russian
 *	Установка списка имен полей.
 *	\~
 */
void
wDBTable::setDefFields(QStringList lst)
{
	fname = lst;
}

/*!
 *	\~english
 *	Sets list of field headers.
 *	\~russian
 *	Установка списка заголовков полей.
 *	\~
 */
void
wDBTable::setDefHeaders(QStringList lst)
{
	hname = lst;
}

/*!
 *	\~english
 *	Sets list of columns width.
 *	\~russian
 *	Установка списка размеров столбцов.
 *	\~
 */
void
wDBTable::setColWidth(QStringList lst)
{
	colWidth = lst;
}

/*!
 *	\~english
 *	Gets list of column id.
 *	\~russian
 *	Установка списка идентификаторов столбцов.
 *	\~
 */
void
wDBTable::setDefIdList(QStringList lst)
{
	idList = lst;
}

/*!
 *	\~english
 *	Sets metadata document id.
 *	\~russian
 *	Установка идентификатора документа метаданных, которому принадлежит виджет.
 *	\~
 */
void
wDBTable::setId( qulonglong fn )
{
	doc_id = fn;
}


/*!
 *	\~english
 *	Gets current table id.
 *	\~russian
 *	Установка идентификатора текущей таблицы.
 *	\~
 */
void
wDBTable::setTblInd( int ind )
{
	tableInd = ind;
}


/*!
 *	\~english
 *	Sets default columns width.
 *	\~russian
 *	Установка размера столбцов по умолчанию.
 *	\~
 */
void
wDBTable::setDefColWidth( int fn )
{
	defColWidth = fn;
}



/*!
 * \en	Creates property editor dialog window. \_en
 * \ru	Создает диалоговое окно редактора свойств. \_ru
 */
void
wDBTable::OpenEditor()
{
	//getBindList();
	setAvailableTables();
	eDBTable e( this->topLevelWidget());
	checkFields();
	e.setData(this,md);
	if ( e.exec()==QDialog::Accepted )
	{
		e.getData(this);
		//updateProp();
	}
}



/*!
 *	\~endlish
 *	Destructor.
 *	\~russian
 *	Деструктор.
 *	\~
 */
wDBTable::~wDBTable()
{
	delete cur;
	cur=0;
}



/*!
 *	\~english
 *	Function init. Sets metadata object and tables object.
 *	\~russian
 *	Функция инициализации. Устанавливает объект метаданных и объект таблицы,
 *	к которому идет обращение для поиска полей или (в случае документа) таблиц
 *	\~
 */
void
wDBTable::init()// aDatabase *adb )
{
	int id=0;
	aCfgItem o, o_table;
	aCfgItem mditem, docitem;

	// set up pixmap for calculated fields

	cur = new Q3SqlCursor("cur",false);
	md = aWidget::parentContainer(this)->getMd();
	if ( md )
	{
		id = aWidget::parentContainer(this)->getId();
		o = md->find(id);
		QString objClass = md->objClass(o);
		if(objClass==md_document)
		{
			o = md->findChild(o,md_tables); // object tables
			tables = o;
		}
		if(objClass==md_journal){
			o = md->findChild(o, md_columns);
			tables = o;
		}
		if(objClass==md_catalogue)
		{
//			verticalHeader()->hide();
			o = md->findChild(o,md_element);
			tables = o;
		}
	}
	else
	{
//		verticalHeader()->hide();
//		printf("name engin\n");
	}

}



/*!
 * \en	Sets available tables, using object tables. \_en
 * \ru 	Заполняет внутренний список доступных таблиц, используя в качестве
 * 	источника информации объект tables.
 * 	Добавляет к имени уже забинденой таблицы звездочку(*). \_ru
 */
void
wDBTable::setAvailableTables()
{
aCfgItem o_table, o = tables;
QString str;
QStringList listIdTable;
int res;
Q3ValueList<int> vList = getBindList();

	if(o.isNull()) return;
	QString objClass = md->objClass(o);
	//printf("obj class `%s'\n",objClass.ascii());
	list_available_tables.clear();
	if(objClass==md_tables)
	{
		res = md->countChild(o,md_table); // ind kol_vo tables in obj tables
		for(int i=0; i<res; i++)
		{
			o_table = md->findChild(o,md_table,i);
			listIdTable << QString("%1").arg(md->id(o_table));
			//printf("id = %li\n", md->id(o_table));
		}
	}
	if(objClass==md_columns)
	{
	//	listIdTable << QString("%1").arg(md->id(o));
		list_available_tables << tr("Available columns");
	}
	if(objClass==md_element)
	{
		list_available_tables << md_element;

	//	listIdTable << QString("%1").arg(md->id(o));
	}

	//res = md->countChild(o,md_table); // ind kol_vo tables in obj tables
	res = listIdTable.count();
	for(int i=0; i<res; i++)
	{
		//o_table = md->findChild(o,md_table,i);

		if(vList.find(listIdTable[i].toInt())!=vList.end())
			str ="* ";
		else
			str ="";
		list_available_tables << str + md->attr(md->find(listIdTable[i].toInt()),mda_name); // add tables name in  combo box
	}
}




/*!
 * \en 	Handler line current change signal. \_en
 * \ru	Обработчик сигнала изменения строки. Генерирует сигналы
 * deleteLine и saveLine. \_ru
 */
void
wDBTable::lineUpdate( QSql::Op mode)
{
QSqlRecord *rec = sqlCursor()->editBuffer();//currentRecord();
	switch(mode)
	{
		case QSql::Update:
	//	rec =  currentRecord();
		break;
		case QSql::Insert:
		break;
		case QSql::Delete:
		//printf("emit del line\n");
		emit(deleteLine(rec));
		return;
		default:
		break;
	}
	//if(!rec) return;
	//printf(" emit saveLine\n");
	emit(saveLine(rec));
}



/*!
 *	\~english
 *	Function paint field in table.
 *	\~russian
 *	Переопределенная функция рисования поля таблицы.
 *	В типе DateTime не отображает время.
 *	Поддерживает вычисляемые поля.
 *	\~
 */
void
wDBTable::paintField ( QPainter * p, const QSqlField * field, const QRect & cr, bool selected )
{
	if(field->name().left(5)=="text_") return;
        if ( sqlCursor()->isCalculated( field->name() ) ){
        	if ( field->name()=="system_icon" )
	        	p->drawPixmap( QRect( 0, 0, cr.width(), cr.height() ), systemIcon() );
                return;
        }
        if ( sqlCursor()->contains( "text_"+field->name() ) ) {
		QSqlField f(*field);
		f.setValue(((aDataTable*)sqlCursor())->sysValue("text_"+f.name()));
		 Q3DataTable::paintField( p, &f, cr, selected );
		 return;
	//	f = * sqlCursor()->field( "text_"+f.name() );
        }
	if(field->type() == QVariant::DateTime)
	{
		QSqlField f(*field);
		f.setValue(field->value().toDate());// don't show time
		Q3DataTable::paintField( p, &f, cr, selected );
		return;
	}
	Q3DataTable::paintField( p, field, cr, selected );
}



/*!
 *	\~english
 *	Return system icon for display object record state.
 *	\russian
 *	Возвращает иконку для отображения состояния строки.
 *	\~
 */
QPixmap
wDBTable::systemIcon()
{
        aWidget *container = aWidget::parentContainer( this );
        QString ctype="";
        QPixmap pm;
        Q3SqlCursor *r = sqlCursor();
        int df=0, cf=0, mf=0;

        if ( container ) ctype = container->className();
        if ( r )
	{
                if ( r->contains("df") ) df = r->field("df").value().toInt();
                if ( r->contains("cf") ) cf = r->field("cf").value().toInt();
		else
		{
			if(ctype=="wJournal")
			{

				aDocJournal* sysObj = new aDocJournal(db);
				if(sysObj)
				{
//for(int i=0; i<r->count(); i++)
//	printf("f[%d]=%s, %s\n",i, r->fieldName(i).ascii(), r->field(i)->value().toString().ascii());
					if(sysObj->findDocument(r->field("id").value().toULongLong()))
					{
						aDocument *doc = sysObj->CurrentDocument();
						cf = doc->IsConducted();
						delete doc;
					}
					else
					{
						printf(">>doc select failed!\n");
					}
					//doc = 0;
					//cf = sysObj->sysValue("cf").toInt();
				}

				delete sysObj;


			}
//>>>>>>> 1.71.2.13
		}
                if ( r->contains("mf") ) mf = r->field("mf").value().toInt();
		if(ctype=="wJournal")
		{
                        pm = t_doc;
                        if ( df ) pm = t_doc_d;
                        if ( cf && !df ) pm = t_doc_t;
                        if ( mf && !df ) pm = t_doc_m;
                        if ( cf && mf && !df ) pm = t_doc_tm;
                }
		else
	                if(ctype=="wCatalogue")
			{
                	        pm = t_cat_e;
				if ( df ) pm = t_cat_ed;
                	}
			else
				if(ctype=="wCatGroupe")
				{
					pm = t_cat_g;
	        	                if ( df ) pm = t_cat_gd;
				}
        }
        return pm;
}



/*!
 * \~english
 *	Sets property `DefFields', `DefHeaders', `ColWidth', `DefIdList' to
 *	values from metadata.
 * \~russian
 *	Устанавливает свойства `DefFields', `DefHeaders', `ColWidth', `DefIdList'
 *	в значения, определенные в метаданных. Старые значения не сохраняются.
 *	Для документа используется idTable.
 *	\~
 *	\param idTable (in) - 	\~english table id
 *				\~russian id таблицы документа.  Не используется для каталогов и журналов.
 *	\~
 */
void
wDBTable::setFields(int idTable)
{
CHECK_POINT
	int field_count,j;
	int i; //,tableCount;
	//QSqlCursor *cur;
	const Q3SqlFieldInfo *field;
	QString str;
	QStringList Cwidth, list_fields,list_id;
	aCfgItem o, o_table, o_field;
	QString mdtag=QString(md_field);

	list_fields.clear();
	o = tables; // object tables
	QString objClass = md->objClass(o);
	//printf("table parent obj class '%s'\n", (const char*)objClass);
	if(objClass==md_tables)
	{
		o_table = md->find(idTable);
	}
	else
	if(objClass==md_columns)
	{
		o_table = o;
		mdtag=QString(md_column);
	}
	else
	if(objClass==md_element)
	{
		o_table = o;
	}
	colWidth.clear();
	i=0;
	while(i<numCols())
	{
		removeColumn(0);
	}
	cur->clear();
	if(!o_table.isNull())
	{
		field_count = md->countChild(o_table,mdtag);
		//printf("table name '%s'\n",(const char*)md->attr(o_table,mda_name));
		//printf("table id '%s'\n",(const char*)md->attr(o_table,mda_id));
		for (j=0; j<field_count; j++)
		{
			o_field = md->findChild(o_table,mdtag,j);
			//printf("field %i name '%s'\n",j,(const char*)md->attr(o_field,mda_name));
			//printf("field %i id '%s'\n",j,(const char*)md->attr ( o_field, mda_id ));
			list_fields << md->attr(o_field,mda_name);
			list_id << md->attr(o_field,mda_id);
			str.setNum(j);
			field = new Q3SqlFieldInfo(md->attr(o_field,"name"));
			cur->append(*field);
			setSqlCursor(cur);
			addColumn(field->name(),field->name(),property("DefaultColWidth").toInt());
			refresh(RefreshColumns);
			Cwidth << property("DefaultColWidth").toString();
		}
	}
	setProperty("DefFields",list_fields);
	setProperty("DefHeaders",list_fields);
	setProperty("ColWidth",Cwidth);
	setProperty("DefIdList",list_id);
}



/*!
 * \en	Gets table id at position table. \_en
 * \ru	Получение id таблицы по ее позиции в метаданных.
 * 	Эта позиция совпадает с индексом таблицы в комбобоксе редактора свойств
 * 	таблицы. \_ru
 * 	\param numTable (in) - 	\en Position table in metadata \_en
 * 				\ru Положение таблицы в метаданных.
 * 				Не используется в каталоге и журнале. \_ru
 * 	\return \en table id \_en \ru id таблицы \_ru
 */
int
wDBTable::getTableId(int numTable)
{
	int res = -1;
	aCfgItem o, o_table;
	if ( numTable == -1 ) return res;
	o = tables; // object tables
	QString objClass = md->objClass(o);
	if(objClass==md_tables)
	{
		o_table = md->findChild(o,md_table,numTable);
		res = md->id(o_table);
	}
	if(objClass==md_columns)
	{
		res = md->id(o);
		//printf("parent name %s\n", (const char*)md->attr(md->parent(o),mda_name));
	}
	if(objClass==md_element)
	{
		res = md->id(o);
	}
	//printf("get table id=%i\n", res);
	return res;
}



/*!
 * \en	Gets table position at id table. \_en
 * \ru	Получение позиции таблицы в метаданных по ее id.
 * 	Эта позиция совпадает с индексом таблицы в комбобоксе редактора свойств
 * 	таблицы. \_ru
 * 	\param id (in) - 	\en Id table \_en
 * 				\ru id таблицы.
 * 				Не используется в каталоге и журнале. \_ru
 * 	\return \en table position for documet or 0 for catalogue and journal\_en
 * 		\ru Для документа возвращает позицию таблицы, для каталога и
 * 		журнала документов возвращает 0 \_ru
 */
int
wDBTable::getTableInd(int id)
{
int j,tableCount;
aCfgItem o, o_table;
	o = tables; // object tables
	QString objClass = md->objClass(o);
	if(objClass==md_columns || objClass==md_element)
	{
		//printf("TableInd=%i\n",tableInd);
		return tableInd;
	}
	tableCount = md->count(o,md_table);
	for(j=tableCount-1; j>=0; j--)
	{
		o_table = md->findChild(o,md_table,j);
		if(md->id(o_table)==id) break;
	}
return j;
}



/*!
 * \en 	Gets list of id fields or list of name fields. \_en
 * \ru 	Получение списка id или имен полей таблицы.
 * 	Возвращаемое значение зависит от параметра GetId. \_ru
 * 	\param idTable (in) - 	\en id of table \_en
 * 				\ru id таблицы документа. Для каталога и журнала
 * 				не используется. \_ru
 * 	\param GetId (ib) - 	\en if GetId == true returns list of id, else
 * 				returns list of names. \_en
 * 				\ru если параметр равен true, возвращается
 * 				список id полей таблицы, иначе возвращается
 * 				список имен полей таблицы. \_ru
 * 	\return \en list of id fields or list of name fields. \_en
 * 		\ru Список id или имен полей таблицы. \_ru
 */
QStringList
wDBTable::getFields(int idTable, bool GetId)
{
	QStringList lst;
	if (idTable==-1) {
		return lst;
	}

	int items_count,j;//,tableCount;
	aCfgItem o, o_table, o_item;
	QString	mdtag=md_field;


//aWidget *wo = aWidget::parentContainer( this );
//	if ( !wo ) return lst;
//	o_table = md->objTable( wo->getId(), idTable );

	o = tables; // object tables
	QString objClass = md->objClass(o);
	if(objClass==md_tables)
	{
		o_table = md->find(idTable);
	}
	else
	if(objClass==md_columns)
	{
		o_table = o;
		mdtag = md_column;
	}
	else
	if(objClass==md_element)
	{
		o_table = o;
	}
	if(!o_table.isNull())
	{
		items_count = md->countChild(o_table,mdtag);
		for (j=0; j<items_count; j++)
		{
			o_item = md->findChild(o_table,mdtag,j);
			if(GetId)
			  lst << md->attr(o_item,mda_id);
			else
			  lst << md->attr(o_item,mda_name);
		}
	}
	else {
		debug_message("Table not found!\n");
	}
return lst;
}



/*!
 *	\~english
 *	Gets field type.
 *	\~russian
 *	Получение типа поля.
 *	\~
 *	\param id - \~english field id \~russian идентификатор поля \~
 *	\return \~english field type \~russian тип поля \~
 */
QString
wDBTable::getFieldType(long id)
{
	aCfgItem o;
	QString str="";

	QString objClass = md->objClass(tables);
	if(objClass==md_columns) {
		id= journalFieldId(id);
	}
	o = md->find(id);
	if(!o.isNull())
	{
		str = md->attr(o,mda_type);
	}
return str;
}



/*!
 *	\~english
 *	Gets field name.
 *	\~russian
 *	Получение имени поля.
 *	\~
 *	\param id - \~english field id \~russian идентификатор поля \~
 *	\return \~english field name \~russian имя поля \~
 */
QString
wDBTable::getFieldName(long id)
{
	aCfgItem o;
	QString str="";
	o = md->find(id);
	if(!o.isNull())
	{
		str = md->attr(o,mda_name);
	}
return str;
}



/*!
 *\~english
 *	function check property name, id, header and width.
 *	If have incorrect field name or id, then print message and all.
 *	If have less or more items in DefHeaders or ColWidth, then set them normal.
 *\~russian
 *	Функция проверяет свойства name, id, DefHeaders и ColWidth.
 *	Если находит ошибки в свойствах name или id, она выводит сообщение об этом, но не исправляет ошибки.
 *	Если находит ошибки в свойствах DefHeaders или ColWidth, то исправляет.
 *\~
 */
void
wDBTable::checkFields()
{
	QStringList fl,hl,cl,il;
	unsigned int i;
	QString str;

	//t = getFields(property("tableInd").toInt(),true); //get list fields id
	fl = property("DefFields").toStringList();
	il = property("DefIdList").toStringList();
	cl = property("ColWidth").toStringList();
	hl = property("DefHeaders").toStringList();
//	proverka na nalichie field in metadata
	for(i=0; i<il.count(); i++)
	{
		str = getFieldName(il[i].toInt());
		if(i<fl.count())
		{
			if(str!=fl[i])
			{
				aLog::print(aLog::Debug, QString("wDBTable unknown field name `%1' or (and) id `%1'\n").arg(str).arg(il[i]));
			}
		}
		else il.remove(il.at(i--));
		if(i>=hl.count()) hl << str;
		if(i>=cl.count()) cl << property("DefaultColWidth").toString();
	}
	while(i<hl.count())
	{
		hl.remove(hl.at(i));
	}
	while(i<cl.count())
	{
		cl.remove(cl.at(i));
	}
	setProperty("DefFields", fl );
	setProperty("DefHeaders", hl );
	setProperty("ColWidth", cl );
	setProperty("DefIdList", il );
}









/*!
 *\~english
 *	Initialisation the widget on form loaded in engine.
 *\~russian
 *	Инициализация виджета при загрузке в форму инжина.
 *\~
 */
void
wDBTable::init(aDatabase *adb, aEngine *e )
{

	aLog::print(aLog::Debug, tr("wDBTable init in engine "));
//	printf("begin init wdbtable\n");
	unsigned int countField,i;
	aCfgItem o, own;
	QString str, ctype;
	QStringList lst,lstHead,lstWidth;
	int tid;
	aWidget *container = NULL;

	t_doc = rcIcon( "t_doc.png" );
        t_doc_d = rcIcon( "t_doc_d.png" );
        t_doc_t = rcIcon( "t_doc_t.png" );
        t_doc_m = rcIcon( "t_doc_m.png" );
        t_doc_tm = rcIcon( "t_doc_tm.png" );
	t_cat_e = rcIcon( "t_cat_e.png" );
        t_cat_ed = rcIcon( "t_cat_ed.png" );
	t_cat_g = rcIcon( "t_cat_g.png" );
	t_cat_gd = rcIcon( "t_cat_gd.png" );

	engine = e;
	setConfirmDelete(true);
	db = adb;
	md = &adb->cfg;
	tid = property("TableInd").toInt();
	container = aWidget::parentContainer( this );
	if ( !container )
	{
		aLog::print(aLog::Error, tr("wDBTable not in Ananas object container "));
		return; //printf("!no wDBTable parent container\n");
	}
	else
	{
		o = md->objTable( container->getId(), tid );
		if ( o.isNull() )
		{
			//debug_message("Table not found\n");
			aLog::print(aLog::Error, tr("wDBTable init meta object not found "));
		}
		ctype = container->className();
		aLog::print(aLog::Info, tr("wDBTable container type is %1 ").arg(ctype));

		setContainerType(ctype);
	}

	//o  = md->find(property("TableInd").toInt());
	if ( o.isNull() )
	{
		aLog::print(aLog::Error, tr("wDBTable init meta object not found "));
		return;
	}
	countField = numCols();
	for(i=0; i<countField;i++)
	{
		removeColumn(0);
	}
	aSQLTable *tbl = NULL;
	//printf("ctype = %s\n",( const char *) ctype );
	if ( containerType() == "wDocument" )
	{
		QString flt;
		flt = QString("idd=%1").arg(container->uid());
		aLog::print(aLog::Info, tr("wDBTable filter is %1 ").arg(flt));
		setFilter(flt);
		//TODO: fix memory leak
		tbl = new aSQLTable( o, adb );
//		printf("new table ok\n");
	//	tbl->first();

	}
	if ( containerType() == "wCatalogue" ) {
                tbl = container->table(); //new aSQLTable( o, adb );
		setFilter(QString("idg=0"));
		newDataId(0);
		tbl->append( Q3SqlFieldInfo("system_icon") );
//		tbl->setGenerated( "system_icon", false );
		tbl->setCalculated("system_icon", true );
          }
	if ( containerType() == "wJournal" ) {
		tbl = container->table(); //new aSQLTable( o, adb );
		tbl->setMode( 0 );
		tbl->append( Q3SqlFieldInfo( "system_icon" ) );
//		tbl->setGenerated( "system_icon", false );
		tbl->setCalculated( "system_icon", true );
//		tbl->append( QSqlFieldInfo( "t1" ) );
//		tbl->setGenerated( "t1", false );
//		tbl->setCalculated("t1", true );
        }
	refresh();
	cur->clear();
	setSqlCursor(tbl,true);
	refresh(RefreshColumns);
	countField = numCols();
	lst = property("DefIdList").toStringList();
	lstHead = property("DefHeaders").toStringList();
	lstWidth = property("ColWidth").toStringList();
	for ( i = 0; i < countField; i++ )
	{
		//remove all columns in wDBTable, not in sql cursor
		removeColumn( 0 );
		QString s = sqlCursor()->fieldName(i);
//		printf(">>>>s = %s\n",s.ascii());
//		if(sqlCursor()->isCalculated(s))
//		{
//			if((s.left(7)=="text_uf" && lst.findIndex(s.mid(7))!=-1) || s == "system_icon")
//			{
//				continue;
//			}
			// not calculate field, if is not contents in wDBTable
//			sqlCursor()->setCalculated(sqlCursor()->fieldName(i),false);
//		}
	}
	if ( containerType() == "wJournal" ) {
		addColumn( "system_icon", "", 20 );
		setColumnReadOnly( 0, true );
		if (md->objClass(*(container->getMDObject()))==md_journal && !((aDocJournal*) container->dataObject())->type() ) {
			// we have common journal
			// Insert journal system columns.
			addColumn( "ddate", tr("Date"), 100 );
			addColumn( "pnum", tr("Prefix"), 200 );
			addColumn( "num", tr("Number"), 100 );
		}
	}
	if ( containerType() == "wCatalogue" ) {
		addColumn( "system_icon", "", 20 );
	//	printf("set column ro\n");
		setColumnReadOnly( 0, true );
	}

	if ( containerType() == "wDocument" || containerType() == "wCatalogue" ) {

	// Задаем сортировку по индентификатору в обратном порядке для
	// табличной части документа
	// чтобы при добавлении новых позиций в список строки не скакали
	    QSqlIndex pk = sqlCursor()->primaryIndex();
	    pk.setDescending( 0, false);
	    setSort( pk );
	    sqlCursor()->select();
	    sqlCursor()->first();
	    refresh();
	}


	//refresh(RefreshColumns);
	if (md->objClass(*(container->getMDObject()))!=md_journal || ((aDocJournal*) container->dataObject())->type() ) {
		// we have not common journal
		for(i=0; i<lst.count();i++)
		{
			// assemble sql table field names
			if ( containerType() == "wJournal" )
			{
//				str = journalFieldName(lst[i].toLong());

				str = "uf"+QString::number(journalFieldId(lst[i].toLong()));
				//printf(">>>>>>ss=%s\n",str.ascii());
			}
			else
			{
				str = "uf"+lst[i];
			}
			// add defined fields
			addColumn(str,lstHead[i],lstWidth[i].toInt());
		}
	}
	refresh(RefreshAll);
	setWFieldEditor();
	aLog::print(aLog::Debug, tr("wDBTable init in engine ok"));
}



/*!
 *
 */
/*!
 *	\~english
 *	Set custom field editor (wField) used property `value'.
 *	\~russian
 *	Устанавливает свой редактор ячейки таблицы (wField).
 *	\~
 */
void
wDBTable::setWFieldEditor()
{
	 aEditorFactory * f = new  aEditorFactory(this,"");
	 f->setMd(md);
	 Q3SqlPropertyMap * m = new Q3SqlPropertyMap();
	 m->insert("wField", "value");
	 installPropertyMap(m);
	 installEditorFactory(f);
}



/*!
 *	\~english
 *	Construct cell editor. Creates new wField object and inited it.
 *	\~russian
 *	Конструктор редактора ячейки. Создает новый экземпляр объекта wField и инициализирует его.
 *	\~
 */
QWidget*
aEditorFactory::createEditor(QWidget * parent, const QSqlField * field)
{
//--WFlags fl=0;
wField * tmp;
wDBTable *t=0;
QString str,stmp;
wField::tEditorType type = wField::Unknown;
	if(md)
	{
		t = (wDBTable*)parent->parent()->parent();

		tmp  = new wField((QWidget*)(parent->parent()),""/*--,fl*/);
		if(t)
		{
			str = field->name();
			str = str.remove(0,2);
			tmp->setFieldType(t->getFieldType(str.toInt()));
			str = t->getFieldType(str.toInt());
			stmp = str.section(' ',0,0);
//			printf("type is %s\n",stmp.ascii());
			if(stmp=="C") type = wField::String;
			if(stmp=="N") type = wField::Numberic;
			if(stmp=="D") type = wField::Date;
			if(stmp=="B") type = wField::Boolean;
			if(stmp=="O")
			{
				int tid;
				//gets object id.
				//stmp = str.section(' ',1,1);
				tid = atoi(str.section(' ',1,1).ascii());
//				printf("tid =%d\n",tid);
				aCfgItem o = md->find(tid);
				if(!o.isNull())
				{
					//gets object class
					str = md->objClass(o);
//					printf("otupe = %s\n",str.ascii());
					if(str == md_catalogue)
						// and set editor
						type = wField::Catalogue;
					if(str == md_document)
						type = wField::Document;

				}
				else
				{
					aLog::print(aLog::Error,tr("aEditorFactory field metaobject not found"));
				}
			}
			tmp->setEditorType(type);
			tmp->initObject( t->db );
			tmp->engine = t->engine;
		}
	}
	else
	{
		aLog::print(aLog::Error,tr("aEditorFactory metadata in null"));
	}
	if(parent && parent->parent() && parent->parent()->parent()) // setted right tabOrder
	// parent->parent()->parent() - pointer to wDBTable;
	QWidget::setTabOrder((QWidget*)parent->parent()->parent(),tmp);
        return tmp;

}
void
aEditorFactory::setMd(aCfg * cfg)
{
	md = cfg;
}


/*!
 *	\~english
 *	Gets list of id binding table.
 *	\~russian
 *	Получение списка уже присутствующих в форме таблиц. Необходимо для того, чтобы одну таблицу не добавили два раза.
 *	\~
 *	\return \~english list of id binding table. \~russian список таблиц \~
 */
Q3ValueList<int>
wDBTable::getBindList()
{
aCfgItem obj;
QObjectList wList;
int id;
wDBTable* wtable;
QObject* wd = aWidget::parentContainer( this );
	listBindings.clear();
    	wList = wd->queryList( "wDBTable" );
	QListIterator<QObject*> it( wList ); // iterate over the wDBTable
	while ( it.hasNext() )
	{
		wtable = qobject_cast<wDBTable*>( it.next() );

		if(strcmp(wtable->name(),this->name())) // don't added current id
		{
		//don.t added deleted widgets
		   if(strncmp("qt_dead_widget_",wtable->name(),strlen("qt_dead_widget_")))
		   {
			id = wtable->property("TableInd").toInt();
			if(id>=0) // don't added negativ id (table while not selected)
			{
				listBindings << id;
			}
		   }
		}
	}
	//--delete wList;
return listBindings;
}



/*!
 *	\~english
 *	Get field value slot. Don't implemented.
 *	\~russian
 *	Получение значения поля. Не реализовано.
 *	\~
 */
QVariant
wDBTable::Value( const QString &colname )
{
	aLog::print(aLog::Error, tr("wDBTable: function Value() call"));
	return QVariant("");
}



/*!
 *\~english
 *	Emit signal on select record.
 *	Signal give information about selected record database id.
 *\~russian
 *	Выдает сигнал при выборе записи.
 *	Сигнал содержит информацию об id, выбранного объекта в базе данных.
 *\param -\~english Not use, only for signal compatibility.\~russian
 *	Не используются, заданы только для совместимости с сигналом таблицы.\~
 *\~
 */
void
wDBTable::lineChange(int, int)
{
	QSqlRecord * rec = currentRecord();
	if ( !rec ) return;
	qulonglong id = 0;
	if(rec->contains("id")) id = rec->value("id").toLongLong();
	//if (containerType() == "wJournal")
	//{
		//if(rec->contains("idd")) id = rec->value("idd").toLongLong();
	//}
	aLog::print(aLog::Info, tr("wDBTable: select document %1").arg(id));
	emit( selectRecord( id ) );
}


/*!
 *\ru
 *	Устанавливает значение системного поля idd во вновь добавляемую запись табличной части документа.
 *\_ru
 */
void
wDBTable::lineInsert(QSqlRecord* rec){

	if (containerType() == "wDocument")
	{
		if(rec->contains("idd")) rec->setValue("idd",QVariant(doc_id));
		if(rec->contains("ln")) rec->setValue("ln",numRows()-1);
	}
	if(containerType() == "wCatalogue")
	{
		if(rec->contains("idg")) rec->setValue("idg",QVariant(cat_group_id));
	}
}


/*!
 *	\~english
 *	Delete line function
 *	\~russian
 *	Функция удаления строки.
 *	\~
 *	\return \~english true, if no error \~russian true, если ошибок не было \~
 */
bool
wDBTable::deleteCurrent()
{
	bool res;
	res = Q3DataTable::deleteCurrent();
	emit(updateCurr(currentRow(),currentColumn()));
	return res;

}


/*!
 *	\~english
 *	Key press handler.
 *	\~russian
 *	Обрабатывает события при нажатии кнопок клавиатуры.
 *	\~
 */
void
wDBTable::keyPressEvent ( QKeyEvent *e )
{
	qulonglong id;

	aWidget *container = NULL;
	if ( searchMode == FALSE && e->text().at( 0 ).isPrint() )
	{
		searchOpen( e->text() );
	}
	else
	{
		searchClose();
	}

	if(containerType() =="wJournal")
	{
		e->ignore();
	}
	if(containerType() =="wCatalogue")
	{
		switch ( e->key() )
		{
		case Qt::Key_Escape:
			e->ignore();
			break;
		case Qt::Key_Return:
			if(currentRecord())
			{
				id = currentRecord()->value(0).toLongLong();
				if ( e->state() == Qt::ShiftModifier )
				{
					//printf("Shift+Return pressed %Ld\n", id);
					EditElement();
				} else
				{
					//printf("Return pressed %Ld\n", id );
					emit( selected( id ) );
				}
				e->accept();
			}
			else
			{
				aLog::print(aLog::Info, tr("wDBTable: current record not setted"));
			}
			break;
		default:
			e->ignore();
			break;
		}
	}
	Q3DataTable::keyPressEvent( e );
}



/*!
 *\ru
 *	Слот обрабатывает ситуацию смены/установки идентификатора документа, устанавливая новый фильтр.
 *	Кроме переустановки фильта следует вызывать слот newDataId, для смены/установки
 *	значения идентификатора документа. Для журнала этого можно не делать.
 *	\param flt - строка вида "idd=999", задающая условия отбора записей в таблицу по значению поля idd.
 *\_ru
 */
void
wDBTable::newFilter(const QString & flt)
{
	setFilter(flt);
	refresh();
}

/*!
 *\ru
 *	Слот обрабатывает ситуацию смены/установки идентификатора документа, запоминая новое значение.
 *	Для смены фильта следует вызывать слот newFilter.
 *	\param id - идентификатор документа, который следует запомнить.
 *\_ru
 */
void
wDBTable::newDataId(const qulonglong id)
{
	if(containerType() == "wDocument")
	{
		setId(id);
	}
	if(containerType() == "wCatalogue")
	{
		cat_group_id = id;
	}
}


/*!
 *	\~english
 *	Opens form for edit catalogue element.
 *	\~russian
 *	Открывает форму по умолчанию для объекта контейнера и настраивает ее на работу с текущей строкой таблицы. Используется только в настраиваемом редакторе каталога.
 *	\~
 */
void
wDBTable::EditElement()
{

	ANANAS_UID id = 0;
//	wGroupTreeItem * item = ( wGroupTreeItem * ) tree->currentItem();
	aForm * f = 0;

//CHECK_POINT
	id = currentRecord()->value(0).toLongLong();
	if ( id ) {
		if ( engine ) {
			f = engine->openForm( aWidget::parentContainer( this )->getId(), 0, md_action_edit, md_form_elem, id, ( aWidget *) this );
			if ( f ) {
				connect(f, SIGNAL( update( ANANAS_UID )), this, SLOT(updateItem( ANANAS_UID )));
//				connect(f, SIGNAL(selected( Q_ULLONG )), this, SLOT(on_selected( Q_ULLONG )));
//				f->closeAfterSelect = true;
			}
		}
	}
}


long
wDBTable::journalFieldId(long columnId){
	aCfgItem item;

	item= md->find(md->find(columnId),md_fieldid);
	return md->text(item).toLong();

}

QString
wDBTable::journalFieldName(long columnId)
{
	aCfgItem item;
	item= md->find(md->find(columnId),md_fieldid);
	item = md->find(md->text(item).toLong());
	if(!item.isNull())
	{
		QString s = md->attr(item,mda_type);
		QChar ch = s[0];
		if(ch.upper()=='O')
		{
			return QString("text_uf%1").arg(md->attr(item,mda_id));
		}
		else
		{
			return QString("uf%1").arg(md->attr(item,mda_id));
		}
	}
	return "uf0";

}

/*!
 *	\~english
 *	Hanler table cell end edit.
 *	\~russian
 *	Обрабатывает окончание редактирования ячейки таблицы.
 *	\~
 */
//bool
void
wDBTable::updateTableCellHandler(int r, int c)
{
//	printf(">>>>update curr row %d  col %d!\n",r,c);
	lastEditedRow= r;
	lastEditedCol= c;
}

/*!
 *	\~english
 *	Reimplemented QDataTable function.
 *	\~russian
 *	Переопределяет функцию QDataTable. Испускает сигнал updateCurr().
 *	\~
 */
bool
wDBTable::updateCurrent()
{
	bool res = Q3DataTable::updateCurrent();
	if(res)
	{
//		printf(">>>>real update curr row %d  col %d!\n",lastEditedRow, lastEditedCol);
		emit(updateCurr(lastEditedRow,lastEditedCol));
	}
return res;
}


/*!
 *	\~english
 *	Reimplemented QDataTable function.
 *	\~russian
 *	Переопределяет функцию QDataTable. Отрабатывет после нажатия Enter в редактируемой ячейке.
 *	Перемещает фокус ввода на следующую ячейку.
 *	\~
 */
void
wDBTable::activateNextCell()
{
	//int r = currentRow(), c = currentColumn();
	//if(r<0) r=0;
	//if(c<0) c=0;
	//tCurrentCell(r,c+1);
}




/*!
 *	\~english
 *	Reimplemented QDataTable function.
 *	\~russian
 *	Переопределяет функцию QDataTable. Выделяет текст в ячейке.
 *	\~
 */
//>>>>>>> 1.71.2.13
QWidget *
wDBTable::beginUpdate ( int row, int col, bool replace )
{
	wField  *wd;
	wd = (wField*)Q3DataTable::beginUpdate(row,col,replace);
	if(wd)
	{
		//inEditMode = true;
		wd->selectAll();
	}
	return wd;
}


void
wDBTable::doubleClickEventHandler(int /*rol*/, int /*col*/, int /*button*/, const QPoint &/*mousePos*/)
{
	if(containerType() =="wCatalogue" || containerType() == "wJournal")
	{
		if(currentRecord())
		{
			qulonglong id = currentRecord()->value(0).toLongLong();
			emit( selected( id ) );
		}
	}

}

/**
 * \ru
 * 		\brief Переопределяем метод базового класса, вызывающий диалог подтверждения.
 * 		Переопределен только диалог подтверждения удаления.
 * \_ru
 */

QSql::Confirm
wDBTable::confirmEdit( QSql::Op m ) {
	if ( m == QSql::Delete ) {
		if ( 0 == QMessageBox::question(
            this,
            tr("Remove record?"),
            tr("You are going to remove record <br>"
                "Are you sure?"),
            tr("&Yes, remove"), tr("&No"),
            QString::null, 0, 1 ) ) {
            	return QSql::Yes;
        } else {
            	return QSql::No;
        }
	} else {
		return Q3DataTable::confirmEdit( m );
	}
}

/**
 * \ru
 * 		\brief Переопределяем метод базового класса, осуществляющий вставку новой строки в таблицу.
 * 		Наш метод будет всегда помещать новую строку в начале таблицы.
 * \_ru
 */

bool
wDBTable::beginInsert() {
	if ( !sqlCursor() || isReadOnly() || !numCols() )
		return FALSE;
    if ( !sqlCursor()->canInsert() )
		return FALSE;

	bool result = Q3DataTable::beginInsert();
	endEdit( currentRow(), currentColumn(), false, false);
	setCurrentCell( numRows(), 0 );
	return result;

}


/*!
 *	\~english
 *	Reimplemented QDataTable function.
 *	\~russian
 *	Переопределяет функцию QDataTable. Если контейнер wJournal, может испускаеть сигналы insertRequest(), updateRequest(), deleteRequest(), viewRequest()
 *	\~
 */
void
wDBTable::contentsContextMenuEvent ( QContextMenuEvent * e )
{
	Q3Table::contentsContextMenuEvent( e );
	QString str, ctype;

	if ( containerType() == "wDocument" || containerType() == "wCatalogue" ) {
   	// Переопределяем всплывающее по правой кнопке мыши меню для табличной части документа
   	// Во-первых, для его локализации
	// Во-вторых, чтобы добавляемая в табличную часть строка всегда вставлялась самой последней.
   		enum {
    		IdInsert=0,
   			IdUpdate,
    		IdDelete,
		};

		QPointer<Q3PopupMenu> popupForDoc = new Q3PopupMenu( this );
		int id[ 3 ];
		id[ IdInsert ] 	= popupForDoc->insertItem( tr( "New" ) );
		id[ IdUpdate ] 	= popupForDoc->insertItem( tr( "Edit" ) );
		id[ IdDelete ] 	= popupForDoc->insertItem( tr( "Delete" ) );

		if ( !sqlCursor() || isReadOnly() || !numCols() ) {
			popupForDoc->setItemEnabled(id[ IdInsert ], false );
			popupForDoc->setItemEnabled(id[ IdUpdate ], false );
			popupForDoc->setItemEnabled(id[ IdDelete ], false );
		}

		int r = popupForDoc->exec( e->globalPos() );
		delete (Q3PopupMenu*) popupForDoc;
		if(r==id[IdInsert]) {
			beginInsert();
		} else if(r==id[IdUpdate]) {
			keyPressEvent( new QKeyEvent( QEvent::KeyPress, Qt::Key_F2, 0, Qt::NoButton));
		} else if(r==id[IdDelete]) {
			Q3DataTable::deleteCurrent();
		}
	}


	if ( containerType() == "wJournal" )
	{
		//id = currentRecord()->value(0).toLongLong();
       		enum {
	    		IdInsert=0,
	   		IdUpdate,
	    		IdDelete,
	    		IdView,
			IdRefresh };
		QPointer<Q3PopupMenu> popup = new Q3PopupMenu( this );
		int id[ 5 ];
		id[ IdInsert ] = popup->insertItem( tr( "New" ) );
		id[ IdUpdate ] = popup->insertItem( tr( "Edit" ) );
		id[ IdDelete ] = popup->insertItem( tr( "Delete" ) );
		id[ IdView ] = popup->insertItem( tr( "View" ) );
		id[ IdRefresh ] = popup->insertItem( tr( "Refresh" ) );
		int r = popup->exec( e->globalPos() );
		delete (Q3PopupMenu*) popup;
		if(r==id[IdInsert])
			emit(insertRequest());
		else
			if(r==id[IdUpdate])
				emit(updateRequest());
			else
				if(r==id[IdDelete])
					emit(deleteRequest());
				else
					if(r==id[IdView])
						emit(viewRequest());
						if(r==id[IdRefresh])
							{
								//recalculate();
								refresh();
							}
	}
	e->accept();

}


void
wDBTable::updateItem( ANANAS_UID db_uid )
{
	refresh();
	emit currentChanged( currentRecord() );
}


int
wDBTable::Select( ANANAS_UID db_uid )
{
	aSQLTable *t = ( aSQLTable *) sqlCursor();

//	printf("id = %Li\n",db_uid);
	ANANAS_UID cur_id = 0;
	uint curr = currentRow(), curc=currentColumn(), row = 0;
	bool found = FALSE;

	while ( t->seek( row ) ){
		cur_id = t->sysValue( "id" ).toULongLong();
		if ( cur_id == db_uid ) {
			found = TRUE;
			break;
		}
		row++;
	}
	if ( found ) {
		setCurrentCell( row, curc );
	} else setCurrentCell( curr, curc );
	return 0;
}


bool
wDBTable::searchColumn( const QString & text, bool FromCurrent, bool Forward )
{

	QString s;
	uint curr = currentRow(), curc=currentColumn(), row = 0, idx;
	bool found = FALSE;
	aSQLTable *t = ( aSQLTable *) sqlCursor();

	if ( FromCurrent ) row = curr;
	if ( Forward ) row++; else row--;
	idx = indexOf( curc );

	while ( t->seek( row ) ){
		s = t->value( idx ).toString();
		if ( s.left( text.length() ) == text ) {
			found = TRUE;
			break;
		}
		if ( Forward ) row++; else row--;
	}
	if ( found ) {
		setCurrentCell( row, curc );
	};
	return found;
}



void
wDBTable::searchOpen( const QString & text )
{
	searchWidget = new aSearchWidget( aWidget::parentContainer( this ), this );
	searchMode = TRUE;
	searchWidget->setFocus();
	searchWidget->search( text );
}


void
wDBTable::searchClose()
{
	if ( searchWidget ) {
		setFocus();
		searchWidget->deleteLater();
		searchMode = FALSE;
		searchWidget = 0;
	}
}


/*!
 *\~english
 *
 *
 *\~russian
 *	Осуществляет интерактивный поиск по первым буквам
 *	текстового значения текущего столбца таблицы.
 *	Пока не реализован поиск для столбцов типа "Объект".
 *	Up 	- предыдущее значение.
 *	Down 	- следующее значение.
 *	ESC,Enter - конец поиска.
 *\~
 */
aSearchWidget::aSearchWidget( QWidget *parent, wDBTable *table )
: Q3Frame( parent )
{
	t = table;
	ftext = "";
	setFrameStyle( Q3Frame::PopupPanel | Q3Frame::Raised );
	setFocusPolicy( Qt::StrongFocus );
	new Q3HBoxLayout( this, 0, 0 );
	l = new QLineEdit( this );
        l->installEventFilter( this );
	setFocusProxy( l );
	layout()->add( l );
//	move (0,0);
	move( 3+t->x()+t->columnPos( t->currentColumn()), t->y()+1/*+t->height()*/);
	resize( t->columnWidth( t->currentColumn() )-2, 25 );
	connect( l, SIGNAL( textChanged( const QString & ) ), this, SLOT( setText( const QString & ) ) );
}


aSearchWidget::~aSearchWidget()
{
}


void
aSearchWidget::search( const QString &t )
{
	show();
	l->setText( t );
}


void
aSearchWidget::setText( const QString &text )
{
	if ( t->searchColumn( text ) ) ftext = text;
	else l->setText( ftext );
}


bool
aSearchWidget::eventFilter( QObject *obj, QEvent *ev )
{
	if ( obj == l ) {
		if ( ev->type() == QEvent::FocusOut ) {
			t->searchClose();
			return TRUE;
		}
		if ( ev->type() == QEvent::KeyPress ) {
		QKeyEvent *e = ( QKeyEvent *) ev;
			switch ( e->key() ){
			case Qt::Key_Return:
			case Qt::Key_Escape:
				//printf("OK\n");
				t->searchClose();
				break;
			case Qt::Key_Up:
				t->searchColumn( ftext, TRUE, FALSE );
				break;
			case Qt::Key_Down:
				t->searchColumn( ftext, TRUE, TRUE );
				break;
			default:
				return FALSE;
				break;
			}
                return TRUE;
            } else {
                return FALSE;
            }
        } else {
            // pass the event on to the parent class
            return Q3Frame::eventFilter( obj, ev );
        }
}
