#include <QtGui>
#include <QtCore>
#include <QtUiTools>
//#include <qvariant.h>
//#include <qimage.h>
//#include <qpixmap.h>

//#include "qtextcodec.h"

//#include <qstring.h>
//#include <qfile.h>
//#include <qstatusbar.h>
#include "acfg.h"
#include "qsproject.h"
#include "qsscript.h"
#include <qsinterpreter.h>
#include <qdialog.h>
//--#include <qwidgetfactory.h>
#include <QFormBuilder>
//#include <qobject.h>
//#include <q3textstream.h>
//#include <qbuffer.h>
#include <qsinterpreter.h>
//#include <qseditor.h>
//#include <q3process.h>
//#include <qapplication.h>
#include "awidget.h"
#include "formdesigner.h"
#include "mainform.h"
#include "alog.h"
#include "tabwidget.h"
#include "editor.h"
#include "textEdit.h"
#include "deditdialog.h"

//#include "qananastable.h"
//#include <unistd.h>

//extern QApplication *application;

/*
 *  Constructs a dEditDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
dEditDialog::dEditDialog(QWidget* parent, const char* name, Qt::WindowFlags fl)
    : QMainWindow(parent, name, fl)
{
    setObjectName("dEditDialog");
    setupUi(this);
    //connect(this,SIGNAL(getCurrentMd(DomCfgItem*)),this,SLOT(getMd(DomCfgItem*)));
    (void)statusBar();
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
dEditDialog::~dEditDialog()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void dEditDialog::languageChange()
{
    retranslateUi(this);
}

void dEditDialog::init()
{
	delete statusBar();
	//eModule->setInterpreter(new QSInterpreter());
//	fd = new aFormDesigner();
}


void dEditDialog::destroy()
{
    updateMD();
    //( (MainForm*)this->topLevelWidget() )->wl->remove( this );
    //( (MainForm*)this->topLevelWidget() )->removeTab(name());
}

void dEditDialog::getMd(DomCfgItem* i)
{
 qDebug() << "dEditDialog::getMd\n";
 i=item;
}

void dEditDialog::setData( DomCfgItem *o )
{
	item=NULL;
	QString parentClass;
	if (o) {
		item = o;
		//aCfg *md = o->md;
		//aCfgItem obj = o->obj;
		aAliasEditor *a = new aAliasEditor( item, tAliases );
		al = a;
		al->setData();
		eName->setText( item->attr( mda_name ) );
		eDescription->setText( item->attr( md_description ) );
	//QWidget* stringModule = tabWidget3->widget(2);
	TabWidget *m_tabEditors = new TabWidget(((configTreeView*)parentWidget()));
	InitCompletion *m_completion = new InitCompletion (this);
	Editor *editorM = new Editor(m_tabEditors, ((configTreeView*)parentWidget()), m_completion ,tr("Module"));

	QDomNode Modul = item->node().namedItem(md_sourcecode);
	editorM->setModule(Modul);
	editorM->open(0);
	//QDomNode groupModul = item->node().namedItem(md_group).namedItem(md_string_view).namedItem(md_svfunction);
	
	m_tabEditors->addTab(editorM, tr("Module"));
	
	moduleGrid->addWidget(m_tabEditors);
	m_tabEditors->setTabPosition(QTabWidget::South);
	m_tabEditors->show();


		//eModule->setText( item->attr( md_sourcecode ) );
		eFormFile->setText( QString("inputform_")+item->attr(mda_id)+QString(".ui"));
		setCaption( tr("Form:") + eName->text() );
		parentClass = item->parent()->node().nodeName();
		if ( parentClass == md_document ) {
//			cbFormMode->insertItem(QObject::tr("Document"));
		}
		if ( parentClass == md_catalogue ) {
//			cbDefault->insertItem(QObject::tr("selection"));
			cbFormMode->clear();
			cbFormMode->insertItem(QObject::tr("Elements list dialog"));
			cbFormMode->insertItem(QObject::tr("Element dialog"));
			cbFormMode->insertItem(QObject::tr("Group dialog"));
		}
		else cbSelect->setHidden( true );
		int i = item->attr( md_defaultmod).toInt();
		cbNew->setChecked( (i>>md_form_new)%2 );
		cbView->setChecked( (i>>md_form_view)%2 );
		cbEdit->setChecked( (i>>md_form_edit)%2 );
		cbSelect->setChecked( (i>>md_form_select)%2 );
		i = item->attr( mda_type ).toInt();
		cbFormMode->setCurrentItem( i );
		i = item->attr( mda_readonly ).toInt();
		cbReadOnly->setChecked( i );
		QString ui= item->attr( md_formdesign );
//		bPreview->setDisabled(ui.isEmpty());
	}
}

//#include "mainform.h"
//#include <formwindow.h>
//extern MainForm *mainform;

void dEditDialog::EditForm()
{

	//aCfg *md = item->md;
	//aCfgItem obj = item->obj, o;
	QString ui, s, tpl_name = "inputform.ui.tpl", tpldir;
	QFile f( eFormFile->text() );
//	char* arg;
//	int rc;
	aLog::print(aLog::Debug,tr("dEditDialog edit form"));
	//MainForm *mw = (MainForm*) topLevelWidget();
	//printf("end getting pointer to Main form\n");
	QStringList env;
//	QProcess proc( this );

#ifdef Q_OS_WIN32
	tpldir = qApp->applicationDirPath()+"/templates/";
#else
	tpldir = "/usr/share/ananas/templates/";
#endif
	tpldir = ":/designer/templates/";


   if (!f.exists())
   {
	aLog::print(aLog::Debug,tr("dEditDialog form file does not exists"));
	QDomNode cur = item->node();
	QDomNode node;
	for(int i=0;i<cur.childNodes().count();i++) {
	node = cur.childNodes().item(i);
	if (node.nodeName()==md_formdesign){
		ui=node.toElement().text();
	}
	}
	if (!ui.isEmpty())
	{

		aLog::print(aLog::Debug, tr("dEditDialog create form file from metadata"));
		QFile f1(eFormFile->text());
		f1.open(QIODevice::WriteOnly);
		f1.writeBlock(ui, strlen(ui));
		f1.close();

	}
	else
	{
		aLog::print(aLog::Debug, tr("dEditDialog create new form file from template"));
		QDomNode cur = item->node();
		QDomNode node;
		node = cur.parentNode().parentNode();
		if ( node.nodeName() == md_document ) tpl_name = "inputformdoc.tpl";
		if ( node.nodeName() == md_catalogue ) tpl_name = "inputformcat.tpl";
		if ( node.nodeName() == md_journal ) tpl_name = "inputformjourn.tpl";
		if ( node.nodeName() == md_report ) tpl_name = "inputformrep.tpl";
		QFile fi( tpldir+tpl_name );
		if ( fi.open( QIODevice::ReadOnly ))
		{
			if(f.open( QIODevice::WriteOnly ))
			{
				QTextStream tso( &f );
				tso.setEncoding( QTextStream::UnicodeUTF8 );
				QString form = fi.readAll();
//			printf("obj id = %i\n", md->id( o ) );
				form.replace( QString("$$$id$$$"), QString("%1").arg( item->attr(mda_id )));
				form.replace( QString("$$$name$$$"), QString("") );
				form.replace( QString("$$$caption$$$"), item->attr(mda_name ) );
				tso << form; //tsi.read();
				fi.close();
				f.close();
			}
			else
			{
				aLog::print(aLog::Error, tr("dEditDialog open file %1 for write").arg(f.name()));
			}
		}
		else
		{
			aLog::print(aLog::Error,tr("dEditDialog open file %1 for read from templates directory %2").arg(tpl_name).arg(tpldir));
		}
//		s=QString("cp inputform.ui.tpl ")+  eFormFile->text();
// 		system((const char *)s);
	}
   }
//}
   	if(!QFile(eFormFile->text()).exists())
   	{

		aLog::print(aLog::Error, tr("dEditDialog file %1 not exists").arg(eFormFile->text()));
   	}
	//formdesigner->addPluginPath("/usr/lib/qt4/plugins/designer/");
	formdesigner->fileOpen( eFormFile->text() );
   	aLog::print(aLog::Debug, tr("dEditDialog show formdesigner"));
	formdesigner->show();

}


void dEditDialog::EditModule()
{
//	cfgobj_editcode(objectptr);
}


void dEditDialog::updateMD()
{

    formdesigner->fileSaveAll();
	//aCfg *md = item->md;
	//aCfgItem obj = item->obj;
	QString ui;
	QFile f(eFormFile->text());
//if (cancelupdate) return;
	//al->updateMD();
	//item->setText(eName->text().stripWhiteSpace() );
aLog::print(aLog::Debug,"dEditDialog::updateMD() "+item->parent()->parent()->nodeName()+"\n");
	item->setAttr(mda_name, eName->text());
	item->setAttr(mda_type, QString::number(cbFormMode->currentItem()));
	item->setAttr(mda_readonly, QString::number(cbReadOnly->isChecked()));
	item->setAttr( md_description, eDescription->text() );
	QList<Editor *> allEditor = findChildren<Editor *>();

	allEditor.at(0)->save();
	//QString module = allEditor.at(0)->moduletext().childNodes().item(0).nodeValue();
	//item->setText(md_sourcecode,module);
//	ui.arg(cbDefault->currentItem());
//	md->setSText( obj, md_defaultmod, QString( "%1" ).arg( cbDefault->currentItem() ) );
	int i = (1<<md_form_new) * cbNew->isChecked() + \
		(1<<md_form_view) * cbView->isChecked() + \
		(1<<md_form_edit) * cbEdit->isChecked() +\
		(1<<md_form_select) * cbSelect->isChecked();
	item->setAttr(md_defaultmod, QString( "%1" ).arg( i ) );
	if (f.open(QIODevice::ReadOnly)){
		QTextStream ts(&f);
		ts.setEncoding(QTextStream::UnicodeUTF8);
		ui=ts.read();
		item->setText(md_formdesign, ui );
		f.close();
		f.remove();
	}

}



void dEditDialog::formPreview()
{
//    if ( !QFile::exists(eFormFile->text())) return;

	//aCfg *md = item->md;
	//aCfgItem obj = item->obj;
	QString ui, s;
	QFile f(eFormFile->text());

	if (!f.exists()){
		QDomNode cur = item->node();
		QDomNode node;
		for(int i=0;i<cur.childNodes().count();i++) {
		node = cur.childNodes().item(i);
		if (node.nodeName()==md_formdesign){
//aLog::print(aLog::Debug,node.toElement().text());
			ui=node.toElement().text();
		}
		}
		//ui= ->sText( obj, md_formdesign );
		if (!ui.isEmpty()) {
			f.open(QIODevice::WriteOnly);
			f.writeBlock(ui, strlen(ui));
			f.close();
		} else {
		    return;
			QFile fi("inputform.ui.tpl");
			if ( fi.open( QIODevice::ReadOnly ) && f.open( QIODevice::WriteOnly ) ){
				QTextStream tsi( &fi ), tso( &f );
				tso << tsi.read();
				fi.close();
				f.close();
			}
		}
	}

	QIODevice *d;
	d= &f;
	d->open(QIODevice::ReadOnly);
	//--QWidget *form = QWidgetFactory::create(d);
    QFormBuilder fb;
	
    fb.addPluginPath("/usr/lib/qt4/plugins/designer/");
//foreach (QString path, fb.pluginPaths())
//   	    printf("%s\n", path.toUtf8().data());
    QWidget *form = fb.load(d);
printf(QString("form ")+form->metaObject()->className()+QString("\n"));
//connect(this,SIGNAL(setMd(DomCfgItem*)),static_cast<aWidget*>(form),SLOT(setMdWidget(DomCfgItem*)));
	//emit setMd(item);
	//static_cast<aWidget*>(form)->item=item;
//printf(QString("Главный виджет ")+form->name()+QString("\n"));
	d->close();
	if (form) form->show();
	else printf("Error create form\n");
}




int dEditDialog::getDocId()
{
    return docId;
}
