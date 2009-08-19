#ifndef DEDITROLE_H
#define DEDITROLE_H

#include "ui_deditrole.h"


class dEditRole : public QMainWindow, public Ui::dEditRole
{
    Q_OBJECT

public:
    dEditRole(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::WType_TopLevel);
    ~dEditRole();

public slots:
    virtual void setData( CfgForm * c, DomCfgItem *o );
    virtual void updateMD();

protected slots:
    virtual void languageChange();

private:
    DomCfgItem *obj;
    CfgForm *cf;
    int roleCount;

    void init();
    void destroy();

};

#endif // DEDITROLE_H
