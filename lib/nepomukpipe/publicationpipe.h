/*
 * Copyright 2011 Jörg Ehrichs <joerg.ehrichs@gmx.de>
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

#ifndef PUBLICATIONPIPE_H
#define PUBLICATIONPIPE_H

#include "nepomukpipe.h"

#include <Nepomuk2/Resource>
#include <Nepomuk2/SimpleResource>
#include <Nepomuk2/SimpleResourceGraph>

#include "ontology/nbib/publication.h"
#include "ontology/nbib/reference.h"
#include "ontology/nbib/journal.h"
#include "ontology/nbib/journalissue.h"
#include "ontology/nbib/encyclopedia.h"

#include "nepomukmetadataextractor_export.h"

#include <QtCore/QUrl>
#include <QtCore/QList>
#include <QtCore/QMap>

class KJob;

namespace NepomukMetaDataExtractor {
namespace Pipe {

class PublicationPipePrivate;

/**
  * @brief Pipes a @c QVariantMap with @c publication @c information into @c Nempomuk
  *
  * The given @c QVariantMap represents publication data as retrieved from BibTeX files and as such represent the @c key/values in the same way.
  * In addition each entry has a type identifier <i>(article, book etc)</i> and citation id.
  *
  * Based on the given type identifier different Ontology classes are used and several keys have a different meaning depending on the used type identifier
  * This class keeps care of this and creates the nepomuk data correctly.
  *
  * Each publication entry is transformed into:
  * @li @c nbib:Reference - points to the publication with additional info like @c chapter, @c page, @c citekey etc
  * @li @c nbib:Publication - the main publication info like @c book, @c article, @c inproceedings etc
  * @li several other classes for the @c authors, @c chapters, @c collections and so on
  *
  * The whole class works in the following way:
  * @li @c importPublication() creates the Reference and Publication SimpleResource and does the cross linking
  * @li @c addPublicationSubTypes() adds the correct subtypes to the publication
  * @li @c handleSpecialCases() handles some special key/value combinations
  * @li @c addContent() handle all other key/value cases with some add*() special cases
  *
  * The @c pipeImport is the start point where each publication is created and later on all references are added to the main publication
  *
  * @see NBIB ontology
  *
  * @todo TODO: remove the graph from the function parameters and make class member from it?
  */
class NEPOMUKMETADATAEXTRACTOR_EXPORT PublicationPipe : public NepomukPipe
{
    Q_OBJECT

public:
    explicit PublicationPipe(QObject *parent = 0);
    virtual ~PublicationPipe();

    void pipeImport(const QVariantMap &bibEntry);

    /**
      * @brief if the @p projectThing is valid all imported data will be related via @c pimo:isRelated to the project
      *
      * Mainly added for Conquirere where sometimes new data is directly added to a specific project.
      */
    void setProjectPimoThing(Nepomuk2::Resource projectThing);

    /**
     * @brief Returns the imported @c nbib:Publication and @c nbib:Reference resource uris
     *
     * first publication
     * second reference
     */
    QPair<QUrl, QUrl> importedPublication() const;

protected:
    /**
     * @brief Create the nbib:Publication and @c nbib:Reference objects and connect them
     *
     * Also adds all the refercen details in here, as all further chages will operate on the nbib:Publication
     * @return QPair with first = publicationUr and second = referenceUri
     */
    QPair<QUrl, QUrl> importPublication( QVariantMap &metaData );

    /**
     * @brief adds some subpublication types (like @c article, book etc) to teh created nbib:Publication
     * @param publication the publication object
     * @param metaData the publication meta data
     */
    void addPublicationSubTypes(Nepomuk2::NBIB::Publication &publication, const QVariantMap &metaData);

    /**
     * @brief handles some special cases of key/value combination to filter out some situations where the combination is important
     *
     * @param metaData the complete publication meta data
     * @param graph the grapgh where al ldata is added to
     * @param publication the used publication
     * @param reference the used reference
     *
     * @see addPublisher
     * @see addJournal
     * @see addSpecialArticle
     */
    void handleSpecialCases(QVariantMap &metaData, Nepomuk2::SimpleResourceGraph &graph, Nepomuk2::NBIB::Publication &publication, Nepomuk2::NBIB::Reference &reference);


    /* Helping functions */
    /**
     * @brief Creates the publisher with the connected address
     *
     * @param publisherValue name of the publisher
     * @param addressValue the address details
     * @param publication the publication where the publsiher is added to
     * @param graph the graph where the data is added to
     */
    void addPublisher(const QString &publisherValue, const QString &addressValue, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph);

    /**
     * @brief Creates an Article in a collection which is added as Journal/Journal issue construct
     *
     * @param journal the journal name
     * @param volume the journal volume
     * @param number the journal isue number
     * @param publication the publication (an @c article here)
     * @param graph the graph where al lthe data is added to
     * @param seriesUrl the type of Series
     * @param issueUrl the type of the issue
     */
    void addJournal(const QString &journal, const QString &volume, const QString &number, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph,
                    QUrl seriesUrl = Nepomuk2::NBIB::Journal().uri(),
                    QUrl issueUrl = Nepomuk2::NBIB::JournalIssue().uri());

    /**
     * @brief Creates Article with collection thats not published in a Journal
     *
     * @param titleValue article title
     * @param article the publication (an @c article in this case)
     * @param graph the graph where all data is added to
     * @param collectionUrl the type of collection that will be created
     */
    void addSpecialArticle(const QString &titleValue, Nepomuk2::NBIB::Publication &article, Nepomuk2::SimpleResourceGraph &graph, QUrl collectionUrl = Nepomuk2::NBIB::Encyclopedia().uri());

    /**
     * @brief handles a huge amout of keys where no special processing or Class creation is necessary
     *
     * @param key the bibtex key
     * @param value the bibtey value
     * @param publication the publication where the data is added to
     * @param reference the reference where the data is added to
     * @param graph the graph where all data will be added to
     * @param originalEntryType the original bibtex publication type
     * @param citeKey the used citykey for the reference
     */
    void addContent(const QString &key, const QString &value, Nepomuk2::NBIB::Publication &publication,
                    Nepomuk2::NBIB::Reference &reference, Nepomuk2::SimpleResourceGraph &graph,
                    const QString & originalEntryType, const QString & citeKey);

    /**
     * @brief Creates @c pimo:Note objects from the note/anote etc bibtex fields.
     *
     * The content will be added as plain text and html text
     * @param content the note text
     * @param noteTitle note title
     * @param publication the publicatio nwhere the data is added to
     * @param graph the graph where the data will be added to
     */
    void addNote(const QString &content, const QString &noteTitle, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph);


    void addAuthor(const QString &content, Nepomuk2::NBIB::Publication &publication, Nepomuk2::NBIB::Reference &reference, Nepomuk2::SimpleResourceGraph &graph, const QString & originalEntryType);
    void addBooktitle(const QString &content, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph, const QString & originalEntryType);
    void addSeriesEditor(const QString &contentValue, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph);
    void addChapter(const QString &content, Nepomuk2::NBIB::Publication &publication, Nepomuk2::NBIB::Reference &reference, Nepomuk2::SimpleResourceGraph &graph);
    void addChapterName(const QString &content, Nepomuk2::NBIB::Publication &publication, Nepomuk2::NBIB::Reference &reference, Nepomuk2::SimpleResourceGraph &graph);
    void addIssn(const QString &content, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph);

    void addOrganization(const QString &content, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph);
    void addCode(const QString &content, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph);
    void addCodeNumber(const QString &content, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph);
    void addCodeVolume(const QString &content, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph);
    void addReporter(const QString &content, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph);
    void addReporterVolume(const QString &content, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph);
    void addEvent(const QString &content, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph);
    void addSeries(const QString &content, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph);
    void addTitle(const QString &content, Nepomuk2::NBIB::Publication &publication, Nepomuk2::NBIB::Reference &reference, Nepomuk2::SimpleResourceGraph &graph, const QString & originalEntryType);
    /**
      * @bug split webpages by ',' or ';' if necessary
      */
    void addWebsite(const QString &content, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph);
    void addFileUrl(const QString &content, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph);
    void addPublicationDate(const QString &fullDate, Nepomuk2::NBIB::Publication &publication);
    void addPublicationDate(const QString &year, const QString &month, const QString &day, Nepomuk2::NBIB::Publication &publication);

    void addTag(const QStringList &content, Nepomuk2::SimpleResource &resource, Nepomuk2::SimpleResourceGraph &graph);
    void addTopic(const QStringList &content, Nepomuk2::SimpleResource &resource, Nepomuk2::SimpleResourceGraph &graph);

    /**
      * @brief creates the contact resource
      */
    void addContact(const QString &contentValue, Nepomuk2::SimpleResource &resource, Nepomuk2::SimpleResourceGraph &graph, QUrl contactProperty );

private:
    Q_DECLARE_PRIVATE(PublicationPipe)
    PublicationPipePrivate *const d_ptr; /**< d-pointer for this class */
};
}
}

#endif // PUBLICATIONPIPE_H

