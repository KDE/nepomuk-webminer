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

#ifndef VIDEOEXTRACTOR_H
#define VIDEOEXTRACTOR_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QRegExp>

#include <KDE/KUrl>

#include "../metadataparameters.h"

/**
  * copy of Sebastian Trüg's tvshowfilenameanalyzer
  */
class VideoExtractor : public QObject
{
    Q_OBJECT
public:
    explicit VideoExtractor(QObject *parent = 0);

    void parseUrl(MetaDataParameters *mdp, const KUrl &fileUrl);

private:
    QList<QRegExp> m_filenameRegExps;

};

#endif // VIDEOEXTRACTOR_H
