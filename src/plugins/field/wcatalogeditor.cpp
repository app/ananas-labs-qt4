/****************************************************************************
** $Id: wcatalogeditor.cpp,v 1.2 2008/11/08 20:16:36 leader Exp $
**
** Code file of the field plugin of Ananas
** Designer and Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) 2003-2004 Grigory Panov, Yoshkar-Ola.
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

#include <qlineedit.h>
#include <qlayout.h>
#include <qvalidator.h>
#include <qlabel.h>
#include <q3listview.h>
#include <qsizepolicy.h>
//Added by qt3to4:
#include <Q3Frame>
#include <QPixmap>
#include "ananas.h"
#include "wcatalogeditor.h"
#include "wfield.h"
#include "wdbtable.h"
#include "catalogform.h"
#include "alog.h"
#include "../ananas/mainform.h"


/*!
 * \en 	Creates catalog editor widget. \_en
 * \ru 	Создает виджет для редактирования каталога.
 * 	В принципе он не предусмотрен использоваться ни как иначе, кроме как часть
 * 	объекта wField, т.е его родительский объект должен быть wField
 * 	или его наследник для обеспечения корректной инициализации.
 * 	При создании объекта runtime не предусмотрено, за исключением случая,
 * 	когда виджет используется в составе wField в объекте wDBTable.
 * 	\param parent - \en parent object. \_en \ru родительский объект. \_ru
 * 	\param name - \en name \_en \ru имя \_ru
 * 	\param catname \en not used \_en \ru не используется \_ru
 */
wCatalogEditor::wCatalogEditor(	wField *parent,
				const char *name,
				const char *catname) : QWidget(parent, name)
{
  QWidget* w =0;
  md = NULL;
  QString str;
  label = new QLabel(parent, name);
	label->setFrameShape(Q3Frame::Box);
	w =(QWidget*) parent->parent()->parent();
	str = parent->getFieldType();
	catId = atoi(str.remove(0,2));// gets catalog id.
   	if(w)
   	{
		if(strcmp(w->className(),"wDBTable")==0) // wField is element wDBTable
		{
			initCat(((wDBTable*)w)->db);
		}
   }
}

wCatalogEditor::wCatalogEditor(QWidget* parent,int cat) : QWidget()
{
	catId = cat;
	label = 0;
	ws = parent;
}


/*!
 * \en	Destructor catalog editor. \_en
 */
wCatalogEditor::~wCatalogEditor()
{
	delete label;
	label = 0;
}



/*!
 * \en	Initialisation catalog editor. \_en
 * \ru	Инициализация редактора каталога.
 * 	Инициализирует внутренние объекты метаданных и базы банных. \_ru
 */
void
wCatalogEditor::initCat(aDatabase * adb)
{
	md = &adb->cfg;
	db = adb;
}


/*!
 * \en	Handler signal click().
 * 	Creates and shows form for select catalog. \_en
 * \ru	Обработчик события нажатия кнопки,
 * 	которая является второй составной частью виджета.
 * 	Сигнал коннектится в объекте wField.
 * 	Создает и отображет в рабочем пространстве енжина форму для
 * 	редактирования каталога или выбора из каталога.
 * 	Производит заполнение дерева групп и элементов группами и формирует
 * 	данные для передачи в форму редактирования каталога.
 * 	Создает и отображет в рабочем пространстве енжина эту форму.\_ru
 */
void
wCatalogEditor::select()
{
	openForm(true);
}
/*!
 * \en	Creates and shows form for edit  catalog. \_en
 * \ru	Обработчик события нажатия кнопки,
 * 	которая является второй составной частью виджета.
 * 	редактирования каталога.
 * 	Производит заполнение дерева групп и элементов группами и формирует
 * 	данные для передачи в форму редактирования каталога.
 * 	Создает и отображет в рабочем пространстве енжина эту форму.\_ru
 */
void
wCatalogEditor::edit()
{
	openForm(false);
}

void
wCatalogEditor::openForm(const bool toSelect)
{


	aLog::print(aLog::Debug, tr("wCatalog Editor open form for select=%1 ").arg((int)toSelect));
	MainForm *mainform;
	if(parent())
	{
		mainform = (MainForm*) topLevelWidget();
		ws = mainform->ws;
	}
//  aWindowsList *wl = mainform->wl;
	int objid = catId;
/*	if ( wl->find( objid ) )
    	{
//		wl->remove(objid);
		wl->get( objid )->setFocus();
		return;
    	}
	else
	{
  		CatalogForm* newform = new CatalogForm(ws,0, WDestructiveClose);
		wl->insert( objid, newform );
	}*/
	CatalogForm* newform = new CatalogForm(ws,0, Qt::WDestructiveClose);

	connect( newform,	SIGNAL(selected(qulonglong)),
  		 this,		SLOT(on_selected( qulonglong )));
	connect( newform, 	SIGNAL(destroyed()),
  	  	 this,		SLOT(on_destroyed_form()));

	aCatalogue *cat = new aCatalogue(md->find(catId),db);
	int count=0;
	bool est=true;
	QMap<qulonglong,Q3ListViewItem*> map, map_el;
	aCfgItem tmp, tmp_f,tmp_el,tmp_group, o;
	Q3ListViewItem * item;
	Q3ListViewItem * p_item;
	qulonglong idGrForm=0, idElForm=0;
	QStringList listPos, listPosGroup;
	newform->ListHint->hide();
	QPixmap pixmap(newform->getGroupPixmap());
	QPixmap pixmap_mark_deleted(newform->getMarkDeletedPixmap());
	tmp = md->find(catId);
	newform->setCaption(md->attr(tmp,mda_name));
	o = md->findChild(tmp, md_forms); // get obj forms
  	if(!o.isNull())
  	{
		count = md->count(o,md_form);
		for(int i=0; i<count; i++)
		{
			tmp_f = md->findChild(o,md_form,i);
			if(!tmp_f.isNull()
			   && atoi(md->attr(tmp_f,mda_type).ascii())==md_form_elem)
	 		{
				aLog::print(aLog::Debug, tr("wCatalog Editor found element forms"));
				idElForm = md->id(tmp_f);
//				 continue;
	 		}
			if(!tmp_f.isNull()
			   && atoi(md->attr(tmp_f,mda_type).ascii())==md_form_group)
	 		{
				aLog::print(aLog::Debug, tr("wCatalog Editor found group forms"));
				idGrForm = md->id(tmp_f);
	 		}
		}
  	}
	else
	{
		aLog::print(aLog::Error, tr("wCatalog Editor meta object forms not found"));
	}
	tmp_el = md->findChild(tmp, md_element);
	tmp_group = md->findChild(tmp,md_group);
	tmp = md->findChild(tmp_el, md_field);
	int count_fields = md->count(tmp_el,md_field);
	listPosGroup = cat->getGroupUserFields();
	int i,level = 0;
	cat->Select();
  	while(est) // add group in tree on levels
  	{
		est = false;
		++level;
		cat->selectByLevel(level-1);

  		if(!cat->FirstInGroupTable()) break;
  		do
  		{
			if(cat->GroupSysValue("level").toInt()==level-1) //all groups, having this level
			{
				est = true;
				QString displayString;
				displayString= cat->GroupSysValue(listPosGroup[0]).toString();
				if(map.contains(cat->GroupSysValue("idp").toULongLong()))
				{
					p_item = map[(cat->GroupSysValue("idp").toULongLong())];
					item = new Q3ListViewItem(p_item);
				}
				else
				{
					item = new Q3ListViewItem(newform->ListView);
					newform->ListView->insertItem(item);
				}
				item->setText(0, displayString);
				if(cat->isGroupMarkDeleted())
					item->setPixmap(0,pixmap_mark_deleted);
				else
					item->setPixmap(0,pixmap);

				map.insert(cat->GroupSysValue("id").toULongLong(),item);
			//printf("%lu\n",cat->GroupSysValue("id").toULongLong());

			}


		}while(cat->NextInGroupTable());

	}
  	listPos = cat->getUserFields();
	checkUserFields(listPos);
	int fid;
	//sets column name
	for(uint i=0; i<listPos.count(); i++)
  	{
		fid = atoi(listPos[i].remove("uf",false).ascii());
		if(!fid)
		{
//			printf("listPos[]=%s",listPos[i].remove("text_uf",false).ascii());
			fid = (listPos[i].remove("text_",false)).toInt();
			//tmp = md->find(fid);
		}
		if(fid)
		{
			tmp = md->find(fid);
			newform->ListView->addColumn(md->attr(tmp,mda_name));
		}
  	}

	listPos.clear();
	listPos = cat->getUserFields();
	checkUserFields(listPos);
  	//Q_ULLONG res = 0;
   	// cat deleted in function catalogform::destroy();

 	 newform->setData(	cat,
		   		map,
		   		listPos,
		   		cat->getGroupUserFields(),
		   		idElForm,
		   		idGrForm,
				toSelect);

	newform->setId(value().toULongLong());
	//--
	if (ws)
        ((QWorkspace*)ws)->addWindow(newform);
	newform->show();
	((QWidget*)newform->parent())->move(0,0);
}
void
wCatalogEditor::checkUserFields( QStringList &lst)
{
	aCfgItem item = md->find(catId);
	int fid;
	if(item.isNull()) return;
	item = md->findChild(item,md_element);
	for(int i=0; i< md->count(item,md_field); i++)
  	{
		aCfgItem mdi = md->findChild(item,md_field,i);
		int ind = lst.findIndex(QString("uf%1").arg(md->attr(mdi,mda_id)));
		if(ind!=-1)
		{
			//--lst.insert(lst.at(i),lst[ind]);
			lst.insert(i,lst[ind]);
			lst.remove(lst.at(ind+1));

		}
		else
		{
			ind = lst.findIndex(QString("text_uf%1").arg(md->attr(mdi,mda_id)));
			if(ind!=-1)
			{
				//--lst.insert(lst.at(i),lst[ind]);
				lst.insert(i,lst[ind]);
				lst.remove(lst.at(ind+1));

			}
		}
  	}

}

void
wCatalogEditor::on_selected( qulonglong uid )
{
	emit( selected(uid) );
}

void
wCatalogEditor::on_destroyed_form( )
{
	emit( destroyed_form() );
}


/*!
 * \en 	Gets catalog editor value. \_en
 * \ru 	Получение значения редактора каталога.
 * 	Т.е. то, что было выбрано или утановлено функцией setValue().
 * 	Сейчас возвращает стринг с id, но может быть
 * 	Для получения осмысленного значения используй функцию displayValue() \_ru
 */
QString
wCatalogEditor::value() const
{
	return vValue;
}


/*!
 * \en 	Gets value for displaying. \_en
 * \ru	Получение осмысленного представления id.
 * 	Строка является представлением уникального id для элемента
 * 	или группы каталога. \_ru
 * 	\param id_element (in) -\en unique id element or group \_en
 * 				\ru уникальный идентификатор элемента или группы \_ru
 */
QString
wCatalogEditor::displayValue(qulonglong id_element) const
{
const QString no_select = "  ";
	if(md)
	{
	 /* 	aCfgItem tmp,field;
		QString res;
		tmp = md->find(catId);
 		tmp = md->findChild(tmp, md_element);
		field = md->findChild(tmp,md_field);
  	 	aSQLTable* te = new aSQLTable(tmp,db);
		te->select(QString("id=%1").arg(id_element));
		if(te->first())
		{
			res = te->sysValue(QString("uf%1").arg(md->id(field))).toString();
		}
		else res = no_select;
		delete te;*/
		return QString("%1").arg(id_element);
	}
	else return no_select;
}


/*!
 * \en 	Sets id for displaying. \_en
 * \ru	Установка id для показа.
 * 	Хранится в стринге для совмесимости с wField. \_ru
 */
void
wCatalogEditor::setValue(QString newvalue)
{
	if(vValue==newvalue) return;
	vValue = newvalue;
	if(label) label->setText(displayValue(newvalue.toULongLong()));
	emit valueChanged(value());
}

