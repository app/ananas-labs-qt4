/****************************************************************************
** $Id: test.h,v 1.1 2008/11/19 20:20:16 leader Exp $
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

#include <math.h>
#include <stdlib.h>

double rnd(double m=1);


double rnd(double m)
{
	double res;
#ifdef _Windows
	res = rand()*m;
#else
	res = random()*m;
#endif
	return res;
	
}
