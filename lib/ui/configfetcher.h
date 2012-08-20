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

#ifndef CONFIGFETCHER_H
#define CONFIGFETCHER_H

#include <QWidget>

namespace NepomukMetaDataExtractor {
namespace Extractor {
    class ExtractorFactory;
}
}

namespace Ui {
class ConfigFetcher;
}

class ConfigFetcher : public QWidget
{
    Q_OBJECT
    
public:
    explicit ConfigFetcher(QWidget *parent = 0);
    ~ConfigFetcher();

    void setExtractorFactory(NepomukMetaDataExtractor::Extractor::ExtractorFactory  *ef);

signals:
    void configChanged(bool changed);

public slots:
    void updateConfiguration();
    void saveConfig();

private:
    void setupUi();
    Ui::ConfigFetcher *ui;
    NepomukMetaDataExtractor::Extractor::ExtractorFactory  *extractorFactory;
};

#endif // CONFIGFETCHER_H
