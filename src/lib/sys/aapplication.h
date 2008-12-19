/****************************************************************************
** $Id: aapplication.h,v 1.2 2008/11/21 20:58:12 leader Exp $
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

#ifndef AAPPLICATION_H
#define AAPPLICATION_H

#include "ananasglobal.h"
#include <QApplication>

class ANANAS_EXPORT AApplication : public QApplication
{
Q_OBJECT
public:
            
    enum AApplicationType {
        Unknown,
        Ananas,
        Designer,
        Administrator
    };

    AApplication(  int & argc, char ** argv, AApplicationType aat = Unknown );
    int applicationType();
    
private:
    int v_aat;        
};

#endif
