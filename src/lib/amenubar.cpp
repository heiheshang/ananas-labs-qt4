/****************************************************************************
** $Id: amenubar.cpp,v 1.1 2008/11/05 21:16:28 leader Exp $
**
** Code file of the Ananas Menu bar of Ananas
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

/******************************************************************
 ******************************************************************/

#include "amenubar.h"
//Added by qt3to4:
#include <QtGui>
#include <QPixmap>
#include "alog.h"

AMenuBar::AMenuBar( QWidget* parent  )
	:QMenuBar( parent ) {

}

AMenuBar::AMenuBar( DomCfgItem *cfg, QWidget* parent )
:QMenuBar( parent )
{
	md = cfg;
	QDomNode nodel = cfg->node();
	DomCfgItemInterfaces *mdl = new DomCfgItemInterfaces(nodel,0,0);
	DomCfgItem *obj = 0;
	obj = mdl->root()->find(md_interface)->find(md_mainmenu);
	ReadMenu(obj);
}

void
AMenuBar::ReadMenu( DomCfgItem *obj )
{
	DomCfgItem	*cobj;
	QMenu	*parent;
	QString		text, aKey ;
	long 		id;

	if ( !md )
		return;
	//cobj = obj->child(0);
	//aLog::print(aLog::Debug,QString("menubar\n"));
	//aLog::print(aLog::Debug,QString::number(obj->childCount()));
	for (int i=0;i<obj->childCount();i++)
	{
		cobj = obj->child(i);
		id = cobj->attr(mda_id).toInt();
		//aLog::print(aLog::Debug,QString("menubar ")+cobj->nodeName()+"\n"); 
		if ( cobj->nodeName() == md_submenu )
		{
			//parent = new QMenu ();
			//insertItem( md->attr( cobj, mda_name ), parent );
			QMenu *parent = addMenu(cobj->attr(mda_name));
			ReadMenu( parent, cobj );
		}
		if ( cobj->nodeName() == md_command )
		{
			text = cobj->attr(md_menutext) ;
			if ( text == "" ) text = cobj->attr(mda_name);
			aKey = cobj->attr(md_key);
			//QMenuBar::insertItem( text, this, //SLOT(on_Item()), QKeySequence( aKey ), id );
			QAction *currentMenu = addAction(text);
			currentMenu->setShortcut(QKeySequence( aKey ));
			connect(currentMenu, SIGNAL(triggered()), this, SLOT(on_Item()));

		}
		if ( cobj->nodeName() == md_separator )
		{
			addSeparator();
		}
		//cobj = md->nextSibling ( cobj );
	}

}

void
AMenuBar::ReadMenu( QMenu *parent, DomCfgItem *obj )
{
	DomCfgItem *cobj;
	DomCfgItem *apix;
	DomCfgItem *pict;
	QMenu	*mparent;
	QString		text, aKey,pid;
	long id;
    	QPixmap		pix;

	if ( !md )
		return;

	//cobj = md->firstChild ( obj );
	for (int i=0;i<obj->childCount();i++ )
	{
		cobj=obj->child(i);
		id = cobj->attr(mda_id).toInt();
		if (cobj->nodeName() == md_submenu )
		{
			//mparent = new QMenu ();
			//parent->insertItem( md->attr ( cobj, mda_name ), //mparent );
			QMenu *mparent = parent->addMenu(cobj->attr(mda_name));
			ReadMenu( mparent, cobj );
		}
		if (cobj->nodeName() == md_command )
		{
aLog::print(aLog::Debug,"md_menutext\n");
			//QDomNode mdmenutext = cobj->child(md_menutext);
			text = cobj->child(md_menutext)->nodeValue();
			//if (mdmenutext==0)
			if ( text == "" )
				text = cobj->attr(mda_name);
			aKey = cobj->child(md_key)->nodeValue();
			pid = cobj->child(md_comaction)->nodeValue();
			QDomNode nodel = md->node();
			apix = new DomCfgItemActions(nodel,0,0);
			pict = apix->root()->findObjectById(pid)->child(md_active_picture);
//Вернуться
			QAction *currentMenu = 0;
	if (pict!=0) {
//aLog::print(aLog::Debug,"apix "+pict->nodeName());
			pix.loadFromData( pict->binary());
			//parent->insertItem( pix, text, this, //SLOT(on_Item()), QKeySequence( aKey ), id );
			currentMenu = //parent->addAction(pix,text);Вернуться
			parent->addAction(QIcon(pix),text);
	}
	else
			currentMenu = parent->addAction(text);

			currentMenu->setShortcut(QKeySequence( aKey ));
			currentMenu->setData(pid);
			//connect(currentMenu,SIGNAL(triggered(QAction*)), this, SLOT(on_Item(QAction*)));
			pix = 0;
		}
		if (cobj->nodeName() == md_separator )
		{
			parent->addSeparator();
		}
		//cobj = md->nextSibling ( cobj );
	}

}


AMenuBar::~AMenuBar(){
}

int AMenuBar::insertItem ( const QString & text, QMenu * popup, int id, int index ) {
	return QMenuBar::insertItem ( text, popup, id, index);
};
