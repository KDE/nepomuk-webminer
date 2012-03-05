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

#include <KDE/Kross/Manager>

KrossExtractor::KrossExtractor(const QString &scriptFile, QObject *parent)
    : WebExtractor(parent)
{
    m_scriptFile = new Kross::Action(this, "WebExtractor");

    m_scriptFile->addObject(this, "WebExtractor", Kross::ChildrenInterface::AutoConnectSignals);

    m_scriptFile->setFile( scriptFile );
    m_scriptFile->trigger();

    // load script info
    QVariantMap result = m_scriptFile->callFunction("info").toMap();
    m_scriptInfo.name = result.value("name").toString();
    m_scriptInfo.identifier = result.value("identifier").toString();
    m_scriptInfo.description = result.value("desscription").toString();
    m_scriptInfo.author = result.value("author").toString();
    m_scriptInfo.email = result.value("email").toString();
    m_scriptInfo.resource = result.value("resource").toString();
    m_scriptInfo.icon = result.value("icon").toString();
    m_scriptInfo.file = m_scriptFile->file();
}

KrossExtractor::~KrossExtractor()
{
    delete m_scriptFile;
}

WebExtractor::Info KrossExtractor::info() const
{
    return m_scriptInfo;
}

void KrossExtractor::search(const QVariantMap &parameters)
{
    emit searchItems(parameters);
}

void KrossExtractor::extractItem(const QUrl &url)
{
    emit extractItemFromUri(url);
}
