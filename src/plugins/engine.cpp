/****************************************************************************
** $Id: engine.cpp,v 1.6 2008/12/24 17:47:41 leader Exp $
**
** Code file of the Ananas Engine of Ananas
** Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) 2003-2004 Grigory Panov, Yoshkar-Ola.
**
** This file is part of the Library of the Ananas
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
#include <qsinterpreter.h>
#include <qsinputdialogfactory.h>
#include <qobject.h>
#include <q3valuelist.h>
#include <qstringlist.h>
#include <qstring.h>
#include <stdlib.h>
#include <qfile.h>
#include <qsproject.h>
#include <qsscript.h>
#include <qdialog.h>
//--#include <qwidgetfactory.h>
#include <qobject.h>
#include <qsutilfactory.h>
#include <qsinputdialogfactory.h>
#include <qapplication.h>
//Added by qt3to4:
#include <QTimerEvent>
//#include <Q3PopupMenu>
#include <QtGui>
#include <QtScript>
#include "ananas.h"
#include "wcatalogeditor.h"
#include "adatafield.h"
#include "alog.h"
#include "acombobox.h"

#include "ametaobject.h"
#include "engine.h"
QScriptValue message(QScriptContext *context, QScriptEngine *engine) {

QString s = context->argument(1).toString();
int num = context->argument(0).toInteger();
cfg_message(num,(const char *)s.utf8());
return QScriptValue();

}
/*!
 *	\~english
 *	Constructor
 *	\~russian
 *	Конструктор. Регистрирует следующие типу объектов для использования их в Ананас.Скрипте
 *	\arg \c PopupMenu всплывающее меню
 *	\arg \c Document документ
 *	\arg \c Catalogue каталог
 *	\arg \c CatalogueEditor редактор каталога
 *	\arg \c Report отчет
 *	\arg \c ARegister накопительный регистр
 *	\arg \c DataField поле (не работает)
 *	\arg \c Journal журнал
 *	\arg \c ATime объект для работы со временем
 *	\~
 */
aObjectsFactory::aObjectsFactory( aEngine *e )
{
	engine = e;
        if ( ! engine ) return;
        db = e->db;
        if ( ! db ) return;


	registerClass("MetaObject",&AMetaObject::staticMetaObject);
	registerClass("MetaGroup",&AMetaGroup::staticMetaObject);
	registerClass("Documents",&AMetaDocuments::staticMetaObject);
	registerClass("Catalogues",&AMetaCatalogues::staticMetaObject);
	registerClass("MetaInfo",&AMetaInfo::staticMetaObject);
	registerClass("MetaGlobal",&AMetaGlobal::staticMetaObject);


	registerClass("PopupMenu",&QMenu::staticMetaObject);
//	registerClass("PopupMenu","QApopupmenu");
	registerClass("Document",&aDocument::staticMetaObject);
	registerClass("Catalogue",&aCatalogue::staticMetaObject);
	registerClass("CatalogEditor",&wCatalogEditor::staticMetaObject);
	registerClass("Report",&aReport::staticMetaObject);
	registerClass("ARegister",&aARegister::staticMetaObject);
	registerClass("IRegister",&aIRegister::staticMetaObject);
	registerClass("ATime",&aTime::staticMetaObject);
	registerClass("DataField",&aDataField::staticMetaObject);
//	registerClass("reg","QAreg");
	registerClass("Journal",&aDocJournal::staticMetaObject);
	registerClass("ComboBox",&AComboBox::staticMetaObject);
//registerClass("djournalview","QAjournview");
//	registerClass("djournalview","QAdocjournal");
//	registerClass("form","AForms");
//	registerClass("report","QAreport");
//	registerClass("table","QAnanasTable");

//Register extensions classes
	QStringList extlist = AExtensionFactory::keys();
	for ( int i=0; i<extlist.count(); i++){
	    registerClass(extlist[i], AExtensionFactory::metaObject( extlist[i] ));
	}
}




/*!
 *	\~english
 *	Creates script object. Mapping Script name to real name.
 *	\~russian
 *	Создает объект. Отображает имена из скрипта в имена объектов библиотеки.
 *	\param className - \~english scripn object name \~russian имя объекта в скрипте \~
 *	\param arguments - \~english sarguments to constructor \~russian аргументы для конструктора \~
 *	\param context - \~english running context \~russian контекст, в котором выполняется скрипт \~
 *	\~
 */
QObject *
aObjectsFactory::create( const QString &className,
                     const QVariantList &arguments, QObject *context )
{
	QObject * res = 0;
//	context=context;
	if (className=="PopupMenu") {
//			return new QApopupmenu();
			res = new QMenu();
	}else if (className=="Document") {
		if (arguments.size()>0) {
			res = new aDocument(arguments[0].toString(), db );
		}
	}else if (className=="Catalogue") {
		if (arguments.size()>0) {
			res = new aCatalogue( arguments[0].toString(), db );
		}
	}else if ( className=="Report" ) {
		if ( arguments.size()>0 )
		{
			if(arguments.size()==2) {
				res = new aReport( arguments[0].toString(),(aReport::RT_type)arguments[1].toInt(), engine );
			}else{
				res = new aReport( arguments[0].toString(),aReport::RT_text, engine );
			}
 		}
	}else if (className =="CatalogEditor") {
		if (arguments.size()>0) {
			DomCfgItem *it;
			it = db->cfg->findByName(QString("Catalogue.%1").arg(arguments[0].toString()));
			if(it!=0) {
				wCatalogEditor * w = new wCatalogEditor(0,it->attr(mda_id).toInt());
				w->initCat(db);
				res = w;
			}
		}
	}else if (className == "ARegister") {
		if (arguments.size() > 0) {
			res = new aARegister(arguments[0].toString(), db);
		}

	}else if (className == "IRegister") {
		if (arguments.size() > 0) {
			res = new aIRegister(arguments[0].toString(), db);
		}

	} else if (className == "Journal") {
		if (arguments.size() > 0) {
			res = new aDocJournal(arguments[0].toString(), db);
		}

	} else if (className == "ATime") {
		  res = new aTime();
	} else if (className == "DataField") {
		  res = new aDataField();
	} else if ( AExtensionFactory::keys().contains( className )) {
//		printf("create extension object %s\n",(const char*) className);
		AExtension *obj = AExtensionFactory::create( className );
		// Insert check existence signal in future!!!!
		QObject::connect( obj, SIGNAL(event(const QString&)),
			 engine, SLOT( on_event(const QString&)));
		obj->init(db);
		res = obj;
	} else if ( className == "ComboBox" ) {
		if (arguments.size() == 1) {
			res = new AComboBox( 0, arguments[0].toString());
		} else {
			res = new AComboBox();
		}
	}
	if(!res) aLog::print(aLog::Error,QObject::tr("Unknown classname '%1' or metaobject '%2'").arg(className).arg(arguments[0].toString()));
	return res;
}



/*!
 *\en
 *	Constructor.
 *\_en
 *\ru
 *	Конструктор. Создает новый объект с именем "sys". Доступ к функциям этого объекта из Ананас.Скрипта возможен примерно так:
 *	\code
 *	sys.Date(); // текущая дата
 *	sys.OpenForm("DocJournal.Системный журнал.Form.Список документов"); // открывает форму "Список Документов" журнала "Системный журнал"
 *	\endcode
 *\_ru
 */
aEngine::aEngine():QObject(0,"sys")
{
	md = 0;
	next_obj_id = 1;
	db = aDatabase::database();
}



/*!
 *\en
 *	Destructor
 *\_en \ru
 *	Деструктор.
 *\_ru
 */
aEngine::~aEngine()
{
	values.clear();
//	delete project;
}



/*!
 * \~english
 * Init engine.
 * Init script subsystem, load global module.
 * \~russian
 *	Инициализирует объект. Инициализирует скриптовую систему, добавляет несколько глобальных функций.
 * \~
 */
bool
aEngine::init( const QString &rcfile )
{
//	QString mGlobal;
	DomCfgItem *gobj, *obj, *obj0;
	QString sysf =
//	"function Message( t, msg ){ sys.Message( t, msg );}"
//	"function StatusMessage( msg ){ sys.StatusMessage( msg );}"
//	"function Date(){ return sys.Date();}"
//	"function Time(){ return sys.Time();}"
//	"function Exit(){ return sys.Exit();}"
	"";
	QScriptValue objectWnd = script.newQObject(this);
		script.globalObject().setProperty("sys", objectWnd);
	bool ok = false;
	if ( db->init( rcfile ) ) {
		md = db->cfg;
		//code = project.interpreter();
//		code->setErrorMode(QSInterpreter::Notify);
		//code->setErrorMode(QSInterpreter::Nothing);
		//connect(code, SIGNAL( error ( const QString &, QObject *, const QString &, int )),
		//	this, SLOT  ( error ( const QString &, QObject *, const QString &, int )));
		//code->addObjectFactory( new QSInputDialogFactory );
		//code->addObjectFactory( new aObjectsFactory( this ) );
		//code->addObjectFactory( new QSUtilFactory );
		//project.addObject( this );
		//project.addObject( md );
		//project.addObject( AMetaData::metadata() );
		
		mGlobal = md->root()->node().namedItem(md_root).namedItem(md_metadata).namedItem(md_globals).namedItem(md_sourcecode).toElement().text();
		if ( ! mGlobal.isEmpty() ) {
//			project.createScript("global", mGlobal);
//			project.createScript(this, mGlobal );
//			project.createScript( "globalmodule", sysf );
//			project.createScript( this, sourcePreprocessor(mGlobal));
			//project.createScript( "globalmodule", sysf+sourcePreprocessor(mGlobal));
			QScriptValue error = script.evaluate(mGlobal);
		if (error.isError())
			cfg_message ( 3, ( const char * ) tr ( error.toString() ).utf8() );
		} else {
//                        printf("Global module is empty\n");
                }
	} else return false;
	return true;
}



/*!
 *\en
 *\_en \ru
 *	Заглушка.
 *\_ru
 */
void
aEngine::done()
{

}



/*!
 *\en
 *	Called on system start.
 *\_en \ru
 *	Автоматически вызывается при старте системы. Если в глобальном модуле определа функция on_systemstart()
 *	то она будет автоматически вызвана.
 *	Это можно использовать для открытия документов или журналов при старте системы, или каких-либо других действий.
 *	\see on_systemstop()
 *\_ru
 */
int
aEngine::on_systemstart(){
	//QScriptValue on_systemstart = script.evaluate("on_systemstart");
	//if (on_systemstart.isError()) {
	//qDebug() << on_systemstart.toString();
	//cfg_message ( 3, ( const char * ) tr ( on_systemstart.toString() ).utf8() );
	//}
	return 0;
}


void
aEngine::on_event( const QString &data )
{
	Q3ValueList<QVariant> lst;
	lst <<  sender()->name();
	lst << data;
	//if (project.interpreter()->functions().findIndex("on_event")!=-1) {
	//	project.interpreter()->call("on_event", QVariantList(lst));
	//}
	//emit event( sender()->name(), data );
}



/*!
 *\en
 *	Called on system stop.
 *\_en \ru
 *	Автоматически вызывается при остановке системы.
 *	Если в глобальном модуле определа функция on_systemstop()
 *	то она будет автоматически вызвана.
 *	\see on_systemstart()
 *\_ru
 */
int
aEngine::on_systemstop(){
	QScriptValue systemstop = script.evaluate("on_systemstop()");
	if (systemstop.isError())
	{
	cfg_message(1,systemstop.toString());
	}
	//if (project.interpreter()->functions().findIndex("on_systemstop")!=-1) {
	//	project.interpreter()->call("on_systemstop",QVariantList());
	//}
	return 0;
}



/*!
 *\en
 *	Stub.
 *\_en \ru
 *	Заглушка.
 *\_ru
 */
void
aEngine::timerEvent(QTimerEvent *e)
{
	e=e; //
//	project->interpreter()->evaluate(pr_timer, this);
}



/*!
 *\en
 *	Stub.
 *\_en \ru
 *	Заглушка. Возвращает всегда строку "???"
 *	В будущем возможно будет возвращать имя конфигурации.
 *\_ru
 */
QString
aEngine::cfgname() {
	return "???"; //cfg_info("name");
}



/*!
 *\en
 *\_en \ru
 *	Возвращает строковое представление текущего времени в формате "hh:mm:ss"
 *\_ru
 */
QString
aEngine::Time() {
	return QTime::currentTime().toString("hh:mm:ss");
}



/*!
 *\en
 *\_en \ru
 *	Возвращает строковое представление текущей даты в формате "dd.MM.yy"
 *\_ru
 */
QString
aEngine::Date() {
	return QDate::currentDate().toString("dd.MM.yy");
}


void
aEngine::Exit() {
	qApp->quit();
}

/*!
 *	\~english
 *	Outputs messages.
 *	\~russian
 *	Вывод сообщений.
 *	\param n - тип сообщения. 0 - уведомление, 1 - предупреждение, 2 - ошибка.
 *	\param msg - текст сообщения.
 *	\~
 *	\see cfg_message(int msgtype, const char *msgfmt,...)
 */
void
aEngine::Message(int n, const QString &msg)
{
	cfg_message(n, (const char *) msg.utf8());
}



/*!
 *\en
 *	Outputs status messages.
 *\_en \ru
 *	Вывод сообщений в строке состояния.
 *	\param msg - текст сообщения.
 *\_ru
 */
void
aEngine::StatusMessage( const QString &msg )
{
        emit statusMessage( msg );
}



/*!
 *\en
 *\_en \ru
 *\_ru
 */
void
aEngine::settimer(int sec, QString proc){
    // TODO Need id timer - get from startTimer
	//--killTimers();
	if (sec) {
		pr_timer=proc;
		startTimer(sec*1000);
	} else {
		pr_timer="";
	}
}



/*!
 *\en
 *\_en \ru
 *	Запускает действие
 *	\param id - идентификатор действия в метаданных.
 *\_ru
 */
void
aEngine::on_MenuBar( QAction *Act )
{
	DomCfgItem *obj;
	int actions, i;

	aLog::print(aLog::Debug,"menu selected "+Act->text()+" "+QString::number(Act->data().toInt())+"\n");
	if ( Act->data().toInt()>0 ) {

		QDomNode nodel = md->node();
		obj = new DomCfgItemActions(nodel,0,0);
		obj = obj->findObjectById(Act->data().toInt());
		if ( obj!=0 ) {
//aLog::print(aLog::Debug,"aEngine::on_MenuBar( QAction *Act )\n");
			//actions = obj->childCount();// md_comaction 
//			printf("obj found %i actions\n", actions);
			//for ( i=0; i<obj->childCount(); i++) {
			//	act = obj->child( i );
				//if (act->nodeName()!=md_comaction) continue;
				//Проверить
				//act = act->nodeValue().toLong() ;
				execAction( obj );
			//}
		}
	}
}



/*!
 *\en
 *\_en \ru
 *	Вспомогательный метод.
 *	Запускает действие в указанном контексте. Правильная работа не гарантируется (где-то там ошибки есть).
 *	\param act - ссылка на объект метаданных, описывающий действие
 *	\param context - контекст выполнения действия.
 *\_ru
 */
void aEngine::execAction( DomCfgItem *act, QObject *context )
{
//Вернуться
 	QString aModule, arg;
 	int atype, satype;
 	long oid, foid;
 	//aForm *form;
 	DomCfgItem *gobj, *obj;
 
 	if ( act!=0 ) {
 		atype = act->attr( mda_type ).toInt();
 		printf("atype=%i\n",atype);
 		switch ( atype ){
 		case 0:
 			satype = act->child(md_actiontype)->nodeValue().toInt();
 			oid = act->child( md_objectid )->nodeValue().toInt();
 			foid = act->child( md_formid )->nodeValue().toLong();
 			arg = act->child( md_argument )->nodeValue();
 			printf("satype=%d, oid=%d,foid=%d,arg=%s",satype,oid,foid,arg.ascii());
 			gobj =  md->findObjectById(oid);
 			if ( foid == 0 )
 			{
 			//Дописать
 			foid = md->getDefaultFormId(gobj,satype,0);
 				break;
 			}
 			if ( foid == 1 )
 			{
 				openEmbedCatalogueEditor(oid, NULL, false); //open to edit
 				break;
 			}
 
 			aForm *f = openForm(oid, foid, satype, 0, 0, 0 );
			if (f)
				f->show();

 			break;
// /*			if ( !arg.isEmpty() ) {
// 				obj = md->findName( md->find( gobj, md_forms ), md_form, arg );
// 				foid = md->id( obj );
// //				if ( !obj.isNull() ) printf("form found\n");
// //				else printf("! not found %s\n", (const char *) arg );
// 			}*/
// 			/*
// 			if ( wl->find( foid ) )
// 			{
// 				printf("form already exist,set focus \n");
// 				wl->get( foid )->setFocus();
// 			}
// 			else
// 			{
// 				printf("execute action\n");
// 			if ( oid )
// 				switch ( satype )
// 				{
// 				case md_action_new://create new
// 					printf("create new\n");
// 					form = new aForm( ws, this, foid );
// //					form->init();
// 					form->New();
// 					form->show();
// 					break;
// 				case md_action_view://view
// 					printf("view\n");
// 					form = new aForm( ws, this, foid );
// 										//form->init();
// 					form->show();
// 					break;
// 				}
// 			}
// 			break;
// 			*/
// 		case 1:
// 			aModule = act->attr( md_sourcecode );
// 			if ( !aModule.isEmpty() ) {
// 				code->evaluate( sourcePreprocessor(aModule), context, act->attr( mda_name ) );
// 			}
// 			break;
// 		case 2:
// 			break;
 		}
// //		printf("executed %s\n", ( const char *) md->attr( act, mda_name ).local8Bit() );
 	}
}



/*!
 *\~english
 *	Create and open new form.
 *\~russian
 *	Создает и открывает новую форму.
 *	Форма создается с учетом её id в конфигураци и действия,
 *	режима работы (просмотр, редактирование, создание нового объекта).
 *	Форма должна поддерживать тот режим, для которого она открывается.
 *	Если форма открыта для просмотра она открывается в режиме отлько чтение.
 *	Если id формы равно 0, делается попытка найти первую форму с
 *	требуемым режимом.
 *\~
 *\param oid - \~english form owner md id,\~russian id владельца формы в конфигурации,\~
 *\param fid - \~english form md id, \~russian id формы в конфигурации\~
 *\param mode - \~english form use mode, \~russian режим работы формы\~
 *\param ido - \~english database id of opening object\~russian идентификатор объекта в базе данных, для которого открывается форма.\~
 *\return \~english link on new form or 0 if form not created.
 			\~russian ссылку на новую форму или 0, если форма не создана.\~
 */
aForm*
aEngine::OpenForm(QString fname, int mode, aForm* selecter)//Q_ULLONG ido)
{
	DomCfgItem *object, *form;
qDebug() << "OpenForm " << fname;
	form = md->root()->findByName(fname);
	if(form!=0)
	{
	//qDebug() << "Parent form " << form->parent()->parent()->attr(mda_id);

//Вернуться
// 		object = md->parent(md->parent(form));
// 		if(object.isNull()) return 0;
 		qulonglong ido =0;
 		//if(selecter) ido = selecter->sysValue("id").toULongLong();
 		return openForm(form->parent()->parent()->attr(mda_id).toInt(), form->attr(mda_id).toInt(), mode, 0, 0);
	}
	else
	{
		return 0;
	}
}



/*!
 *\~english
 *	Create and open new form.
 *\~russian
 *	Создает и открывает новую форму.
 *	Форма создается с учетом её id в конфигураци и действия,
 *	режима работы (просмотр, редактирование, создание нового объекта).
 *	Форма должна поддерживать тот режим, для которого она открывается.
 *	Если форма открыта для просмотра она открывается в режиме отлько чтение.
 *	Если id формы равно 0, делается попытка найти первую форму с
 *	требуемым режимом.
 *	При необходимости поля формы заполняются в соответствии с параметром  \a id
 *\~
 *\param oid - \~english form owner md id,\~russian id владельца формы в конфигурации,\~
 *\param fid - \~english form md id, \~russian id формы в конфигурации,\~
 *\param defaultfor - \~english form use mode, \~russian режим работы формы,\~
 *\param mode - \~english not use yet\~russian пока не используется,\~
 *\param id - \~english database id of opening object\~russian идентификатор объекта в базе данных, для которого открывается форма. Используется только в режиме просмотра или редактирования.\~
 *\param caller - \~english caller widget\~russian Вызывающий данную функцию объект. Он будет обновлен автоматически при закрытии формы. \~
 *\return \~english link on new form or 0 if form not created.
 			\~russian ссылку на новую форму или 0, если форма не создана.\~
 */
aForm *
aEngine::openForm(int oid, int fid, int defaultfor, int mode, ANANAS_UID id, aWidget* caller )
{
	aForm *form = 0;
	if ( fid == 0 )
	{
//Вернуться
	fid = md->getDefaultFormId( md->findObjectById(oid), defaultfor, mode );
	}
	printf("engine:try found %d %llu in wl\n", fid, id);
	if ( wl->find( fid, id ) )
	{
		//printf("foubd!, set focus\n");
		wl->get( fid, id )->setFocus();
	}
	else {
		qDebug() << "not found!\n";
		aLog::print(aLog::Info,tr("aEngine open form %1 in mode %2, select %3").arg(fid).arg(mode).arg(id));
		if ( oid ) {
			switch ( defaultfor ) {
			case md_action_new://create new
				form = new aForm( ws, this, fid, caller );
				if(form)
				{
					form->setMode(0);
					form->New();
				}
				else
				{
					aLog::print(aLog::Error,tr("aEngine form create error"));
				}
				//printf("open new form\n");
				break;
			case md_action_view://view
				form = new aForm( ws, this, fid, caller );
				if(form)
				{
					form->setMode(1);
					form->SetReadOnly(true);
					form->Select( id );
				}
				else
				{
					aLog::print(aLog::Error,tr("aEngine form create error"));
				}
				//printf(" open form for view %llu\n",id);
				break;
			case md_action_edit:
				form = new aForm( ws, this, fid, caller );
				if(form)
				{
					form->setMode(2);
					form->Select( id );
				}
				else
				{
					aLog::print(aLog::Error,tr("aEngine form create error"));
				}
				//printf("open form for edit %llu\n ",id);
				break;
			default:
				aLog::print(aLog::Error, tr("aEngine open form mode %1 not supported").arg(defaultfor));
			}
			connect( this, SIGNAL( event(const QString &, const QString&)),
				 form, SLOT( on_event(const QString &, const QString&)));
		}
	}
	if(form && caller)
	{
		connect(form,SIGNAL(changedData()),caller,SLOT(Refresh()));
	}
//	printf("before clear\n");
//	this->project.interpreter()->clear();
	if ( form ) {
		ws->addWindow(form->form);
		//QDockWidget *dock = new QDockWidget(form->form->caption(),parentWidget);
		//((QMainWindow*)parentWidget)->addDockWidget( Qt::AllDockWidgetAreas,dock);
		//form->show();
		//QDockWidget *dock = parentWidget->findChild<QDockWidget *>(form->form->caption());
		//form->show();
};
//	printf("end open form\n");
	return form;
}



/*!
 *\~english
 *	Create and open new embedded catalog editor form.
 *\~russian
 *	Создает и открывает новую форму встроенного редактора каталога.
 *\~
 *\param oid - \~english catalogue md id,\~russian id каталога конфигурации,\~
 *\param parent - \~english parent wField object, \~russian родительский объект wField\~
 *\param toSelect - \~english Mode. true - for select, false - for edit\~russian Режим. true - выбор, false - редактирование\~
 */
void
aEngine::openEmbedCatalogueEditor(int oid, QWidget* parent,const bool toSelect)
{
	wCatalogEditor * w = new wCatalogEditor(parent,oid);
	if( parent )
	{
		connect(w,		SIGNAL(selected( qulonglong )),
		        (wField*)parent,SLOT(on_selected( qulonglong )));
		connect(w,		SIGNAL(destroyed_form()),
			(wField*)parent,SLOT(setFocus()));
	}

	w->initCat(db);
	if(toSelect)
	{
		w->select();
	}
	else
	{
		w->edit();
	}
}



/*!
 *\en
 *\_en \ru
 *	Печатает сообщение об ощибке.
 *\_ru
 */
void
aEngine::error ( const QString & message, QObject * context, const QString & scriptName, int lineNumber )
{
	//Message( 2, tr("Line:%1 Message:%2 Stack:(%3)").arg(lineNumber).arg(message).arg(code->stackTraceString()) );
}



/*!
 *\en
 *\_en \ru
 * Функция препроцессора исходного текста скрипта.
 * Для разработчиков:
 * В первую очередь может быть использована для замены коротких
 * имен из локального контекста на квалифицированные имена
 * глобальных функций и объектов, например Message() в контексте
 * формы может заменяться на global.Message() перед выполнением скрипта
 * и выполняться корректно из любого места.
 * В противном случае приходится дублировать глобальные
 * встроенные функции во всех контекстах.
 *\_ru
 */
QString
aEngine::sourcePreprocessor( const QString &src )
{
	QString psrc = src;
	// Global functions preprocessing
//	psrc.replace( "Message(", "global.Message(");
//	psrc.replace( "Statusglobal.Message(", "global.StatusMessage(");
//	psrc.replace( "Date(", "global.Date(");
//	psrc.replace( "Time(", "global.Time(");
	// Russian key words
//	psrc.replace( QString::fromUtf8("функция"), "function");
//	psrc.replace( QString::fromUtf8("если"), "if");
//	psrc.replace( QString::fromUtf8("то"), "then");
//	psrc.replace( QString::fromUtf8("иначе"), "else");
//	psrc.replace( QString::fromUtf8("начало"), "{");
//	psrc.replace( QString::fromUtf8("конец"), "}");
	return psrc;
}



/*!
 *\en
 *\_en \ru
 * Получение значения глобальной переменной.
 *\_ru
 */
QVariant
aEngine::value( const QString &name )
{
	QVariant v, *pv;
	pv = values.find( name );
	if ( pv ) return *pv;
	return v;
}



/*!
 *\en
 *\_en \ru
 * Установка значения глобальной переменной.
 *\_ru
 */
void
aEngine::setValue( const QString &name, QVariant value )
{
    values.remove( name );
    if ( !value.isValid() ) values.insert( name, new QVariant( value ) );
}



/*!
 *\en
 *\_en \ru
 * Установка значения глобальной переменной.
 *\_ru
 */
aDataField*
aEngine::enterValue( const QString &FieldType, const QString &title )
{
	aDataField *f = new aDataField( "", FieldType );
	QDialog *d = new QDialog( ws, title, true );
	wField *wf = new wField( d, "" );
	QPushButton *b_ok = new QPushButton( d, tr("OK") );
	QPushButton *b_cancel =new QPushButton( d, tr("Cancel") );
	connect( b_ok, SIGNAL( pressed() ), d, SLOT( accept() ) );
	connect( b_cancel, SIGNAL( pressed() ), d, SLOT( reject() ) );
	if ( d->exec() == QDialog::Accepted ) {
//		printf("accepted\n");
	}
	delete d;
	return f;
}

