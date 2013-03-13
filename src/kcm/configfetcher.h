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

#ifndef CONFIGFETCHER_H
#define CONFIGFETCHER_H

#include <QtGui/QWidget>

namespace Ui
{
class ConfigFetcher;
}

class QDBusServiceWatcher;

namespace NepomukWebMiner
{
namespace Extractor
{
class ExtractorFactory;
}

/**
 * @brief Simple Widget to control the KConfig parameters for the fetcher
 *
 * Shows the list of available completers and lets the user select its favorite.
 * Allows to set some other options to alter the fetching of citations, poster/banners.
 *
 * Manages the Nepomuk2::Service (enable/disable) and what kind of data should be fetched
 */
class ConfigFetcher : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigFetcher(QWidget *parent = 0);
    ~ConfigFetcher();

    void setExtractorFactory(NepomukWebMiner::Extractor::ExtractorFactory  *ef);

signals:
    void configChanged();

public slots:
    void serviceEnabled(bool enabled);

    void serviceRegistered();
    void serviceUnregistered();

    void saveConfig();
    void loadConfig();

private:
    void setupUi();
    Ui::ConfigFetcher *ui;
    QDBusServiceWatcher *m_watcher;
    NepomukWebMiner::Extractor::ExtractorFactory  *extractorFactory;
};

}

#endif // CONFIGFETCHER_H
