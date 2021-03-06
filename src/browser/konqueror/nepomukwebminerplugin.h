/*
 * Copyright 2012 Jörg Ehrichs <joerg.ehrichs@gmx.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NEPOMUKWEBMINERPLUGIN_H
#define NEPOMUKWEBMINERPLUGIN_H

#include <KParts/Plugin>
#include <KParts/ReadOnlyPart>
#include <KDE/KUrlLabel>

namespace NepomukWebMiner
{
namespace Extractor
{
class ExtractorFactory;
}
}

/**
 * @brief KPart to integrate the meta fetcher into @c Konqueror
 *
 * Shows a little nepomuk icon in the bottom right status par when a supported side is found, left click the
 * icon to start the fetching.
 */
class NepomukWebMinerPlugin: public KParts::Plugin
{
    Q_OBJECT

public:
    explicit NepomukWebMinerPlugin(QObject *parent = 0, const QVariantList &args = QVariantList());
    virtual ~NepomukWebMinerPlugin();

protected slots:
    void urlSwitched();
    void extract();

private slots:
    void lateInitialization();
    void pushDataToNepomuk(const QString &resourceType, const QVariantMap &entry);

private:
    KParts::ReadOnlyPart *m_Part;
    KUrlLabel m_icon;

    NepomukWebMiner::Extractor::ExtractorFactory *m_ef;
    bool extractionInProgress;

};

#endif // NEPOMUKWEBMINERPLUGIN_H
