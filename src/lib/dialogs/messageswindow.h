/****************************************************************************
** $Id: messageswindow.h,v 1.1 2008/11/05 21:16:28 leader Exp $
**
** Header file of the Mesages window of Ananas
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

/*
*/
#ifndef MESSAGESWINDOW_H
#define MESSAGESWINDOW_H

#include "ananasglobal.h"
#include <QtGui>
//#include <q3dockwindow.h>
//Added by qt3to4:
#include <QHideEvent>

class QTextBrowser;

void messageproc(int n, const char *msg);

class  ANANAS_EXPORT MessagesWindow : public QDockWidget
{
    Q_OBJECT

public:
//	bool empty;
    MessagesWindow( const QString & title, QWidget * parent = 0, Qt::WindowFlags flags = Qt::WType_TopLevel); 
    ~MessagesWindow();

    QTextBrowser* msgBrowser;
private:
    bool expanded;
public slots:
	void message( int msgtype, const QString &msg);
protected slots:
	virtual void languageChange();
	virtual void hideEvent ( QHideEvent *e );
	void on_click();
	void setExpanded(bool exp);
	bool isExpanded();
};

#endif // MESSAGESWINDOW_H
