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

#include "dms-copy/simpleresourcegraph.h"
#include "dms-copy/simpleresource.h"

#include "sro/nbib.h"
#include "sro/nbib/publication.h"
#include "sro/nbib/reference.h"

#include <Nepomuk/Resource>
#include <Nepomuk/Thing>

#include <QtCore/QUrl>
#include <QtCore/QList>
#include <QtCore/QMap>

class MetaDataParameters;
class KJob;

/**
  * @brief Pipes the content of a @c MetaDataParameters entry to the Nepomuk storage
  *
  * @see NBIB ontology
  */
class PubEntryToNepomukPipe : public NepomukPipe
{
    Q_OBJECT
public:
    explicit PubEntryToNepomukPipe(QObject *parent = 0);
    virtual ~PubEntryToNepomukPipe();

    /**
      * Does the piping action
      */
    void pipeImport(QList<MetaDataParameters*> & bibEntries);
    void pipeImport(MetaDataParameters* bibEntries);

private slots:
    void slotSaveToNepomukDone(KJob *job);

private:
    void addPublicationSubTypes(Nepomuk::NBIB::Publication &publication, const QVariantMap &metaData);
    void handleSpecialCases(QVariantMap &metaData, Nepomuk::SimpleResourceGraph &graph, Nepomuk::NBIB::Publication &publication, Nepomuk::NBIB::Reference &reference);


    /* Helping functions */
    void addPublisher(const QString &publisherValue, const QString &addressValue, Nepomuk::NBIB::Publication &publication, Nepomuk::SimpleResourceGraph &graph);

    void addJournal(const QString &journal, const QString &volume, const QString &number, Nepomuk::NBIB::Publication &publication, Nepomuk::SimpleResourceGraph &graph,
                    QUrl seriesUrl = Nepomuk::Vocabulary::NBIB::Journal(),
                    QUrl issueUrl = Nepomuk::Vocabulary::NBIB::JournalIssue());
    void addSpecialArticle(const QString &titleValue, Nepomuk::NBIB::Publication &article, Nepomuk::SimpleResourceGraph &graph, QUrl collectionUrl = Nepomuk::Vocabulary::NBIB::Encyclopedia());

    void addContent(const QString &key, const QString &value, Nepomuk::NBIB::Publication &publication,
                    Nepomuk::NBIB::Reference &reference, Nepomuk::SimpleResourceGraph &graph,
                    const QString & originalEntryType, const QString & citeKey);

    void addNote(const QString &content, const QString &noteType, Nepomuk::NBIB::Publication &publication, Nepomuk::SimpleResourceGraph &graph);


    void addAuthor(const QString &content, Nepomuk::NBIB::Publication &publication, Nepomuk::NBIB::Reference &reference, Nepomuk::SimpleResourceGraph &graph, const QString & originalEntryType);
    void addBooktitle(const QString &content, Nepomuk::NBIB::Publication &publication, Nepomuk::SimpleResourceGraph &graph, const QString & originalEntryType);
    void addSeriesEditor(const QString &contentValue, Nepomuk::NBIB::Publication &publication, Nepomuk::SimpleResourceGraph &graph);
    void addChapter(const QString &content, Nepomuk::NBIB::Publication &publication, Nepomuk::NBIB::Reference &reference, Nepomuk::SimpleResourceGraph &graph);
    void addChapterName(const QString &content, Nepomuk::NBIB::Publication &publication, Nepomuk::NBIB::Reference &reference, Nepomuk::SimpleResourceGraph &graph);
    void addIssn(const QString &content, Nepomuk::NBIB::Publication &publication, Nepomuk::SimpleResourceGraph &graph);

    void addOrganization(const QString &content, Nepomuk::NBIB::Publication &publication, Nepomuk::SimpleResourceGraph &graph);
    void addCode(const QString &content, Nepomuk::NBIB::Publication &publication, Nepomuk::SimpleResourceGraph &graph);
    void addCodeNumber(const QString &content, Nepomuk::NBIB::Publication &publication, Nepomuk::SimpleResourceGraph &graph);
    void addCodeVolume(const QString &content, Nepomuk::NBIB::Publication &publication, Nepomuk::SimpleResourceGraph &graph);
    void addReporter(const QString &content, Nepomuk::NBIB::Publication &publication, Nepomuk::SimpleResourceGraph &graph);
    void addReporterVolume(const QString &content, Nepomuk::NBIB::Publication &publication, Nepomuk::SimpleResourceGraph &graph);
    void addEvent(const QString &content, Nepomuk::NBIB::Publication &publication, Nepomuk::SimpleResourceGraph &graph);
    void addSeries(const QString &content, Nepomuk::NBIB::Publication &publication, Nepomuk::SimpleResourceGraph &graph);
    void addTitle(const QString &content, Nepomuk::NBIB::Publication &publication, Nepomuk::NBIB::Reference &reference, Nepomuk::SimpleResourceGraph &graph, const QString & originalEntryType);
    /**
      * @bug split webpages by ',' or ';' if necessary
      */
    void addWebsite(const QString &content, Nepomuk::NBIB::Publication &publication, Nepomuk::SimpleResourceGraph &graph);
    void addFileUrl(const QString &content, Nepomuk::NBIB::Publication &publication, Nepomuk::SimpleResourceGraph &graph);
    void addPublicationDate(const QString &fullDate, Nepomuk::NBIB::Publication &publication);
    void addPublicationDate(const QString &year, const QString &month, const QString &day, Nepomuk::NBIB::Publication &publication);

    void addTag(const QStringList &content, Nepomuk::SimpleResource &resource, Nepomuk::SimpleResourceGraph &graph);
    void addTopic(const QStringList &content, Nepomuk::SimpleResource &resource, Nepomuk::SimpleResourceGraph &graph);

    /**
      * creates the contact resource and push it to nepomuk if necessary
      */
    void addContact(const QString &contentValue, Nepomuk::SimpleResource &resource, Nepomuk::SimpleResourceGraph &graph, QUrl contactProperty, QUrl contactType );

    /**
      * simply sets the value
      */
    void addValue(const QString &content, Nepomuk::SimpleResource &resource, QUrl property);


    /**
      * Used to transform the KBibTeX Person ValueItem.
      *
      * This is necessary because the Person does not contain a full name and this way around it makes
      * it easier to handle cases where one of the entries are missing
      */
    struct Name {
        QString first;
        QString last;
        QString suffix;
        QString full;
    };

    QList<PubEntryToNepomukPipe::Name> splitPersonList(const QString & persons);

    // taken from KBibtex, so
    // @author Thomas Fischer <fischer@unix-ag.uni-kl.de> with some modifications
    PubEntryToNepomukPipe::Name splitPersonString(const QString & persons);
};

#endif // BIBTEXTONEPOMUKPIPE_H

