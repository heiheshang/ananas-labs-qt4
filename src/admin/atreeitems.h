/****************************************************************************
** $Id: atreeitems.h,v 1.1 2008/11/05 21:16:27 leader Exp $
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

#ifndef ATREEITEMS_H
#define ATREEITEMS_H
#include <QtCore>
#include <QtGui>
//#include <QDomNode>
//#include <QHash>
#include "acfg.h"
//class QWidget;

class ananasListViewModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	//int id;
	//aCfgItem obj;
	//QWidget *editor;
	//aCfg *md;
     ananasListViewModel(DomCfgItem *document, QObject *parent = 0);
    ~ananasListViewModel();

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    //bool hasChildren(const QModelIndex &parent) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    //bool isDragEnabled(const QModelIndex &index) const;
    QString info() const;
    bool hasChildren ( const QModelIndex & parent = QModelIndex() ) const; 
private:

    DomCfgItem *rootItem;
    //QDomDocument domDocument;
    QModelIndex createIndexByTags(const QString & md_const,int row, int column,DomCfgItem *parent) const;
	//ananasListViewItem( QTreeWidget *parent, aCfg * cfgmd, aCfgItem //cfgobj, const QString &name = QString::null );
	//ananasListViewItem( QTreeWidget *parent, QTreeWidgetItem *after, //aCfg * cfgmd, aCfgItem cfgobj, const QString &name = //QString::null );
	//ananasListViewItem( ananasListViewItem *parent, //ananasListViewItem *after, aCfg * cfgmd,
	//					aCfgItem cfgobj, const /QString &name = QString::null );

	//void clearTree();
	//void moveUp ();
	//void moveDown ();

	//ananasListViewItem *previousSibling();
	//ananasListViewItem *nextSibling();
	//ananasListViewItem *getLastChild();
//protected:
//	virtual void okRename( int col );
};
class ananasTreeView : public QTreeView
{
	Q_OBJECT
public:
    //aCfg *md;

	ananasTreeView(QWidget *parent=0);

	void ContextMenuAdd(  QMenu * m );
	void deleteItem();
	void moveUpItem ();
	void moveDownItem ();
	void renameItem();
	void saveItem();
	void loadItem();
};

class configTreeView : public QTreeView
{
    Q_OBJECT
public:
    //aCfg *md;

	configTreeView(DomCfgItem *config,QWidget *parent=0);

	//void ContextMenuAdd(  QMenu * m );
	//void deleteItem();
	//void moveUpItem ();
	//void moveDownItem ();
	//void renameItem();
	//void saveItem();
	//void loadItem();
private:
	//QDomDocument xml;
private slots:
	void showmenu (const QPoint &p);
	void actionTree (QAction *a);
	void doubleClicked ( const QModelIndex & index );
};


#endif
