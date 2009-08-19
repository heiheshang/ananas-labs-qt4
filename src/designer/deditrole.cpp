#include <QtCore>
#include <QtGui>
#include "acfg.h"
//#include "awindowslist.h"
#include "deditrole.h"

//#include <qvariant.h>
//#include <qimage.h>
//#include <qpixmap.h>

#include "acfg.h"
//#include <qstatusbar.h>
//#include <qmessagebox.h>

/*
 *  Constructs a dEditRole as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
dEditRole::dEditRole(QWidget* parent, const char* name, Qt::WindowFlags fl)
    : QMainWindow(parent, name, fl)
{
    setupUi(this);

    (void)statusBar();
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
dEditRole::~dEditRole()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void dEditRole::languageChange()
{
    retranslateUi(this);
}

//extern aCfg cfg;

void dEditRole::setData( CfgForm *c, DomCfgItem *o)
{
//    aCfgItem alias;
//    int i, n, count;

    cf = c;
    obj = o;

    setCaption( tr("Role:") + obj->attr(mda_name));
    eName->setText( obj->attr(mda_name));
    eDescription->setText(obj->attr(md_description));
}

void dEditRole::init()
{
    delete statusBar();
}

void dEditRole::destroy()
{
    updateMD();
    //( (MainForm*)this->topLevelWidget() )->removeTab(name());
}
void
dEditRole::updateMD()
{
    obj->node().toElement().setAttribute(mda_name, eName->text().stripWhiteSpace());
    obj->node().toElement().setAttribute(md_description, eDescription->text() );
    //cf->initRoles();
    //( (MainForm*)this->topLevelWidget() )->wl->remove( this );
}

