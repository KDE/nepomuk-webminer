/*
 * Copyright 2012 Jörg Ehrichs <joerg.ehrichs@gmx.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "krossextractor.h"

#include <kross/core/manager.h>
#include <kross/core/action.h>

#include <QLabel>
#include <QDebug>

KrossExtractor::KrossExtractor(const QString &scriptFile, QObject *parent)
    : WebExtractor(parent)
{
    m_scriptFile = new Kross::Action(this, "WebExtractor");

    QLabel *l = new QLabel;
    m_scriptFile->addObject(this, "WebExtractor", Kross::ChildrenInterface::AutoConnectSignals);

    m_scriptFile->setFile( scriptFile );
    m_scriptFile->trigger();
}

KrossExtractor::~KrossExtractor()
{
    delete m_scriptFile;
}

WebExtractor::Info KrossExtractor::info() const
{
    QVariantMap result = m_scriptFile->callFunction("info").toMap();

    WebExtractor::Info scriptInfo;
    scriptInfo.name = result.value("name").toString();
    scriptInfo.description = result.value("desscription").toString();
    scriptInfo.author = result.value("author").toString();
    scriptInfo.email = result.value("email").toString();
    scriptInfo.resource = result.value("resource").toString();
    scriptInfo.file = m_scriptFile->file();

    return scriptInfo;
}

void KrossExtractor::search(const QVariantMap &parameters)
{
    emit searchItems(parameters);
}

void KrossExtractor::extractItem(const QUrl &url)
{
    emit extractItemFromUri(url);
}
