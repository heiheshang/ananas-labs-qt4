/****************************************************************************
** $Id: aextexample.cpp,v 1.1 2008/11/05 21:16:27 leader Exp $
**
** Extension object implementation file of
** Ananas application library
**
** Created : 20031201
**
** Copyright (C) 2003-2006 Leader InfoTech.  All rights reserved.
** Copyright (C) 2003-2006 Valery Grazdankin, Orenburg.
** Copyright (C) 2003-2005 Grigory Panov, Yoshkar-Ola.
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

#include "aextexample.h"
#include "acfg.h"

AExtExample::AExtExample()
    : AExtension("AExtExample")
{
    startTimer( 5000 );
}



AExtExample::~AExtExample()
{
}

void 
AExtExample::PrintMessage( const QString &msg )
{
    setLastMessage( msg );
    cfg_message(0, "AExtTest MSG:%s\n", (const char *) msg);
}

void 
AExtExample::timerEvent( QTimerEvent * )
{
    emit event("10");
}

#include <aextensionplugin.h>
typedef AExtensionPlugin<AExtExample> AExtExamplePlugin;
A_EXPORT_PLUGIN( AExtExamplePlugin )
