/****************************************************************************
** $Id: amoney.cpp,v 1.1 2008/11/05 21:16:28 leader Exp $
**
** Service functions header file of
** Ananas application library
**
** Copyright (C) 2007 Andrey Paskal.
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
**
**********************************************************************/

#include "amoney.h"
#include	<math.h>


/**
 * \ru
 *		\brief Конструктор
 * \_ru
 */
AMoney::AMoney(){
	this->amount = 0.0;
	this->degree = new Degree();
	this->currency = new Currency("RUR");
}


/**
 * \ru
 *		\brief Конструктор
 * 		\param initialValue - первоначальное количество денег
 * 		\param currency - тип валюты. По умолчанию "RUR". Допустимые значения: "RUR" - рубль, "EUR" - евро, "USD" - доллары США
 * \_ru
 */
AMoney::AMoney(double initialValue, QString currency){
	this->amount = initialValue;
	this->degree = new Degree();
	this->currency = new Currency( currency );
}


/**
 * \ru
 *		\brief Конвертор в строку
 * 		Периводит числовое значение к значению типа QString. Для преобразования числа в текст (пропись)
 * 		используйте метод toText()
 * \_ru
 */
QString
AMoney::toString(){
	return QString::number( this->amount );
}


/**
 * \ru
 *		\brief Сравнивает два экземпляра объектов данного класса.
 *
 * \_ru
 */
bool
AMoney::equal( AMoney &money){
	return this->amount == money.getAmount();
}


/**
 * \ru
 *		\brief Возвращает количество денег.
 *
 * \_ru
 */
double
AMoney::getAmount() {
	return this->amount;
}

/**
 * \ru
 *		\brief Задает количество денег.
 * 		Валюта не меняется. Если были рубли, то и остаються рубли.
 * \_ru
 */
void
AMoney::setAmount(double val) {
	this->amount = val;
}

/**
 * \ru
 *		\brief Задает количество денег в требуемой валюте.
 * 		\param cur - валюта. Допустимые значения смотри в описании конструктора.
 * \_ru
 */
void
AMoney::setAmount(double val, QString cur) {
	this->amount = val;
	this->currency->setId( cur );
}

/**
 * \ru
 *		\brief Возвращает валюту хранимой суммы.
 * 		Возможные значения смотри в описании конструктора.
 * \_ru
 */
QString
AMoney::getCurrency(){
	return this->currency->getId();
}

/**
 * \ru
 *		\brief Задает валюту хранимой суммы.
 * 		Возможные значения смотри в описании конструктора.
 * \_ru
 */
void
AMoney::setCurrency(QString cur) {
	this->currency->setId( cur );
}

/**
 * \ru
 *		\brief Конвертор суммы в пропись. Публичный интерфейс.
 * 		Возвращает значение хранимой суммы прописью с учетом валюты.
 * 		Например, "Сто долларов США 20 центов"
 * \_ru
 */
QString
AMoney::toText(){

	uint integerKopeiki;
	qulonglong integerRubli;

	integerRubli = QVariant( this->amount).toULongLong();

	double decimal = this->amount-integerRubli;

#ifdef Q_OS_UNIX
	integerKopeiki = round( decimal*100.0 );
#else
	integerKopeiki = decimal * 100.0;
#endif

	QString rubliText;
	if ( integerRubli > 0 ) {
		rubliText = valueToText( integerRubli );
	} else {
		rubliText = "Ноль";
	}

	QString kopeikiText;
	if ( integerKopeiki > 0 ) {
		kopeikiText = decimalValueToText( integerKopeiki );
	} else {
		kopeikiText = "ноль";
	}
	return this->firstUp(
			rubliText + " " + this->integerCurrencyName( rubliText ) + " " +
			QString::number( integerKopeiki ) + " " + this->decimalCurrencyName( kopeikiText )
		);
}


/**
 * \ru
 *  	\brief Собирает строку из текстовых троек, вставляя между ними слова "сотен", "тысяч", "миллионов", ...
 *
 * 		Внутренний метод. Не предназнвчен для внешнего использования.
 * 		\param  value - числовое значение целого типа, которое необходимо перевести в пропись.
 * 		\return возвращает пропись для заданного числа без валюты
 * \_ru
 */
QString
AMoney::valueToText( qulonglong value ){
	QString sValue = QString::number( value);
	QString tripleText;
	QString coupleSingleText;
	QString degreeText;
	QString result;
	this->degree->setValue(0);

	while ( sValue.length() >= 3 ) {
		if ( !sValue.endsWith("000")) {
			tripleText = tripleToText( sValue.right(3) );
			result = tripleText + this->degreeText( tripleText, this->degree->getValue() ) + result ;
		}
		sValue = sValue.left( sValue.length() - 3);
		this->degree->setValue( this->degree->getValue() +1);

	}
	if (sValue.length() > 0) {
		if (sValue.length() == 2) {
			coupleSingleText = coupleToText( sValue );
		} else {
			coupleSingleText = singleToText( sValue );
		}
		degreeText = this->degreeText( coupleSingleText, this->degree->getValue() );
		if ( result != "" && (coupleSingleText + degreeText) != "") {
			result =  coupleSingleText + degreeText + " " + result ;
		} else {
			result =  coupleSingleText + degreeText + result ;
		}
	}
	return result;
}


/**
 * \ru
 *  	\brief Вспомогательная функция для копеек, центов....
 *
 * 		Используется для перевода цифры в пропись для дробной части суммы.
 * 		Эта функция необходима, так как дробная часть может имерь род отличный от целой части суммы.
 * 		Например, рубль - муж.род, копейка - жен.род.
 *
 * \_ru
 */
QString
AMoney::decimalValueToText( qulonglong value ){
	QString currencyId = this->currency->getId();
	this->currency->setId( "d"+ currencyId);
	QString result = valueToText( value );
	this->currency->setId( currencyId );
	return result;
}

/**
 * \ru
 *  	\brief Преобразует однозначное число в текст.
 *
 * 		Учитывается род Муж. или Жен. преобразуемого числа
 * 		\param string - строка, содержащая один символ [0-9]
 * 		\return  возвращает "один" или "одна" или "два" или "две" или "три" ..."девять".
 *
 * \_ru
 */
QString
AMoney::singleToText( QString string ) {

	bool male;

	if ( this->degree->getValue() > 0 ) {
		male = this->degree->isMale();
	} else {
		male = this->currency->isMale();
	}

	if ( male ) {
		if (string == "1") return "один";
		if (string == "2") return "два";
	} else {
		if (string == "1") return "одна";
		if (string == "2") return "две";
	}
	if (string == "0") return "";
	if (string == "3") return "три";
	if (string == "4") return "четыре";
	if (string == "5") return "пять";
	if (string == "6") return "шесть";
	if (string == "7") return "семь";
	if (string == "8") return "восемь";
	if (string == "9") return "девять";

	return "undefinded";
}

/**
 * \ru
 *  	\brief Преобразует двузначное число в текст.
 *
 * 		\param string - строка, содержащая два символа [0-9][0-9]
 * 		\return  возвращает пропись двузначного числа.
 *
 * \_ru
 */
QString
AMoney::coupleToText( QString string ) {

	QString result = "";

	if (string.left(1) == "0") return singleToText( string.right(1) );

	if (string == "10") return "десять";
	if (string == "11") return "одинадцать";
	if (string == "12") return "двенадцать";
	if (string == "13") return "тринадцать";
	if (string == "14") return "четырнадцать";
	if (string == "15") return "пятнадцать";
	if (string == "16") return "шестьнадцать";
	if (string == "17") return "семьнадцать";
	if (string == "18") return "восемьнадцать";
	if (string == "19") return "девятьнадцать";

	if (string.left(1) == "2" ) result = "двадцать";
	if (string.left(1) == "3" ) result = "тридцать";
	if (string.left(1) == "4" ) result = "сорок";
	if (string.left(1) == "5" ) result = "пятьдесят";
	if (string.left(1) == "6" ) result = "шестьдесят";
	if (string.left(1) == "7" ) result = "семьдесят";
	if (string.left(1) == "8" ) result = "восемьдесят";
	if (string.left(1) == "9" ) result = "девяносто";


	if (string.right(1) != "0") {
		result += " " + singleToText( string.right(1) );
	}

	return result;
}

/**
 * \ru
 *  	\brief Преобразует трехначное число в текст.
 *
 * 		\param string - строка, содержащая три символа [0-9][0-9][0-9]
 * 		\return  возвращает пропись трехзначного числа.
 *
 * \_ru
 */
QString
AMoney::tripleToText( QString string ) {
	QString result = "";

	if ( string.left(1) == "0" ) return coupleToText( string.right(2) );

	if ( string.left(1) == "1" ) result = "сто";
	if ( string.left(1) == "2" ) result = "двести";
	if ( string.left(1) == "3" ) result = "триста";
	if ( string.left(1) == "4" ) result = "четыреста";
	if ( string.left(1) == "5" ) result = "пятьсот";
	if ( string.left(1) == "6" ) result = "шестьсот";
	if ( string.left(1) == "7" ) result = "семьсот";
	if ( string.left(1) == "8" ) result = "восемьсот";
	if ( string.left(1) == "9" ) result = "девятьсот";

	return result + coupleToText( string.right(2) );
}

/**
 * \ru
 * 		\brief Возвращает слово "тысячи", "миллионы", "миллиарды" в  нужной численности.
 * 		\param string - пропись числа, для которого нужно вернуть название порядка
 * 		\param 	degreeLevel - значение порядка. 1 - тысячи, 2 - миллионы, 3 - миллиарды, 4 - триллионы.
 * \_ru
 */
QString
AMoney::degreeText( QString string, int degreeLevel) {
	QString result = "";

	if ( degreeLevel < 1 ) return result;

	if (degreeLevel == 1) {
		if ( string.endsWith("одна") ) {
			result = " тысяча";
		} else 	if ( string.endsWith("две") || string.endsWith("три") || string.endsWith("четыре") ) {
			result = " тысячи";
		} else {
			result = " тысяч";
		}
	} else  if (degreeLevel == 2) {
		if ( string.endsWith("один") ) {
			result = " миллион";
		} else 	if ( string.endsWith("два") || string.endsWith("три") || string.endsWith("четыре") ) {
			result = " миллиона";
		} else {
			result = " миллионов";
		}
	} else  if (degreeLevel == 3) {
		if ( string.endsWith("один") ) {
			result = " миллиард";
		} else 	if ( string.endsWith("два") || string.endsWith("три") || string.endsWith("четыре") ) {
			result = " миллиарда";
		} else {
			result = " миллиардов";
		}
	} else  if (degreeLevel == 4) {
		if ( string.endsWith("один") ) {
			result = " триллион";
		} else 	if ( string.endsWith("два") || string.endsWith("три") || string.endsWith("четыре") ) {
			result = " триллиона";
		} else {
			result = " триллионов";
		}
	}

	return result;
}

/**
 * 	\ru
 *		\brief Возвращает прописью название валюты для целой части числа.
 * 		\param  string - пропись числа, для которого нужно вернуть название валюты прописью.
 *
 * 	\_ru
 */
QString
AMoney::integerCurrencyName( QString string ){
	QString result = "";

	if (string == "") return "";

	if ( this->currency->is("USD") ) {
		if ( string.endsWith("один") ) {
			result = "доллар США";
		} else 	if ( string.endsWith("два") || string.endsWith("три") || string.endsWith("четыре") ) {
			result = "доллара США";
		} else {
			result = "долларов США";
		}
	} else if ( this->currency->is("EUR") ) {
		result = "евро";
	} else {
		if ( string.endsWith("один") ) {
			result = "рубль";
		} else 	if ( string.endsWith("два") || string.endsWith("три") || string.endsWith("четыре") ) {
			result = "рубля";
		} else {
			result = "рублей";
		}
	}

	return result;
}

/**
 * 	\ru
 *		\brief Возвращает прописью название валюты для дробной части числа.
 * 		\param  string - пропись числа, для которого нужно вернуть название валюты прописью.
 *
 * 	\_ru
 */
QString
AMoney::decimalCurrencyName(QString string ){
	QString result = "";

	if (string == "") return "";

	if ( this->currency->is("USD") || this->currency->is("EUR") ) {
		if ( string.endsWith("один") ) {
			result = "цент";
		} else 	if ( string.endsWith("два") || string.endsWith("три") || string.endsWith("четыре") ) {
			result = "цента";
		} else {
			result = "центов";
		}
	} else {
		if ( string.endsWith("одна") ) {
			result = "копейка";
		} else 	if ( string.endsWith("две") || string.endsWith("три") || string.endsWith("четыре") ) {
			result = "копейки";
		} else {
			result = "копеек";
		}
	}

	return result;
}

/**
 * \ru
 * 		\brief	Поднимает регистр первой буквы слова, если первой идет одна из букв: о,д,т,ч,п,ш,с,в.
 * 		\param str - строка, первый символ которой нужно перевести в верхний регистр.
 * \_ru
 */
QString
AMoney::firstUp( QString str) {

	return str.left(1).upper() + str.right( str.length() - 1);
}

/**
 * \ru
 * 		\brief Конструктор
 * 		\param - значение порядка. 0 - сотни (по умолчанию), 1 - тысячи, 2 - миллионы, 3 - миллиарды, 4 - триллионы.
 * \_ru
 */
Degree::Degree( int val) {
	this->setValue( val );
}


/**
 * \ru
 * 		\brief Возвращает значение порядок.
 * 		Возможные значения смотри в описании конструктора.
 * \_ru
 */
int
Degree::getValue() {
	return this->value;
}

/**
 * \ru
 * 		\brief Задает значение порядка.
 * 		Допустимые значения смотри в описании конструктора.
 * \_ru
 */
void
Degree::setValue( int val ) {
	this->value = val;
	if (this->value == 1) {
		this->male = false;
	} else {
		this->male = true;
	}
}

/**
 * \ru
 * 		\brief Возвращает true, если текущий порядок имеет мужской род.
 *
 * 		Например, миллион, миллиард.
 * 		Возвращает false, если текущий порядок имеет женский род
 * 		Например, тысяча
 * \_ru
 */
bool
Degree::isMale() {
	return this->male;
}


/**
 * \ru
 * 		\brief Конструктор
 * 		\param - Обозначение валюты.
 * 		Допустимые значения параметра:
 * 		RUR - русский рубль, значение по умолчанию
 * 		dRUR - копейка
 * 		USD - доллар США
 * 		dUSD - цент
 * 		EUR - евро
 * 		dEUR = dUSD
 * \_ru
 */
Currency::Currency( QString newId) {
	setId( newId );
}


/**
 * \ru
 *  	\brief Возвращает истину, если текущая валюта равна значению, указанному в рараметре.
 * 		Допустимые значения параметра смотри в описании конструктора.
 * \_ru
 */
bool
Currency::is( QString str ){
	return this->id == str;
}

/**
 * \ru
 *  	\brief Возвращает истину, если текущая валюта имеет мужской род.
 * 		Например, копейка.
 * \_ru
 */
bool
Currency::isMale( ){
	return this->male;
}


/**
 * \ru
 *  	\brief Возвращает символьное обозначение, идентификатор валюты.
 * 		Допустимые значения смотри в описании конструктора.
 * \_ru
 */
QString
Currency::getId() {
	return this->id;
}

/**
 * \ru
 *  	\brief Установка типа валюты.
 * 		Допустимые значения смотри в описании конструктора.
 * \_ru
 */
void
Currency::setId(QString newId) {

	if ( newId == "dRUR") {
		this->male = false;
	} else {
		this->id = newId;
		this->male = true;
	}
}
