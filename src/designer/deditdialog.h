#ifndef DEDITDIALOG_H
#define DEDITDIALOG_H

#include "ui_deditdialog.h"


class dEditDialog : public QMainWindow, public Ui::dEditDialog
{
    Q_OBJECT

public:
    dEditDialog(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::WType_TopLevel);
    ~dEditDialog();

    int docId;
    aAliasEditor *al;
    aFormDesigner *fd;
    DomCfgItem  *item;

    virtual int getDocId();

public slots:
    virtual void init();
    virtual void destroy();
    virtual void setData( DomCfgItem *o );
    virtual void EditForm();
    virtual void EditModule();
    virtual void updateMD();
    virtual void getMd(DomCfgItem* i);
//signals:
//    void setMd(DomCfgItem *m);

protected slots:
    virtual void languageChange();

private slots:
    virtual void formPreview();

};

#endif // DEDITDIALOG_H
