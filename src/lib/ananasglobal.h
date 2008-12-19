/****************************************************************************
** $Id: ananasglobal.h,v 1.1 2008/11/05 21:16:28 leader Exp $
**
** Header file of the Ananas Library of Ananas
** Designer and Engine applications
**
** Created : 20040901
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) 2003-2004 Linux Center (Orenburg).  All rights reserved.
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

#ifndef ANANASGLOBAL_H
#define ANANASGLOBAL_H

#include <qglobal.h>

#define ANANAS_DEBUG

#define ANANAS_VERSION_STRING "0.4.3"
#define ANANAS_VERSION_MAJOR 0
#define ANANAS_VERSION_MINOR 4
#define ANANAS_VERSION_MICRO 3

// helper macros
#define ANANAS_MAKE_VERSION(maj, min, mic) (((maj) << 16) | ((min) << 8) | (mic))
#define ANANAS_VERSION \
    ANANAS_MAKE_VERSION(ANANAS_VERSION_MAJOR, ANANAS_VERSION_MINOR, ANANAS_VERSION_MICRO)

// evaluate to true if this version is (maj, min, mic) at minimum
#define ANANAS_IS_VERSION(maj, min, mic) \
    (ANANAS_VERSION >= ANANAS_MAKE_VERSION(maj, min, mic))

#if defined(Q_OS_WIN32) && !defined(ANANAS_NO_DLL)
#  if defined(ANANAS_DLL)
#    define ANANAS_EXPORT __declspec(dllexport)
#  else
#    define ANANAS_EXPORT __declspec(dllimport)
#  endif
#else
#  define ANANAS_EXPORT
#endif

#ifdef _MSC_VER
#define LLU_SPEC "%I64u"
#else
#define LLU_SPEC "%llu"
#endif

#if defined (ANANAS_DLL)
#  define ANANAS_TEMPLATE_EXTERN
#elif defined (Q_TEMPLATEDLL)
#  define ANANAS_TEMPLATE_EXTERN Q_TEMPLATE_EXTERN
#else
#  define ANANAS_TEMPLATE_EXTERN
#endif


//#if defined(Q_ULLONG)
#	define ANANAS_UID qulonglong
#	define ANANAS_UID_VALUE( v ) v.toULongLong()
//#error yes
//#else
//#	define ANANAS_UID unsigned int
//#	define ANANAS_UID_VALUE( v ) v.toUInt()
//#error no
//#endif

#endif
