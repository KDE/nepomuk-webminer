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

#ifndef POPPLEREXTRACTOR_H
#define POPPLEREXTRACTOR_H

#include <QtCore/QObject>

#include <QtXml/QDomDocument>
#include <KDE/KUrl>

#include "publicationentry.h"

namespace Poppler {
    class Document;
}

/**
  * @brief Extractor for pdf documents via the poppler library
  *
  * Extracts the RDF content of the pdf file and the TOC.
  * Also parse the first page to get the title from it (as often the title is not correctly
  * inserted into the RDF part. It find the text with the biggest font size that has more
  * than 1 character, this is most likely the title.
  */
class PopplerExtractor : public QObject
{
    Q_OBJECT

public:
    explicit PopplerExtractor(QObject *parent = 0);

    /**
      * Creates a new publication entry and fills it.
      *
      * The caller gets the ownership of the returned object.
      * returns 0 if an error occured
      */
    void parseUrl(MetaDataParameters *mdp, const KUrl &fileUrl);

private:
    void tocCreation(const QDomDocument &toc, QDomNode &node);
    void parseFirstpage();

    Poppler::Document *m_pdfdoc;
    MetaDataParameters  *m_publicationEntry;
};

#endif // POPPLEREXTRACTOR_H