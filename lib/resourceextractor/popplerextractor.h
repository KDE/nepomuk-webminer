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

#ifndef POPPLEREXTRACTOR_H
#define POPPLEREXTRACTOR_H

#include <QtCore/QObject>
#include <QtXml/QDomDocument>

#include <KDE/KUrl>

#include "nepomukmetadataextractor_export.h"

namespace Poppler
{
class Document;
}

namespace NepomukMetaDataExtractor
{
namespace Extractor
{

class MetaDataParameters;
class PopplerExtractorPrivate;

/**
  * @brief Extractor for pdf documents via the poppler library
  *
  * Extracts the RDF content of the pdf file and the TOC.
  * Also parse the first page to get the title from it (as often the title is not correctly
  * inserted into the RDF part. It find the text with the biggest font size that has more
  * than 1 character, this is most likely the title.
  */
class NEPOMUKMETADATAEXTRACTOR_EXPORT PopplerExtractor : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Standard constructor
     *
     * @param parent the paretn object
     */
    explicit PopplerExtractor(QObject *parent = 0);

    /**
     * @brief Clean up and delete poppler object
     */
    ~PopplerExtractor();

    /**
      * @brief Fills the MetaDataParameters with some more values from the RDF and first page
      *
     * @param mdp the MetaDataParameters where the parsed data will be saved to
     * @param fileUrl the url of the file on the harddrive
      */
    void parseUrl(MetaDataParameters *mdp, const KUrl &fileUrl);

private:
    /**
     * @brief creates the TOC from the given pdf data
     *
     * @param toc the doc xml document of the pdf file
     * @param node the current node
     */
    void tocCreation(const QDomDocument &toc, QDomNode &node);

    /**
     * @brief Exctract additional data from the pdf first page
     */
    void parseFirstpage();

    Q_DECLARE_PRIVATE(PopplerExtractor)
    PopplerExtractorPrivate *const d_ptr; /**< d-pointer for this class */
};
}
}

#endif // POPPLEREXTRACTOR_H
