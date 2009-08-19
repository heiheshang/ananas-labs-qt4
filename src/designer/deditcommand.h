#ifndef DEDITCOMMAND_H
#define DEDITCOMMAND_H

#include "ui_deditcommand.h"


class dEditCommand : public QMainWindow, public Ui::dEditCommand
{
    Q_OBJECT

public:
    dEditCommand(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::WType_TopLevel);
    ~dEditCommand();

    //ananasTreeView *vComActions;
    //aActionTreeView *actiontree;
    //InterfaceListViewItem * item;
    aAliasEditor *al;
    DomCfgItem *md;
    
public slots:
    virtual void updateMD();
    virtual void setData(DomCfgItem *o);
    virtual void bAddAction_clicked();
    virtual void bRemoveAction_clicked();
    virtual void bMoveUp_clicked();
    virtual void bMoveDown_clicked();

protected slots:
    virtual void languageChange();

private:
    void destroy();
    void init();

};

#endif // DEDITCOMMAND_H
