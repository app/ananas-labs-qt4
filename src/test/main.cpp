/****************************************************************************
** $Id: main.cpp,v 1.3 2008/12/06 22:16:55 leader Exp $
** 
** Tool for automatic running Ananas tests
**
** Created : 20051024
**
** Copyright (C) 2008. Valery Grazhdankin <vg at leaderit.ru>, Moscow
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

#include "ananas.h"
#include <QApplication>
#include <QtTest/QtTest>

#include "testametadata.h"

int main(int argc, char *argv[]) 
{ 
    QApplication app(argc, argv); 
    int rc = 0;
    
//    TestQString test1;
    TestAMetaData test2;
    
//    rc += QTest::qExec(&test1, argc, argv);
    rc += QTest::qExec(&test2, argc, argv); 
    printf("\nTotal fails: %i\n",rc);
    return rc; 
}
