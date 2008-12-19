#include "deditcfg.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

#include "acfg.h"
#include "qlayout.h"
#include "qsproject.h"
#include "qsscript.h"
//#include "qseditor.h"
#include <qsinterpreter.h>
#include <qstatusbar.h>

#ifndef QSA_NO_IDE
#include "qsworkbench.h"
#endif

/*
 *  Constructs a dEditCfg as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
dEditCfg::dEditCfg(QWidget* parent, const char* name, Qt::WindowFlags fl)
    : Q3MainWindow(parent, name, fl)
{
    setupUi(this);

    (void)statusBar();
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
dEditCfg::~dEditCfg()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void dEditCfg::languageChange()
{
    retranslateUi(this);
}

void dEditCfg::init()
{
	delete statusBar();

	eDBType->clear();
	eDBType->insertItem(trUtf8("Unknown"), 0);
	eDBType->insertItem(trUtf8("MySQL"), 1);
	eDBType->insertItem(trUtf8("Postgres"), 2);
//	QGridLayout *l=new QGridLayout(tabWidget->page(2));
//	eModule=NULL;
//	eModule = new QSEditor(tabWidget->page(2), "global module");
//	l->addWidget(eModule, 5, 5, 0);
	eModule->setInterpreter(new QSInterpreter());
}



void dEditCfg::destroy()
{
    updateMD();
    ( (MainForm*)this->topLevelWidget() )->wl->remove( this );
    ( (MainForm*)this->topLevelWidget() )->removeTab(name());
    //	item->editor = 0;
}


void dEditCfg::setData( aListViewItem *o )
{
//	int n;
//	char to[100]="";
//	cfg_objptr code, remark, context=NULL;
//	char	*id0;
            item = o;
            aCfg *md = item->md;
	aCfgItem obj = item->obj;

	QString v;
//	int rc=0;

	setCaption( tr("Business scheme parameters: ") + md->info( md_info_name ) );
	v = md->rc.value("dbtype");
	eDBType->setCurrentItem(0);
	if ( v == "postgres" ) eDBType->setCurrentItem(2);
	if ( v == "mysql" ) eDBType->setCurrentItem(1);

	eCfgName->setText( md->rc.value("configfile") );
	eWorkDir->setText( md->rc.value("workdir") );
	eDBHost->setText( md->rc.value("dbhost") );
	eDBName->setText( md->rc.value("dbname") );
	eDBHeader->setText( md->rc.value("dbtitle") );
	eDBPass->setText( md->rc.value("dbpass") );
	eDBUserName->setText( md->rc.value("dbuser") );
	eName->setText( md->info( md_info_name ) );
	eAuthor->setText( md->info( md_info_author ) );
	eDate->setText( md->info( md_info_date ) );
	eDescription->setText( md->info( md_info_remark ) );
	globals = md->find( obj, md_globals, 0 );
	eModule->setText( md->sText( globals, md_sourcecode ) );

/*
	n=cfgobj_count(context, aot_remark);
	if (n) {
		id0=cfgobj_idn(context, aot_remark, 1, &remark);
		if (remark) eRemark->setText(trUtf8(cfgobj_text(remark)));
	}
	n=cfgobj_count(context, aot_sourcecode);
	if (n) {
		id0=cfgobj_idn(context, aot_sourcecode, 1, &code);
		if (code) eModule->setText(trUtf8(cfgobj_text(code)));
	}
*/
}



void dEditCfg::updateMD()
{
//	int n;
//	char to[100]="";
//	cfg_objptr code, remark, context=NULL;
//	char	*id;
//	cfg_objptr o;
	aCfg *md = item->md;
	aCfgItem obj = item->obj;

	md->setInfo( md_info_name, eName->text() );
	md->setInfo( md_info_remark, eDescription->text() );
	md->setInfo( md_info_author, eAuthor->text() );
	md->setInfo( md_info_date, eDate->text() );
	md->rc.setValue( "configfile", eCfgName->text().stripWhiteSpace() );
	md->rc.setValue( "workdir", eWorkDir->text().stripWhiteSpace() );
	md->rc.setValue( "dbhost", eDBHost->text().stripWhiteSpace() );
	md->rc.setValue( "dbtitle", eDBHeader->text().stripWhiteSpace() );
	md->rc.setValue( "dbname", eDBName->text().stripWhiteSpace() );
	md->rc.setValue( "dbpass", eDBPass->text().stripWhiteSpace() );
	md->rc.setValue( "dbuser", eDBUserName->text().stripWhiteSpace() );
	if (eDBType->currentItem()==1) md->rc.setValue( "dbtype", "mysql" );
	if (eDBType->currentItem()==2) md->rc.setValue( "dbtype", "postgres" );
	globals = md->find( obj, md_globals, 0 );
	md->setSText( globals, md_sourcecode, eModule->text() );

/*
	if (cancelupdate) return;
	item->setText(0, eName->text());
//	item->setText(1, eID->text());
//	item->setText(2, eType->text());
//	o=cfgobj_find(eID->text());
//	if (o) {
//		printf("founded id=%s!\n", cfgobj_attr(o,"id"));
	cfg_setinfo("name", eName->text().utf8());
	n=cfgobj_count(context, aot_remark);
	if (!n) remark=cfgobj_new(context, aot_remark, trUtf8("Комментарий"), NULL);
	else id=cfgobj_idn(context, aot_remark, 1, &remark);
	if (remark) cfgobj_settext(remark, eRemark->text().utf8());
	n=cfgobj_count(context, aot_sourcecode);
	if (!n) code=cfgobj_new(context, aot_sourcecode, trUtf8("Глобальный модуль"), NULL);
	else id=cfgobj_idn(context, aot_sourcecode, 1, &code);
	if (code) cfgobj_settext(code, eModule->text().utf8());

*/
}
