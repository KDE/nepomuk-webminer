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

#include "publicationpipe.h"

#include "dms-copy/simpleresource.h"
#include "dms-copy/datamanagement.h"
#include "dms-copy/storeresourcesjob.h"
#include <KDE/KJob>

#include "sro/nbib/series.h"
#include "sro/nbib/collection.h"
#include "sro/nbib/documentpart.h"
#include "sro/nbib/chapter.h"
#include "sro/nbib/article.h"
#include "sro/nbib/proceedings.h"
#include "sro/nbib/codeoflaw.h"
#include "sro/nbib/legislation.h"
#include "sro/nbib/courtreporter.h"
#include "sro/nbib/legalcasedocument.h"

#include "sro/pimo/note.h"
#include "sro/pimo/topic.h"
#include "sro/pimo/event.h"
#include "sro/nie/informationelement.h"
#include "sro/nao/tag.h"
#include "sro/nfo/filedataobject.h"
#include "sro/nfo/paginatedtextdocument.h"
#include "sro/nfo/website.h"
#include "sro/nfo/document.h"
#include "sro/nco/postaladdress.h"
#include "sro/nco/personcontact.h"
#include "sro/nco/contact.h"
#include "sro/nco/organizationcontact.h"

#include <Soprano/Vocabulary/RDF>

#include <Nepomuk/Vocabulary/PIMO>
#include <Nepomuk/Vocabulary/NIE>
#include <Nepomuk/Vocabulary/NCO>
#include <Nepomuk/Vocabulary/NFO>
#include <Nepomuk/Vocabulary/NCAL>
#include <Nepomuk/Vocabulary/NUAO>
#include <Soprano/Vocabulary/NAO>
#include <Nepomuk/Variant>

#include <KDE/KDebug>

#include <QtCore/QSharedPointer>
#include <QtGui/QTextDocument>
#include <QtCore/QUuid>

using namespace Nepomuk::Vocabulary;
using namespace Soprano::Vocabulary;

 NepomukMetaDataExtractor::Pipe::PublicationPipe::PublicationPipe(QObject *parent)
    : NepomukPipe(parent)
{
}

 NepomukMetaDataExtractor::Pipe::PublicationPipe::~PublicationPipe()
{

}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::pipeImport(const QVariantMap &bibEntry)
{
    QVariantMap bibEntryNonConst(bibEntry);
    // The MetaDataParameters contain the metadata for the publication as bibEntry->metaData
    // also if it is related to a file the bibEntry->resourceUri points to it.
    // if we have some information about the references used inthe publication the
    // bibEntry->metaData.value("references") has a list of more publications to add

    // here we add all this stuff step by step.

    // 1. split the reference details so the next function does not complain that it does not know about
    // this key
    QVariantList references = bibEntryNonConst.value(QLatin1String("references")).toList();
    bibEntryNonConst.remove( QLatin1String("references") );

    // 2. create the main publication resource
    QPair<QUrl,QUrl> mainPublicationUris = importPublication( bibEntryNonConst );

    // 3. if we have some references, we create them now
    foreach(const QVariant &reference, references) {
        kDebug() << "import reference";
        QVariantMap referenceMetaData = reference.toMap();

        QPair<QUrl,QUrl> referenceUris = importPublication( referenceMetaData );

        // and connect it to the mainPublication
        QList<QUrl> resUri; resUri << mainPublicationUris.first;
        QVariantList value; value << referenceUris.second;
        Nepomuk::addProperty(resUri, NBIB::citedReference(), value);
    }

    // 4. if the main resource has a file attachment, we add the publishedAs crossrefs to it
    QString file = bibEntryNonConst.value(QLatin1String("resourceuri")).toString();
    KUrl fileurl(file);
    if( fileurl.isLocalFile()) {
        kDebug() << "add locafile crosref";
        // first we create a nepomu kresource from the file
        // if the resoruce existed already (due to fileanalyzer added it before), we get the existing resource back
        // if the analyzer failed a new resource is created
        Nepomuk::SimpleResourceGraph graph;
        Nepomuk::NFO::PaginatedTextDocument localFile(fileurl);

        localFile.setTitle( bibEntryNonConst.value(QLatin1String("title")).toString() );

        localFile.addType( NFO::FileDataObject() );
        localFile.setProperty( NFO::fileName(), fileurl.fileName() );

        graph << localFile;

        Nepomuk::StoreResourcesJob *srj = Nepomuk::storeResources(graph,Nepomuk::IdentifyNew, Nepomuk::OverwriteProperties);
        srj->exec();

        QUrl fileResourceUri = srj->mappings().value( localFile.uri() );

        // now add the crossrefs
        QList<QUrl> resUri; resUri << fileResourceUri;
        QVariantList value; value << mainPublicationUris.first;
        Nepomuk::setProperty(resUri, NBIB::publishedAs(), value);

        resUri.clear(); resUri << mainPublicationUris.first;
        value.clear(); value << fileResourceUri;
        Nepomuk::addProperty(resUri, NBIB::isPublicationOf(), value);
    }
}

QPair<QUrl, QUrl>  NepomukMetaDataExtractor::Pipe::PublicationPipe::importPublication( QVariantMap &metaData )
{
    QString originalEntryType = metaData.value(QLatin1String("bibtexentrytype")).toString().toLower();

    QString citeKey = metaData.value(QLatin1String("bibtexcitekey")).toString();
    if(citeKey.isEmpty()) {
        // if we have no citekey defined, take the first 100 chars (without whitespace) and use this one
        QString title = metaData.value(QLatin1String("title"), QLatin1String("unknwon")).toString();
        title.remove(' ');
        citeKey = title.left(10);

        // also add the year to it if available
        QString year = metaData.value(QLatin1String("year"), QLatin1String("")).toString();
        citeKey.append(year);
    }

    Nepomuk::SimpleResourceGraph graph;
    Nepomuk::NBIB::Publication publication;
    addPublicationSubTypes(publication, metaData);
    // we remove it, othewise addContent complains about an unknown key
    // as it is not used anymore, this is fine and reduce wrong debug output
    //metaData.remove( QLatin1String("bibtexentrytype") );

    Nepomuk::NBIB::Reference reference;
    reference.setCiteKey( citeKey );
    reference.addProperty( NAO::prefLabel(), citeKey);

    reference.setPublication( publication.uri() );
    publication.addReference( reference.uri() );

    publication.addProperty(NAO::hasSubResource(), reference.uri() ); // remove reference when publication is deleted

    //before we go through the whole list one by one, we take care of some special cases
    handleSpecialCases(metaData, graph, publication, reference);

    //now go through the list of all remaining entries
    QMapIterator<QString, QVariant> i(metaData);
    while (i.hasNext()) {
        i.next();
        if(!i.value().toString().isEmpty())
            addContent(i.key().toLower(), i.value().toString(), publication, reference, graph, originalEntryType, citeKey);
    }


    graph << publication << reference;

    Nepomuk::StoreResourcesJob *srj = Nepomuk::storeResources(graph,Nepomuk::IdentifyNew, Nepomuk::OverwriteProperties);
    connect(srj, SIGNAL(result(KJob*)), this, SLOT(slotSaveToNepomukDone(KJob*)));
    srj->exec();

    // get the nepomu kuri of the newly created resource
    QUrl publicationUri( srj->mappings().value( publication.uri() ));
    QUrl referenceUri( srj->mappings().value( reference.uri() ));

    QPair<QUrl,QUrl> uris;
    uris.first = publicationUri;
    uris.second = referenceUri;

    return uris;
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::slotSaveToNepomukDone(KJob *job)
{
    if(job->error()) {
        kDebug() << "Failed to store information in Nepomuk. " << job->errorString();
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addPublicationSubTypes(Nepomuk::NBIB::Publication &publication, const QVariantMap &metaData)
{
    QString entryType = metaData.value(QLatin1String("bibtexentrytype")).toString().toLower();

    //################################################################
    //# Article and subtypes
    //################################################################
    if(entryType == QLatin1String("article") || entryType == QLatin1String("inproceedings") ||
       entryType == QLatin1String("conference") || entryType == QLatin1String("encyclopediaarticle")) {
        publication.addType(NBIB::Article());
    }
    else if(entryType == QLatin1String("blogpost")) {
        publication.addType(NBIB::Article());
        publication.addType(NBIB::BlogPost());
    }
    else if(entryType == QLatin1String("forumpost")) {
        publication.addType(NBIB::Article());
        publication.addType(NBIB::ForumPost());
    }
    else if(entryType == QLatin1String("webpage")) {
        publication.addType(NBIB::Article());
        publication.addType(NBIB::Webpage());
    }

    //################################################################
    //# Book and subtypes
    //################################################################
    else if(entryType == QLatin1String("book") || entryType == QLatin1String("inbook") || entryType == QLatin1String("incollection")) {
        publication.addType(NBIB::Book());
    }
    else if(entryType == QLatin1String("dictionaryentry") || entryType == QLatin1String("dictionary")) {
        publication.addType(NBIB::Book());
        publication.addType(NBIB::Dictionary());
    }
    else if(entryType == QLatin1String("booklet")) {
        publication.addType(NBIB::Booklet());
    }

    //################################################################
    //# Collection and subtypes
    //################################################################
    else if(entryType == QLatin1String("collection")) {
        publication.addType(NBIB::Collection());
    }
    else if(entryType == QLatin1String("proceedings")) {
        publication.addType(NBIB::Collection());
        publication.addType(NBIB::Proceedings());
    }
    else if(entryType == QLatin1String("forum")) {
        publication.addType(NBIB::Collection());
        publication.addType(NBIB::Forum());
    }
    else if(entryType == QLatin1String("blog")) {
        publication.addType(NBIB::Collection());
        publication.addType(NBIB::Blog());
    }
    else if(entryType == QLatin1String("website")) {
        publication.addType(NBIB::Collection());
        publication.addType(NBIB::Website());
    }

    //################################################################
    //# Thesis and subtypes
    //################################################################
    else if(entryType == QLatin1String("thesis")) {
        publication.addType(NBIB::Thesis());
    }
    else if(entryType == QLatin1String("bachelorthesis")) {
        publication.addType(NBIB::Thesis());
        publication.addType(NBIB::BachelorThesis());
    }
    else if(entryType == QLatin1String("mastersthesis")) {
        publication.addType(NBIB::Thesis());
        publication.addType(NBIB::MastersThesis());
    }
    else if(entryType == QLatin1String("phdthesis")) {
        publication.addType(NBIB::Thesis());
        publication.addType(NBIB::PhdThesis());
    }

    //################################################################
    //# Report and subtypes
    //################################################################
    else if(entryType == QLatin1String("report")) {
        publication.addType(NBIB::Report());
    }
    else if(entryType == QLatin1String("techreport")) {
        publication.addType(NBIB::Report());
        publication.addType(NBIB::Techreport());
    }

    //################################################################
    //# LegalDocument and subtypes
    //################################################################
    else if(entryType == QLatin1String("legaldocument")) {
        publication.addType(NBIB::LegalDocument());
    }
    else if(entryType == QLatin1String("legislation")) {
        publication.addType(NBIB::LegalDocument());
        publication.addType(NBIB::Legislation());
    }
    else if(entryType == QLatin1String("statute")) {
        publication.addType(NBIB::LegalDocument());
        publication.addType(NBIB::Legislation());
        publication.addType(NBIB::Statute());
    }
    else if(entryType == QLatin1String("bill")) {
        publication.addType(NBIB::LegalDocument());
        publication.addType(NBIB::Legislation());
        publication.addType(NBIB::Bill());
    }
    else if(entryType == QLatin1String("case")) {
        publication.addType(NBIB::LegalDocument());
        publication.addType(NBIB::LegalCaseDocument());
    }
    else if(entryType == QLatin1String("decision")) {
        publication.addType(NBIB::LegalDocument());
        publication.addType(NBIB::LegalCaseDocument());
        publication.addType(NBIB::Decision());
    }
    else if(entryType == QLatin1String("brief")) {
        publication.addType(NBIB::LegalDocument());
        publication.addType(NBIB::LegalCaseDocument());
        publication.addType(NBIB::Brief());
    }

    //################################################################
    //# All types without further subtypes
    //################################################################
    else if(entryType == QLatin1String("electronic")) {
        publication.addType(NBIB::Electronic());
    }
    else if(entryType == QLatin1String("manual")) {
        publication.addType(NBIB::Manual());
    }
    else if(entryType == QLatin1String("presentation")) {
        publication.addType(NBIB::Presentation());
    }
    else if(entryType == QLatin1String("script") || entryType == QLatin1String("manuscript")) {
        publication.addType(NBIB::Script());
    }
    else if(entryType == QLatin1String("unpublished")) {
        publication.addType(NBIB::Unpublished());
    }
    else if(entryType == QLatin1String("patent")) {
        publication.addType(NBIB::Patent());
    }
    else if(entryType == QLatin1String("standard")) {
        publication.addType(NBIB::Standard());
    }
    else if(entryType == QLatin1String("map")) {
        publication.addType(NBIB::Map());
    }
    else if(entryType == QLatin1String("misc")) {
        // do nothing, stays Publication only
    }

    // erro case
    else {
        kWarning() << "try to import unknown bibliographic resource type" << entryType;
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::handleSpecialCases(QVariantMap &metaData, Nepomuk::SimpleResourceGraph &graph, Nepomuk::NBIB::Publication &publication, Nepomuk::NBIB::Reference &reference)
{
    // I. publisher/school/institution + address
    //    means address belongs to publisher
    if(metaData.contains(QLatin1String("address"))) {
        QString publisher;
        if(metaData.contains(QLatin1String("publisher"))) {
            publisher = metaData.value(QLatin1String("publisher")).toString();
        }
        else if(metaData.contains(QLatin1String("school"))) {
            publisher = metaData.value(QLatin1String("school")).toString();
        }
        else if(metaData.contains(QLatin1String("institution"))) {
            publisher = metaData.value(QLatin1String("institution")).toString();
        }

        if(!publisher.isEmpty()) {
            addPublisher(publisher, metaData.value(QLatin1String("address")).toString(), publication, graph);
            metaData.remove(QLatin1String("address"));
            metaData.remove(QLatin1String("publisher"));
            metaData.remove(QLatin1String("school"));
            metaData.remove(QLatin1String("institution"));
        }
    }

    // II. encyclopediaarticle / blogpost / forumpost / webpage / other article type that must be in a collection
    //     as retrieved from zotero mostly
    if(metaData.contains(QLatin1String("articletype"))) {
        QUrl seriesURL;
        QUrl collectionURL;
        QString emptyCollectionName;

        QString type = metaData.value(QLatin1String("articletype")).toString();
        if(type == QLatin1String("encyclopedia")) {
            collectionURL = NBIB::Encyclopedia();
            emptyCollectionName = i18n("unknown encyclopedia");
        }
        else if(type == QLatin1String("blog")) {
            publication.addType(NBIB::BlogPost());
            collectionURL = NBIB::Blog();
            emptyCollectionName = i18n("unknown blog");
        }
        else if(type == QLatin1String("webpage")) {
            publication.addType(NBIB::Webpage());
            collectionURL = NBIB::Website();
            emptyCollectionName = i18n("unknown webpage");
        }
        else if(type == QLatin1String("forum")) {
            publication.addType(NBIB::ForumPost());
            collectionURL = NBIB::Forum();
            emptyCollectionName = i18n("unknown forum");
        }
        if(type == QLatin1String("magazine")) {
            seriesURL = NBIB::Magazin();
            collectionURL = NBIB::MagazinIssue();
            emptyCollectionName = i18n("unknown magazine");
        }
        else if(type == QLatin1String("newspaper")) {
            seriesURL = NBIB::Newspaper();
            collectionURL = NBIB::NewspaperIssue();
            emptyCollectionName = i18n("unknown newspaper");
        }
        else if(type == QLatin1String("journal")) {
            seriesURL = NBIB::Journal();
            collectionURL = NBIB::JournalIssue();
            emptyCollectionName = i18n("unknown journal");
        }

        // special series case, article in collection(Issue), in a series
        if(seriesURL.isValid()) {
            QString journalName = metaData.value(QLatin1String("journal")).toString();
            if(journalName.isEmpty()) {
                journalName = emptyCollectionName;
            }

            addJournal(journalName,
                       metaData.value(QLatin1String("volume")).toString(),
                       metaData.value(QLatin1String("number")).toString(),
                       publication, graph, seriesURL, collectionURL);

            metaData.remove(QLatin1String("journal"));
            metaData.remove(QLatin1String("number"));
            metaData.remove(QLatin1String("volume"));
            metaData.remove(QLatin1String("articletype"));
        }
        // other types like article in a blogpost, or encyclopedia
        else if(collectionURL.isValid()) {
            QString titleValue;
            if(metaData.contains(QLatin1String("booktitle"))) {
                titleValue = metaData.value(QLatin1String("booktitle")).toString();
            }
            else if(metaData.contains(QLatin1String("journal"))) {
                titleValue = metaData.value(QLatin1String("journal")).toString();
            }

            if(titleValue.isEmpty() ) {
                addSpecialArticle(emptyCollectionName,publication, graph, collectionURL);
            }
            else {
                addSpecialArticle(titleValue,publication, graph, collectionURL);
            }

            metaData.remove(QLatin1String("journal"));
            metaData.remove(QLatin1String("booktitle"));
            metaData.remove(QLatin1String("articletype"));
        }
    }

    // III. journal + number + volume + zotero articletype
    //      as defined in any usual bibtex file
    if(metaData.contains(QLatin1String("journal"))) {
        QUrl seriesURL;
        QUrl issueURL;

        QString type = metaData.value(QLatin1String("articletype")).toString();
        if(type == QLatin1String("magazine")) {
            seriesURL = NBIB::Magazin();
            issueURL = NBIB::MagazinIssue();
        }
        else if(type == QLatin1String("newspaper")) {
            seriesURL = NBIB::Newspaper();
            issueURL = NBIB::NewspaperIssue();
        }
        else {
            seriesURL = NBIB::Journal();
            issueURL = NBIB::JournalIssue();
        }

        addJournal(metaData.value(QLatin1String("journal")).toString(),
                   metaData.value(QLatin1String("volume")).toString(),
                   metaData.value(QLatin1String("number")).toString(),
                   publication, graph, seriesURL, issueURL);

        metaData.remove(QLatin1String("journal"));
        metaData.remove(QLatin1String("number"));
        metaData.remove(QLatin1String("volume"));
        metaData.remove(QLatin1String("articletype"));
    }

    // IV. archivePrefix + eprint
    //TODO implement archivePrefix stuff


    // V. publication date
    if(metaData.contains(QLatin1String("date"))) {
        addPublicationDate( metaData.value(QLatin1String("date")).toString(), publication );
        metaData.remove(QLatin1String("date"));
        metaData.remove(QLatin1String("year"));
        metaData.remove(QLatin1String("month"));
        metaData.remove(QLatin1String("day"));

    }
    else if(metaData.contains(QLatin1String("year")) || metaData.contains(QLatin1String("month"))) {
        addPublicationDate( metaData.value(QLatin1String("year")).toString(),
                            metaData.value(QLatin1String("month")).toString(),
                            metaData.value(QLatin1String("day")).toString(),
                            publication );

        metaData.remove(QLatin1String("date"));
        metaData.remove(QLatin1String("year"));
        metaData.remove(QLatin1String("month"));
        metaData.remove(QLatin1String("day"));
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addContent(const QString &key, const QString &value, Nepomuk::NBIB::Publication &publication,
                                     Nepomuk::NBIB::Reference &reference, Nepomuk::SimpleResourceGraph &graph,
                                     const QString & originalEntryType, const QString & citeKey)
{
    //############################################
    // Simple set commands

    if(key == QLatin1String("abstract")) {
        addValue(value, publication, NBIB::abstract() );
    }
    else if(key == QLatin1String("archive")) {
        addValue(value, publication, NBIB::archive());
    }
    else if(key == QLatin1String("history")) {
        addValue(value, publication, NBIB::history());
    }
    else if(key == QLatin1String("scale")) {
        addValue(value, publication, NBIB::mapScale());
    }
    else if(key == QLatin1String("archivelocation")) {
        addValue(value, publication, NBIB::archiveLocation());
    }
    else if(key == QLatin1String("librarycatalog")) {
        addValue(value, publication, NBIB::libraryCatalog());
    }
    else if(key == QLatin1String("copyright")) {
        addValue(value, publication, NIE::copyright());
    }
    else if(key == QLatin1String("doi")) {
        addValue(value, publication, NBIB::doi());
    }
    else if(key == QLatin1String("edition")) {
        addValue(value, publication, NBIB::edition());
    }
    else if(key == QLatin1String("eprint")) {
        addValue(value, publication, NBIB::eprint());
    }
    else if(key == QLatin1String("howpublished")) {
        addValue(value, publication, NBIB::publicationMethod());
    }
    else if(key == QLatin1String("isbn")) {
        addValue(value, publication, NBIB::isbn());
    }
    else if(key == QLatin1String("language")) {
        addValue(value, publication, NIE::language());
    }
    else if(key == QLatin1String("lccn")) {
        addValue(value, publication, NBIB::lccn());
    }
    else if(key == QLatin1String("mrnumber")) {
        addValue(value, publication, NBIB::mrNumber());
    }
    else if(key == QLatin1String("number")) {
        addValue(value, publication, NBIB::number());
    }
    else if(key == QLatin1String("pages")) {
        addValue(value, reference, NBIB::pages());
    }
    else if(key == QLatin1String("numpages") || key == QLatin1String("numberofpages")) {
        QString numpages = value;
        if(numpages.isEmpty()) {
            numpages = '0';
        }
        addValue(numpages, publication, NBIB::numberOfPages());
    }
    else if(key == QLatin1String("numberofvolumes")) {
        QString numvolumes = value;
        if(numvolumes.isEmpty()) {
            numvolumes = '0';
        }
        addValue(numvolumes, publication, NBIB::numberOfVolumes());
    }
    else if(key == QLatin1String("pubmed")) {
        addValue(value, publication, NBIB::pubMed());
    }
    else if(key == QLatin1String("shorttitle")) {
        addValue(value, publication, NBIB::shortTitle());
    }
    else if(key == QLatin1String("type")) {
        addValue(value, publication, NBIB::publicationType());
    }
    else if(key == QLatin1String("applicationnumber")) {
        addValue(value, publication, NBIB::applicationNumber());
    }
    else if(key == QLatin1String("prioritynumbers")) {
        addValue(value, publication, NBIB::priorityNumbers());
    }
    else if(key == QLatin1String("legalstatus")) {
        addValue(value, publication, NBIB::legalStatus());
    }
    else if(key == QLatin1String("references")) {
        addValue(value, publication, NBIB::patentReferences());
    }
    else if(key == QLatin1String("filingdate")) {
        QString accessdate = value;
        QDateTime dateTime = QDateTime::fromString(accessdate, Qt::ISODate);

        if(!dateTime.isValid()) { dateTime = QDateTime::fromString(accessdate, "yyyy-MM-dd"); }
        if(!dateTime.isValid()) { dateTime = QDateTime::fromString(accessdate, "dd-MM-yyy"); }
        if(!dateTime.isValid()) { dateTime = QDateTime::fromString(accessdate, "yyyy-MM"); }
        if(!dateTime.isValid()) { dateTime = QDateTime::fromString(accessdate, "MM-yyyy"); }
        if(!dateTime.isValid()) { dateTime = QDateTime::fromString(accessdate, "yyyy.MM.dd"); }
        if(!dateTime.isValid()) { dateTime = QDateTime::fromString(accessdate, "dd.MM.yyy"); }
        if(!dateTime.isValid()) { dateTime = QDateTime::fromString(accessdate, "MM.yyy"); }
        if(!dateTime.isValid()) { dateTime = QDateTime::fromString(accessdate, "yyyy.MM"); }
        if(!dateTime.isValid()) { dateTime = QDateTime::fromString(accessdate, "yyyy"); }
        if(!dateTime.isValid()) { dateTime = QDateTime::fromString(accessdate, "yy"); }

        if(dateTime.isValid()) {
            QString newDate = dateTime.toString(Qt::ISODate);

            addValue(newDate, publication, NBIB::filingDate());
        }
        else {
            kDebug() << "could not parse accessdate" << accessdate;
        }
    }
    else if(key == QLatin1String("volume")) {
        addValue(value, publication, NBIB::volume());
    }
    else if(key == QLatin1String("accessdate")) {
        QString accessdate = value;
        QDateTime dateTime = QDateTime::fromString(accessdate, Qt::ISODate);

        if(!dateTime.isValid()) { dateTime = QDateTime::fromString(accessdate, "yyyy-MM-dd"); }
        if(!dateTime.isValid()) { dateTime = QDateTime::fromString(accessdate, "dd-MM-yyy"); }
        if(!dateTime.isValid()) { dateTime = QDateTime::fromString(accessdate, "yyyy-MM"); }
        if(!dateTime.isValid()) { dateTime = QDateTime::fromString(accessdate, "MM-yyyy"); }
        if(!dateTime.isValid()) { dateTime = QDateTime::fromString(accessdate, "yyyy.MM.dd"); }
        if(!dateTime.isValid()) { dateTime = QDateTime::fromString(accessdate, "dd.MM.yyy"); }
        if(!dateTime.isValid()) { dateTime = QDateTime::fromString(accessdate, "MM.yyy"); }
        if(!dateTime.isValid()) { dateTime = QDateTime::fromString(accessdate, "yyyy.MM"); }
        if(!dateTime.isValid()) { dateTime = QDateTime::fromString(accessdate, "yyyy"); }
        if(!dateTime.isValid()) { dateTime = QDateTime::fromString(accessdate, "yy"); }

        if(dateTime.isValid()) {
            QString newDate = dateTime.toString(Qt::ISODate);

            addValue(newDate, publication, NUAO::lastUsage());
        }
        else {
            kDebug() << "could not parse accessdate" << accessdate;
        }
    }
    else if(key == QLatin1String("date")) {
        addValue(value, publication, NBIB::publicationDate());
    }

    //############################################
    // more advanced processing needed here

    else if(key == QLatin1String("author")) {
        addAuthor(value, publication, reference, graph, originalEntryType);
    }
    else if(key == QLatin1String("bookauthor")) {
        addContact(value, publication, graph, NCO::creator(), NCO::PersonContact());
    }
    else if(key == QLatin1String("contributor")) {
        addContact(value, publication, graph, NBIB::contributor(), NCO::PersonContact());
    }
    else if(key == QLatin1String("translator")) {
        addContact(value, publication, graph, NBIB::translator(), NCO::PersonContact());
    }
    else if(key == QLatin1String("reviewedauthor")) {
        addContact(value, publication, graph, NBIB::reviewedAuthor(), NCO::PersonContact());
    }
    else if(key == QLatin1String("attorneyagent")) {
        addContact(value, publication, graph, NBIB::attorneyAgent(), NCO::PersonContact());
    }
    else if(key == QLatin1String("counsel")) {
        addContact(value, publication, graph, NBIB::counsel(), NCO::PersonContact());
    }
    else if(key == QLatin1String("cosponsor")) {
        addContact(value, publication, graph, NBIB::coSponsor(), NCO::PersonContact());
    }
    else if(key == QLatin1String("commenter")) {
        addContact(value, publication, graph, NBIB::commenter(), NCO::PersonContact());
    }
    else if(key == QLatin1String("serieseditor")) {
        addSeriesEditor(value, publication, graph);
    }
    else if(key == QLatin1String("booktitle")) {
        addBooktitle(value, publication, graph, originalEntryType);
    }
    else if(key == QLatin1String("chapter")) {
        addChapter(value, publication, reference, graph);
    }
    else if(key == QLatin1String("chaptername")) {
        addChapterName(value, publication, reference, graph);
    }
    else if(key == QLatin1String("editor")) {
        addContact(value, publication, graph, NBIB::editor(), NCO::PersonContact());
    }
    else if(key == QLatin1String("institution")) {
        addPublisher(value, QString(), publication, graph);
    }
    else if(key == QLatin1String("issn")) {
        addIssn(value, publication, graph);
    }
    else if(key == QLatin1String("organization") ||
    key == QLatin1String("legislativebody") ) {
        addOrganization(value, publication, graph);
    }
    else if(key == QLatin1String("code")) {
        addCode(value, publication, graph);
    }
    else if(key == QLatin1String("codenumber")) {
        addCodeNumber(value, publication, graph);
    }
    else if(key == QLatin1String("codevolume")) {
        addCodeVolume(value, publication, graph);
    }
    else if(key == QLatin1String("reporter")) {
        addReporter(value, publication, graph);
    }
    else if(key == QLatin1String("reportervolume")) {
        addReporterVolume(value, publication, graph);
    }
    else if(key == QLatin1String("publisher")) {
        addPublisher(value, QString(), publication, graph);
    }
    else if(key == QLatin1String("school")) {
        addPublisher(value, QString(), publication, graph);
    }
    else if(key == QLatin1String("series")) {
        addSeries(value, publication, graph);
    }
    else if(key == QLatin1String("conferencename") ||
            key == QLatin1String("meetingname") ||
            key == QLatin1String("event")) {
        addEvent(value, publication, graph);
    }
    else if(key == QLatin1String("title")) {
        addTitle(value, publication, reference, graph, originalEntryType);
    }
    else if(key == QLatin1String("url")) {
        addWebsite(value, publication, graph);
    }
    else if(key == QLatin1String("localfile") ||
            key == QLatin1String("biburl") ||
            key == QLatin1String("bibsource") ||
            key == QLatin1String("ee")) {
        addFileUrl(value, publication, graph);
    }
    else if(key == QLatin1String("address")) {
        QString addressValue = value;
        if(addressValue.contains(QLatin1String("http:"))) {
            addWebsite(addressValue, publication, graph);
        }
        //ignore else case, as the address as a postal address is handled above when a publisher is available
    }
    else if(key == QLatin1String("keywords") || key == QLatin1String("keyword")) {
        QString keywordString = value;
        QStringList keywords;
        if(keywordString.contains(";")) {
            keywords = keywordString.split(';');
        }
        else {
            keywords = keywordString.split(',');
        }

        addTopic(keywords, publication, graph);
    }
    else if(key == QLatin1String("assignee")) {
        addContact(value, publication, graph, NBIB::assignee(), QUrl());
    }
    else if(key == QLatin1String("descriptor") ||
            key == QLatin1String("classification") ||
            key == QLatin1String("thesaurus") ||
            key == QLatin1String("subject")) {

        QString keywordString = value;
        QStringList keywords;
        if(keywordString.contains(";")) {
            keywords = keywordString.split(';');
        }
        else {
            keywords = keywordString.split(',');
        }

        addTopic(keywords, publication, graph);
    }
    else if(key.startsWith(QLatin1String("note")) || key.startsWith(QLatin1String("annote"))) {
        QString title = citeKey + ": " + key;
        addNote( value, title, publication, graph);
    }
    else {
        kDebug() << "unknown bibtex key ::" << key << value;
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addNote(const QString &contentVale, const QString &noteType, Nepomuk::NBIB::Publication &publication, Nepomuk::SimpleResourceGraph &graph)
{
    Nepomuk::PIMO::Note note;
    note.addType(NIE::InformationElement());

    note.setProperty( NAO::prefLabel(), noteType );
    note.setProperty( NIE::title(), noteType );

    QTextDocument content;
    content.setHtml( contentVale.simplified() );
    note.setProperty( NIE::plainTextContent(), content.toPlainText());
    note.setProperty( NIE::htmlContent(), content.toHtml());

    publication.addProperty(NAO::isRelated(), note);
    publication.addProperty(NAO::hasSubResource(), note);

    graph << note;
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addPublisher(const QString &publisherValue, const QString &addressValue, Nepomuk::NBIB::Publication &publication, Nepomuk::SimpleResourceGraph &graph)
{
    QString addressString = addressValue.toUtf8();

    //FIXME extendedAddress is not correct, but determining which part of the @p address is the street/location and so on is nearly impossible
    Nepomuk::NCO::PostalAddress postalAddress;
    if(!addressString.isEmpty()) {
        postalAddress.setExtendedAddress( addressString );
        graph << postalAddress;
    }

    QList<Name> personList = splitPersonList( publisherValue );
    foreach(const Name &publisher, personList) {

        Nepomuk::NCO::Contact publisherResource;

        publisherResource.setFullname( publisher.full );
        if(!addressString.isEmpty()) {
            publisherResource.addPostalAddress( postalAddress.uri() );
            publisherResource.addProperty(NAO::hasSubResource(), postalAddress.uri() );
        }

        graph << publisherResource;
        publication.addProperty(NCO::publisher(), publisherResource);
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addJournal(const QString &journalValue, const QString &volumeValue, const QString &numberValue,
                                     Nepomuk::NBIB::Publication &publication, Nepomuk::SimpleResourceGraph &graph, QUrl seriesUrl, QUrl issueUrl)
{
    QString journalString = journalValue.toUtf8();
    QString volumeString = volumeValue.toUtf8();
    QString numberString = numberValue.toUtf8();

    //find existing journal or create a new series of them
    Nepomuk::NBIB::Collection collection;
    collection.addType( issueUrl );
    Nepomuk::NBIB::Series series;
    series.addType( seriesUrl );

    // create the resources, the DMS will merge them later on together again
    series.setProperty( NIE::title(), journalString );

    collection.setNumber( numberString );
    collection.setProperty( NBIB::volume(), volumeString );

    // the collection name should be created by the series name + collection number/volume
    // we do this here so we get nicer results when we use genericLabel/prefLabel
    QString issueName = journalString + ' ' + volumeString;
    if(!numberString.isEmpty()) {
        issueName += ' ' + i18n("Vol. %1",numberString);
    }
    collection.setProperty( NIE::title(), issueName.trimmed() );

    // connect issue <-> journal
    collection.setInSeries( series.uri() );
    series.addSeriesOf( collection.uri() );

    // connect article <-> collection
    publication.setProperty(NBIB::collection(), collection );
    collection.addArticle( publication.uri() );
    collection.addProperty(NAO::hasSubResource(), publication.uri() ); // delete article when collection is removed

    graph << collection << series;
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addSpecialArticle(const QString &titleValue, Nepomuk::NBIB::Publication &article, Nepomuk::SimpleResourceGraph &graph, QUrl collectionUrl)
{
    QString collectionString = titleValue.toUtf8();

    Nepomuk::NBIB::Collection collection;
    collection.addType( collectionUrl );

    // create the resources, the DMS will merge them later on together again
    collection.setProperty( NIE::title(), collectionString );

    // connect article <-> collection
    article.setProperty(NBIB::collection(), collection.uri() );
    collection.addArticle( article.uri() );
    collection.addProperty(NAO::hasSubResource(), article.uri() ); // delete article when collection is removed

    graph << collection;
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addAuthor(const QString &contentValue, Nepomuk::NBIB::Publication &publication, Nepomuk::NBIB::Reference &reference, Nepomuk::SimpleResourceGraph &graph, const QString & originalEntryType)
{
    //in case of @incollection the author is used to identify who wrote the chapter not the complete book/collection
    if(originalEntryType == QLatin1String("incollection") ) {

        QUrl chapterUrl = reference.referencedPart();

        // if no chapter resources existed up to now, create one and use it
        if(!chapterUrl.isValid()) {
            Nepomuk::NBIB::Chapter chapterResource;
            chapterResource.addType(NIE::DataObject());
            chapterResource.setProperty(NIE::title(), i18n("unknown chapter") );
            chapterResource.setChapterNumber( 0 );
            // the chapter needs a "unique identifier, otherwise we merge them with other chapters together";
            chapterResource.setProperty(NAO::identifier(), QUuid::createUuid().toString());
            chapterResource.setProperty(NIE::url(), QUuid::createUuid().toString());

            // connect refrence <-> chapter <-> publication
            chapterResource.setDocumentPartOf( publication.uri() );
            publication.addDocumentPart( chapterResource.uri() );
            publication.addProperty(NAO::hasSubResource(), chapterResource.uri() ); //delete chapter when publication is removed
            reference.setReferencedPart( chapterResource.uri() );

            graph << chapterResource;

            addContact(contentValue, graph[chapterResource.uri()], graph, NCO::creator(), NCO::PersonContact());
        }
        else {
            addContact(contentValue, graph[chapterUrl], graph, NCO::creator(), NCO::PersonContact());
        }
    }
    else {
        addContact(contentValue, publication, graph, NCO::creator(), NCO::PersonContact());
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addBooktitle(const QString &content, Nepomuk::NBIB::Publication &publication, Nepomuk::SimpleResourceGraph &graph, const QString & originalEntryType)
{
    QString utfContent = content.toUtf8();

    //two specialities occur here
    // I. "booktitle" means the title of a book, where "title" than means the title of the article in the book where the author fits to
    // this is valid for any publication other than @InProceedings/@encyclopediaarticle
    //
    // II. "booktitle" marks the title of the @Proceedings/@Encyclopedia (Collection) from an @InProceedings/@encyclopediaarticle (Article)

    if(originalEntryType == QLatin1String("inproceedings")) {

        //create a new collection resource, the DMS will merge if necessary
        Nepomuk::NBIB::Collection collection;

        if(originalEntryType == QLatin1String("inproceedings")) {
            collection.addType( NBIB::Proceedings() );
        }
        else if(originalEntryType == QLatin1String("encyclopediaarticle")) {
            collection.addType( NBIB::Encyclopedia() );
        }

        collection.setProperty(NIE::title(), utfContent );

        graph << collection;

        // connect collection <-> article publication
        publication.setProperty(NBIB::collection(), collection.uri() );
        collection.addArticle( publication.uri() );
        collection.addProperty(NAO::hasSubResource(), publication.uri() ); // delete article when collection is removed
    }
    else {
        // put booktitle as the main title of the publication
        publication.setProperty(NIE::title(), utfContent);
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addSeriesEditor(const QString &contentValue, Nepomuk::NBIB::Publication &publication, Nepomuk::SimpleResourceGraph &graph)
{
    QUrl seriesUrl = publication.inSeries();

    if(!seriesUrl.isValid()) {
        Nepomuk::NBIB::Series newSeries;

        newSeries.addSeriesOf( publication.uri() );
        publication.setInSeries( newSeries.uri() );

        graph << newSeries;

        addContact(contentValue, newSeries, graph, NBIB::editor(), NCO::PersonContact());
    }
    else {
        addContact(contentValue, graph[seriesUrl], graph, NBIB::editor(), NCO::PersonContact());
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addChapter(const QString &content, Nepomuk::NBIB::Publication &publication, Nepomuk::NBIB::Reference &reference, Nepomuk::SimpleResourceGraph &graph)
{
    QString utfContent = content.toUtf8();

    // If we import some thing we assume no reference already existed and we have a new one
    // thus referencedPart() is not valid
    // if it is valid we assume this was already a nbib:Chapter rather than an generic nbib:DocumentPart

    QUrl chapterUrl = reference.referencedPart();

    if(!chapterUrl.isValid()) {
        Nepomuk::NBIB::Chapter chapterResource;
        chapterResource.addType(NIE::DataObject());
        chapterResource.setProperty( NIE::title(), i18n("unknown chapter") );
        chapterResource.setChapterNumber( utfContent );
        // the chapter needs a "unique identifier, otherwise we merge them with other chapters together";
        chapterResource.setProperty(NAO::identifier(), QUuid::createUuid().toString());
        chapterResource.setProperty(NIE::url(), QUuid::createUuid().toString());

        // connect refrence <-> chapter <-> publication
        chapterResource.setDocumentPartOf( publication.uri() );
        publication.addDocumentPart( chapterResource.uri() );
        publication.addProperty(NAO::hasSubResource(), chapterResource.uri() ); //delete chapter when publication is removed
        reference.setReferencedPart( chapterResource.uri() );

        graph << chapterResource << reference;
    }
    else {
        graph[chapterUrl].setProperty(NBIB::chapterNumber(), utfContent);
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addChapterName(const QString &content, Nepomuk::NBIB::Publication &publication, Nepomuk::NBIB::Reference &reference, Nepomuk::SimpleResourceGraph &graph)
{
    QString utfContent = content.toUtf8();

    QUrl chapterUrl = reference.referencedPart();

    // If we import some thing we assume no reference already existed and we have a new one
    // thus referencedPart() is not valid
    // if it is valid we assume this was already a nbib:Chapter rather than an generic nbib:DocumentPart

    if(!chapterUrl.isValid()) {
        Nepomuk::NBIB::Chapter chapterResource;
        chapterResource.addType(NIE::DataObject());
        chapterResource.setProperty( NIE::title(), utfContent );
        // the chapter needs a "unique identifier, otherwise we merge them with other chapters together";
        chapterResource.setProperty(NAO::identifier(), QUuid::createUuid().toString());
        chapterResource.setProperty(NIE::url(), QUuid::createUuid().toString());

        // connect refrence <-> chapter <-> publication
        chapterResource.setDocumentPartOf( publication.uri() );
        publication.addDocumentPart( chapterResource.uri() );
        publication.addProperty(NAO::hasSubResource(), chapterResource.uri() ); //delete chapter when publication is removed
        reference.setReferencedPart( chapterResource.uri() );

        graph << chapterResource;
    }
    else {
        graph[chapterUrl].setProperty(NIE::title(), utfContent);
        graph << graph[chapterUrl];
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addIssn(const QString &content, Nepomuk::NBIB::Publication &publication, Nepomuk::SimpleResourceGraph &graph)
{
    QString utfContent = content.toUtf8();

    QUrl seriesUrl;
    if(publication.contains(RDF::type(), NBIB::Article())) {
        if( publication.property(NBIB::collection()).isEmpty() ) {
            kWarning() << "we added an article without any collection and try to set the ISSN";
            Nepomuk::NBIB::Collection collection;
            collection.setProperty(NIE::title(), i18n("unknown collection"));

            Nepomuk::NBIB::Series series;
            series.setProperty(NIE::title(), i18n("unknown series"));

            // connect series <-> collection
            collection.setInSeries( series.uri() );
            series.addSeriesOf( collection.uri() );

            // connect article <-> collection
            publication.setProperty(NBIB::collection(), collection );
            collection.addArticle( publication.uri() );

            seriesUrl = series.uri();
            graph << collection << series;
        }
        else {
            QUrl collectionUrl = publication.property(NBIB::collection()).first().toUrl();

            if( !graph[collectionUrl].property(NBIB::inSeries()).isEmpty() ) {
                seriesUrl = graph[collectionUrl].property(NBIB::inSeries()).first().toUrl();
            }
        }
    }
    else {
        // for anything else than an article, we attach the ISSN to a series directly to the publication
        seriesUrl = publication.inSeries();
    }

    if(!seriesUrl.isValid()) {
        Nepomuk::NBIB::Series newSeries;
        newSeries.addSeriesOf( publication.uri() );
        publication.setInSeries( newSeries.uri() );
        newSeries.setIssn( utfContent );

        graph << newSeries;
    }
    else {
        graph[seriesUrl].setProperty(NBIB::issn(), utfContent);
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addOrganization(const QString &content, Nepomuk::NBIB::Publication &publication, Nepomuk::SimpleResourceGraph &graph)
{
    QString utfContent = content.toUtf8();

    Nepomuk::NCO::OrganizationContact organization;
    organization.setFullname( utfContent );

    if(publication.contains(RDF::type(), NBIB::Article())) {
        // special case if the publication is an article, we assume the organization was responsible for the full collection
        // organization of a proceedings/conference or respnsible for teh blog/forum etc

        QUrl collectionUrl;
        if( !publication.property(NBIB::collection()).isEmpty()) {
            collectionUrl = publication.property(NBIB::collection()).first().toUrl();
        }

        if(!collectionUrl.isValid()) {
            // create new proceedings resource, as non existed yet
            Nepomuk::NBIB::Proceedings newProceedings;
            newProceedings.addArticle( publication.uri() );
            publication.setProperty(NBIB::collection(), newProceedings.uri() );

            newProceedings.setOrganization( organization.uri() );

            graph << newProceedings;
        }
        else {
            // add organization to previous created proceedings
            graph[collectionUrl].setProperty(NBIB::organization(), organization );
        }
    }
    else {
        // publication seems to be proceedings already
        publication.setProperty(NBIB::organization(), organization );
    }

    graph << organization;
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addCode(const QString &content, Nepomuk::NBIB::Publication &publication, Nepomuk::SimpleResourceGraph &graph)
{
    Nepomuk::NBIB::CodeOfLaw codeOfLaw;
    codeOfLaw.setProperty(NIE::title(), QString(content.toUtf8()));

    publication.setProperty(NBIB::codeOfLaw(), codeOfLaw.uri()  );

    graph << codeOfLaw;
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addCodeNumber(const QString &content, Nepomuk::NBIB::Publication &publication, Nepomuk::SimpleResourceGraph &graph)
{
    QUrl codeOfLawUrl = Nepomuk::NBIB::Legislation(publication).codeOfLaw();

    if(!codeOfLawUrl.isValid()) {
        // create new code of law, seems none existed up to now
        Nepomuk::NBIB::CodeOfLaw codeOfLaw;
        codeOfLaw.setProperty(NIE::title(), QString(content.toUtf8()));

        publication.setProperty(NBIB::codeOfLaw(), codeOfLaw.uri()  );
        codeOfLaw.setProperty(NBIB::codeNumber(), QString(content.toUtf8()) );

        graph << codeOfLaw;
    }
    else {
        // add to existing code
        graph[codeOfLawUrl].setProperty(NBIB::codeNumber(), QString(content.toUtf8()) );
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addCodeVolume(const QString &content, Nepomuk::NBIB::Publication &publication, Nepomuk::SimpleResourceGraph &graph)
{
    QUrl codeOfLawUrl = Nepomuk::NBIB::Legislation(publication).codeOfLaw();

    if(!codeOfLawUrl.isValid()) {
        // create new code of law, seems none existed up to now
        Nepomuk::NBIB::CodeOfLaw codeOfLaw;
        codeOfLaw.setProperty(NIE::title(), QString(content.toUtf8()));

        publication.setProperty(NBIB::codeOfLaw(), codeOfLaw.uri()  );
        codeOfLaw.setProperty(NBIB::volume(), QString(content.toUtf8()) );

        graph << codeOfLaw;
    }
    else {
        // add to existing code
        graph[codeOfLawUrl].setProperty(NBIB::volume(), QString(content.toUtf8()) );
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addReporter(const QString &content, Nepomuk::NBIB::Publication &publication, Nepomuk::SimpleResourceGraph &graph)
{
    Nepomuk::NBIB::CourtReporter courtReporter;
    courtReporter.setProperty(NIE::title(), QString(content.toUtf8()));
    publication.setProperty(NBIB::courtReporter(), courtReporter.uri()  );

    graph << courtReporter;
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addReporterVolume(const QString &content, Nepomuk::NBIB::Publication &publication, Nepomuk::SimpleResourceGraph &graph)
{
    QUrl courtReporterUrl;
    if(!publication.property(NBIB::courtReporter()).isEmpty()) {
        courtReporterUrl = publication.property(NBIB::courtReporter()).first().toUrl();
    }

    if(!courtReporterUrl.isValid()) {
        // create new ccourt reporter, seems none existed up to now
        Nepomuk::NBIB::CourtReporter courtReporter;
        publication.setProperty(NBIB::courtReporter(), courtReporter.uri() );
        courtReporter.setProperty(NBIB::volume(), QString(content.toUtf8()) );

        graph << courtReporter;
    }
    else {
        // add to existing court reporter
        graph[courtReporterUrl].setProperty(NBIB::volume(), QString(content.toUtf8()) );
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addEvent(const QString &content, Nepomuk::NBIB::Publication &publication, Nepomuk::SimpleResourceGraph &graph)
{
    Nepomuk::PIMO::Event event;

    event.setProperty(NAO::prefLabel(), QString(content.toUtf8()));

    event.addProperty(NBIB::eventPublication(), publication);
    publication.addEvent( event.uri() );

    graph << event;
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addSeries(const QString &content, Nepomuk::NBIB::Publication &publication, Nepomuk::SimpleResourceGraph &graph)
{
    Nepomuk::NBIB::Series series;

    if( publication.contains(RDF::type(), NBIB::Book()) || publication.contains(RDF::type(), NBIB::Booklet()) ) {
        series.addType( NBIB::BookSeries() );
    }

    QString utfContent = content.toUtf8();
    series.setProperty( NIE::title(), utfContent );

    series.addSeriesOf( publication.uri() );
    publication.setInSeries( series.uri()  );

    graph << series;
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addTitle(const QString &content, Nepomuk::NBIB::Publication &publication, Nepomuk::NBIB::Reference &reference, Nepomuk::SimpleResourceGraph &graph, const QString & originalEntryType)
{
    QString utfContent = content.toUtf8();

    // in the case of @InCollection title means title of the article in the book
    // while booktitle is the actual title of the book
    if(originalEntryType == QLatin1String("incollection") || originalEntryType == QLatin1String("dictionaryentry")) {

        QUrl chapterUrl = reference.referencedPart();

        if(!chapterUrl.isValid()) {
            Nepomuk::NBIB::Chapter chapter;
            chapter.addType(NIE::DataObject());
            chapter.setProperty( NIE::title(), utfContent);
            // the chapter needs a "unique identifier, otherwise we merge them with other chapters together";
            chapter.setProperty(NAO::identifier(), QUuid::createUuid().toString());
            chapter.setProperty(NIE::url(), QUuid::createUuid().toString());

            // connect reference <-> chapter <-> document
            reference.setReferencedPart( chapter.uri() );
            publication.addDocumentPart( chapter.uri() );
            chapter.setDocumentPartOf( publication.uri() );
            publication.addProperty(NAO::hasSubResource(), chapter.uri() ); // delete chapter when publication is removed

            graph << chapter;
        }
        else {
            graph[chapterUrl].setProperty( NIE::title(), utfContent);
        }
    }
    else {
        publication.setProperty(NIE::title(), utfContent);
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addWebsite(const QString &content, Nepomuk::NBIB::Publication &publication, Nepomuk::SimpleResourceGraph &graph)
{
    //TODO differentiate between webpage and webseite
    // TODO split webpages if necessary
    KUrl url( QString(content.toUtf8()) );

    QString protocol = url.scheme();

    if(protocol.isEmpty()) {
        kDebug() << "tried to add invalid website without protocol. Add http:// for " << content;
        url.setScheme(QLatin1String("http"));
    }

    if( url.scheme().startsWith("http") && url.isValid()) {
        Nepomuk::NFO::Website website ( url );
        website.addType(NFO::WebDataObject());

        publication.addProperty(NIE::links(), website.uri() );

        graph << website;

    }
    else if( url.scheme().startsWith("file") || url.scheme().startsWith("/home")  || url.scheme().startsWith('~')) {
        addFileUrl(content, publication, graph);
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addFileUrl(const QString &content, Nepomuk::NBIB::Publication &publication, Nepomuk::SimpleResourceGraph &graph)
{
    KUrl url( QString(content.toUtf8()) );

    if( url.isLocalFile() ) {
        Nepomuk::NFO::Document localFile( url );
        localFile.addType(NFO::FileDataObject());

        publication.addProperty(NBIB::isPublicationOf(), localFile.uri() );
        localFile.addProperty(NBIB::publishedAs(), publication.uri() );

        graph << localFile;
    }
    else {
        QString protocol = url.scheme();

        if(protocol.isEmpty()) {
            kDebug() << "tried to add invalid remote ur without protocol. Add http:// for " << content;
            url.setScheme(QLatin1String("http"));
        }

        Nepomuk::NFO::Document remoteFile( url );
        remoteFile.addType(NFO::FileDataObject());
        remoteFile.addType(NFO::RemoteDataObject());

        publication.addProperty(NBIB::isPublicationOf(), remoteFile.uri() );
        remoteFile.addProperty(NBIB::publishedAs(), publication.uri() );

        graph << remoteFile;
    }
}
void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addPublicationDate(const QString &fullDate, Nepomuk::NBIB::Publication &publication)
{
    // try to find out what format was used to specify the full date
    QDateTime dateTime = QDateTime::fromString(fullDate, Qt::ISODate);

    if(!dateTime.isValid()) { dateTime = QDateTime::fromString(fullDate, "yyyy-MM-dd"); }
    if(!dateTime.isValid()) { dateTime = QDateTime::fromString(fullDate, "dd-MM-yyy"); }
    if(!dateTime.isValid()) { dateTime = QDateTime::fromString(fullDate, "yyyy-MM"); }
    if(!dateTime.isValid()) { dateTime = QDateTime::fromString(fullDate, "MM-yyyy"); }
    if(!dateTime.isValid()) { dateTime = QDateTime::fromString(fullDate, "yyyy.MM.dd"); }
    if(!dateTime.isValid()) { dateTime = QDateTime::fromString(fullDate, "dd.MM.yyy"); }
    if(!dateTime.isValid()) { dateTime = QDateTime::fromString(fullDate, "MM.yyy"); }
    if(!dateTime.isValid()) { dateTime = QDateTime::fromString(fullDate, "yyyy.MM"); }
    if(!dateTime.isValid()) { dateTime = QDateTime::fromString(fullDate, "yyyy"); }
    if(!dateTime.isValid()) { dateTime = QDateTime::fromString(fullDate, "yy"); }

    if(dateTime.isValid()) {
        QString newDate = dateTime.toString(Qt::ISODate);

        publication.setProperty( NBIB::publicationDate(), newDate);
    }
    else {
        kDebug() << "could not parse publication date " << fullDate;
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addPublicationDate(const QString &year, const QString &month, const QString &day, Nepomuk::NBIB::Publication &publication)
{
    QString finalYear = year;
    QString finalMonth = month;
    QString finalDay = day;

    if(finalYear.size() != 4) {
        finalYear.prepend(QLatin1String("20")); // transforms '11 into 2011
    }

    if(finalMonth.isEmpty()) {
        finalMonth = QLatin1String("01");
    }
    else {
        bool monthIsInt;
        finalMonth.toInt(&monthIsInt);
        if(monthIsInt) {
            finalMonth = month;
        }
        else {
            QString contentMonth = finalMonth.toLower().toUtf8();
            //transform bibtex month to numbers
            if(contentMonth.contains(QLatin1String("jan"))) {
                contentMonth = QString::number(1);
            }
            else if(contentMonth.contains(QLatin1String("feb"))) {
                contentMonth = QString::number(2);
            }
            else if(contentMonth.contains(QLatin1String("mar"))) {
                contentMonth = QString::number(3);
            }
            else if(contentMonth.contains(QLatin1String("apr"))) {
                contentMonth = QString::number(4);
            }
            else if(contentMonth.contains(QLatin1String("may"))) {
                contentMonth = QString::number(5);
            }
            else if(contentMonth.contains(QLatin1String("jun"))) {
                contentMonth = QString::number(6);
            }
            else if(contentMonth.contains(QLatin1String("jul"))) {
                contentMonth = QString::number(7);
            }
            else if(contentMonth.contains(QLatin1String("aug"))) {
                contentMonth = QString::number(8);
            }
            else if(contentMonth.contains(QLatin1String("sep"))) {
                contentMonth = QString::number(9);
            }
            else if(contentMonth.contains(QLatin1String("oct"))) {
                contentMonth = QString::number(10);
            }
            else if(contentMonth.contains(QLatin1String("nov"))) {
                contentMonth = QString::number(11);
            }
            else if(contentMonth.contains(QLatin1String("dec"))) {
                contentMonth = QString::number(12);
            }
            else {
                contentMonth = QLatin1String("01");
            }

            finalMonth = contentMonth;
        }

    }

    if(finalMonth.size() != 2) {
        finalMonth = finalMonth.prepend(QLatin1String("0")); // transforms '1 into 01
    }

    if(day.isEmpty()) {
        finalDay = QLatin1String("01");
    }
    else if(day.size() == 1) {
        finalDay.prepend(QLatin1String("0")); // transforms '1 into 01
    }

    QString newDate = finalYear + QLatin1String("-") + finalMonth + QLatin1String("-") + finalDay + QLatin1String("T00:00:00");

    publication.setProperty( NBIB::publicationDate(), newDate);
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addTag(const QStringList &content, Nepomuk::SimpleResource &resource, Nepomuk::SimpleResourceGraph &graph)
{
    foreach(const QString &tagText, content) {
        //TODO does this need a change? works differently than pimo:Topic creation, still works though...
        // there exist 2 apis for the ontology class, one that uses pointers the other that uses references ...
        Nepomuk::SimpleResource tagResource;
        Nepomuk::NAO::Tag tag (&tagResource);

        tag.addPrefLabel( tagText );
        tagResource.addProperty( NAO::identifier(), KUrl::fromEncoded(tagText.toUtf8()));

        graph << tagResource;
        resource.addProperty( NAO::hasTag(), tagResource.uri());
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addTopic(const QStringList &content, Nepomuk::SimpleResource &resource, Nepomuk::SimpleResourceGraph &graph)
{
    foreach(const QString &topicText, content) {
        Nepomuk::PIMO::Topic topic;

        topic.setTagLabel(topicText);
        topic.addProperty( NAO::identifier(), KUrl::fromEncoded(topicText.toUtf8()) );

        graph << topic;
        resource.addProperty( NAO::hasTopic(), topic.uri());
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addContact(const QString &contentValue, Nepomuk::SimpleResource &resource, Nepomuk::SimpleResourceGraph &graph, QUrl contactProperty, QUrl contactType )
{
    QList<Name> personList = splitPersonList( contentValue );
    foreach(const Name &author, personList) {

        // create new contact resource, duplicates will be merged by the DMS later on
        Nepomuk::NCO::PersonContact authorResource;
        authorResource.setFullname( author.full );
        authorResource.setNameGiven( author.first );
        authorResource.setNameFamily( author.last );
        QStringList suffixes;
        suffixes << author.suffix;
        authorResource.setNameHonorificSuffixs( suffixes );

        resource.addProperty(contactProperty, authorResource);
        graph << authorResource;
    }

}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addValue(const QString &content, Nepomuk::SimpleResource &resource, QUrl property)
{
    resource.setProperty(property, QString(content.toUtf8()));
}

QList<NepomukMetaDataExtractor::Pipe::PublicationPipe::Name>  NepomukMetaDataExtractor::Pipe::PublicationPipe::splitPersonList(const QString & persons)
{
    // split list of names into single Strings
    // first remove any  { } rom the string
    QString cleanedPersonString = persons;
    cleanedPersonString = cleanedPersonString.remove('{');
    cleanedPersonString = cleanedPersonString.remove('}');

    QStringList personStringList;

    // first Try bibtex like description: Hans Wurst and Jochen Junker
    personStringList = cleanedPersonString.split(QLatin1String(" and "));
    if(personStringList.size() == 1) {
        // jus tin case: Hans Wurst; Jochen Junker
        personStringList = cleanedPersonString.split(QLatin1String(";"));
    }

    // ok now we have 1 to several names, they can be specified in different versions
    // Hans Wurst or Wurst, hans or H. Wurst or Hans W. and so on.
    QList<PublicationPipe::Name> personList;

    foreach(const QString &person, personStringList) {
        personList.append( splitPersonString(person) );
    }

    return personList;
}

NepomukMetaDataExtractor::Pipe::PublicationPipe::Name  NepomukMetaDataExtractor::Pipe::PublicationPipe::splitPersonString(const QString & person)
{

    QStringList personTokens = person.split(' ');
    /**
     * Sequence of tokens may contain somewhere a comma, like
     * "Tuckwell," "Peter". In this case, fill two string lists:
     * one with tokens before the comma, one with tokens after the
     * comma (excluding the comma itself). Example:
     * partA = ( "Tuckwell" );  partB = ( "Peter" );  partC = ( "Jr." )
     * If a comma was found, boolean variable gotComma is set.
     */
    QStringList partA, partB, partC;
    int commaCount = 0;
    foreach(const QString &token, personTokens) {

        /// Position where comma was found, or -1 if no comma in token
        int p = -1;
        if (commaCount < 2) {
            /// Only check if token contains comma
            /// if no comma was found before
            int bracketCounter = 0;
            for (int i = 0; i < token.length(); ++i) {
                /// Consider opening curly brackets
                if (token[i] == QChar('{')) ++bracketCounter;
                /// Consider closing curly brackets
                else if (token[i] == QChar('}')) --bracketCounter;
                /// Only if outside any open curly bracket environments
                /// consider comma characters
                else if (bracketCounter == 0 && token[i] == QChar(',')) {
                    /// Memorize comma's position and break from loop
                    p = i;
                    break;
                } else if (bracketCounter < 0)
                    /// Should never happen: more closing brackets than opening ones
                    kWarning() << "Opening and closing brackets do not match!";
            }
        }

        if (p >= 0) {
            if (commaCount == 0) {
                if (p > 0) partA.append(token.left(p));
                if (p < token.length() - 1) partB.append(token.mid(p + 1));
            } else if (commaCount == 1) {
                if (p > 0) partB.append(token.left(p));
                if (p < token.length() - 1) partC.append(token.mid(p + 1));
            }
            ++commaCount;
        } else if (commaCount == 0)
            partA.append(token);
        else if (commaCount == 1)
            partB.append(token);
        else if (commaCount == 2)
            partC.append(token);
    }

    if (commaCount > 0) {
        Name newName;
        newName.first = partC.isEmpty() ? partB.join(QChar(' ')) : partC.join(QChar(' '));
        newName.last = partA.join(QChar(' '));
        newName.suffix = partC.isEmpty() ? QString::null : partB.join(QChar(' '));
        newName.full = newName.first + ' ' + newName.last + newName.suffix;
        newName.full = newName.full.trimmed();

        return newName;
    }

    /**
     * PubMed uses a special writing style for names, where the
     * last name is followed by single capital letters, each being
     * the first letter of each first name. Example: Tuckwell P H
     * So, check how many single capital letters are at the end of
     * the given token list
     */
    partA.clear(); partB.clear();
    bool singleCapitalLetters = true;
    QStringList::ConstIterator it = personTokens.constEnd();
    while (it != personTokens.constBegin()) {
        --it;
        if (singleCapitalLetters && it->length() == 1 && it->at(0).isUpper())
            partB.prepend(*it);
        else {
            singleCapitalLetters = false;
            partA.prepend(*it);
        }
    }
    if (!partB.isEmpty()) {
        /// Name was actually given in PubMed format
        Name newName;
        newName.first = partB.join(QChar(' '));
        newName.last = partA.join(QChar(' '));
        //newName.suffix;
        newName.full = newName.first + ' ' + newName.last;

        return newName;
    }

    /**
     * Normally, the last upper case token in a name is the last name
     * (last names consisting of multiple space-separated parts *have*
     * to be protected by {...}), but some languages have fill words
     * in lower caps beloning to the last name as well (example: "van").
     * Exception: Special keywords such as "Jr." can be appended to the
     * name, not counted as part of the last name
     */
    partA.clear(); partB.clear(); partC.clear();
    it = personTokens.constEnd();
    while (it != personTokens.constBegin()) {
        --it;
        if (partB.isEmpty() && (it->toLower().startsWith(QLatin1String("jr")) || it->toLower().startsWith(QLatin1String("sr")) || it->toLower().startsWith(QLatin1String("iii"))))
            /// handle name suffices like "Jr" or "III."
            partC.prepend(*it);
        else if (partB.isEmpty() || it->at(0).isLower())
            partB.prepend(*it);
        else
            partA.prepend(*it);
    }
    if (!partB.isEmpty()) {
        /// Name was actually like "Peter Ole van der Tuckwell",
        /// split into "Peter Ole" and "van der Tuckwell"
        Name newName;
        newName.first = partA.join(QChar(' '));
        newName.last = partB.join(QChar(' '));
        newName.suffix = partC.isEmpty() ? QString::null : partC.join(QChar(' '));
        newName.full = newName.first + ' ' + newName.last + newName.suffix;
        newName.full = newName.full.trimmed();

        return newName;
    }

    kWarning() << "Don't know how to handle name" << personTokens.join(QChar(' '));
    Name newName;
    return newName;
}
