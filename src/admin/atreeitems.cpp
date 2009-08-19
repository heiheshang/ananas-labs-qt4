/****************************************************************************
** $Id: atreeitems.cpp,v 1.2 2008/11/08 20:16:35 leader Exp $
**
** Header file of the Ananas visual tree object
** of Ananas Designer applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
**
** This file is part of the Designer application  of the Ananas
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
#include <QtCore>
#include <QtGui>
#include "alog.h"
#include <QtSql>
#include "acfg.h"
#include "configinfo.h"
#include "deditcat.h"
#include "deditfield.h"
#include "deditdialog.h"
#include "atreeitems.h"

ananasListViewModel::ananasListViewModel(QDomDocument document, QObject *parent)
    : QAbstractItemModel(parent), domDocument(document)
{
    rootItem = new DomCfgItem(domDocument, 0);
}

ananasListViewModel::~ananasListViewModel()
{
    delete rootItem;
}

int ananasListViewModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 1;
}
QString ananasListViewModel::info() const
{
  QDomElement rootnode = domDocument.documentElement();
  return rootnode.namedItem(md_info ).toElement().namedItem("name").toElement().text();
}

QVariant ananasListViewModel::data(const QModelIndex &index, int role) const
{
if ( !index.isValid() )
	return QVariant();
if ( role == Qt::DecorationRole )
{
	DomCfgItem *item = static_cast<DomCfgItem*> ( index.internalPointer() );

	QDomNode node = item->node();
	return item->iconNode();

}
if ( role == Qt::DisplayRole )
{
	DomCfgItem *item = static_cast<DomCfgItem*> ( index.internalPointer() );
//connect(this,SIGNAL(customContextMenuRequested(QPoint)),item,SLOT(item->s//howContextMenu(QPoint)));
	QDomNode node = item->node();
	QString nodeName = node.nodeName();
	if ( nodeName=="xml" )
	{
		return info();
	}
	return QObject::tr ( item->cfgName() );
}
return QVariant();
}

Qt::ItemFlags ananasListViewModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant ananasListViewModel::headerData(int section, Qt::Orientation orientation,
                              int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
         return info();
    }

    return QVariant();
}

QModelIndex ananasListViewModel::createIndexByTags(const QString & md_const,int row, int column, DomCfgItem *parent) const
{
	QDomNodeList listNodes = domDocument.elementsByTagName(md_const);
	QDomNode node = listNodes.item(0);
	DomCfgItem *nodeMd = new DomCfgItem(node,row,parent);
	return createIndex(row, column, nodeMd);
}

QModelIndex ananasListViewModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    DomCfgItem *parentItem;

    if (!parent.isValid()) 
        parentItem = rootItem;
    else
        parentItem = static_cast<DomCfgItem*>(parent.internalPointer());

    DomCfgItem *childItem = parentItem->child(row);

    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex ananasListViewModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    DomCfgItem *childItem = static_cast<DomCfgItem*>(child.internalPointer());
    DomCfgItem *parentItem = childItem->parent();

    if (!parentItem || parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int ananasListViewModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    DomCfgItem *parentItem;

    if (!parent.isValid())
	{
        parentItem = rootItem;
	return md_row_count;
	}
    else
        parentItem = static_cast<DomCfgItem*>(parent.internalPointer());
    return parentItem->childCount();
}

bool ananasListViewModel::hasChildren ( const QModelIndex & parent ) const
{
   DomCfgItem *item;
if (!parent.isValid()) {
    return true;
} else
    item = static_cast<DomCfgItem*>(parent.internalPointer());
QDomNode node = item->node();
if (node.nodeName()==md_field)
    return false;
if (item->hasChildren())
 return true;
return false;
}
// ananasListViewItem::ananasListViewItem( QTreeWidget *parent, aCfg * cfgmd, aCfgItem cfgobj,const QString &name )
// : QTreeWidgetItem( parent,QTreeWidgetItem::Type )
// {
// 	obj = cfgobj;
// 	md = cfgmd;
// 	if ( name.isNull() ) setText( 0,md->attr( obj, mda_name ) );
// 	else setText( 0,name );
// 	id = md->id(obj);
// }
// 
// ananasListViewItem::ananasListViewItem( QTreeWidget *parent, QTreeWidgetItem *after, aCfg * cfgmd, aCfgItem cfgobj, const QString &name ): QTreeWidgetItem( parent ,after ,QTreeWidgetItem::Type)
// {
// 	obj = cfgobj;
// 	md = cfgmd;
// 	if ( name.isNull() ) setText(0, md->attr( obj, mda_name ) );
// 	else setText(0, name );
// 	id = md->id(obj);
// }
// 
// ananasListViewItem::ananasListViewItem( ananasListViewItem *parent, ananasListViewItem *after, aCfg * cfgmd, aCfgItem cfgobj, const QString &name )
// : QTreeWidgetItem(parent,after,QTreeWidgetItem::Type)
// {
// 	obj = cfgobj;
// 	md = cfgmd;
// 	if ( name.isNull() ) setText(0, md->attr( obj, mda_name ) );
// 	else setText( 0,name );
// 	id = md->id(obj);
// }
// 
// void
// ananasListViewItem::clearTree()
// {
// 	QTreeWidgetItem	*item, *nextitem;
// 
// 	// clear tree
// 	//item = firstChild();
// 	//while( item )
// 	//{
// 	//	nextitem = item->nextSibling();
// 	//	delete item;
// 	//	item = nextitem;
// 	//}
// }
// 
// /*
// void
// ananasListViewItem::moveItem ( QListViewItem * after )
// {
// CHECK_POINT
// 	aCfgItem temp = obj, temp2 = ((ananasListViewItem*)after)->obj;
// 	QListViewItem::moveItem( after );
// 	obj = temp;
// 	((ananasListViewItem*)after)->obj = temp2;
// }
// */
// void
// ananasListViewItem::moveUp ()
// {
// 	if(!previousSibling()) return; // not previous item! - no changes
// 	aCfgItem item = previousSibling()->obj;
// 
// 	if ( obj.isNull() )
// 	{
// 		aLog::print(aLog::Error, QObject::tr(" Ananas List View Item %1 is null").arg(md->attr(obj,mda_name)));
// 		return;
// 	}
// 	if( md->swap( obj, item ) )
// 	{
// 		previousSibling()->moveItem( this );
// 		aLog::print(aLog::Debug, QObject::tr("Ananas List View Item swaping"));
// 	}
// 	else
// 	{
// 		aLog::print(aLog::Error, QObject::tr("Ananas List View Item swaping"));
// 	}
// }
// 
// void
// ananasListViewItem::moveDown ()
// {
// 	if(!nextSibling()) return; // not next item! - no changes
// 	aCfgItem item = nextSibling()->obj;
// 	if ( item.isNull() )
// 	{
// 		aLog::print(aLog::Error, QObject::tr(" Ananas List View Item %1 is null").arg(md->attr(obj,mda_name)));
// 		return;
// 	}
// 	if( md->swap( obj, item ) )
// 	{
// 		moveItem( nextSibling() );
// 		aLog::print(aLog::Debug, QObject::tr("Ananas List View Item swaping"));
// 	}
// 	else
// 	{
// 		aLog::print(aLog::Error, QObject::tr("Ananas List View Item swaping"));
// 	}
// }
// 
// ananasListViewItem *
// ananasListViewItem::previousSibling() // becose QListViewItem not have function previousSibling();
// {
// 	QTreeWidget *parent, *item;
// 	parent = this->parent();
// 	item = parent->firstChild();
// 	while ( item )
// 		if ( (ananasListViewItem *)item->nextSibling() == this )
// 			return (ananasListViewItem *) item;
// 		else item = item->nextSibling();
// 	return 0;
// }
// 
// ananasListViewItem*
// ananasListViewItem::nextSibling()
// {
// 	return (ananasListViewItem *)QTreeWidgetItem::nextSibling();
// }
// 
// 
// void
// ananasListViewItem::okRename( int col )
// {
// 	Q3ListViewItem::okRename( col );
// 	if ( id && !obj.isNull() && col == 0 ) {
// 		setText( 0, text( 0 ).stripWhiteSpace() );
// 		md->setAttr( obj, mda_name, text( 0 ) );
// 	}
// }
// 
// /*
// ananasListViewItem*
// ananasListViewItem::getLastChild( QListViewItem * parent )
// {
// 	QListViewItem *item, *nextitem;
// 	item = parent->firstChild();
// 	while( item )
// 	{
// 		nextitem = item->nextSibling();
// 		if ( nextitem )
// 			item = nextitem;
// 		else
// 			return (ananasListViewItem*) item;
// 	}
// 	return 0;
// };
// */
// 
// ananasListViewItem*
// ananasListViewItem::getLastChild()
// {
// 	QListWidgetItem *item, *nextitem;
// 	item = firstChild();
// 	while( item )
// 	{
// 		nextitem = item->nextSibling();
// 		if ( nextitem )
// 			item = nextitem;
// 		else
// 			return (ananasListViewItem*) item;
// 	}
// 	return 0;
// };
// 
// 
ananasTreeView::ananasTreeView ( QWidget *parent )
:QTreeView ( parent )
{

};

void
ananasTreeView::ContextMenuAdd( QMenu* m )
{
	//--QLabel *caption = new QLabel( tr("<font color=darkblue><u><b>" "Context Menu</b></u></font>"), this );
	//--caption->setAlignment( Qt::AlignCenter );
	//--m->insertItem( caption );
	//m->insertItem( tr("&Rename"), this, SLOT( itemRename() ), //Qt::CTRL+Qt::Key_R);
	QAction* rename = m->addAction(tr("&Rename"));
	rename->setShortcut(Qt::CTRL+Qt::Key_R);
	connect(rename,SIGNAL(triggered()),SLOT(itemRename()));
	//m->insertItem( tr("&Edit"),  this, SLOT( itemEdit() ), //Qt::CTRL+Qt::Key_E );
	QAction* edit = m->addAction(tr("&Edit"));
	edit->setShortcut(Qt::CTRL+Qt::Key_E);
	connect(edit,SIGNAL(triggered()),SLOT(itemEdit()));
	//m->insertItem( tr("&Delete"), this, SLOT( itemDelete() ), //Qt::CTRL+Qt::Key_D );
	QAction* del = m->addAction(tr("&Delete"));
	del->setShortcut(Qt::CTRL+Qt::Key_D);
	connect(del,SIGNAL(triggered()),SLOT(itemDelete()));
	//m->insertItem( tr("&MoveUp"), this, SLOT( itemMoveUp() ), //Qt::CTRL+Qt::Key_U );
	QAction* moveUp = m->addAction(tr("&MoveUp"));
	moveUp->setShortcut(Qt::CTRL+Qt::Key_U);
	connect(moveUp,SIGNAL(triggered()),SLOT(itemMoveUp()));
	//m->insertItem( tr("&MoveDown"), this, SLOT( itemMoveDown() ), //Qt::CTRL+Qt::Key_M );
	QAction* moveDown = m->addAction(tr("&MoveDown"));
	moveDown->setShortcut(Qt::CTRL+Qt::Key_M);
	connect(moveDown,SIGNAL(triggered()),SLOT(itemMoveDown()));
	//m->insertItem( tr("&SaveItem"), this, SLOT( itemSave() ) );
	QAction* saveItem = m->addAction(tr("&SaveItem"));
	connect(saveItem,SIGNAL(triggered()),SLOT(itemSave()));
	//m->insertItem( tr("&LoadItem"), this, SLOT( itemLoad() ) );
	QAction* loadItem = m->addAction(tr("&LoadItem"));
	connect(loadItem,SIGNAL(triggered()),SLOT(itemLoad()));
	m->addSeparator();
};

void
ananasTreeView::deleteItem()
{
// 	ananasListViewModel *i = (ananasListViewModel *) currentIndex()->internalPointer();
// 	if ( i )
// 	{
// 		if ( i->id ) {
// 			md->remove( i->obj );
// 			delete i;
// 		}
// 	}
};

void
ananasTreeView::moveUpItem()
{
// 	ananasListViewItem *i = (ananasListViewItem *) selectedItem();
// 	if ( i )
// 	{
// 		if ( i->id ) {
// 			i->moveUp();
// 		}
// 	}
}

void
ananasTreeView::moveDownItem()
{
// 	ananasListViewItem *i = (ananasListViewItem *) selectedItem();
// 	if ( i )
// 	{
// 		if ( i->id ) {
// 			i->moveDown();
// 		}
// 	}
}

void
ananasTreeView::renameItem()
{
//	if ( selectedItem() ) selectedItem()->startRename( 0 );
}

configTreeView::configTreeView(QDomDocument config,QWidget *parent):QTreeView(parent),xml(config)
{
	QAbstractItemModel *data = new ananasListViewModel(xml,parent);
	QItemSelectionModel *selections = new QItemSelectionModel(data);
	//QTreeView *mdtree = new QTreeView;
	setModel(data);
	setHeaderHidden(true);
	setSelectionModel(selections);
	setUniformRowHeights(true);
	header()->setStretchLastSection(false);
	viewport()->setAttribute(Qt::WA_StaticContents);
	header()->setResizeMode(QHeaderView::Stretch);
	setAttribute(Qt::WA_MacShowFocusRect, false);
	setContextMenuPolicy(Qt::CustomContextMenu);
	setWindowFlags(Qt::Widget);
connect(this,
            SIGNAL( customContextMenuRequested(const QPoint &) ),
            this,SLOT(showmenu(const QPoint &)));
connect(this,SIGNAL(doubleClicked ( const QModelIndex & )),this,SLOT(doubleClicked ( const QModelIndex & )));

};
void configTreeView::showmenu (const QPoint &pos)
{
	DomCfgItem *item = static_cast<DomCfgItem*> ( currentIndex().internalPointer() );
        
        QMenu *contextMenu = item->menu();
	if (contextMenu) {
	connect(contextMenu,
            SIGNAL(triggered(QAction *)),
            this,SLOT(actionTree( QAction *)));
	contextMenu->exec(viewport()->mapToGlobal(pos));
	}
}
void configTreeView::actionTree(QAction *a)
{
    if (a->text()==tr("Property")) {

        DomCfgItem *item = static_cast<DomCfgItem*> ( currentIndex().internalPointer() );
        QDomNode node = item->node();
        if (node.nodeName()=="xml") {
            configInfo *info = new configInfo(item,this,Qt::Dialog|Qt::WDestructiveClose);
            info->show();
        }
	if (node.nodeName()==md_catalogue) {
	   dEditCat *dialog = new dEditCat(this,"",Qt::Dialog|Qt::WDestructiveClose);
           dialog->setData(item);
	   dialog->show();
	}
    }
}
void configTreeView::doubleClicked ( const QModelIndex & index )
{
if ( index.isValid() ) {
	DomCfgItem *item = static_cast<DomCfgItem*> ( index.internalPointer() );
	QString nodeName = item->node().nodeName();
	if ( nodeName==md_field )
	{
		dEditField *dialog = new dEditField(this,"",Qt::Dialog|Qt::WDestructiveClose);
    		dialog->setData(item);
    		dialog->show();
	}
	if ( nodeName==md_form )
	{
		dEditDialog *dialog = new dEditDialog(this,"",Qt::Dialog|Qt::WDestructiveClose);
    		dialog->setData(item);
    		dialog->show();
	}


 }
}