
#include <QtCore>
#include <QtGui>
//#include <QtScript>
#include <qvariant.h>
#include <qimage.h>
//#include <qpixmap.h>

#include <qstatusbar.h>
#include <qsinterpreter.h>
#include <qseditor.h>
#include "acfg.h"
#include "alog.h"
#include "tabwidget.h"
#include "InitCompletion.h"
#include "editor.h"
#include "deditfield.h"
#include "deditcat.h"
/*
 *  Constructs a dEditCat as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
dEditCat::dEditCat(QWidget* parent, const char* name, Qt::WindowFlags fl)
    : QMainWindow(parent, name, fl)
{
    setupUi(this);

    (void)statusBar();
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
dEditCat::~dEditCat()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void dEditCat::languageChange()
{
    retranslateUi(this);
}

void dEditCat::setData( DomCfgItem *o )
{
//     long int i;
//     int fid, fcount, fieldid, stdf;
     item = o;
//     aCfg *md = o->md;
//     aCfgItem obj = o->obj, sv, g, e, field;
// 
//     aAliasEditor *a = new aAliasEditor( md, obj, tAliases );
//     al = a;
//     al->setData();
//     aRoleEditor *r = new aRoleEditor( md, obj, tRoles, md_catalogue );
//     re = r;
//     re->setData();
     setCaption( tr("Catalogue:") + o->attr( mda_name ) );
     eName->setText( o->attr( mda_name ) );
     eDescription->setText( o->attr( md_description ) );
//     g = md->find( obj, md_group ); // Find group context
//     e = md->find( obj, md_element ); // Find Element context
//     sv = md->find( g, md_string_view ); // Group string view
// //====================================
     //eStrViewFG->setText( o->attr(  md_svfunction ) );
     //eSvG->insertItem( "[ standart function ]" );
     //eSvG->insertItem( "[ user function ]" );
//     fid = md->sText( sv, md_fieldid ).toInt();
//     stdf = md->attr( sv, mda_stdf ).toInt();
//     eSvG->setCurrentItem( 0 );
     DomCfgItem *fields = o->find(md_element);//->find(md_field);
     listField->setSortingEnabled(false);
     QTableWidgetItem *newItem;
     listField->setRowCount(fields->childCount());
     listField->setColumnCount(2);
     for(int i=0;i<fields->childCount();i++) {
	if (fields->child(i)->nodeName()==md_field) {
	
	newItem = new QTableWidgetItem(fields->child(i)->attr(mda_name));
	newItem->setFlags(Qt::ItemIsSelectable);
	listField->setItem(i,0,newItem);
	QStringList type = fields->child(i)->attr(mda_type).split(QRegExp("\\b"));
aLog::print(aLog::Debug,o->getNameByType(type));	
	newItem = new QTableWidgetItem(o->getNameByType(type));
		newItem->setFlags(Qt::ItemIsSelectable);
		listField->setItem(i,1,newItem);
		listField->setRowHeight(i,20);
	}
	}
	connect(listField,SIGNAL(cellDoubleClicked ( int , int ) ),this,SLOT(doubleClicked(int,int)));
	connect(editButton,SIGNAL(pressed() ),this,SLOT(edit_field()));

	//QWidget* stringModule = tabWidget2->widget(2);
	/*TabWidget *m_tabEditors = new TabWidget(((configTreeView*)parentWidget()));
	InitCompletion *m_completion = new InitCompletion (this);
	Editor *editorE = new Editor(m_tabEditors, ((configTreeView*)parentWidget()), m_completion ,tr("Element module"));

	QDomNode elementModul = item->node().namedItem(md_element).namedItem(md_string_view).namedItem(md_svfunction);
	editorE->setModule(elementModul);
	editorE->open(0);
	QDomNode groupModul = item->node().namedItem(md_group).namedItem(md_string_view).namedItem(md_svfunction);
	editorE->setModule(groupModul);
	editorE->open(0);
	Editor *editorG = new Editor(m_tabEditors, ((configTreeView*)parentWidget()), m_completion ,tr("Group module"));
	m_tabEditors->addTab(editorE, tr("Element module"));
	m_tabEditors->addTab(editorG, tr("Group module"));
	modulLayout->addWidget(m_tabEditors);
	m_tabEditors->setTabPosition(QTabWidget::South);
	m_tabEditors->show();*/
//     for ( i = 0; i < md->count( g, md_field ); i++ ) {
// 	field = md->find( g, md_field, i );
// 	eSvG->insertItem( QString("%1").arg( md->attr( field, mda_name ) ) );
// 	fieldid = md->id( field );
// 	fieldsg.insert( i, new int( fieldid ) );
// 	if ( fid == fieldid && !stdf ) {
// 	    eSvG->setCurrentItem( i + 2 );
// 	    eStrViewFG->setEnabled( FALSE );
// 	}
//     }
//     if ( !fid && !stdf ) {
// 	eStrViewFG->setEnabled( TRUE );
// 	eSvG->setCurrentItem( 1 );
//     }
// 
// //====================================
//     sv = md->find( e, md_string_view );
//     eStrViewF->setText( md->sText( sv, md_svfunction ) );
//     eSv->insertItem( "[ standart function ]" );
//     eSv->insertItem( "[ user function ]" );
//     fid = md->sText( sv, md_fieldid ).toInt();
//     stdf = md->attr( sv, mda_stdf ).toInt();
//     eSv->setCurrentItem( 0 );
//     for ( i = 0; i < md->count( e, md_field ); i++ ) {
// 	field = md->find( e, md_field, i );
// 	eSv->insertItem( QString("%1").arg( md->attr( field, mda_name ) ) );
// 	fieldid = md->id( field );
// 	fields.insert( i, new int( fieldid ) );
// 	if ( fid == fieldid && !stdf ) {
// 	    eSv->setCurrentItem( i + 2 );
// 	    eStrViewF->setEnabled( FALSE );
// 	}
//     }
//     if ( !fid && !stdf ) {
// 	eStrViewF->setEnabled( TRUE );
// 	eSv->setCurrentItem( 1 );
//     }
}
void dEditCat::edit_field()
{
    int currentRow;
    dEditField *dialog = new dEditField(this,"",Qt::Dialog|Qt::WDestructiveClose);
    currentRow = listField->currentRow();
    if (currentRow==-1) {
        currentRow=1;
    }
    dialog->setData(item->child(0)->child(currentRow));
    dialog->show();
}
void dEditCat::doubleClicked ( int row, int ) {
    dEditField *dialog = new dEditField(this,"",Qt::Dialog|Qt::WDestructiveClose);
    dialog->setData(item->child(0)->child(row));
    dialog->show();
}
void dEditCat::init()
{
	delete statusBar();
	
//	eStrViewF->setInterpreter( new QSInterpreter() );
//	eStrViewF->setEnabled( FALSE );
}


void dEditCat::updateMD()
{
//     	aCfg *md = item->md;
// 	aCfgItem obj = item->obj, sv, g, e;
// 
// 	al->updateMD();
// 	re->updateMD();
// 	item->setText( 0, eName->text().stripWhiteSpace() );
// 	md->setAttr( obj, mda_name, eName->text().stripWhiteSpace() );
// 	md->setSText( obj, md_description, eDescription->text() );
// 	g = md->find( obj, md_group ); // Find group context
// 	e= md->find( obj, md_element ); // Find Element context
// 	//======== Element view save
// 	sv = md->find( e, md_string_view );
// 	if ( sv.isNull() ) sv = md->insert( e, md_string_view );
// 	md->setSText( sv, md_svfunction, eStrViewF->text() );
// 	if ( eSv->currentItem() == 0 ) md->setAttr( sv, mda_stdf, "1" );
// 	else {
// 	    md->setAttr( sv, mda_stdf, "0" );
// 	    if ( eSv->currentItem() == 1 ) md->setSText( sv, md_fieldid, "0" );
// 	    else md->setSText( sv, md_fieldid, QString("%1").arg( *(fields.find( eSv->currentItem() - 2 ) ) ) );
// 	}
// 	//======== Group view save
// 	sv = md->find( g, md_string_view );
// 	if ( sv.isNull() ) sv = md->insert( g, md_string_view );
// 	md->setSText( sv, md_svfunction, eStrViewFG->text() );
// 	if ( eSvG->currentItem() == 0 ) md->setAttr( sv, mda_stdf, "1" );
// 	else {
// 	    md->setAttr( sv, mda_stdf, "0" );
// 	    if ( eSvG->currentItem() == 1 ) md->setSText( sv, md_fieldid, "0" );
// 	    else md->setSText( sv, md_fieldid, QString("%1").arg( *(fieldsg.find( eSvG->currentItem() - 2 ) ) ) );
// 	}

}

void dEditCat::destroy()
{
    updateMD();
//     ( (MainForm*)this->topLevelWidget() )->wl->remove( this );
//     ( (MainForm*)this->topLevelWidget() )->removeTab(name());
}




void dEditCat::eSv_activated( int index )
{
//    if ( index == 1 ) eStrViewF->setEnabled( TRUE );
//    else eStrViewF->setEnabled( FALSE );
}

void dEditCat::eSvG_activated( int index )
{
//    if ( index == 1 ) eStrViewFG->setEnabled( TRUE );
//    else eStrViewFG->setEnabled( FALSE );
}
