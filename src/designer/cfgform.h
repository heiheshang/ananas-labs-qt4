#ifndef CFGFORM_H
#define CFGFORM_H
#include <QtGui>
#include <QtCore>
#include "awindowslist.h"
#include "acfg.h"
#include "adatabase.h"
//#include "ui_cfgform.h"

//class InterfaceTreeView;
//class aActionTreeView;
//class aMetadataTreeView;
//class aWindowsList;
class ANANAS_EXPORT CfgForm : public QDockWidget
{
    Q_OBJECT

public:
    CfgForm(const QString & title, QWidget * parent = 0, Qt::WindowFlags flags = Qt::WType_TopLevel);
    ~CfgForm();

    QHash<long, QListWidgetItem*> idList;
    //InterfaceTreeView *interfacetree;
    //aActionTreeView *actiontree;
    DomCfgItem *cfg;
    aDatabase *db;
    QWorkspace *ws;
    QTreeView *mdtree;
    //InterfaceTreeView *toolbartree;
    QString rcfile;
    aWindowsList *wl;

public slots:
    virtual void fileNew();
    virtual void fileOpen();
    virtual void fileSave();
    virtual void fileSaveAs();
    virtual void filePrint();
    virtual void fileExit();
    virtual void init(DomCfgItem *incfg, bool flagNew );
    virtual void initImageCollection();
    virtual void bAddImage_clicked();
    virtual void bRemoveImage_clicked();
    virtual void vImageCollection_itemRenamed( QListWidgetItem  * item, const QString & name );
    virtual void initLang();
    virtual void initRoles();
    virtual void save();
    virtual void bAdd_clicked();
    virtual void bDelete_clicked();
    virtual void bEdit_clicked();
    virtual void tLang_doubleClicked( int, int, int, const QPoint & );
    virtual void bKill_clicked();
    virtual void newObj();
    virtual void listCfg_onItem( QListWidgetItem  * );
    virtual void newField();
    virtual void newForm();
    virtual void newTable();
    virtual void tabWidget_selected( const QString & tab );
    virtual void bAddRole_clicked();
    virtual void bDeleteRole_clicked();
    virtual void bEditRole_clicked();
    virtual void bKillRole_clicked();
    virtual void tRole_doubleClicked( int, int, int, const QPoint & );

signals:
    void wActivate();
    void wDeactivate();

protected slots:
    virtual void languageChange();

private:
    void destroy();
    virtual void closeEvent( QCloseEvent * e );
    virtual int closeSubWindows();
};

#endif // CFGFORM_H
