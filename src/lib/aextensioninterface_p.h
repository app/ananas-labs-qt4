/****************************************************************************
** $Id: aextensioninterface_p.h,v 1.1 2008/11/05 21:16:28 leader Exp $
**
** Definition of AExtensionInterface class
**
** Created : 20060209
**
** Copyright (C) 2003-2006 Leader InfoTech.  All rights reserved.
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

#ifndef AEXTENSIONINTERFACE_H
#define AEXTENSIONINTERFACE_H

#include <ananasglobal.h>
#ifndef QT_H
#include <private/qcom_p.h>
#endif // QT_H

#ifndef QT_NO_COMPONENT

// {EDDD5AD8-DF3C-400c-A711-163B72FE5F61}
#ifndef IID_AExtensionFactory
#define IID_AExtensionFactory QUuid(0xeddd5ad8, 0xdf3c, 0x400c, 0xa7, 0x11, 0x16, 0x3b, 0x72, 0xfe, 0x5f, 0x61)
#endif

class AExtension;

struct ANANAS_EXPORT AExtensionFactoryInterface : public QFeatureListInterface
{
    virtual AExtension* create( const QString& name ) = 0;
};

#endif //QT_NO_COMPONENT
#endif // AEXTENSIONINTERFACE_P_H
