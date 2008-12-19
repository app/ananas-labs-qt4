/****************************************************************************
** $Id: ananas.h,v 1.3 2008/12/10 21:04:03 leader Exp $
**
** Header file of the Ananas Library of Ananas
** Designer and Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
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

/*! \ru
 *\mainpage Проект Ананас. Справочное руководство по библиотеке ananaslib
 *
 *
 * В Справочном руководстве приведено описание классов, составляющих ядро системы Ананас.
 * Документ предназначен для использования в качестве Справочника по интерфейсу прикладного программирования Ананаса.
 * Интерфейс предоставляет доступ к управлению метаданными Ананаса, данными, хранящимися в базе данных,
 * прикладными объектами системы. Справочное руководство предназначено для программистов. Пользовательское
 * описание системы Ананас содержится в Руководстве пользователя.
 *
 * \section intro_sec Введение
 * Ананас - вольная платформа, обеспечивающая ускоренное создание приложений автоматизации учетной деятельности,
 * бизнес процессов организаций и компаний, работающих в различных сферах деятельности.
 * Библиотека ananaslib, являясь частью платформы Ананас, обеспечивает удобный и понятный доступ к реализованным
 * в системе функциям, для программ на языке программирования С++. Настоящее справочное руководство содержит описание
 * методов доступа.
 *
 * \section main_classes_sec Основные классы
 * \subsection adocument_class_sec Класс aDocument
 * Один из важных аспектов методологии работы Ананаса с данными - управление первичными учетными документами.
 * Для отражения хозяйственных операций Ананас предусматривает их регистрацию с помощью электронных документов.
 * Электронные документы могут иметь произвольную структуру, состоящую из набора полей и одной или нескольких
 * табличных частей. Информация о структуре документов хранится в системе. Управление документами осуществляется
 * через экземпляры класса aDocument.

 * \subsection acatalog_class_sec Класс aCatalogue
 * Ананас поддерживает ведение баз данных (картотек) произвольной структуры
 * с возможностью классификации/разнесения элементов по иерархически
 * организованным группам. Типичным примером использования этой возможности является ведение Каталога товаров,
 * Справочника контрагентов, Реестра сотрудников. Унифицированный интерфейс доступа к таким базам данных (картотекам)
 * обеспечивает класс aCatalogue.
 *
 * \subsection adocjournal_class_sec Класс aDocJournal
 * Все электронные документы, хранящиеся в базе данных Ананаса, регистрируются системой в момент их создания в
 * журнале документов. Для работы с журналом документов используется класс aDocJournal.
 *
 *
 * \subsection airegister_class_sec Класс aIRegister, aARegister
 * Ананас поддерживает ведение Регистров. Регисты служат для хронологической записи информации. В системе существует
 * два типа регистров: информационные и накопительные. Накопительный, отличается от информационного поддержкой
 * автоматически пересчитываемых остатков по разрезам учета. Для работы с информационными регистрами используется
 * класс aIRegister. Для работы с накопительными - aARegister.
 *
 * \subsection acfg_class_sec Класс aCfg
 * Ананас разработан с учетом требований адаптируемости к различным сферам учетной деятельности. Для этих целей
 * поддерживается работа с произвольно настраиваемыми структурами данных. Такой подход требует хранения информации,
 * описывающей структуры данных. Такую информацию принято называть метаданными. Для управления структурами данных и для
 * работы с метаданными используется класс aCfg.
 *
 * \subsection aform_class_sec Класс aForm
 * Все бизнес объекты Ананаса, атрибуты которых может редактировать пользователь, имеют хотя бы одну экранную
 * форму, работая с которой, пользователь осуществляет изменение значений атрибутов бизнес объектов.
 * Класс aForm предоставляет набор методов, предназначенных для использования Ананас.Скриптом модуля экранной формы
 * бизнес объекта Ананаса. Проектируя и программируя пользовательский интерфейс (Presentation layer)
 * бизнес схемы Ананаса разработчик имеет дело с объектами класса aForm и расположенными в них виджетами.
 * \_ru
 */

#ifndef ANANAS_H
#define ANANAS_H

#include "acfg.h"
#include "acfgrc.h"
#include "ametadata.h"
#include "adatabase.h"
#include "asqlfield.h"
#include "adatafield.h"
#include "asqltable.h"
#include "aform.h"
#include "engine.h"
#include "aobject.h"
#include "awidget.h"
#include "messageswindow.h"
#include "adocjournal.h"
#include "adocument.h"
#include "acatalogue.h"
#include "airegister.h"
#include "aaregister.h"
#include "atemplate.h"
#include "areport.h"
#include "aextension.h"
#include "aextensionfactory.h"
#include "adataexchange.h"
//#include "dlogin.h"
//#include "deditrc.h"
//#include "dselectdb.h"
#include "atime.h"
#include "auser.h"
#include "arole.h"
#include "aservice.h"
#include "atests.h"
#include "afilter.h"

#define ANANAS_LIBVERSION "0.9.6"
extern const char ANANAS_EXPORT *ananas_libversion();
//extern bool ANANAS_EXPORT ananas_login( QString &rcfile, QString &username, QString &userpassword );
extern bool ANANAS_EXPORT ananas_login( QString &rcfile, QString &username, QString &userpassword, aDatabase *db=0, int appId = 0 );
extern void ANANAS_EXPORT ananas_logout( aDatabase *db=0 );
extern QString ANANAS_EXPORT ananas_objectstr( aDatabase *db, qulonglong uid, int oid );


#endif
