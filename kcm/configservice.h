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

#ifndef CONFIGSERVICE_H
#define CONFIGSERVICE_H

#include <QWidget>

namespace Ui
{
class ConfigService;
}

class QDBusServiceWatcher;

namespace NepomukMetaDataExtractor
{

/**
 * @brief The ConfigService class is used to enable/disable and setup the Nepomuk2::Service for the metadata extractor.
 */
class ConfigService : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigService(QWidget *parent = 0);
    ~ConfigService();

signals:
    void configChanged(bool changed);

public slots:
    void saveConfig();
    void resetConfig();

private slots:
    void serviceEnabled(bool enabled);

    void serviceRegistered();
    void serviceUnregistered();
    void changeSettings();

private:
    Ui::ConfigService *ui;
    QDBusServiceWatcher *m_watcher;
};
}
#endif // CONFIGSERVICE_H
