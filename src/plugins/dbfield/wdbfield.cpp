/****************************************************************************
** $Id: wdbfield.cpp,v 1.1 2008/11/05 21:16:29 leader Exp $
**
** Code file of the field plugin of Ananas
** Designer and Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) 2003-2004 Grigory Panov, Yoshkar-Ola.
**
** This file is part of the Ananas Plugins of the Ananas
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

#include <stdlib.h>
#include <QtCore>
//#include <q3listbox.h>
//Added by qt3to4:
//#include <Q3ValueList>

#include "ananas.h"
#include "wfield.h"
#include "edbfield.h"
#include "wdbfield.h"
#include "addfdialog.h"
//--#include "mainform.h"
#include "alog.h"
//#include "deditdialog.h"


/*!
 * \en	Constructor without name. \_en
 * \ru 	Создает конструктор без указания имени виджета. \_ru
 */
wDBField::wDBField( QWidget *parent, Qt::WFlags fl )
    : wField( parent, "wDBField", fl)
{
	init();
}



/*!
 * \en	Constructor. \_en
 * \ru	Конструктор. \_ru
 */
wDBField::wDBField( QWidget *parent, const char *name, Qt::WFlags fl )
    : wField( parent, name, fl)
{
	init();
}



/*!
 * \en Destructor. \_en
 * \ru Деструктор. \_ru
 */
wDBField::~wDBField()
{

}


/*!
 * \en	Creates property editor dialog window. \_en
 * \ru 	Создает окно диалога редактора свойств.\_ru
 * 	\param parent - \en parent \_en \ru родитель \_ru
 */
QDialog*
wDBField::createEditor( QWidget *parent )
{
	return new addfdialog( parent );
}


/*!
 * \en	Opens property editor dialog form. \_en
 * \ru	Открывает окно диалога редактора свойств. \_ru
 */
void wDBField::openEditor()
{
/*
	QString s;
  QValueList<Q_ULLONG> bindList = getBindList();
  addfdialog e(this->topLevelWidget());
  getFields();
  e.setData( defDisplayFields, defFields,defId);
  e.ListBox->setCurrentItem(e.ListBox->findItem(getFieldName()));
	if ( e.exec()==QDialog::Accepted )
    	{
		s = e.getData(true);
		if(s.isEmpty()) return;
		setFieldName(s);
		s = e.getData(false);
		if(s=="0") return;
		// if select binding field
		if(bindList.find(s.toULongLong()) != bindList.end())
		{
			cfg_message(0, tr("field already binding, please select another field."));
			return;
		}
		setId(s.toInt());
		setEditorType();
    		updateProp();
    }
    */
}







/*!
 * \en 	Sets property value `fieldName'. \_en
 * \ru	Устанавливает значение свойства `fieldName'. \_ru
 */
void
wDBField::setFieldName( QString n )
{
    FieldName=n;
}


/*!
 * \en 	Gets field name in metadata. \_en
 * \ru	Получение имени поля в метаданных, на которое настроен виджет. \_ru
 * return - \en Field name. \_en \ru Имя поля в метаданных.\_ru
 */
/*
QString
wDBField::getMdFieldName() const
{
	aCfgItem o = md->find(getId());
	if(!o.isNull())
	{
		return md->attr(o,mda_name);
	}
	else
	{
		return getFieldName();
	}
}
*/
/*!
 * \en	Inits widget. Gets copy of metadata from designer window. \_en
 * \ru	Инициализирунт виджет.
 * 	Получает метаданные из окна дизайнера. \_ru
 */
void
wDBField::init()
{
	qulonglong id;
	id=0;
	//get copy of metadata
qDebug() << "wDBField:: 1 getMd()\n";
	md = getMd();
qDebug() << "wDBField:: 2 getMd()\n";
	if(md)
	{
		// get id of container object - catalogue or document
		qDebug() << "if(md)\n";
		id = aWidget::parentContainer(this)->getId();
		head = md->findObjectById(id);
	}
qDebug() << "wDBField::init()\n";
}


/*!
 * \en	Gets list of fields and id for Document.Header or Catalogue.Element and
 * 	Catalogue.Group \_en
 * \ru	Возвращает список полей, определенных в метаданных, для шапки документа
 * 	или элемента и группы каталога.
 *	К имени уже забинденного поля добавляет впереди звездочку.
 *	Для групп и элементов каталога добавляет в конце (element) или (group) \_ru
 * 	\return \en List of field name \_en \ru Список имен полей \_ru
 */
QStringList
wDBField::getFields()
{
  QStringList lst, dlst;
  QString str;
  int res,i;
  Q3ValueList<qulonglong> bindList = getBindList();
  DomCfgItem *o, *o_head;
  defId.clear();
  defFields.clear();
  defDisplayFields.clear();
qDebug() << "wDBField::getFields()";
  if(head!=0)
  {
	if(head->nodeName() == md_catalogue)
	{
//		printf("getting fields from metadata\n");
		o = head->child(md_element); //object element
	    	res = o->childCount();
//		printf("find elements\n");
	    	for( i = 0; i < res; i++ )
	    	{
//			printf("find %d elem\n",i);
			o_head = o->child(i);
			if (o_head->nodeName()!=md_field) continue;
			if(o_head->attr(mda_type).at(0)!=' ')
			{
//				printf("mda_type = %s\n",md->attr(o_head,mda_type).ascii());
				lst << o_head->attr(mda_name);
				dlst << o_head->attr(mda_name) + " (element)";
				defId << o_head->attr(mda_id);
			}
			else
			{
//				printf("calculated field not allowed here\n");
			}
	    	}
		o = head->child(md_group); // object group
	    	res = head->childCount();
//		printf("find groups\n");
	    	for( i = 0; i < res; i++ )
	    	{
//			printf("find %d group\n",i);
			o_head = o->child(i);
			if (o_head->node().nodeName()!=md_field) continue;
			//--if(md->attr(o,mda_type).left(1)!=' ')
			if(o->attr(mda_type).at(0)!=' ')
			{
				lst << o_head->attr(mda_name);
				dlst << o_head->attr(mda_name) + " (group)";
				defId << o_head->attr(mda_id);
			}
	    	}
	}
	else
	{
		o = head->child(md_header); // object header
	    	res = o->childCount();
	    	for( i = 0; i < res; i++ )
	    	{
			o_head = o->child(i);
			if (o_head->node().nodeName()!=md_field) continue;
			dlst << o_head->attr(mda_name);
			lst << o_head->attr(mda_name);
			defId << o_head->attr(mda_id);
	    	}
	}
	res = lst.count();
	for(int i=0; i<res; i++)
	{
		if(bindList.find(defId[i].toULongLong())!=bindList.end())
			str ="* ";
		else
			str ="";
		dlst[i] = str + dlst[i]; // addes available for binding fields name
	//	printf("defField[%d]=%s\n",i,defFields[i].ascii());
	}
  }
//<<<<<<< wdbfield.cpp
	defDisplayFields = dlst;
//	defDisplayFields = s;
	    	defFields = lst;
//=======
//>>>>>>> 1.37.2.4
return defFields;
}



//<<<<<<< wdbfield.cpp
/*
 * \en	Updates object. \_en
 * \ru 	Записывает значение объекта в базу. \_ru
*/
/*
int
wDBField::Update()
{
	if(dbobj)
	{
		return dbobj->Update();
	}
	else
	{
		return err_abstractobj;
	}
}
*/


/*
 * \en	Inits object. \_en
 * \ru 	Инициализирует объект. \_ru
*/
/*
aObject *
wDBField::createDBObject(aCfgItem o, aDatabase *adb )
{
//  wField::initObject( adb );
 // aCfgItem o;
 / o = md->find( getId() );
  aObject* obj = new aObject(o,adb);
  aCfgItem g = md->parent( o );//, e = md->find( obj, md_element );
  obj->tableInsert( aDatabase::tableDbName( *md, g ), g );
  debug_message("table inserted\n");
  return obj;
}
*/
void
wDBField::initObject(aDatabase *adb )
{
qDebug() << "wDBField::initObject(aDatabase *adb )";
  wField::initObject( adb );
qDebug() << "wDBField::initObject(aDatabase *adb ) after";
  //aCfgItem o;
  //o = md->find( getId() );
  //aObject* obj = new aObject(o,adb);
  //tableInsert( aDatabase::tableDbName( *md, o ), o );
//  debug_message("init dbfield  \n");
}

//=======
//>>>>>>> 1.37.2.4
/*!
 * \en	Set widget type, use property `Id'. \_en
 * \ru 	Устанавливает тип виджета, используя свойство `Id'. \_ru
 */
void
wDBField::setEditorType ()
{
qDebug() << "wDBField::setEditorType ()";
    DomCfgItem *o_head,*o;
    QString str, type;
    int id;
	if(head!=0)
	{
		id = property("Id").toInt();
		o_head = md->findObjectById(id);
		if(o_head!=0)
		{
			type = o_head->attr(mda_type);
			setFieldType(type);
			str = type.section(' ',0,0);
			// gets type of editor
			if(str=="N") wField::setEditorType(Numberic);
			if(str=="C") wField::setEditorType(String);
			if(str=="D") wField::setEditorType(Date);
			if(str=="B") wField::setEditorType(Boolean);
			if(str=="O")
			{
			//editor type is Object .
			//May be Catalog or Document.
				qulonglong tid;
				//gets object id.
				str = type.section(' ',1,1);
				tid = atol(str);
				o = md->findObjectById(tid);
				if(o!=0)
				{
					//gets object class
					str = o->node().nodeName();
					if(str == md_catalogue)
						// and set editor
						wField::setEditorType(Catalogue);
					if(str == md_document)
						wField::setEditorType(Document);

				}

			}

		}

	}

}


/*!
 * \en	Gets list of binding fields id. \_en
 * \ru	Получает список id забинденых полей. Корректно работает с удаленными
 * 	виджетами. \_ru
 * 	\return \en List of binding fields id. \_en
 * 		\ru Список id забинденых полей. \_ru
 */
QList<qulonglong>
wDBField::getBindList()
{
qDebug() << "QList<qulonglong>wDBField::getBindList()\n";
DomCfgItem *obj;
//QObjectList wList;
int id;
QList<qulonglong> listBindings;
wDBField* wfield;
QObject* wd = aWidget::parentContainer (this);
	listBindings.clear();
 QList<wDBField *> wList = wd->findChildren<wDBField *>();

    	//wList = wd->queryList( "wDBField" );
	QListIterator<wDBField*> it( wList ); // iterate over the wDBTable
	while ( it.hasNext() )
	{
		//wfield = qobject_cast<wDBField*>( it.next() );
		wfield = it.next();
		if(strcmp(wfield->name(),this->name())) // don't added current id
		{
		//don.t added deleted widgets
		   if(strncmp("qt_dead_widget_",wfield->name(),strlen("qt_dead_widget_")))
		   {
			id = wfield->property("Id").toULongLong();
	//		if(id>=0) // don't added negativ id (table while not selected)
	//		{
				listBindings << id;
				qDebug() << ">>>added id = " << id;
	//		}
		   }
		}
	}
	//--delete wList;
return listBindings;
}

