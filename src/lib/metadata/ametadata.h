/****************************************************************************
** $Id: ametadata.h,v 1.5 2008/12/24 16:19:38 leader Exp $
**
** Header file of the Ananas configuration objects of Ananas
** Designer and Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) Andrey Paskal <app at lrn dot ru>, Yoshkar-Ola
** Copyright (C) 2003-2005 Grigory Panov <gr1313 at mail dot ru >, Yoshkar-Ola
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
#ifndef AMETADATA_H
#define AMETADATA_H

#include <QObject>
#include <QDate>

#include "ametaobject.h"
#include "ametaobjectgroup.h"
#include "ametadocument.h"
#include "ametacatalogue.h"
#include "ametainforegister.h"
#include "ametaaccregister.h"
#include "ametareport.h"
#include "ametajournal.h"
#include "ametainfo.h"
#include "ametaglobal.h"


#ifdef __BORLANDC__
#define CHECK_POINT 	printf("%s:%i %s()\n",__FILE__,__LINE__,__FUNC__);
#else
#define CHECK_POINT 	printf("%s:%i %s()\n",__FILE__,__LINE__,__FUNCTION__);
#endif

/*!
 * \~english Metadata object methods return codes. \~russian
 * Коды возвратов методов объекта метаданных. \~
 */
#define RC_OK		0	/**< All right - no errors */
#define RC_ERROR	1	/**< Error while executing method */


//#define aPermission		int

#include <QHash>
#include <QVariant>

/*!
 *\en
 *	Metadata object for metadata script access.
 *\_en
 *\ru
 *	\brief Объект метаданных для обеспечения доступа из скриптов.
 *	Наследует QObject.
 *\_ru
 */
class AMetaData: public AMetaGroup
{
Q_OBJECT
//Q_PROPERTY( AMetaDataInfo info READ info SCRIPTABLE true )
//Q_PROPERTY( AMetaDataGlobal* global READ global SCRIPTABLE true )
public:
    AMetaData();
    void clear();

    static AMetaData* metadata();

public slots:
    AMetaInfo *info(){ return &v_info;};
    AMetaGlobal *global(){ return &v_global;};

    AMetaDocuments*      documents();
    AMetaCatalogues*     catalogues();
    AMetaReports*        reports();
    AMetaJournals*       journals();
    AMetaInfoRegisters*  inforegisters();
    AMetaAccRegisters*   accregisters();

    QString test(){ return QString("AMETADATA TEST STRING");};

private:
    AMetaInfo v_info;
    AMetaGlobal v_global;
//groups
    AMetaCatalogues    v_catalogues;
    AMetaDocuments     v_documents;
    AMetaReports       v_reports;
    AMetaJournals      v_journals;
    AMetaInfoRegisters v_inforegisters;
    AMetaAccRegisters  v_accregisters;
};

#endif
