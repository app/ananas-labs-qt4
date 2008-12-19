#include "edbtable.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

#include <stdlib.h>
#include "wdbtable.h"
//Added by qt3to4:
#include <Q3ValueList>
#include <Q3SqlFieldInfo>

/*
 *  Constructs a eDBTable as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
eDBTable::eDBTable(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
eDBTable::~eDBTable()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void eDBTable::languageChange()
{
    retranslateUi(this);
}

void eDBTable::init()
{
}


void
eDBTable::destroy()
{
//	tablerow_free( r );
}


/*
void eDBTable::insertColumn(const char *Header, const char *Name,
int idxt, int cw, int tw, int td)
{

tablefield *f;
char s[30];

sprintf(s, "%s %i %i", (const char *)otypes[idxt], tw, td);

f = tablefield_newsys(Name, s, "");
tablefield_setheader(f, Header);
f->sizex = cw;
f->flen = tw;
f->decimals = td;
f->ftypeindex = idxt;
tablerow_insertcolumn(r, f, -1);

//	blockSignals( true );
ListCol->insertItem(trUtf8(Header));
//	eColHeader->setText( trUtf8(Header));
//	eColName->setText(trUtf8(Name));
//	eColWidth->setValue(cw);
//	eColTWidth->setValue(tw);
//	eColTDec->setValue(td);
//	blockSignals( false );
if (ListCol->count()) ListCol->setCurrentItem(ListCol->count()-1);

}
*/



void eDBTable::ColumnDel()
{

int idx;
QString str;
idx = ListCol->currentItem();
if (idx != -1 && ListCol->count()>0)
{
	str = fname[idx];
	table->removeColumn(idx);
	deletedFields.append(str);
	deletedFieldsId.append(idlist[idx]);
	idlist.remove(idlist.at(idx));
	fname.remove(fname.at(idx));
	cwidth.remove(cwidth.at(idx));
	ListCol->removeItem(idx);
//	table->removeColumn(idx);
}
}

void eDBTable::ColumnL()
{
 int idx;
 QString str;
 QStringList::Iterator it;
 	idx = ListCol->currentItem();
	if(idx>0)
	{
		str = fname[idx];
		//--it = fname.at(idx);
		//--fname.remove(it);
		fname.removeAt(idx);
		//--it = fname.at(idx-1);
		//--fname.insert(it,str);
		fname.insert(idx-1,str);

		str = idlist[idx];
		//--it = idlist.at(idx);
		//--idlist.remove(it);
		idlist.removeAt(idx);
		//--it = idlist.at(idx-1);
		//--idlist.insert(it,str);
		idlist.insert(idx-1,str);

		str = cwidth[idx];
		//--it = cwidth.at(idx);
		//--cwidth.remove(it);
		cwidth.removeAt(idx);
		//--it = cwidth.at(idx-1);
		//--cwidth.insert(it,str);
		cwidth.insert(idx-1,str);

	 	str = ListCol->currentText();
		ListCol->removeItem(idx);
		ListCol->insertItem(str,idx-1);
		ListCol->setSelected(idx-1,true);
	}
}


void eDBTable::ColumnR()
{
 uint idx;
 QString str;
 QStringList::Iterator it;
 	idx = ListCol->currentItem();
	if(idx<ListCol->count()-1)
	{
		str = fname[idx];
		//--it = fname.at(idx);
		//--fname.remove(it);
		fname.removeAt(idx);
		//--it = fname.at(idx+1);
		//--fname.insert(it,str);
		fname.insert(idx+1,str);

		str = idlist[idx];
		//--it = idlist.at(idx);
		//--idlist.remove(it);
		idlist.removeAt(idx);
		//--it = idlist.at(idx+1);
		//--idlist.insert(it,str);
		idlist.insert(idx+1,str);

		str = cwidth[idx];
		//--it = cwidth.at(idx);
		//--cwidth.remove(it);
		cwidth.removeAt(idx);
		//--it = cwidth.at(idx+1);
		//--cwidth.insert(it,str);
		cwidth.insert(idx+1,str);

		str = ListCol->currentText();
		ListCol->removeItem(idx);
		ListCol->insertItem(str,idx+1);
		ListCol->setSelected(idx+1,true);
	}

}
void eDBTable::ColumnSel(int col)
{
QString str,s;
int ind = 0;
QStringList lst;
	eColHeader->blockSignals(true);
	eColWidth->blockSignals(true);
	eType->clear();
	eColTWidth->setValue(0);
	eColTDec->setValue(0);
	eColHeader->setText("");
	eColWidth->setValue(0);
	eColName->setText("");
//	str = table->getFieldType((const char*) ComboBoxTable->currentText().utf8(),(const char*)ListCol->currentText().utf8());
	//if(eColHeader->text="")
//	ListCol->blockSignals( TRUE );
	//ListCol->changeItem(s, ListCol->currentItem());

//	printf(">>>read property DefHeaders in %s\n", table->name());
//	lst = table->property("DefHeaders").toStringList();
//	printf(">>>ok!\n");
//	ListCol->clear();
//	printf(">>>insert str list\n");
//	ListCol->insertStringList(lst);
//	ListCol->setCurrentItem(col);
//	ListCol->blockSignals( FALSE );
	if(col==-1 || ListCol->count()==0) return;
//	printf(">>>set text in eColHeader\n");
	eColHeader->setText(ListCol->currentText());
	eColHeader->blockSignals(false);
	ind = ListCol->currentItem();
	str = cwidth[ind];
	eColWidth->setValue(atoi(str));
	eColWidth->blockSignals(false);
	str = fname[ind];
	eColName->setText(str);
//	eType->clear();
//	printf(">>>get fields type\n");
//	str = eColName->text();

	str = table->getFieldType(idlist[ind].toInt());
//	parse string with type, num, numdec
	if(!str.isEmpty())
	{
		eType->setText(str.left(1));
		str.remove(0,2);
		if(!str.isEmpty())
		{
//			printf(">>>%s\n",str.ascii());
			eColTWidth->setValue(atoi(str));
		}
		ind = str.find(" ");
		if(ind)// poisk decimal chasti
		{
			s = str.remove(0,str.find(" "));
		}
		else
		{
			str="0";
		}
//		printf(">>>%s\n",str.ascii());
		eColTDec->setValue(atoi(str));
	}
	else
	{
//		printf(">>>unknown field\n");
	}
	/*
	tablefield *f;

	if (col >= 0 && col < tablerow_columns( r )) {
		f = tablerow_column( r, col );
		blockSignals( true );
		eColHeader->setText(trUtf8(tablefield_header( f )));
		eColWidth->setValue(f->sizex);
		eType->setCurrentItem(f->ftypeindex);
		eColName->setText(trUtf8(f->name));
		eColTWidth->setValue(f->flen);
		eColTDec->setValue(f->decimals);
		blockSignals( false );
	}
*/
}


void eDBTable::ColumnUpd()
{
QStringList lst;
int id;
Q3ValueList<int> listTableId;
	id = table->getTableId(ComboBoxTable->currentItem()-1);
	listTableId = table->getBindList();
	if(listTableId.find(id)!=listTableId.end())
	{
		cfg_message(0,tr("table in use, please select another table"));
	//	ComboBoxTable->setCurrentItem(table->getTableInd(table->property("TableInd").toInt()));
		ListCol->clear();
		ListCol->insertItem("table in use!");
		ListCol->setEnabled(false);
		bOK->setEnabled(false);
		ColumnSel(-1); // clear all line edit and labels
		return;
	}
	if(!ListCol->isEnabled()) ListCol->setEnabled(true);
	if(!bOK->isEnabled()) bOK->setEnabled(true);
	lst = table->getFields(id);
	ListCol->clear();
	fname = lst;
	idlist = table->getFields(id,true);
	ListCol->insertStringList(lst,0);
	cwidth.clear();
	for(uint i=0; i<lst.count();i++)
	{
		cwidth << table->property("DefaultColWidth").toString();
	}
	//fname = table->fname;
	findDeletedFields(id,idlist);

}


void eDBTable::getData( wDBTable *t )
{
QStringList lst;
QString str, str2;
int i;
const Q3SqlFieldInfo *f;
	if(ComboBoxTable->count())
	{
	//	t->setProperty("TableInd", tables[ComboBoxTable->currentItem()].section("\t",0,0).toInt())
		t->setProperty("TableInd",t->getTableId(ComboBoxTable->currentItem()-1));
		i=0;
		while(i<t->numCols())
		{
			t->removeColumn(0);
			++i;
		}
		t->cur->clear();
		for(uint j=0; j<ListCol->count(); j++)
		{
			ListCol->setCurrentItem(j);
			str = ListCol->currentText();
			lst << str;
			f = new Q3SqlFieldInfo(fname[j]);
			t->cur->append(*f);
			t->setSqlCursor(t->cur);
			t->addColumn(f->name(),str,atoi(cwidth[j].ascii()));
			t->refresh(Q3DataTable::RefreshColumns);
		}
		t->setProperty("DefHeaders",lst);
		t->setProperty("DefFields",fname);
		t->setProperty("DefIdList",idlist);
		t->setProperty("ColWidth", cwidth);
	}
	else
	{
		t->setProperty("TableInd",-1);
	}
}


void eDBTable::setData( wDBTable * t, aCfg *md )

{
QStringList sl;
QString ft,h,n;
int idTable;
bool ok;
QStringList lst,lst_full;
aCfgItem own;
aWidget *wo;
         table = t;
//        md->print();
//		own =
	//	wo = aWidget::parentContainer( t );
	//	if ( !wo ) return;
	//	tables = md->objTables( wo->getId() );
	//	QString wotype = wo->className();
//		if ( wotype == "wDocument" ) tables.remove( tables[0] );
//		if ( wotype == "wCatalogue" ) tables.remove( tables[1] );
//		printf("tables=\n%s\n", ( const char *) tables.join("\n") );
//		for ( unsigned int i=0; i < tables.count(); i++ ) {
//		    ComboBoxTable->insertItem( tables[ i ].section("\t", 1 ) );
//		}
//	ComboBoxTable->insertItem("None");
	ComboBoxTable->insertStringList(table->list_available_tables, 1);
	idTable = table->property("TableInd").toInt(&ok);
        //    printf("table ind = %i, id table =%i\n", t->getTableInd(idTable), idTable );

        	ComboBoxTable->setCurrentItem(1+t->getTableInd(idTable));
	if(ok && idTable>-1)
	{
        //	v = new  QVariant(lst);
        //	}
        //	else
        //	{
	//	        table->setFields(t->getTableId(0));
	        //	v = new  QVariant(lst);
        		//table->setProperty("DefFields",*v);
        //	}
  //      	v = new  QVariant(lst);
//		table->setProperty("DefHeaders",*v);
	//	table->setProperty("ColWidth",*v);
	//ComboBoxTable->setFocus();
//       	tmp_table = *t;
		lst = table->property("DefHeaders").toStringList();
		ListCol->insertStringList(lst,0);
		cwidth = table->property("ColWidth").toStringList();
		fname = table->property("DefFields").toStringList();
		idlist = table->property("DefIdList").toStringList();
		findDeletedFields(idTable,idlist);
	}
	else
	{
		ColumnUpd();
	}
	ListCol->setFocus();
	/*	if(!ListCol->isEmpty())
		{
			ColumnSel(0);
		}
	*/
	/*
		if (!t->getDefineCols().isEmpty()) {
			sl = QStringList::split("\n",t->getDefineCols());
			for (i = 0; i < sl.count(); i++) {
				sscanf((const char *)sl[i].section("|",3,3),"%s %d %d", st, &l, &d);
				if (st[0]=='O') ft.sprintf("O %d",l);
				else ft=st;
				w = sl[i].section("|",2,2).toInt();
				h = sl[i].section("|",5,5);
				n = sl[i].section("|",4,4);
				for (idxt = otypes.count()-1; idxt>0; idxt--) {
					if (ft[0]=='O' && otypes[idxt]==ft) break;
					else if (otypes[idxt][0]==ft[0]) break;
				}
				printf("set data %i = %s %s %i %i %i\n", i,
				(const char *)h, (const char *)n, idxt, w, l);
				insertColumn(h,n,idxt,w,l,d);
			}
		}
	*/


}


void eDBTable::ColumnTextUpd( const QString &s )
{
CHECK_POINT

	int idx;
	idx = ListCol->currentItem();
	if (idx == -1) return;
	ListCol->blockSignals( TRUE );
	ListCol->changeItem(s, ListCol->currentItem());
	ListCol->blockSignals( FALSE );

}


void eDBTable::doOK()
{
	getData(table);
	accept();
}


void eDBTable::ColumnAdd()
{
QString s;
QString idx;
QStringList::Iterator it;
	addfdialog e( this->topLevelWidget());
	e.setData(deletedFields,deletedFields,deletedFieldsId);
	if ( e.exec()==QDialog::Accepted )
	{
		idx = e.getData(false);
//		printf(">>>idx = %s\n",idx.ascii());
		if(idx=="0") return;
		s = table->getFieldName(idx.toInt());
		idlist << idx;
//		printf(">>> add field = '%s'\n",s.ascii());
		ListCol->insertItem(s,ListCol->count());
		fname << s;
		cwidth << table->property("DefaultColWidth").toString();
		it = deletedFields.find(s);
		deletedFields.remove(it);
		it = deletedFieldsId.find(idx);
		deletedFieldsId.remove(it);
//		table->addColumn(s,s,100);
	}
}


void eDBTable::ColWidthChange( int value )
{
	int ind;
	QString num;
	ind = ListCol->currentItem();
	if(ind!=-1)
	{
		cwidth[ind] = num.setNum(value);
	}

}


QStringList
eDBTable::findDeletedFields( int t_num ,QStringList idls)
{
	QStringList lst_full;
	deletedFields.clear();
	//formiruem list udalennix poley
	lst_full = table->getFields(t_num,true);
	for(unsigned int i=0;i<lst_full.count();i++)
	{
//		printf(">>>field '%s'\n",lst_full[i].ascii());
		if(idls.find(lst_full[i])==idls.end())
		{
			deletedFieldsId << lst_full[i];
			deletedFields << table->getFieldName(lst_full[i].toInt());
		}
	}
return deletedFields;
}


void eDBTable::doCancel()
{
}

