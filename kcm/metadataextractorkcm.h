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

#ifndef METADATAEXTRACTORKCM_H
#define METADATAEXTRACTORKCM_H

#include <KDE/KCModule>

namespace NepomukMetaDataExtractor
{

class ConfigFetcher;
class PluginList;
class ConfigService;

namespace Extractor
{
class ExtractorFactory;
}

/**
 * @brief KCM class to show and manage all config widgets for the MetaData Extractor
 */
class MetaDataExtractorKCM : public KCModule
{
    Q_OBJECT
public:
    explicit MetaDataExtractorKCM(QWidget *parent, const QVariantList &list);

    void save();
    void load();

private:
    Extractor::ExtractorFactory *m_ef;
    ConfigFetcher *cfd;
    PluginList *pl;
    ConfigService *csd;
};
}

#endif // METADATAEXTRACTORKCM_H
