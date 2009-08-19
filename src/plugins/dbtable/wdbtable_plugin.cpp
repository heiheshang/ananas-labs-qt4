/****************************************************************************
**
** Code file
**
** Copyright (C) 2006 Dmitriy Pavlyuk <dm-p@rambler.ru>
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
**********************************************************************/

#include "wdbtable_plugin.h"

#include <QtPlugin>
#include <QDesignerFormEditorInterface>
#include <QExtensionManager>

#include "wdbtable_taskmenu.h"
#include "wdbtable.h"
#include "alog.h"

wDBTablePlugin::wDBTablePlugin(QObject *parent)
    : QObject(parent), m_initialized(false)
{
    m_initialized = false;
}

bool wDBTablePlugin::isContainer() const
{
    return false;
}

bool wDBTablePlugin::isInitialized() const
{
    return m_initialized;
}

void wDBTablePlugin::initialize(QDesignerFormEditorInterface *core)
{
    if (m_initialized)
        return;
    Q_UNUSED(core);

    QExtensionManager *manager = core->extensionManager();
    Q_ASSERT(manager != 0);
    manager->registerExtensions(new wDBTableTaskMenuFactory(manager),
                                Q_TYPEID(QDesignerTaskMenuExtension));

    m_initialized = true;
printf("wDBTable inited\n");
}

QWidget *wDBTablePlugin::createWidget(QWidget *parent)
{
qDebug() << "Создаем wDBTable inited\n";
    wDBTable *w = new wDBTable("", parent, 0);
qDebug() << "Создали wDBTable inited\n";
    return w;
}

QString wDBTablePlugin::name() const
{
qDebug() << "QString wDBTablePlugin::name()\n";
    return QLatin1String("wDBTable");
}

QString wDBTablePlugin::group() const
{
qDebug() << "QString wDBTablePlugin::group()\n";
    return QLatin1String("Ananas");
}

QString wDBTablePlugin::toolTip() const
{
    return QString("Ananas for QT database Table Widget");
}

QString wDBTablePlugin::whatsThis() const
{
    return QString("Entry to modify database tables values of Ananas's object");
}

QString wDBTablePlugin::includeFile() const
{
    return QLatin1String("wdbtable.h");
}

QIcon wDBTablePlugin::icon() const
{
    //return QIcon();
printf("QIcon wDBTablePlugin::icon()");
    return QIcon(":/images/wdbtable.png");
}

QString wDBTablePlugin::domXml() const
{
    return "<ui language=\"c++\">\n"
           "<widget class=\"wDBTable\" name=\"wDBTable\">\n</widget>\n"
	   "</ui>\n";
}
