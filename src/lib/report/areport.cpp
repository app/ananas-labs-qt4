/****************************************************************************
** $Id: areport.cpp,v 1.2 2008/11/08 20:16:36 leader Exp $
**
** Report metadata object implementation file of
** Ananas application library
**
** Created : 20040701
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) 2003-2006 Grigory Panov <gr1313 at mail.ru>, Yoshkar-Ola.
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

#include	<qlayout.h>
#include	<q3toolbar.h>
#include	<qaction.h>
#include	<q3vbox.h>
#include	<q3simplerichtext.h>
#include	<qpainter.h>
#include	<q3paintdevicemetrics.h>
#include	<qkeysequence.h>
#include	<qprinter.h>
#include 	<q3process.h>
#include 	<qmessagebox.h>
#include 	<q3filedialog.h>
//Added by qt3to4:
#include <QPixmap>
#include	"acfg.h"
#include	"aobject.h"
#include	"adatabase.h"
#include	"areport.h"
#include	"acalctemplate.h"
#include 	"aservice.h"
#include 	"alog.h"
#include 	"stdlib.h"
/*!
*	\en
*	Constuct object
*	\_en
*	\ru
*	Создает объект
*	\_ru
*/
aReportBrowser::aReportBrowser(  QWidget *parent, const char *name, Qt::WFlags f )
:Q3MainWindow( parent, name, f )
{
	QAction *a;

	Q3ToolBar *t = new Q3ToolBar( this, "ReportTool" );
	a = new QAction(
	//QPixmap::fromMimeSource("print.png"),
	QPixmap(":/images/print.png"),
	tr("Print"),
	QKeySequence("Ctrl+P"),
	t,
	tr("Print report")
	);
	a->addTo( t );
	connect( a, SIGNAL( activated() ), this, SLOT( print() ) );
	t->show();

	textBrowser = new Q3TextBrowser( this, "textBrowser" );
	textBrowser->setTextFormat( Qt::RichText );
	textBrowser->setFocus();
//	textBrowser->showMaximized();
    	setCentralWidget( textBrowser );
//	if ( layout() ) delete layout();
//	QGridLayout *l = new QGridLayout( this );
//	l->addWidget( textBrowser, 1, 0 );
	languageChange();
}



/*!
*	\en
*	Appends text to browser.
*	\_en
*	\ru
*	Добавляет текст в браузер
*	\_ru
*	\param text - \en text to append \_en \ru текст для добавления \_ru
*/
void
aReportBrowser::append( const QString &text )
{
	textBrowser->append( text );
}



/*!
*	\en
*	Clears browser.
*	\_en
*	\ru
*	Очищает браузер
*	\_ru
*/
void
aReportBrowser::clear()
{
	textBrowser->clear();
}



/*!
*	\en
*	Print browser content.
*	\_en
*	\ru
*	Печатает содержимое браузера.
*	\_ru
*/
void
aReportBrowser::print()
{
	QPrinter printer;
	QPainter p;

	if (!printer.setup()) return;
	if ( p.begin( &printer ) ){
            Q3PaintDeviceMetrics metrics( p.device() );
            int dpiy = metrics.logicalDpiY();
            int margin = (int) ( (2/2.54)*dpiy ); // 2 cm margins
            QRect body( margin, margin, metrics.width() - 2*margin, metrics.height() - 2*margin );
            Q3SimpleRichText richText( textBrowser->text(),
                                      QFont(),
                                      textBrowser->context(),
                                      textBrowser->styleSheet(),
                                      textBrowser->mimeSourceFactory(),
                                      body.height() );
            richText.setWidth( &p, body.width() );
            QRect view( body );
            int page = 1;
            do {
                richText.draw( &p, body.left(), body.top(), view, colorGroup() );
                view.moveBy( 0, body.height() );
                p.translate( 0 , -body.height() );
                p.drawText( view.right() - p.fontMetrics().width( QString::number( page ) ),
                            view.bottom() + p.fontMetrics().ascent() + 5, QString::number( page ) );
                if ( view.top()  >= richText.height() )
                    break;
                printer.newPage();
                page++;
            } while (TRUE);
	}
}



/**!
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void aReportBrowser::languageChange()
{
}



/*!
*	\en
*	Constuct object
*	\_en
*	\ru
*	Создает объект
*	\param context - объект метаданных для инициализации.
*	\param report_type - тип отчета. RT_text - для отчета в формате HTML, RT_office - для отчета в формате OpenOffice
*	\param e - енжин.
*	\_ru
*/
aReport::aReport( aCfgItem context, RT_type report_type, aEngine * e )
:aObject( context, 0, 0, "aReport")
{
	engine = e;
	md = e->md;
	browser = new aReportBrowser( e->ws );
	type = report_type;
	if(report_type==RT_text)
	{
		tpl = new aTemplate();
	}
	if(report_type == RT_office_writer)
	{
		tpl = new aOOTemplate();
	}
	if(report_type==RT_office_calc)
	{
		tpl = new aCalcTemplate();
	}
}



/*!
*	\en
*	Constuct object
*	\_en
*	\ru
*	Создает объект
*	\param name - имя объекта.
*	\param report_type - тип отчета. RT_text - для отчета в формате HTML, RT_office - для отчета в формате OpenOffice
*	\param e - енжин.
*	\_ru
*/
aReport::aReport(QString name, RT_type report_type, aEngine * e )
:aObject( name, 0, 0, "aReport")
{
	engine = e;
	md = e->md;
	browser = new aReportBrowser( e->ws );
	type = report_type;
	if(report_type==RT_text)
	{
		tpl = new aTemplate();
	}
	if(report_type == RT_office_writer)
	{
		tpl = new aOOTemplate();
	}
	if(report_type == RT_office_calc)
	{
		tpl = new aCalcTemplate();
	}
}



/*!
*	\en
*	Destructor
*	\_en
*	\ru
*	Деструктор
*	\_ru
*/
aReport::~aReport()
{
	if(tpl)
	{
		delete tpl;
		tpl=0;
	}
}



/*!
*	\en
*	Init object.
*	\_en
*	\ru
*	Инициализирует объект. Объект не содержит никаких таблиц в базе данных.
*	\_ru
*/
ERR_Code
aReport::initObject()
{
	ERR_Code err = aObject::initObject();
	if ( err ) return err;
	return err_noerror;
}



/*!
*	\en
*	Set template.
*	\_en
*	\ru
*	Задает имя файла шаблона. Расширение указывать необходимо.
*	\_ru
*	\param name - \en template name \_en \ru имя шаблона \_ru
*	\see path2workdir()
*/
void
aReport::setTable( const QString &name )
{
	tpl->setDir(path2workdir());
	tpl->open( name );
	tplName = name;
}



/*!
*	\en
*	Sets tag value.
*	\_en
*	\ru
*	Задает значение тега.
*	\_ru
*	\param name - \en tag name \_en \ru имя тега \_ru
*	\param value - \en tag value \_en \ru значение тега \_ru
*/
void
aReport::setValue( const QString &name, const QString &value )
{
	tpl->setValue( name, value );
}



/*!
*	\en
*	Gets tag value.
*	\_en
*	\ru
*	Возвращает значение тега.
*	\_ru
*	\param name - \en tag name \_en \ru имя тега \_ru
*	\return \en tag value \_en \ru значение тега \_ru
*/
QString
aReport::value( const QString &name )
{
	return tpl->getValue( name );
}



/*!
*	\en
*	Clear browser content.
*	\_en
*	\ru
*	Очищает содержимое браузра. Используется для текстово отчета.
*	\_ru
*/
void
aReport::clear()
{
	browser->clear();
}



/*!
*	\en
*	Update report value.
*	\_en
*	\ru
*	Обновляет репорт на основе заданных перед этим функцией setValue() значений.
*	\_ru
*/
void
aReport::exec( const QString &section )
{
	tpl->exec( section );
}



/*!
*	\en
*	Shows report.
*	\_en
*	\ru
*	Показывает отчет.
*	\_ru
*/
void
aReport::show()
{
	if(type==RT_text)
	{
		browser->textBrowser->setText( tpl->result() );
		browser->show();
	}
	if(type==RT_office_writer || type==RT_office_calc)
	{
		QString fileName = getName4NewTemplate();
		tpl->cleanUpTags();
		tpl->save(fileName);
		QString startCatalog = "/usr/bin";
		QString filter;
		bool sofficeFound = false;

		bool ok;
		QString oowriter = aService::readConfigVariable("OpenOfficeExecutable", &ok);
		if(!ok || oowriter == "" )
		{
#ifdef Q_OS_WIN32
			aLog::print(aLog::Info,tr("aReport OpenOfficeExecutable not found in registery"));
			startCatalog = QString("%1\\Program Files\\OpenOffice.org1.1.1\\program").arg(getenv("HOMEDRIVE"));
			QFile soffice(startCatalog + "\\soffice.exe");
			if(!soffice.exists())
			{
				startCatalog = QString("%1\\Program Files\\OpenOffice.org 2.0\\program").arg(getenv("HOMEDRIVE"));
			        soffice.setName(startCatalog + "\\soffice.exe");
				if(!soffice.exists())
				{
					startCatalog = QString("%1\\Program Files").arg(getenv("HOMEDRIVE"));
				}
				else
				{
					aService::writeConfigVariable("OpenOfficeExecutable",startCatalog + "\\soffice.exe");
					aLog::print(aLog::Info,tr("aReport OpenOfficeExecutable found %1").arg(startCatalog + "\\soffice.exe"));
				}
			}
			else
			{
				aService::writeConfigVariable("OpenOfficeExecutable",startCatalog + "\\soffice.exe");
				aLog::print(aLog::Info,tr("aReport OpenOfficeExecutable found %1").arg(startCatalog + "\\soffice.exe"));
			}
			filter = "Executable files (*.exe)";
#else
			aLog::print(aLog::Info,tr("aReport OpenOfficeExecutable not found in ~/.ananas/configrc"));
			QFile soffice(startCatalog + "/ooffice");
			if(!soffice.exists())
			{
				soffice.setName(startCatalog + "/ooffice2");
				if(soffice.exists())
				{
					aService::writeConfigVariable("OpenOfficeExecutable",startCatalog + "/ooffice2");
					aLog::print(aLog::Info,tr("aReport OpenOfficeExecutable found %1").arg(startCatalog + "/ooffice2"));
				}
			}
			else
			{
				aService::writeConfigVariable("OpenOfficeExecutable",startCatalog + "/ooffice");
				aLog::print(aLog::Info,tr("aReport OpenOfficeExecutable found %1").arg(startCatalog + "/ooffice"));
			}
#endif
		}
		oowriter = aService::readConfigVariable("OpenOfficeExecutable", &ok);
		if(!ok || oowriter == "" )
		{

			Q3FileDialog dlg(0,"select_file_dialog",true);
			dlg.addFilter( filter );
			dlg.setMode(Q3FileDialog::ExistingFile);
			dlg.setDir(QDir(startCatalog));
			dlg.setCaption("Для отображения отчета необходим OpenOffice. Укажите исполняемый файл OpenOffice");
			if(dlg.exec()==QDialog::Accepted)
			{
				oowriter = dlg.selectedFile();
				//printf("select %s", oowriter.ascii());
				Q3Process process( oowriter );
				process.addArgument( "-n" );
				process.addArgument( QDir::convertSeparators( fileName ) );
				if( !process.start() )
				{
					QMessageBox::warning(0, tr("Warning"), tr("Unable to start OpenOffice (%1)").arg(oowriter), QMessageBox::Ok,QMessageBox::NoButton);
//			 printf("Unable to start OpenOffice Writer\n");
				}
				else
				{
					aService::writeConfigVariable("OpenOfficeExecutable",oowriter);
					aLog::print(aLog::Info,tr("aReport OpenOfficeExecutable set to %1").arg(oowriter));
				}
			}
		}
		else
		{

			Q3Process process( oowriter );
			process.addArgument( "-n" );
			process.addArgument( QDir::convertSeparators( fileName ) );
			if( !process.start() )
			{
				QMessageBox::warning(0, tr("Warning"), tr("Unable to start OpenOffice (%1)").arg(oowriter), QMessageBox::Ok,QMessageBox::NoButton);
//			 printf("Unable to start OpenOffice Writer\n");
			}
			else
			{
				aService::writeConfigVariable("OpenOfficeExecutable",oowriter);
				aLog::print(aLog::Info,tr("aReport OpenOfficeExecutable set to %1").arg(oowriter));
			}
		}

		//tpl->open( tplName );
	}
}



/*!
*	\en
*	Return path to working dir.
*	\_en
*	\ru
*	Возвращает путь к рабочей директории.
*	\_ru
* 	\see setTable( const QString &name )
*/
QString
aReport::path2workdir()
{
	QString res;
#ifndef Q_OS_WIN32
	res = "/usr/share/ananas/";
#else
	res = "";
#endif
	if(md!=NULL)
	{
		res = md->rc.value("workdir");
	}
	aLog::print(aLog::Debug, tr("aReport working dir = %1").arg(res));
	return res;
}



/*!
*	\en
*	Close report.
*	\_en
*	\ru
*	Закрывает отчет. Удаляет временные файлы.
*	\_ru
*/
void
aReport::close()
{
	tpl->close();
}


/*!
*	\en
*	Gets name for new template. Need for OpenOffice v2.
*	\_en
*	\ru
*	Получение имени для нового шаблона. Нужна из-за блокировок в OpenOffice v2.
*	\_ru
*/
QString
aReport::getName4NewTemplate()
{
	uint count=0;
	QFile tmpf;
	QString suff = ".odt";
	QString fname;
	if(type==RT_office_calc) suff = ".ods";
	do
	{
		// tpl->getDir() должно заканчиваться на /
		fname =  QDir::convertSeparators(QString(tpl->getDir()+".ananas-report%1%2").arg(count).arg(suff));
		tmpf.setName(fname);
		if(tmpf.exists())
		{
			if(tmpf.remove()) break;
			else ++count;
		}
		else
		{
			break;
		}
	}while(count<100);

	aLog::print(aLog::Debug, tr("aReport name for template = %1").arg(fname));
	return fname;
}
