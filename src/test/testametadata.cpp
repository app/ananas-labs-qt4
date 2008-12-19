/****************************************************************************
** $Id: testametadata.cpp,v 1.5 2008/12/15 22:22:48 leader Exp $
** 
** Tool for automatic running Ananas tests
**
** Created : 20081119
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

#include "testametadata.h"
#include "ametadataioxml.h"
#include "ametafield.h"
#include "ananas.h"
#include <QtTest/QtTest>


void TestAMetaData::initTestCase()
{
//    qDebug("TEST INIT");
//    md = new AMetaData(QString("КлассОбъекта"), fromUtf8("ИмяОбъекта"),);
//    mdo = new AMetaObject();
}



void TestAMetaData::cleanupTestCase()
{
//    qDebug("TEST DONE");
//    delete md;
//    delete mdo;
}

/*
void TestAMetaData::toUpper_data()
{
    QTest::addColumn<QString>("string");
    QTest::addColumn<QString>("result");

    QTest::newRow("all lower") << "hello" << "HELLO";
    QTest::newRow("mixed")     << "Hello" << "HELLO";
    QTest::newRow("all upper") << "HELLO" << "HELLO";
}

void TestAMetaData::toUpper()
{
    QFETCH(QString, string);
    QFETCH(QString, result);

    QCOMPARE(string.toUpper(), result);
}
*/

void TestAMetaData::testAMetaObject()
{
    QByteArray ba("1234567890АБВГД");
    QString ts = QString::fromUtf8("Проверочный текст UTF8");

    AMetaObject *o = new AMetaObject(QString::fromUtf8("КлассОбъекта"), 
                                     QString::fromUtf8("ИмяОбъекта"), 0 );

    o->setDescription( ts );
    QCOMPARE( o->description(), ts );
    
    QCOMPARE( o->className(), QString::fromUtf8("КлассОбъекта"));
    QCOMPARE( o->name(),      QString::fromUtf8("ИмяОбъекта"));

    o->setId(123456);
    QVERIFY( o->id()==123456);

    o->setAttr("String","String");
    o->setAttr("Int", 123456 );
    o->setText("Text", QString::fromUtf8("Проверочный текст"));
    o->setRawdata("RAW1", ba );
    o->setRawdata("RAW2", ba );

// Test attr functions
    QVERIFY( o->attrCount() == 2 );
    QVERIFY( o->attrExists("String") );
    QVERIFY( o->attrExists("Int") );
    QVERIFY( o->attrExists("IntX") == false );
    QVERIFY( o->attr("String") == "String" );
    QVERIFY( o->attr("Int") == 123456 );
    QVERIFY( o->attr( 0 ) == "String" );
    QVERIFY( o->attr( 1 ) == 123456 );
    QVERIFY( o->attr( -1 ) == QVariant() );
    QVERIFY( o->attr( 10 ) == QVariant() );
// Test text functions
    QVERIFY( o->textCount() == 1+1 );
    QVERIFY( o->textExists("Text") );
    QVERIFY( o->textExists("Text0") == false );
    QVERIFY( o->text("Text") == QString::fromUtf8("Проверочный текст") );
    QVERIFY( o->text( 1 ) == QString::fromUtf8("Проверочный текст") );
    QVERIFY( o->text( -1 ) == QString::null );
    QVERIFY( o->text( 10 ) == QString::null );
// Test rawdata functions
    QVERIFY( o->rawdataCount() == 2 );
    QVERIFY( o->rawdataExists("RAW1") );
    QVERIFY( o->rawdataExists("RAW2") );
    QVERIFY( o->rawdataExists("RAW3") == false );
    QVERIFY( o->rawdata("RAW1") == QString( ba.data() ) );
    QVERIFY( o->rawdata("RAW2") == QString( ba.data() ) );
    QVERIFY( o->rawdata( 0 ) == QString( ba.data() ) );
    QVERIFY( o->rawdata( 1 ) == QString( ba.data() ) );
    QVERIFY( o->rawdata( -1 ) == QByteArray() );
    QVERIFY( o->rawdata( 10 ) == QByteArray() );
    delete o;
}



void TestAMetaData::testAMetaObjectGroup()
{
    AMetaObjectGroup *o = new AMetaObjectGroup( QString::fromUtf8("ИмяОбъекта") );

    QCOMPARE( o->name(),      QString::fromUtf8("ИмяОбъекта"));
    delete o;
}



void TestAMetaData::testAMetaData()
{
    AMetaData *o = new AMetaData();
    QCOMPARE( o->name(), QString("MetaData") );
    delete o;
}


void TestAMetaData::testAMetaField()
{
    AMetaField *o = new AMetaField();
    QCOMPARE( o->className(), QString("MetaField") );
    QCOMPARE( o->description(), QString("") );
    QCOMPARE( o->fieldType(), 0 );
    QCOMPARE( o->fieldSubType(), 0 );
    QCOMPARE( o->width(), 0 );
    QCOMPARE( o->notNull(), false );
    QCOMPARE( o->notNegative(), false );
    QCOMPARE( o->calcSumm(), false );
    delete o;
}


void TestAMetaData::testAMetaDataIOXML()
{
    AMetaDataIOXML *o = new AMetaDataIOXML();
    QString xml;
    
    QCOMPARE( o->write("test.cfg"), 0 );
    QCOMPARE( o->read("test1.cfg"), 0 );
    xml = o->text();
    QCOMPARE( o->write("test2.cfg"), 0 );
    QCOMPARE( o->read("test2.cfg"), 0 );
    QCOMPARE( xml, o->text() );
    //QCOMPARE( o->name(), QString("ROOT") );
    delete o;
}
