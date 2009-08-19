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
#include "editor.h"
#include "tabwidget.h"
//#include "findfileimpl.h"
#include "cpphighlighter.h"
#include "InitCompletion.h"
#include "optionsimpl.h"
#include "pluginsinterfaces.h"
#include "misc.h"
#include "atreeitems.h"

ananasListViewModel::ananasListViewModel(DomCfgItem *document, QObject *parent)
    : QAbstractItemModel(parent)
{
    rootItem = document;
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
  QDomElement rootnode = rootItem->node().toDocument().documentElement();
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
	return QObject::tr (item->cfgName());
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
	QDomNodeList listNodes = rootItem->node().toDocument().elementsByTagName(md_const);
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

configTreeView::configTreeView(DomCfgItem *config,QWidget *parent):QTreeView(parent)
{
	QAbstractItemModel *data = new ananasListViewModel(config,parent);
	QItemSelectionModel *selections = new QItemSelectionModel(data);
	m_tabEditors=0;
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
	m_tabStopWidth = 4;
	m_lineNumbers = m_selectionBorder = m_autoIndent = m_cppHighlighter = true;
	crossButton = 0;
	menuBookmarks=((MainForm*)parent->parentWidget())->menuBookmarks;
	actionActiveBookmark=menuBookmarks->activeAction();
	//setCrossButton( !m_closeButtonInTabs );
//connect(actionOptions, SIGNAL(triggered()), this, SLOT(slotOptions()));
        loadINI();

}

void configTreeView::slotOptions()
{
     OptionsImpl *options = new OptionsImpl(this, m_font, m_lineNumbers, m_selectionBorder,
                                           m_autoIndent, m_cppHighlighter, m_tabStopWidth, m_saveBeforeBuild, m_restoreOnStart,
                                           m_formatPreprocessorText, m_formatQtText, m_formatSingleComments,
                                           m_formatMultilineComments, m_formatQuotationText, m_formatMethods,
                                           m_formatKeywords, m_autoMaskDocks, m_endLine, m_tabSpaces, m_autoCompletion,
                                           m_backgroundColor, m_promptBeforeQuit, m_highlightCurrentLine, m_currentLineColor, m_autobrackets, m_autocomments,
                                           m_showTreeClasses, m_intervalUpdatingClasses, m_projectsDirectory, m_match, m_matchingColor,
                                           m_closeButtonInTabs, m_pluginsDirectory, m_makeOptions, m_mibCodec,
                                           m_includeDirectory, m_displayEditorToolbars, m_displayWhiteSpaces, m_rightMarginLine, m_documentationDirectory,
                                           m_textColor, m_automaticCompilation, m_wordWrap, m_hideFindReplace, m_smartClick
     );

    if ( options->exec() == QDialog::Accepted )
    {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        m_showTreeClasses = options->showTreeClasses->isChecked();
        m_automaticCompilation = options->compileAuto->isChecked();
        m_intervalUpdatingClasses = options->interval->value();
        m_font = QFont( options->font().family(), options->font().pointSize() );
        m_tabStopWidth = options->tabStopWidth->value();
        m_cppHighlighter = options->highlight->isChecked();
        m_lineNumbers = options->numbers->isChecked();
        m_autoIndent = options->indent->isChecked();
        m_selectionBorder = options->selectionBorder->isChecked();
        m_saveBeforeBuild = options->saveAll->isChecked();
        m_restoreOnStart = options->restore->isChecked();
        m_endLine = (EndLine)options->endLine->currentIndex();
        m_tabSpaces = options->tabSpaces->isChecked();
        m_autoCompletion = options->completion->isChecked();
        m_autobrackets = options->brackets->isChecked();
        // Divius: comments autoclose
        m_autocomments = options->comments->isChecked();
        //
        m_match = options->match->isChecked();
        m_highlightCurrentLine = options->groupHighlightCurrentLine->isChecked();
        m_promptBeforeQuit = options->promptBeforeQuit->isChecked();
        m_projectsDirectory = options->projectsDirectory->text();
        m_pluginsDirectory = options->pluginsDirectory->text();
        m_includeDirectory = options->includeDirectory->text();
        m_documentationDirectory = options->documentationDirectory->text();
        m_makeOptions = options->makeOptions->text();
        m_closeButtonInTabs = options->closeButton->isChecked();
        //setCrossButton( !m_closeButtonInTabs );
        //
        m_formatPreprocessorText = options->syntaxe()->preprocessorFormat();
        m_formatQtText = options->syntaxe()->classFormat();
        m_formatSingleComments = options->syntaxe()->singleLineCommentFormat();
        m_formatMultilineComments = options->syntaxe()->multiLineCommentFormat();
        m_formatQuotationText = options->syntaxe()->quotationFormat();
        m_formatMethods = options->syntaxe()->functionFormat();
        m_formatKeywords = options->syntaxe()->keywordFormat();
        m_backgroundColor = options->backgroundColor();
        m_textColor = options->textColor();
        m_currentLineColor = options->currentLineColor();
        m_matchingColor = options->matchingColor();
        m_mibCodec = options->mib();
        m_displayEditorToolbars = options->showEditorToolbars->isChecked();
        m_displayWhiteSpaces = options->displayWhiteSpaces->isChecked();
        if (options->rightMarginLine->isChecked())
            m_rightMarginLine = options->rightMarginPos->value();
        else
            m_rightMarginLine = 0;
        m_wordWrap = options->wordwrap->isChecked();
        if (options->hideFindReplace->isChecked())
                m_hideFindReplace = options->findReplaceDelay->value();
        else
                m_hideFindReplace = 0;
        m_smartClick = options->smartClick->isChecked();

        /*slotUpdateOtherFileActions();
        foreach(Editor *editor, allEditors() )
        {
            editor->setShowTreeClasses( m_showTreeClasses );
            editor->setIntervalUpdatingTreeClasses( m_intervalUpdatingClasses );
            editor->setFont( m_font );
            editor->setTabStopWidth( m_tabStopWidth );
            editor->setSyntaxHighlight( m_cppHighlighter );
            editor->setLineNumbers( m_lineNumbers );
            editor->setAutoIndent( m_autoIndent );
            editor->setMatch( m_match );
            editor->setHighlightCurrentLine( m_highlightCurrentLine );
            editor->setSelectionBorder( m_selectionBorder );
            editor->setAutoCompletion( m_autoCompletion );
            editor->setEndLine( m_endLine );
            editor->setTabSpaces( m_tabSpaces );
            editor->setBackgroundColor( m_backgroundColor );
            editor->setTextColor( m_textColor );
            editor->setCurrentLineColor( m_currentLineColor );
            editor->setMatchingColor( m_matchingColor );
            editor->setAutobrackets( m_autobrackets );
            editor->setAutocomments( m_autocomments );
            editor->setShowWhiteSpaces( m_displayWhiteSpaces );
            editor->setRightMargin( m_rightMarginLine );
            editor->displayEditorToolbar( m_displayEditorToolbars );
            editor->setSyntaxColors
            (
                m_formatPreprocessorText,
                m_formatQtText,
                m_formatSingleComments,
                m_formatMultilineComments,
                m_formatQuotationText,
                m_formatMethods,
                m_formatKeywords
            );
            editor->setWordWrap(m_wordWrap);
            editor->setHideFindReplace(m_hideFindReplace);
            editor->smartClick = m_smartClick;
        }*/
        /* TODO
        if (!m_showTreeClasses) //ToolsOptions/General
            tabExplorer->setTabToolTip( 1, tr("Classes explorer is disabled, please enable it in the Options dialog") );
        else
            tabExplorer->setTabToolTip( 1, "" );
        */

        //m_assistant->setdocumentationDirectory( m_documentationDirectory );
        saveINI();
        QApplication::restoreOverrideCursor();
    }
    delete options;
}

QString configTreeView::loadINI()
{
    QSettings settings(QApplication::applicationDirPath() + "/ananas-designer.ini", QSettings::IniFormat);

    QString projectName;
    settings.beginGroup("Options");
    QString s = settings.value("m_font", m_font.toString()).toString();
    m_font.fromString(s);
    m_tabStopWidth = settings.value("m_tabStopWidth", m_tabStopWidth).toInt();
    m_cppHighlighter = settings.value("m_cppHighlighter", m_cppHighlighter).toBool();
    m_automaticCompilation = settings.value("m_automaticCompilation", m_automaticCompilation).toBool();
    m_lineNumbers = settings.value("m_lineNumbers", m_lineNumbers).toBool();
    m_autoIndent = settings.value("m_autoIndent", m_autoIndent).toBool();
    m_autoCompletion = settings.value("m_autoCompletion", m_autoCompletion).toBool();
    m_autobrackets = settings.value("m_autobrackets", m_autobrackets).toBool();
    // Divius: comments autoclose
    m_autocomments = settings.value("m_autocomments", m_autocomments).toBool();
    //
    m_selectionBorder = settings.value("m_selectionBorder", m_selectionBorder).toBool();
    m_saveBeforeBuild = settings.value("m_saveBeforeBuild", m_saveBeforeBuild).toBool();
    m_restoreOnStart = settings.value("m_restoreOnStart", m_restoreOnStart).toBool();
    m_promptBeforeQuit = settings.value("m_promptBeforeQuit", m_promptBeforeQuit).toBool();
    m_checkEnvironmentOnStartup = settings.value("m_checkEnvironmentOnStartup", m_checkEnvironmentOnStartup).toBool();
    m_autoMaskDocks = settings.value("m_autoMaskDocks", m_autoMaskDocks).toBool();
    m_endLine = (EndLine)settings.value("m_endLine", m_endLine).toInt();
    m_tabSpaces = settings.value("m_tabSpaces", m_tabSpaces).toBool();
    m_match = settings.value("m_match", m_match).toBool();
    m_highlightCurrentLine = settings.value("m_highlightCurrentLine", m_highlightCurrentLine).toBool();
    m_backgroundColor = QColor(settings.value("m_backgroundColor", m_backgroundColor).toString());
    m_textColor = QColor(settings.value("m_textColor", m_textColor).toString());
    m_currentLineColor = QColor(settings.value("m_currentLineColor", m_currentLineColor).toString());
    m_matchingColor = QColor(settings.value("m_matchingColor", m_matchingColor).toString());
    m_projectsDirectory = settings.value("m_projectsDirectory", m_projectsDirectory).toString();
    m_pluginsDirectory = settings.value("m_pluginsDirectory", m_pluginsDirectory).toString();
    m_includeDirectory = settings.value("m_includeDirectory", m_includeDirectory).toString();
    m_documentationDirectory = settings.value("m_documentationDirectory", m_documentationDirectory).toString();
    m_makeOptions = settings.value("m_makeOptions", m_makeOptions).toString();
    m_showTreeClasses = settings.value("m_showTreeClasses", m_showTreeClasses).toBool();
    m_closeButtonInTabs = settings.value("m_closeButtonInTabs", m_closeButtonInTabs).toBool();
    m_displayEditorToolbars = settings.value("m_displayEditorToolbars", m_displayEditorToolbars).toBool();
    //slotUpdateOtherFileActions();
    m_displayWhiteSpaces = settings.value("m_displayWhiteSpaces", m_displayWhiteSpaces).toBool();
    m_rightMarginLine = settings.value("m_rightMarginLine", m_rightMarginLine).toInt();
    m_wordWrap = settings.value("m_wordWrap", m_wordWrap).toBool();
    m_hideFindReplace = settings.value("m_hideFindReplace", m_hideFindReplace).toInt();
    m_smartClick = settings.value("m_smartClick", m_smartClick).toBool();

        //if( !QDir().exists(m_includeDirectory) )
        //{
        //        QMessageBox::warning(0,
        //                "Ananas-designer", tr("The Qt include directory doesn't exist,\nYou can change in Options dialog."), tr("Ok") );
//
       // }
    //setCrossButton( !m_closeButtonInTabs );
    m_intervalUpdatingClasses = settings.value("m_intervalUpdatingClasses", m_intervalUpdatingClasses).toInt();
    if ( m_currentLineColor == Qt::black )
        m_currentLineColor = QColor();
    m_mibCodec = settings.value("m_mibCodec", m_mibCodec).toInt();
    //
    m_formatPreprocessorText.setForeground( QColor(settings.value("m_formatPreprocessorText", m_formatPreprocessorText.foreground().color().name()).toString() ) );
    m_formatQtText.setForeground( QColor(settings.value("m_formatQtText", m_formatQtText.foreground().color().name()).toString() ) );
    m_formatSingleComments.setForeground( QColor(settings.value("m_formatSingleComments", m_formatSingleComments.foreground().color().name()).toString() ) );
    m_formatMultilineComments.setForeground( QColor(settings.value("m_formatMultilineComments", m_formatMultilineComments.foreground().color().name()).toString() ) );
    m_formatQuotationText.setForeground( QColor(settings.value("m_formatQuotationText", m_formatQuotationText.foreground().color().name()).toString() ) );
    m_formatMethods.setForeground( QColor(settings.value("m_formatMethods", m_formatMethods.foreground().color().name()).toString() ) );
    m_formatKeywords.setForeground( QColor(settings.value("m_formatKeywords", m_formatKeywords.foreground().color().name()).toString() ) );
    //actionEditor_mode->setChecked( settings.value("editorMode", actionEditor_mode->isChecked()).toBool() );
    settings.endGroup();

    /* TODO
    if (!m_showTreeClasses) //ToolsOptions/General
        tabExplorer->setTabToolTip( 1, tr("Classes explorer is disabled, please enable it in the Options dialog") );
    else
        tabExplorer->setTabToolTip( 1, "" );
    */
    //m_assistant->setdocumentationDirectory( m_documentationDirectory );
    // Load shortcuts
    settings.beginGroup("Shortcuts");
    QList<QObject*> childrens;
    childrens = children();
    QListIterator<QObject*> iterator(childrens);
    while ( iterator.hasNext() )
    {
        QObject *object = iterator.next();
        QAction *action = qobject_cast<QAction*>(object);

        if ( (action) && (!(action->data().toString().contains( "Recent|" ))) && (!action->objectName().isEmpty()) )
        {
            QString text = object->objectName();

            if ( !text.isEmpty() )
            {
                QString shortcut = action->shortcut();
                shortcut = settings.value(text, shortcut).toString();
                action->setShortcut( shortcut );
            }
        }
    }
    settings.endGroup();

    if ( m_restoreOnStart )
    {
        settings.beginGroup("Project");
        projectName = settings.value("absoluteNameProjectFile").toString();
        settings.endGroup();
    }
    //
    settings.beginGroup("mainwindowstate");
#ifdef Q_OS_WIN32
    // Restores position, size and state for both normal and maximized/fullscreen state. Problems reported unter X11.
    // See Qt doc: Geometry: Restoring a Window's Geometry for details.
    restoreGeometry(settings.value("geometry", saveGeometry()).toByteArray()); // Window geometry and state.
#else
    // Restores position, size and state including maximized/fullscreen.
    move(settings.value("pos", pos()).toPoint()); // Window position.
    resize(settings.value("size", size()).toSize()); // Window size.
    // Note: Yes, the window can be maximized and fullscreen!
    if (settings.value("maximized", isMaximized()).toBool()) // Window maximized.
        setWindowState(windowState() | Qt::WindowMaximized);
    if (settings.value("fullscreen", isFullScreen()).toBool()) // Window fullscreen.
        setWindowState(windowState() | Qt::WindowFullScreen);
#endif
    //restoreState(settings.value("state", saveState()).toByteArray()); // Toolbar and DockWidget state.
    settings.endGroup();

        //settings.beginGroup("editormode");
        //QList<QDockWidget*> dockWidgets = findChildren<QDockWidget*>();
        //foreach( QWidget *w, dockWidgets )
        //        toolbarStatus[w] = settings.value(w->objectName(),w->isVisible()).toBool();
        //settings.endGroup();

    return projectName;
}

void configTreeView::saveINI()
{
    qDebug() << "saveINI()";
    QSettings settings(qApp->applicationDirPath () + "/ananas-designer.ini", QSettings::IniFormat);

    settings.beginGroup("Options");
    settings.setValue("m_showTreeClasses", m_showTreeClasses);
    settings.setValue("m_intervalUpdatingClasses", m_intervalUpdatingClasses);
    settings.setValue("m_automaticCompilation", m_automaticCompilation);
        settings.setValue("m_font", m_font.toString());
    settings.setValue("m_tabStopWidth", m_tabStopWidth);
    settings.setValue("m_cppHighlighter", m_cppHighlighter);
    settings.setValue("m_lineNumbers", m_lineNumbers);
    settings.setValue("m_autoIndent", m_autoIndent);
    settings.setValue("m_selectionBorder", m_selectionBorder);
    settings.setValue("m_saveBeforeBuild", m_saveBeforeBuild);
    settings.setValue("m_restoreOnStart", m_restoreOnStart);
    settings.setValue("m_promptBeforeQuit", m_promptBeforeQuit);
    settings.setValue("m_autoCompletion", m_autoCompletion);
    settings.setValue("m_autobrackets", m_autobrackets);
    // Divius: comments autoclose
    settings.setValue("m_autocomments", m_autocomments);
    //
    settings.setValue("m_closeButtonInTabs", m_closeButtonInTabs);
    settings.setValue("m_match", m_match);
    settings.setValue("m_highlightCurrentLine", m_highlightCurrentLine);
    settings.setValue("m_checkEnvironmentOnStartup", m_checkEnvironmentOnStartup);
    settings.setValue("m_endLine", m_endLine);
    settings.setValue("m_tabSpaces", m_tabSpaces);
    settings.setValue("m_backgroundColor", m_backgroundColor.name());
    settings.setValue("m_textColor", m_textColor.name());
    settings.setValue("m_currentLineColor", m_currentLineColor.name());
    settings.setValue("m_matchingColor", m_matchingColor.name());
    settings.setValue("m_projectsDirectory", m_projectsDirectory);
    settings.setValue("m_pluginsDirectory", m_pluginsDirectory);
    settings.setValue("m_includeDirectory", m_includeDirectory);
    settings.setValue("m_documentationDirectory", m_documentationDirectory);
    settings.setValue("m_makeOptions", m_makeOptions);
    settings.setValue("m_mibCodec", m_mibCodec);
    //
    settings.setValue("m_formatPreprocessorText", m_formatPreprocessorText.foreground().color().name());
    settings.setValue("m_formatQtText", m_formatQtText.foreground().color().name());
    settings.setValue("m_formatSingleComments", m_formatSingleComments.foreground().color().name());
    settings.setValue("m_formatMultilineComments", m_formatMultilineComments.foreground().color().name());
    settings.setValue("m_formatQuotationText", m_formatQuotationText.foreground().color().name());
    settings.setValue("m_formatMethods", m_formatMethods.foreground().color().name());
    settings.setValue("m_formatKeywords", m_formatKeywords.foreground().color().name());
    settings.setValue("m_displayEditorToolbars", m_displayEditorToolbars);
    settings.setValue("m_displayWhiteSpaces", m_displayWhiteSpaces);
    settings.setValue("m_rightMarginLine", m_rightMarginLine);
    //settings.setValue("editorMode", actionEditor_mode->isChecked() );
    settings.setValue("m_wordWrap", m_wordWrap);
    settings.setValue("m_hideFindReplace", m_hideFindReplace);
    settings.setValue("m_smartClick", m_smartClick);
    settings.endGroup();

    // Save shortcuts
    settings.beginGroup("Shortcuts");
    QList<QObject*> childrens;
    childrens = children();
    QListIterator<QObject*> iterator(childrens);
    while ( iterator.hasNext() )
    {
        QObject *object = iterator.next();
        QAction *action = qobject_cast<QAction*>(object);

        if ( (action) && (!(action->data().toString().contains( "Recent|" ))))
        {
            QString text = action->objectName();
            if (!text.isEmpty())
            {
                QString shortcut = action->shortcut();
                settings.setValue( text, shortcut );
            }
        }
    }
    settings.endGroup();

    //if ( !m_projectManager )
    //return;

    //if ( m_restoreOnStart )
    //{
    //    settings.beginGroup("Project");
        //QString projectDirectory;
        //if ( m_projectManager )
        //    projectDirectory = m_projectManager->absoluteNameProjectFile(treeFiles->topLevelItem(0));
        //settings.setValue("absoluteNameProjectFile", projectDirectory);
    //    settings.endGroup();
    //}
    //
    settings.beginGroup("mainwindowstate");
    if (!isMinimized() && !isMaximized() && !isFullScreen())
    {
        settings.setValue("pos", pos());
        settings.setValue("size", size());
    }
    settings.setValue("maximized", isMaximized());
    settings.setValue("fullscreen", isFullScreen());
    settings.setValue("geometry", saveGeometry()); // Window geometry and state (only needed for Windows!).
    //settings.setValue("state", saveState()); // Toolbar and DockWidget state.
    settings.endGroup();

        //settings.beginGroup("editormode");
        //QList<QDockWidget*> dockWidgets = findChildren<QDockWidget*>();
        //foreach( QWidget *w, dockWidgets )
        //        settings.setValue( w->objectName(), toolbarStatus[w] );;
        //settings.endGroup();
}

void configTreeView::closeEvent( QCloseEvent * event )
{
    int response = 0;
//    if ( m_promptBeforeQuit )
//    {
//        response = QMessageBox::question(this, "Ananas-designer",
//                                         tr("Do you want to quit Ananas-designer ?"),
//                                         tr("Yes"), tr("No"), QString(), 0, 1);
//    }
//    if ( response == 1 )
//    {
//        event->ignore();
//        return;
//    }
    saveINI();

    //if ( slotCloseProject(true) )
    //{
    //    delete m_assistant;
    //    delete m_designer;
    //    this->hide();  //give an impression of quick close
    //    event->accept();
    //}
    //else
    //    event->ignore();
}

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
DomCfgItem *item = static_cast<DomCfgItem*> ( currentIndex().internalPointer() );
    if (a->text()==tr("Property")) {
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
    if (a->text()==tr("Open global module")) {
	if (m_tabEditors==0) {
		m_tabEditors = new TabWidget(this);
    		connect(m_tabEditors, SIGNAL(currentChanged(int)), this, SLOT(slotCurrentTabChanged(int)) );
    		connect(m_tabEditors, SIGNAL(currentChanged(int)), this, SLOT(slotUpdateOtherFileActions()) );
    
    
    	((QMainWindow*) parent()->parent())->setCentralWidget( m_tabEditors );
	}
	//m_tabEditors->tabInserted(0);
	m_completion = new InitCompletion (this);
	Editor *editor = new Editor(m_tabEditors, this, m_completion ,tr("Global module"));
	//m_tabEditors->setCurrentIndex( m_tabEditors->addTab(editor, tr("Global module")));
	
	QDomNode global =  item->root()->node().namedItem(md_root).namedItem(md_metadata).namedItem(md_globals).namedItem(md_sourcecode);
// //aLog::print(aLog::Debug,global.toElement().text());
	editor->setSyntaxHighlight( m_cppHighlighter );
    	editor->setLineNumbers( m_lineNumbers );
    	editor->setAutoIndent( m_autoIndent );
    	editor->setSelectionBorder( m_selectionBorder );
    	editor->setEndLine( m_endLine );
	editor->setModule(global);
	editor->open(0);
    	editor->setFocus();
	//setCrossButton( !m_closeButtonInTabs );
	m_tabEditors->addTab(editor, tr("Global module"));
	m_tabEditors->show();
	setCrossButton(true);
	//}
	}
   if (a->text()==tr("Open element view")) {
	openSprModule();
	}
}
void configTreeView::openSprModule()
{
	DomCfgItem *item = static_cast<DomCfgItem*> ( currentIndex().internalPointer() );
	if (m_tabEditors==0) {
		m_tabEditors = new TabWidget(this);
    		connect(m_tabEditors, SIGNAL(currentChanged(int)), this, SLOT(slotCurrentTabChanged(int)) );
    		connect(m_tabEditors, SIGNAL(currentChanged(int)), this, SLOT(slotUpdateOtherFileActions()) );
    
    
    	((QMainWindow*) parent()->parent())->setCentralWidget( m_tabEditors );
	}
	//m_tabEditors->tabInserted(0);
	QDomNode stringView =  item->node();
	m_completion = new InitCompletion (this);
	Editor *editor = new Editor(m_tabEditors, this, m_completion ,item->parent()->configName()+tr(" module"));

	editor->setSyntaxHighlight( m_cppHighlighter );
    	editor->setLineNumbers( m_lineNumbers );
    	editor->setAutoIndent( m_autoIndent );
    	editor->setSelectionBorder( m_selectionBorder );
    	editor->setEndLine( m_endLine );

	editor->setModule(stringView);
	editor->open(0);
    	editor->setFocus();
	//setCrossButton( !m_closeButtonInTabs );
	m_tabEditors->addTab(editor, item->parent()->configName()+" "+tr("module"));
	m_tabEditors->show();
	setCrossButton(true);
	//}
}
void configTreeView::setCrossButton(bool activate)
{
    if (crossButton && !activate)
    {
        crossButton->hide();
    }
    else if ( activate )
    {
        if ( !crossButton )
        {
            crossButton = new QToolButton(m_tabEditors);
            crossButton->setIcon( QIcon(":/images/cross.png") );
            connect(crossButton, SIGNAL(clicked()), this, SLOT(slotCloseCurrentTab()) );
            crossButton->setGeometry(0,0,32,32);
            m_tabEditors->setCornerWidget(crossButton);
        }
        crossButton->show();
    }
    m_tabEditors->setCloseButtonInTabs( !activate );
}

Editor * configTreeView::currentEditor()
{
	Editor *editor = 0;
	int currentIndex = m_tabEditors->currentIndex();
	if( currentIndex != -1 )
		editor = (Editor*) (m_tabEditors->widget( currentIndex ));
	return editor;
}

Editor * configTreeView::givenEditor(int i)
{
	return qobject_cast<Editor*> (m_tabEditors->widget(i));
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
	if ( nodeName==md_svfunction)
	{
		openSprModule();
	}


 }
}
void configTreeView::slotCloseCurrentTab()
{
    Editor *editor = currentEditor();
    if ( editor && !editor->close() )
        return;
    delete editor;
    //slotUpdateOtherFileActions();
}
//
void configTreeView::closeTab(int numTab)
{
    Editor *editor = givenEditor(numTab);
    if ( editor && !editor->close() )
        return;
    delete editor;
    //slotUpdateOtherFileActions();
}
//
void configTreeView::closeOtherTab(int numTab)
{
    Editor *noClose = givenEditor(numTab);
    QList<Editor *> editorlist;
    for (int i=0; i<m_tabEditors->count(); i++)
        editorlist.append( givenEditor(i) );
    foreach(Editor *editor, editorlist)
    {
        if ( editor != noClose )
        {

            if ( editor && !editor->close() )
                return;
            delete editor;
        }
    }
    //slotUpdateOtherFileActions();
}
configTreeView::~configTreeView()
{
    if ( m_completion )
    {
        m_completion->terminate();
        m_completion->wait();
        delete m_completion;
        m_completion = 0;
    }
    if( m_buildQtDatabase )
    {
    	m_buildQtDatabase->setStopRequired();
    	m_buildQtDatabase->wait();
    	delete m_buildQtDatabase;
    	m_buildQtDatabase = 0;
   	}
}

void configTreeView::checkQtDatabase()
{
 	if( !QFile::exists(ctagsName()) )
 		return;
 	//actionNewQtVersion->setEnabled(false);
     m_buildQtDatabase = new InitCompletion (this);
     connect(m_buildQtDatabase, SIGNAL(finished()), m_buildQtDatabase, SLOT(deleteLater()) );
     connect(m_buildQtDatabase, SIGNAL(showMessage(QString)), this, SLOT(slotShowMessage(QString)) );
     connect(m_buildQtDatabase, SIGNAL(finished()), this, SLOT(slotBuildQtDatabaseEnded()) );
     QStringList includes;
     includes << m_includeDirectory;
 #ifdef WIN32
     includes << QDir::cleanPath( QFileInfo(m_qmakeName).absoluteDir().path()+"/../src" ) ;
 #endif
     m_buildQtDatabase->setCtagsCmdPath( ctagsName() );
     m_buildQtDatabase->setQtInclude( includes );
     m_buildQtDatabase->slotInitParse(InitCompletion::CheckQtDatabase, QString(), QString(), true, false, true, QString());
	
}

void configTreeView::slotQmake()
{
	//slotBuild(false, false, true);
}
//
void configTreeView::slotShowMessage(QString message)
{
	//QMessageBox::information(this, "QDevelop", message);
}
//
void configTreeView::slotBuildQtDatabaseEnded()
{
// 	actionNewQtVersion->setEnabled(true);
// 	m_buildQtDatabase = 0;
//     if( m_buildQtDatabaseAsked )
//     	QMessageBox::information(this, "QDevelop", tr("The Qt classes database build is ended.") );
}
void configTreeView::slotGotoImplementation()
{
    Editor *editor = currentEditor();
    if ( editor  )
        editor->gotoImplementation();
}

void configTreeView::toggleBookmark(Editor *editor, QString text, bool activate, QTextBlock block)
{
    Bookmark bookmark;
    bookmark.first = editor;
    bookmark.second = block;
    int line = editor->currentLineNumber( block );
    if ( activate )
    {
        QString s = text;
        if ( s.length() > 50 )
            s = s.left(50)+" ...";
        QAction *action = new QAction(s, menuBookmarks);
        connect(action, SIGNAL(triggered()), this, SLOT(slotActivateBookmark()));
        //
        QAction *before = 0;
        QList<QAction *> actionsList = menuBookmarks->actions();
        foreach(QAction *actionBefore, actionsList)
        {
            Bookmark bookmarkAction = actionBefore->data().value<Bookmark>();
            if ( bookmarkAction.first == editor && editor->currentLineNumber( bookmarkAction.second ) > line )
            {
                before = actionBefore;
                break;
            }
        }
        //
        menuBookmarks->insertAction(before, action);
        QVariant v;
        v.setValue( bookmark );
        action->setData( v );
        actionActiveBookmark = action;
    }
    else
    {
        QList<QAction *> actionsList = menuBookmarks->actions();
        foreach(QAction *action, actionsList)
        {
            Bookmark bookmarkAction = action->data().value<Bookmark>();
            if ( bookmarkAction.first == editor && bookmarkAction.second == block )
            {
                delete action;
                break;
            }
        }
    }
}
