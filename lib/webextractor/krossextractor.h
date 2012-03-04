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

#ifndef KROSSEXTRACTOR_H
#define KROSSEXTRACTOR_H

#include "webextractor.h"

#include <KDE/Kross/Action>

class KrossExtractor : public WebExtractor
{
    Q_OBJECT
public:
    explicit KrossExtractor(const QString &scriptFile, QObject *parent = 0);
    ~KrossExtractor();

    WebExtractor::Info info() const;

    void search(const QVariantMap &parameters);
    void extractItem(const QUrl &url);

signals:
    void searchItems(const QVariantMap &parameters);
    void extractItemFromUri(const QUrl &url);

private:
    Kross::Action *m_scriptFile;
};

#endif // KROSSEXTRACTOR_H
