/****************************************************************************
** $Id: atoolbar.cpp,v 1.1 2008/11/05 21:16:29 leader Exp $
**
** Code file of the Ananas Tool bar of Ananas
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

//#include <qimage.h>
#include "atoolbar.h"
//Added by qt3to4:
#include <QPixmap>
#include "alog.h"
aToolBar::aToolBar( DomCfgItem *obj, aEngine *e, QMainWindow* parent , const char* name )
: QToolBar( name ,parent )
{
	md = obj;
	en = e;
	DomCfgItem *o=0;
	QDomNode nodel = obj->node();
	DomCfgItemInterfaces *mdl = new DomCfgItemInterfaces(nodel,0,0);
 	o = mdl->root()->find(md_interface)->find(md_toolbars);
	for (int i=0;i<o->childCount();i++) {
		if (o->child(i)->attr(mda_name)==name)
			ReadTool(o->child(i));
	}
}

aToolBar::~aToolBar(){
}

void
aToolBar::ReadTool( DomCfgItem *obj )
{
    DomCfgItem *aobj, *apix,*pict;	// action and pixmap XML data
    QString aKey;	// key sequence
    long pid;	// action id
    QPixmap		pix;
    //aobj = md->firstChild( obj );	//from first child action
    for (int i=0;i<obj->childCount();i++) {		// foreach not null
	aobj = obj->child(i);

	if (aobj->nodeName()!=md_command) continue;
	for (int j=0;j<aobj->childCount();j++) {
	pict=0;
	//if (aobj->child(j)->nodeName()!=md_command) continue;
	aKey = aobj->child(j)->attr (md_key);	//key sequence
	//pid = aobj->child(j)->attr(mda_id).toInt();	// action id
	if (aobj->child(j)->nodeName()==md_comaction) {
			pid = aobj->child(j)->nodeValue().toLong();
			QDomNode nodel = md->node();
			apix = new DomCfgItemActions(nodel,0,0);
			pict = apix->root()->findObjectById(pid)->child(md_active_picture);
	}
QAction *a=0;
//aLog::print(aLog::Debug,"QAction "+aobj->attr(mda_name)+"\n");
if (pict!=0) {
	pix.loadFromData( pict->binary());
	a = new QAction(
		QIcon(pix), // pixmap
		aobj->child(j)->attr(mda_name), // name
		aKey, // key sequence
		this, // owner
		aobj->attr(mda_name) // name
	);	// create new action
	}
else
	a = new QAction(
		aobj->child(j)->attr(mda_name), // name
		aKey, // key sequence
		this, // owner
		aobj->attr(mda_name) // name
	);
	actions.insert( pid, a );	// add action to dict
	addAction( a );	// put action into toolbar
	connect( a, SIGNAL(activated()), this, SLOT(on_Item()) );	// connect to slot
	}
	//aobj = md->nextSibling( aobj );	// get next action
    }
}

void
aToolBar::on_Item()
{
    QHashIterator<int,QAction*> it( actions );	//dict iterator
    while (it.hasNext()) {
	it.next();
	if ( it.value() == sender() ) {	// sender object
	    //Вернуться en->on_MenuBar( it.key() );	// call slot
	    break;	// break cycle
	}
    }
}
