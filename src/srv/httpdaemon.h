/****************************************************************************
** $Id: httpdaemon.h,v 1.1 2008/12/22 21:23:23 leader Exp $
**
** Header file of the HTTP Daemon file of Ananas
** Web Engine application
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
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

#ifndef HTTP_DAEMON_H
#define HTTP_DAEMON_H

#include <QTcpServer>

class QTextStream;

class HttpDaemon : public QTcpServer
{
	Q_OBJECT
public:
	HttpDaemon( QObject* parent=0 );
	void newConnection( int socket );
	void fLogin( QTextStream &ts );
	void fMain( QTextStream &ts );
	void fIcon( QTextStream &ts, QString f );
signals:
	void newConnect();
	void endConnect();
	void wroteToClient();

private slots:
	void readClient();
	void discardClient();
};

#endif
