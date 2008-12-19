#include "addfdialog.h"

#include <qvariant.h>
#include "wdbfield.h"

#include <Q3ValueList>
/*
 *  Constructs a addfdialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
addfdialog::addfdialog(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
addfdialog::~addfdialog()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void addfdialog::languageChange()
{
    retranslateUi(this);
}

/*!
 * Sets current field name and field id for return with function getData()
 */

void addfdialog::doOk()
{
	int idx;
	idx = ListBox->currentItem();
	if(idx>=0)
	{
	        id = idlist[idx];
		field = list[idx];
	}
	else id="0";
	accept();
}


void addfdialog::init()
{
	id="0";
	field="";
}


void addfdialog::destroy()
{

}


/*!
 * Function return field name (if param name == true) or field id otherwise
 * \return field name or field id
 */
const QString addfdialog::getData(bool name)
{
	if(name) return field;
	else return id;
}

/*!
 * Sets list of field name and list of field id.
 * \param flst (in) - list of field name
 * \param ilst (in) - list of field id
 */
void addfdialog::setData( QStringList displst, QStringList flst, QStringList ilds)
{
	list = flst;
	ListBox->insertStringList(displst);
	idlist = ilds;
}




void addfdialog::setData( QWidget *o, aCfg *md )
{

	wDBField *w = (wDBField *)o;
	w->getFields();
	setData( w->defDisplayFields, w->defFields,w->defId);
	ListBox->setCurrentItem(ListBox->findItem(w->getFieldName()));
}


void addfdialog::getData( QWidget *o )
{
	QString s;
	wDBField *w = (wDBField *)o;
	Q3ValueList<qulonglong> bindList = w->getBindList();
		s = getData(true);
		if(s.isEmpty()) return;
		w->setFieldName(s);
		s = getData(false);
		if(s=="0") return;
		// if select binding field
		if(bindList.find(s.toULongLong()) != bindList.end())
		{
			cfg_message(0, tr("field already binding, please select another field."));
			return;
		}
		w->setId(s.toInt());
		w->setEditorType();
    		//w->updateProp();

}
