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
class Editor;
class TabWidget;
class InitCompletion;
typedef QPair<Editor *, QTextBlock> Bookmark;
Q_DECLARE_METATYPE(Bookmark);

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
    //DomCfgItem *md;

	configTreeView(DomCfgItem *config,QWidget *parent=0);
	~configTreeView();
	Editor * currentEditor();
	Editor * givenEditor(int i);
	enum EndLine { Default, Unix, Windows };
	void closeTab(int numTab);
	void closeOtherTab(int numTab);
	InitCompletion* buildQtDatabase() { return m_buildQtDatabase; }
	void checkQtDatabase();
	int mibCodec() { return m_mibCodec;}
	void toggleBookmark(Editor *editor, QString text, bool activate, QTextBlock block);
        QMenu *bookmarksMenu() { return menuBookmarks; }
        QString ctagsName() { return m_ctagsName; }
	void addTabEditor(TabWidget* tab);
	//void ContextMenuAdd(  QMenu * m );
	//void deleteItem();
	//void moveUpItem ();
	//void moveDownItem ();
	//void renameItem();
	//void saveItem();
	//void loadItem();
private:
	QMenu *menuBookmarks;
	QAction *actionActiveBookmark;
	TabWidget* m_tabEditors;
	InitCompletion *m_completion;
	InitCompletion *m_buildQtDatabase;
	int m_mibCodec;
	bool m_smartClick;
	int m_tabStopWidth;
	QString m_qmakeName;
	QString m_makeName;
	QString m_makeOptions;
	QString m_gdbName;
	QString m_ctagsName;
	QString m_linguistName;
	QString m_lupdateName;
	QString m_lreleaseName;
	QString m_designerName;
	QString m_qtInstallHeaders;
	QString m_projectsDirectory;
	QString m_pluginsDirectory;
	QString m_includeDirectory;
	QString m_documentationDirectory;
	bool m_lineNumbers, m_selectionBorder, m_autoIndent, m_cppHighlighter;
	bool m_saveBeforeBuild;
	bool m_restoreOnStart;
	bool m_promptBeforeQuit;
	bool m_buildAfterDebug;
	bool m_checkEnvironmentOnStartup;
	bool m_autoMaskDocks;
	bool m_autoCompletion;
	bool m_autobrackets;
	// Divius: comments autoclose
	bool m_autocomments;
	//
	bool m_match;
	bool m_highlightCurrentLine;
	bool m_ctagsIsPresent;
	bool m_configureCompletionNeeded;
	QColor m_backgroundColor;
	QColor m_currentLineColor;
	QColor m_matchingColor;
	EndLine m_endLine;
	bool m_tabSpaces;
	bool m_closeButtonInTabs;
	bool m_displayEditorToolbars;
	bool m_displayWhiteSpaces;
	int m_rightMarginLine;
	bool m_wordWrap;
	int m_hideFindReplace;
        bool m_showTreeClasses;
        bool m_automaticCompilation;
        int m_intervalUpdatingClasses;
        QFont m_font;
        QColor m_textColor;
        QTextCharFormat m_formatPreprocessorText, m_formatQtText, m_formatSingleComments;
        QTextCharFormat m_formatMultilineComments, m_formatQuotationText, m_formatMethods, m_formatKeywords;
	void setCrossButton(bool activate);
	QToolButton *crossButton;
	void openSprModule();
        void saveINI();
        QString loadINI();
protected:
        void closeEvent( QCloseEvent * event );
private slots:
	void showmenu (const QPoint &p);
	void actionTree (QAction *a);
	void doubleClicked ( const QModelIndex & index );
	void slotCloseCurrentTab();
	void slotQmake();
	void slotShowMessage(QString);
	void slotGotoImplementation();
 	void slotOptions();
private slots:
        /*void slotClearAllWarningsErrors();
        void slotPreviousWarningError();
        void slotNextWarningError();
        void slotQmake();
        void slotOpen();
        void slotOpenProject();
        void slotSaveFile();
        void slotCompleteCode();
        void slotSaveFileAs();
        bool slotSaveAll();
        void slotBuild(bool clean=false, bool build=true, bool forceQmake=false, QString forceMode = QString());
        void slotBuildRelease();
        void slotBuildDebug();
        void slotForgetBuildMode();
        void slotNewProject();
        void slotRebuild();
        void slotStopBuild();
        void slotClean();
        void slotCut();
        void slotCopy();
        void slotPaste();
        void slotUndo();
        void slotRedo();
        void slotGotoLine();
        void slotSelectAll();
        void slotUnindent();
        void slotIndent();
        void slotFind();
        void slotReplace();
        void slotFindContinue();
        void slotFindPrevious();
        void slotEndBuild();
        bool slotDebug(bool executeOnly=false);
        void slotExecuteWithoutDebug();
        void slotContinueDebug();
        void slotEndDebug();
        void slotMessagesDebug(QString message);
        void slotFindFilesActivated(QListWidgetItem *item, QListWidgetItem *);
        void slotDoubleClickFindLines( QListWidgetItem *item);
        void slotModifiedEditor(Editor *editor, bool modified);
        void slotStepInto();
        void slotStepOver();
        void slotStepOut();
        void slotEditToGdb(QString);
        void slotOnPause();
        void slotOpenRecentFile();
        void slotOpenRecentProject();
        void slotAbout();
        void slotHelpQtWord();
        void slotOptions();
        void slotClickTreeFiles(QTreeWidgetItem *item, int);
        void slotCloseCurrentTab();
        void slotPreviousTab();
        void slotNextTab();
        void slotOtherFile();
        void slotSetFocusToEditor();
        void slotToggleBreakpoint();
        void slotShortcuts();
        void slotFindInFiles();
        void slotNewFile();
        void slotToggleComment();
        void slotComment();
        void slotUncomment();
        void slotParameters();
        void slotCurrentTabChanged(int index);
        void slotDebugVariables( QList<Variable> list);
        void slotAddDebugVariable();
        void slotRemoveDebugVariable();
        void slotToggleBookmark();
        void slotActivateBookmark(QAction *action=0);
        void slotPreviousBookmark();
        void slotNextBookmark();
        void slotClearAllBookmarks();
        void slotGotoMatchingBracket();
        void slotTextEditPlugin();
        void slotConfigPlugin();
        void slotPrint();
        void slotOpenFile();
        void slotGotoDeclaration();
        void slotGotoImplementation();
        void slotMethodsList();
        void slotNewQtVersion();
        void slotUpdateOtherFileActions();
        void on__triggered();
        void on_actionFullDocksView_triggered();
        void on_actionMinimalDocksView_triggered();        */
public slots:	
	void slotBuildQtDatabaseEnded();
};


#endif
