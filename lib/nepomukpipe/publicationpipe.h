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

#ifndef BIBTEXTONEPOMUKPIPE_H
#define BIBTEXTONEPOMUKPIPE_H

#include "nepomukpipe.h"

#include <Nepomuk2/Resource>
#include <Nepomuk2/SimpleResource>
#include <Nepomuk2/SimpleResourceGraph>

//#include "sro/nbib.h"
#include "sro/nbib/publication.h"
#include "sro/nbib/reference.h"
#include "sro/nbib/journal.h"
#include "sro/nbib/journalissue.h"
#include "sro/nbib/encyclopedia.h"

#include <Nepomuk2/Resource>
#include <Nepomuk2/Thing>

#include <QtCore/QUrl>
#include <QtCore/QList>
#include <QtCore/QMap>

class KJob;

namespace NepomukMetaDataExtractor {
namespace Pipe {
/**
  * @brief Pipes a QVariantMap with publication information into Nempomuk
  *
  * @todo TODO: needs a rewrite. This evolved from the "non-dms" appraoch. Could be simplified a lot now
  *
  * @see NBIB ontology
  */
class PublicationPipe : public NepomukPipe
{
    Q_OBJECT

public:
    explicit PublicationPipe(QObject *parent = 0);
    virtual ~PublicationPipe();

    /**
      * Does the piping action
      */
    void pipeImport(const QVariantMap &bibEntry);

private:
    /**
      * @return QPair with first = publicationUr and second = referenceUri
      */
    QPair<QUrl, QUrl> importPublication( QVariantMap &metaData );
    void addPublicationSubTypes(Nepomuk2::NBIB::Publication &publication, const QVariantMap &metaData);
    void handleSpecialCases(QVariantMap &metaData, Nepomuk2::SimpleResourceGraph &graph, Nepomuk2::NBIB::Publication &publication, Nepomuk2::NBIB::Reference &reference);


    /* Helping functions */
    void addPublisher(const QString &publisherValue, const QString &addressValue, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph);

    void addJournal(const QString &journal, const QString &volume, const QString &number, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph,
                    QUrl seriesUrl = Nepomuk2::NBIB::Journal().uri(),
                    QUrl issueUrl = Nepomuk2::NBIB::JournalIssue().uri());
    void addSpecialArticle(const QString &titleValue, Nepomuk2::NBIB::Publication &article, Nepomuk2::SimpleResourceGraph &graph, QUrl collectionUrl = Nepomuk2::NBIB::Encyclopedia().uri());

    void addContent(const QString &key, const QString &value, Nepomuk2::NBIB::Publication &publication,
                    Nepomuk2::NBIB::Reference &reference, Nepomuk2::SimpleResourceGraph &graph,
                    const QString & originalEntryType, const QString & citeKey);

    void addNote(const QString &content, const QString &noteType, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph);


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
      * creates the contact resource and push it to nepomuk if necessary
      */
    void addContact(const QString &contentValue, Nepomuk2::SimpleResource &resource, Nepomuk2::SimpleResourceGraph &graph, QUrl contactProperty, QUrl contactType );

};
}
}

#endif // BIBTEXTONEPOMUKPIPE_H

