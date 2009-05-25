/****************************************************************************
** $Id: aextsql.h,v 1.1 2009/05/25 16:35:10 app Exp $
**
** Header file of the Report Result Object of Ananas
** Designer and Engine applications
**
** Created : 20070819
**
** Copyright (C) 2005-2007 Grigory Panov <grigory.panov at gmail.com>, Moscow.
** Copyright (C) 2005-2007 Ananas Project.
**
** This file is part of the Library of the Ananas
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
#ifndef AEXTSQL_H
#define AEXTSQL_H


#include <qobject.h>
#include <Q3SqlSelectCursor>
#include "adatabase.h"
#include "aextension.h"
#include "aobject.h"

/**
 *	\en
 *	\_en
 *	\ru
 *		\brief Предоставляет возможность использования SQL запросов в отчетах.
 *
 *		Пример использования. Код взят из модуля экранной формы отчета "Остатки товаров".
 *		\code
 *		// Oбрабатывает пользовательские нажатия на кнопки экранной формы
 *		function on_button(name)
 *		{
 *		    if(name == "report_button")
 *		    {
 *			generate_report();
 *		    }
 *		    if(name == "run_button")
 *		    {
 *			run_report();
 *		    }
 *		}
 *		
 *		// Формирует текст SQL запроса и в демонстрационных целях помещает его в поле report_text
 *		// пользовательского диалога для показа пользователю.
 *		function generate_report()
 *		{
 *		       var cat = new Catalogue("Каталог товаров");
 *		       var reg = new ARegister("Главная книга");
 *		       var sql = new SQL();
 *		       var innerQuery = "SELECT "+
 *		                             sql.SqlFieldName(cat, "Наименование") + ", " +
 *		                             sql.SqlFieldName(cat, "Артикул") + ", " +
 *		                             sql.SqlFieldName(reg, "Количество") + " as Amount, " +
 *		                             "idg " +
 *		                             "FROM " + sql.SqlTableName(cat) + " as CAT " +
 *		                             "LEFT JOIN " + sql.SqlTableName(reg, "Товар" ) + " as SALDO " +
 *		                             "on CAT.id = SALDO." + sql.SqlFieldName(reg, "Товар") + " " +
 *		                             "GROUP BY " + sql.SqlFieldName(cat, "Наименование") + " " +
 *		                             "HAVING max(SALDO.date) ";
 *		       var groupQuery = "SELECT sum( Amount ) as summ, "+
 *		                                 "t.idg, " +
 *		                                  sql.SqlFieldName(cat, "Название группы", "group") + " as gname " +
 *		                                 "FROM (" + innerQuery + ") as t " +
 *		                                 "LEFT JOIN " + sql.SqlTableName(cat, "group") + " as GR " +
 *		                                 "on GR.id = t.idg GROUP BY t.idg";
 *
 *		       var outerQuery = "SELECT G.gname, G.summ, E.* from ( " + innerQuery + " ) as E " +
 *		                                 "LEFT JOIN ( " + groupQuery + " ) as G " +
 *		                                 "on E.idg = G.idg";
 *		        Widget("report_text").text = outerQuery;
 *		}
 *
 *		// Запускает на исполнение SQL запрос, сформированный функцией generate_report()
 *		// и строит отчет в OpenOffice.org Calc c использованием полученных данных.
 *		function run_report()
 *		{
 *		       var sql = new SQL();
 *		       if( !sql.ExecQuery(Widget("report_text").text) ) 
 *		       {
 *			   sys.Message(1,"you have error in your sql query:" + sql.LastError());
 *			   return;
 *		       }
 *		       var p = new Report("templ_ostatki.ods",2);
 *		       p.setTable("templ_ostatki.ods");
 *		       if(sql.First())
 *		       {
 *			    do{
 *				 p.setValue("f0",sql.Value(2));
 *		                             p.setValue("f1",sql.Value(4)); 
 *		                             p.setValue("f2",sql.Value(0)); 
 *		                             p.setValue("f3",sql.Value(1)); 
 *		                             p.exec("строка");                
 *			    } while(sql.Next());
 *		        }
 *		        p.setValue("param", ConvertDateFromIso(Date()));
 *		        p.exec("Текущая дата");
 *		        p.show();
 *		        p.close();
 *		}
 *		\endcode
 *	\_ru
 *
 *
 *
 */

class ANANAS_EXPORT aExtSQL: public AExtension
{
	Q_OBJECT
public:
	aExtSQL();
	virtual int init( aDatabase *database );
	~aExtSQL();
private:
	Q3SqlSelectCursor *cursor;
public slots:
	Q3SqlSelectCursor *	Cursor() const;
	QVariant		Value(int col) const;
	int			Size() const;
	int 			Count() const;
	bool			ExecQuery( const QString & query);
	QVariant 		ExecScalar( const QString & query);
	bool			First();
	bool			Next();
	bool			Last();
	bool			Prev();
	QString			SqlFieldName(aObject * obj, const QString & userFieldName, const QString &tableType="") const;
	QString			SqlTableName(aObject * obj, const QString &tableType="") const;
	QString			SqlTableName(const QString & objName) const;
	QString			SqlFieldName(const QString & fieldName) const;
	QString			LastError() const;
};

#endif
