// /****************************************************************************
// ** $Id: interfacetree.h,v 1.1 2008/11/05 21:16:27 leader Exp $
// **
// ** Header file of the Interface Tree of Ananas Designer applications
// **
// ** Created : 20031201
// **
// ** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
// **
// ** This file is part of the Designer application  of the Ananas
// ** automation accounting system.
// **
// ** This file may be distributed and/or modified under the terms of the
// ** GNU General Public License version 2 as published by the Free Software
// ** Foundation and appearing in the file LICENSE.GPL included in the
// ** packaging of this file.
// **
// ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// **
// ** See http://www.leaderit.ru/page=ananas or email sales@leaderit.ru
// ** See http://www.leaderit.ru/gpl/ for GPL licensing information.
// **
// ** Contact org@leaderit.ru if any conditions of this licensing are
// ** not clear to you.
// **
// **********************************************************************/
// 
// #ifndef INTERFACETREE_H
// #define INTERFACETREE_H
// //#include <q3listview.h>
// //#include <q3intdict.h>
// #include <QListWidget>
// #include <QHash>
// //#include "acfg.h"
// #include "atreeitems.h"
// 
// class QWidget;
// 
// class InterfaceListViewItem : public ananasListViewModel
// {
// public:
// 	InterfaceListViewItem( QListWidget *parent, DomCfgItem *cfgobj, const QString &name = QString::null );
// 	InterfaceListViewItem( ananasListViewModel *parent, ananasListViewModel *after,DomCfgItem *cfgobj,const QString &name = QString::null );
// 	void loadTree();
// 	void newCommand ();
// 	void newSubmenu ();
// 	void newSeparator ();
// 	void edit ();
// private:
// 	void loadSubmenu ( ananasListViewModel * parent, ananasListViewModel *after, DomCfgItem *child );
// 	void loadCommand ( ananasListViewModel * parent, ananasListViewModel *after, DomCfgItem  *child );
// 	void loadSeparator ( ananasListViewModel * parent, ananasListViewModel *after, DomCfgItem  *child);
// };
// 
// 
// 
// #endif //INTERFACETREE_H
