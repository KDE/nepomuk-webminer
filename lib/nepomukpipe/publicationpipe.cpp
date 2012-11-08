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

#include <Nepomuk2/SimpleResource>
#include <Nepomuk2/DataManagement>
#include <Nepomuk2/StoreResourcesJob>
#include <KDE/KJob>

#include "ontology/nbib/nbib.h"
#include "ontology/nbib/series.h"
#include "ontology/nbib/collection.h"
#include "ontology/nbib/documentpart.h"
#include "ontology/nbib/chapter.h"
#include "ontology/nbib/article.h"
#include "ontology/nbib/proceedings.h"
#include "ontology/nbib/codeoflaw.h"
#include "ontology/nbib/legislation.h"
#include "ontology/nbib/courtreporter.h"
#include "ontology/nbib/legalcasedocument.h"

#include "ontology/pimo/note.h"
#include "ontology/pimo/topic.h"
#include "ontology/ncal/event.h"
#include "ontology/nie/informationelement.h"
#include "ontology/nao/tag.h"
#include "ontology/nfo/filedataobject.h"
#include "ontology/nfo/paginatedtextdocument.h"
#include "ontology/nfo/website.h"
#include "ontology/nfo/webdataobject.h"
#include "ontology/nfo/document.h"
#include "ontology/nco/postaladdress.h"
#include "ontology/nco/personcontact.h"
#include "ontology/nco/contact.h"
#include "ontology/nco/organizationcontact.h"

#include <Soprano/Vocabulary/RDF>

#include <Nepomuk2/Vocabulary/PIMO>
#include <Nepomuk2/Vocabulary/NIE>
#include <Nepomuk2/Vocabulary/NCO>
#include <Nepomuk2/Vocabulary/NFO>
#include <Nepomuk2/Vocabulary/NCAL>
#include <Nepomuk2/Vocabulary/NUAO>
#include <Soprano/Vocabulary/NAO>
#include <Nepomuk2/Variant>
#include <Nepomuk2/File>

#include <KDE/KDebug>

#include <QtGui/QTextDocument>
#include <QtCore/QUuid>

using namespace Nepomuk2::Vocabulary;
using namespace Soprano::Vocabulary;

namespace NepomukMetaDataExtractor {
namespace Pipe {
class PublicationPipePrivate {
public:
    Nepomuk2::Resource projectThing;
    QPair<QUrl, QUrl> importedPublicationUris;
};
}
}



NepomukMetaDataExtractor::Pipe::PublicationPipe::PublicationPipe(QObject *parent)
    : NepomukPipe(parent)
    , d_ptr( new NepomukMetaDataExtractor::Pipe::PublicationPipePrivate )
{
}

NepomukMetaDataExtractor::Pipe::PublicationPipe::~PublicationPipe()
{

}

void NepomukMetaDataExtractor::Pipe::PublicationPipe::pipeImport(const QVariantMap &bibEntry)
{
    QVariantMap bibEntryNonConst(bibEntry);
    // The MetaDataParameters contain the metadata for the publication as bibEntry->metaData
    // also if it is related to a file the bibEntry->resourceUri points to it.
    // if we have some information about the references used in the publication the
    // bibEntry->metaData.value("bibreferences") has a list of more publications to add

    // here we add all this stuff step by step.

    // 1. split the reference details so the next function does not complain that it does not know about this key
    QVariantList references = bibEntryNonConst.value(QLatin1String("bibreferences")).toList();
    bibEntryNonConst.remove( QLatin1String("bibreferences") );

    // 2. create the main publication resource
    Q_D( PublicationPipe );
    d->importedPublicationUris = importPublication( bibEntryNonConst );

    // 3. if we have some references, we create them now
    foreach(const QVariant &reference, references) {
        kDebug() << "import reference";
        QVariantMap referenceMetaData = reference.toMap();

        QPair<QUrl,QUrl> referenceUris = importPublication( referenceMetaData );

        // and connect it to the mainPublication
        QList<QUrl> resUri; resUri << d->importedPublicationUris.first;
        QVariantList value; value << referenceUris.second;
        Nepomuk2::addProperty(resUri, NBIB::citedReference(), value);
    }

    // 4. if the main resource has a file attachment, we add the publishedAs crossrefs to it
    QString file = bibEntryNonConst.value(QLatin1String("resourceuri")).toString();

    KUrl fileurl;
    fileurl.setEncodedUrl(file.toLatin1());

    if( fileurl.isLocalFile()) {
        kDebug() << "add nbib:publicationOf to the file ::" << file;

        // get the resource for the local file uri
        Nepomuk2::File localFileRes(fileurl);

        // remove any metadata from this file created by the metadata extractor
        KJob *job = Nepomuk2::removeDataByApplication(QList<QUrl>() << fileurl, Nepomuk2::NoRemovalFlags, KComponentData(componentName().toLatin1()) );
        if (!job->exec() ) {
            kWarning() << job->errorString();
        }
        else {
            kDebug() << "Successfully removed old metadata from " << fileurl;
        }

        // now sometimes the fileanalyzer fails to index pdf files due to the streamanalyzer crashing when it tries to
        // read the pdf data. This leaves a Nepomuk Resource without the NFO::FileDataObject Type and thus won#t be listed
        // in conquirere as actual document.
        // Here we are going to fix it and add the necessary parts for it. Also we set the nie:title to the file
        // so it can show up in dolphin, while all the other information is hidden behind the nbib:publishedAs property

        // this works differently than all the other double typed resources (movie/tvshow/musicpiece etc)
        Nepomuk2::SimpleResourceGraph graph;
        Nepomuk2::NFO::PaginatedTextDocument localFile(localFileRes.uri());

        localFile.setTitle( bibEntryNonConst.value(QLatin1String("title")).toString() );

        localFile.addType( NFO::FileDataObject() );
        localFile.setProperty( NFO::fileName(), fileurl.fileName() );

        graph << localFile;

        Nepomuk2::StoreResourcesJob *srj = Nepomuk2::storeResources(graph,Nepomuk2::IdentifyNew, Nepomuk2::OverwriteProperties,
                                                                    QHash<QUrl,QVariant>(),KComponentData(componentName().toLatin1()));
        connect(srj, SIGNAL(result(KJob*)), this, SLOT(slotSaveToNepomukDone(KJob*)));
        srj->exec();

        // now get the resource uri again.
        QUrl fileResourceUri = srj->mappings().value( localFile.uri() );

        // and add the cross reference properties
        // so the actual publication resource is linked to the file resource
        QList<QUrl> resUri; resUri << fileResourceUri;
        QVariantList value; value << d->importedPublicationUris.first;
        Nepomuk2::setProperty(resUri, NBIB::publishedAs(), value);

        resUri.clear(); resUri << d->importedPublicationUris.first;
        value.clear(); value << fileResourceUri;
        Nepomuk2::addProperty(resUri, NBIB::isPublicationOf(), value);
    }
}

void NepomukMetaDataExtractor::Pipe::PublicationPipe::setProjectPimoThing(Nepomuk2::Resource projectThing)
{
    Q_D( PublicationPipe );
    d->projectThing = projectThing;
}

QPair<QUrl, QUrl> NepomukMetaDataExtractor::Pipe::PublicationPipe::importedPublication() const
{
    return d_ptr->importedPublicationUris;
}

QPair<QUrl, QUrl> NepomukMetaDataExtractor::Pipe::PublicationPipe::importPublication( QVariantMap &metaData )
{
    QString originalEntryType = metaData.value(QLatin1String("bibtexentrytype")).toString().toLower();

    QString citeKey = metaData.value(QLatin1String("bibtexcitekey")).toString();
    if(citeKey.isEmpty()) {
        //FIXME: Better automatic citykey detection
        // if we have no citekey defined, take the first 10 chars (without whitespace) and use this one
        QString title = metaData.value(QLatin1String("title"), QLatin1String("unknwon")).toString();
        title.remove(' ');
        citeKey = title.left(10);

        // also add the year to it if available
        QString year = metaData.value(QLatin1String("year"), QLatin1String("")).toString();
        citeKey.append(year);
    }

    Nepomuk2::SimpleResourceGraph graph;
    Nepomuk2::NBIB::Publication publication;
    addPublicationSubTypes(publication, metaData);
    // we remove the entry type key, othewise addContent() complains about an unknown key
    // as it is not used anymore, this is fine and reduce wrong debug output
    metaData.remove( QLatin1String("bibtexentrytype") );

    Nepomuk2::NBIB::Reference reference;
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

    Q_D( PublicationPipe );
    if(d->projectThing.isValid()) {
        publication.addProperty( NAO::isRelated(), d->projectThing.uri());
        reference.addProperty( NAO::isRelated(), d->projectThing.uri());
    }

    graph << publication << reference;

    Nepomuk2::StoreResourcesJob *srj = Nepomuk2::storeResources(graph,Nepomuk2::IdentifyNew, Nepomuk2::OverwriteProperties,
                                                                QHash<QUrl,QVariant>(),KComponentData(componentName().toLatin1()));
    connect(srj, SIGNAL(result(KJob*)), this, SLOT(slotSaveToNepomukDone(KJob*)));
    srj->exec();

    // get the nepomuk uri of the newly created resource
    QUrl publicationUri( srj->mappings().value( publication.uri() ));
    QUrl referenceUri( srj->mappings().value( reference.uri() ));

    QPair<QUrl,QUrl> uris;
    uris.first = publicationUri;
    uris.second = referenceUri;

    return uris;
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addPublicationSubTypes(Nepomuk2::NBIB::Publication &publication, const QVariantMap &metaData)
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

    // error case
    else {
        kWarning() << "try to import unknown bibliographic resource type" << entryType;
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::handleSpecialCases(QVariantMap &metaData, Nepomuk2::SimpleResourceGraph &graph, Nepomuk2::NBIB::Publication &publication, Nepomuk2::NBIB::Reference &reference)
{
    Q_UNUSED(reference);

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
    //TODO: implement archivePrefix stuff


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

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addContent(const QString &key, const QString &value, Nepomuk2::NBIB::Publication &publication,
                                                                  Nepomuk2::NBIB::Reference &reference, Nepomuk2::SimpleResourceGraph &graph,
                                                                  const QString & originalEntryType, const QString & citeKey)
{
    //############################################
    // Simple set commands

    if(key == QLatin1String("abstract")) {
        publication.setAbstract( value );
    }
    else if(key == QLatin1String("archive")) {
        publication.setArchive( value );
    }
    else if(key == QLatin1String("history")) {
        publication.setProperty( NBIB::history(), value );
    }
    else if(key == QLatin1String("scale")) {
        publication.setProperty( NBIB::mapScale(), value );
    }
    else if(key == QLatin1String("archivelocation")) {
        publication.setArchiveLocation( value );
    }
    else if(key == QLatin1String("librarycatalog")) {
        publication.setLibraryCatalog( value );
    }
    else if(key == QLatin1String("copyright")) {
        publication.addCopyright( value );
    }
    else if(key == QLatin1String("doi")) {
        publication.setDoi( value );
    }
    else if(key == QLatin1String("edition")) {
        publication.setEdition( value );
    }
    else if(key == QLatin1String("eprint")) {
        publication.setEprint( value );
    }
    else if(key == QLatin1String("howpublished")) {
        publication.setPublicationMethod( value );
    }
    else if(key == QLatin1String("isbn")) {
        publication.setIsbn( value );
    }
    else if(key == QLatin1String("language")) {
        publication.setLanguage( value );
    }
    else if(key == QLatin1String("lccn")) {
        publication.setLccn( value );
    }
    else if(key == QLatin1String("mrnumber")) {
        publication.setMrNumber( value );
    }
    else if(key == QLatin1String("number") || key == QLatin1String("issue")) {
        publication.setNumber( value );
    }
    else if(key == QLatin1String("pages")) {
        reference.setPages( value );
    }
    else if(key == QLatin1String("numpages") || key == QLatin1String("numberofpages")) {
        QString numpages = value;
        if(numpages.isEmpty()) {
            numpages = '0';
        }
        publication.setNumberOfPages( numpages.toInt() );
    }
    else if(key == QLatin1String("numberofvolumes")) {
        QString numvolumes = value;
        if(numvolumes.isEmpty()) {
            numvolumes = '0';
        }
        publication.setNumberOfVolumes( numvolumes.toInt() );
    }
    else if(key == QLatin1String("pubmed")) {
        publication.setPubMed( value );
    }
    else if(key == QLatin1String("shorttitle")) {
        publication.setShortTitle( value );
    }
    else if(key == QLatin1String("type")) {
        publication.setPublicationType( value );
    }
    else if(key == QLatin1String("applicationnumber")) {
        publication.setProperty( NBIB::applicationNumber(), value );
    }
    else if(key == QLatin1String("prioritynumbers")) {
        publication.setProperty( NBIB::priorityNumbers(), value );
    }
    else if(key == QLatin1String("legalstatus")) {
        publication.setProperty( NBIB::legalStatus(), value );
    }
    else if(key == QLatin1String("references")) {
        publication.setProperty( NBIB::patentReferences(), value );
    }
    else if(key == QLatin1String("filingdate")) {

        QDateTime filingDate = createDateTime( value );

        if(filingDate.isValid()) {
            QString dateString = filingDate.toString(Qt::ISODate);
            publication.setProperty( NBIB::filingDate(), dateString );
        }
    }
    else if(key == QLatin1String("volume")) {
            publication.setVolume( value );
    }
    else if(key == QLatin1String("accessdate")) {
        QDateTime accessDate = createDateTime( value );

        if(accessDate.isValid()) {
            QString dateString = accessDate.toString(Qt::ISODate);
            publication.setProperty( NUAO::lastUsage(), dateString );
        }
    }
    else if(key == QLatin1String("date")) {
        publication.setPublicationDate( createDateTime( value ) );
    }

    //############################################
    // more advanced processing needed here

    else if(key == QLatin1String("author")) {
        addAuthor(value, publication, reference, graph, originalEntryType);
    }
    else if(key == QLatin1String("bookauthor")) {
        addContact(value, publication, graph, NCO::creator());
    }
    else if(key == QLatin1String("contributor")) {
        addContact(value, publication, graph, NBIB::contributor());
    }
    else if(key == QLatin1String("translator")) {
        addContact(value, publication, graph, NBIB::translator());
    }
    else if(key == QLatin1String("reviewedauthor")) {
        addContact(value, publication, graph, NBIB::reviewedAuthor());
    }
    else if(key == QLatin1String("attorneyagent")) {
        addContact(value, publication, graph, NBIB::attorneyAgent());
    }
    else if(key == QLatin1String("counsel")) {
        addContact(value, publication, graph, NBIB::counsel());
    }
    else if(key == QLatin1String("cosponsor")) {
        addContact(value, publication, graph, NBIB::coSponsor());
    }
    else if(key == QLatin1String("commenter")) {
        addContact(value, publication, graph, NBIB::commenter());
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
        addContact(value, publication, graph, NBIB::editor());
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
        addContact(value, publication, graph, NBIB::assignee());
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
    else if(key == QLatin1String("bibtexentrytype") || key == QLatin1String("resourceuri") || key == QLatin1String("bibtexcitekey") ) {
        //ignore these cases
        return;
    }
    else {
        kDebug() << "unknown bibtex key ::" << key << value;
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addNote(const QString &contentVale, const QString &noteTitle, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph)
{
    Nepomuk2::PIMO::Note note;
    note.addType(NIE::InformationElement());

    note.setProperty( NAO::prefLabel(), noteTitle );
    note.setProperty( NIE::title(), noteTitle );

    QTextDocument content;
    content.setHtml( contentVale.simplified() );
    note.setProperty( NIE::plainTextContent(), content.toPlainText());
    note.setProperty( NIE::htmlContent(), content.toHtml());

    publication.addProperty(NAO::isRelated(), note);
    publication.addProperty(NAO::hasSubResource(), note);

    graph << note;
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addPublisher(const QString &publisherValue, const QString &addressString, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph)
{
    //FIXME: extendedAddress is not correct, but determining which part of the @p address is the street/location and so on is nearly impossible
    Nepomuk2::NCO::PostalAddress postalAddress;
    if(!addressString.isEmpty()) {
        postalAddress.setExtendedAddress( addressString );
        graph << postalAddress;
    }

    QList<Nepomuk2::NCO::OrganizationContact> orgList = createOrganizationContacts( publisherValue );

    foreach(Nepomuk2::NCO::OrganizationContact org, orgList) {

        if(!addressString.isEmpty()) {
            org.addPostalAddress( postalAddress.uri() );
            org.addProperty(NAO::hasSubResource(), postalAddress.uri() );
        }

        publication.addProperty(NCO::publisher(), org);

        graph << org;
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addJournal(const QString &journalString, const QString &volumeString, const QString &numberString,
                                                                  Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph, QUrl seriesUrl, QUrl issueUrl)
{
    //find existing journal or create a new series of them
    Nepomuk2::NBIB::Collection collection;
    collection.addType( issueUrl );
    Nepomuk2::NBIB::Series series;
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

    Q_D( PublicationPipe );
    if(d->projectThing.isValid()) {
        collection.addProperty( NAO::isRelated() , d->projectThing.uri());
        series.addProperty( NAO::isRelated() , d->projectThing.uri());
    }

    graph << collection << series;
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addSpecialArticle(const QString &collectionString, Nepomuk2::NBIB::Publication &article, Nepomuk2::SimpleResourceGraph &graph, QUrl collectionUrl)
{
    Nepomuk2::NBIB::Collection collection;
    collection.addType( collectionUrl );

    // create the resources, the DMS will merge them later on together again
    collection.setProperty( NIE::title(), collectionString );

    // connect article <-> collection
    article.setProperty(NBIB::collection(), collection.uri() );
    collection.addArticle( article.uri() );
    collection.addProperty(NAO::hasSubResource(), article.uri() ); // delete article when collection is removed

    Q_D( PublicationPipe );
    if(d->projectThing.isValid()) {
        collection.addProperty( NAO::isRelated() , d->projectThing.uri());
    }

    graph << collection;
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addAuthor(const QString &contentValue, Nepomuk2::NBIB::Publication &publication, Nepomuk2::NBIB::Reference &reference, Nepomuk2::SimpleResourceGraph &graph, const QString & originalEntryType)
{
    //in case of @incollection the author is used to identify who wrote the chapter not the complete book/collection
    if(originalEntryType == QLatin1String("incollection") ) {

        QUrl chapterUrl = reference.referencedPart();

        // if no chapter resources existed up to now, create one and use it
        if(!chapterUrl.isValid()) {
            Nepomuk2::NBIB::Chapter chapterResource;
            chapterResource.setTitle( i18n("unknown chapter") );
            chapterResource.setChapterNumber( 0 );
            //BUG: the chapter needs a "unique identifier, otherwise we merge them with other chapters together";
            chapterResource.addType(NIE::DataObject());
            chapterResource.setProperty(NAO::identifier(), QUuid::createUuid().toString());
            chapterResource.setProperty(NIE::url(), QUuid::createUuid().toString());

            // connect refrence <-> chapter <-> publication
            chapterResource.setDocumentPartOf( publication.uri() );
            publication.addDocumentPart( chapterResource.uri() );
            publication.addProperty(NAO::hasSubResource(), chapterResource.uri() ); //delete chapter when publication is removed
            reference.setReferencedPart( chapterResource.uri() );

            graph << chapterResource;

            addContact(contentValue, graph[chapterResource.uri()], graph, NCO::creator());
        }
        else {
            addContact(contentValue, graph[chapterUrl], graph, NCO::creator());
        }
    }
    else {
        addContact(contentValue, publication, graph, NCO::creator());
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addBooktitle(const QString &content, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph, const QString & originalEntryType)
{
    //two specialities occur here
    // I. "booktitle" means the title of a book, where "title" than means the title of the article in the book where the author fits to
    // this is valid for any publication other than @InProceedings/@encyclopediaarticle
    //
    // II. "booktitle" marks the title of the @Proceedings/@Encyclopedia (Collection) from an @InProceedings/@encyclopediaarticle (Article)

    if(originalEntryType == QLatin1String("inproceedings")) {

        //create a new collection resource, the DMS will merge if necessary
        Nepomuk2::NBIB::Collection collection;

        if(originalEntryType == QLatin1String("inproceedings")) {
            collection.addType( NBIB::Proceedings() );
        }
        else if(originalEntryType == QLatin1String("encyclopediaarticle")) {
            collection.addType( NBIB::Encyclopedia() );
        }

        collection.setTitle( content );

        graph << collection;

        // connect collection <-> article publication
        publication.setProperty(NBIB::collection(), collection.uri() );
        collection.addArticle( publication.uri() );
        collection.addProperty(NAO::hasSubResource(), publication.uri() ); // delete article when collection is removed
    }
    else {
        // put booktitle as the main title of the publication
        publication.setTitle( content );
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addSeriesEditor(const QString &contentValue, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph)
{
    QUrl seriesUrl = publication.inSeries();

    if(!seriesUrl.isValid()) {
        Nepomuk2::NBIB::Series newSeries;

        newSeries.addSeriesOf( publication.uri() );
        publication.setInSeries( newSeries.uri() );

        graph << newSeries;

        addContact(contentValue, newSeries, graph, NBIB::editor());
    }
    else {
        addContact(contentValue, graph[seriesUrl], graph, NBIB::editor());
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addChapter(const QString &content, Nepomuk2::NBIB::Publication &publication, Nepomuk2::NBIB::Reference &reference, Nepomuk2::SimpleResourceGraph &graph)
{
    // If we import some thing we assume no reference already existed and we have a new one
    // thus referencedPart() is not valid
    // if it is valid we assume this was already a nbib:Chapter rather than an generic nbib:DocumentPart

    QUrl chapterUrl = reference.referencedPart();

    if(!chapterUrl.isValid()) {
        Nepomuk2::NBIB::Chapter chapterResource;
        chapterResource.setTitle( i18n("unknown chapter") );
        chapterResource.setChapterNumber( content );
        //BUG: the chapter needs a "unique identifier, otherwise we merge them with other chapters together";
        chapterResource.addType(NIE::DataObject());
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
        graph[chapterUrl].setProperty(NBIB::chapterNumber(), content);
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addChapterName(const QString &content, Nepomuk2::NBIB::Publication &publication, Nepomuk2::NBIB::Reference &reference, Nepomuk2::SimpleResourceGraph &graph)
{
    QUrl chapterUrl = reference.referencedPart();

    // If we import some thing we assume no reference already existed and we have a new one
    // thus referencedPart() is not valid
    // if it is valid we assume this was already a nbib:Chapter rather than an generic nbib:DocumentPart

    if(!chapterUrl.isValid()) {
        Nepomuk2::NBIB::Chapter chapterResource;
        chapterResource.setTitle( content );
        //BUG: the chapter needs a "unique identifier, otherwise we merge them with other chapters together";
        chapterResource.addType(NIE::DataObject());
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
        graph[chapterUrl].setProperty(NIE::title(), content);
        graph << graph[chapterUrl];
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addIssn(const QString &content, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph)
{
    Q_D( PublicationPipe );
    QUrl seriesUrl;
    if(publication.contains(RDF::type(), NBIB::Article())) {
        if( publication.property(NBIB::collection()).isEmpty() ) {
            kWarning() << "we added an article without any collection and try to set the ISSN";
            Nepomuk2::NBIB::Collection collection;
            collection.setTitle( i18n("unknown collection") );

            Nepomuk2::NBIB::Series series;
            series.setTitle( i18n("unknown series") );

            // connect series <-> collection
            collection.setInSeries( series.uri() );
            series.addSeriesOf( collection.uri() );

            // connect article <-> collection
            publication.setProperty(NBIB::collection(), collection );
            collection.addArticle( publication.uri() );

            seriesUrl = series.uri();

            if(d->projectThing.isValid()) {
                series.addProperty( NAO::isRelated() , d->projectThing.uri() );
                collection.addProperty( NAO::isRelated() , d->projectThing.uri() );
            }

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
        Nepomuk2::NBIB::Series newSeries;
        newSeries.addSeriesOf( publication.uri() );
        publication.setInSeries( newSeries.uri() );
        newSeries.setIssn( content );

        if(d->projectThing.isValid()) {
            newSeries.addProperty( NAO::isRelated() , d->projectThing.uri() );
        }

        graph << newSeries;
    }
    else {
        graph[seriesUrl].setProperty(NBIB::issn(), content);
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addOrganization(const QString &content, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph)
{
    Nepomuk2::NCO::OrganizationContact organization;
    organization.setFullname( content );

    if(publication.contains(RDF::type(), NBIB::Article())) {
        // special case if the publication is an article, we assume the organization was responsible for the full collection
        // organization of a proceedings/conference or respnsible for teh blog/forum etc

        QUrl collectionUrl;
        if( !publication.property(NBIB::collection()).isEmpty()) {
            collectionUrl = publication.property(NBIB::collection()).first().toUrl();
        }

        if(!collectionUrl.isValid()) {
            // create new proceedings resource, as non existed yet
            Nepomuk2::NBIB::Proceedings newProceedings;
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

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addCode(const QString &content, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph)
{
    Nepomuk2::NBIB::CodeOfLaw codeOfLaw;
    codeOfLaw.setTitle( content );

    publication.setProperty(NBIB::codeOfLaw(), codeOfLaw.uri()  );

    Q_D( PublicationPipe );
    if(d->projectThing.isValid()) {
        codeOfLaw.addProperty( NAO::isRelated() , d->projectThing.uri() );
    }

    graph << codeOfLaw;
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addCodeNumber(const QString &content, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph)
{
    QUrl codeOfLawUrl = Nepomuk2::NBIB::Legislation(publication).codeOfLaw();

    if(!codeOfLawUrl.isValid()) {
        // create new code of law, seems none existed up to now
        Nepomuk2::NBIB::CodeOfLaw codeOfLaw;
        codeOfLaw.setTitle( content );

        publication.setProperty(NBIB::codeOfLaw(), codeOfLaw.uri()  );
        codeOfLaw.setCodeNumber( content );

        Q_D( PublicationPipe );
        if(d->projectThing.isValid()) {
            codeOfLaw.addProperty( NAO::isRelated() , d->projectThing.uri() );
        }

        graph << codeOfLaw;
    }
    else {
        // add to existing code
        graph[codeOfLawUrl].setProperty(NBIB::codeNumber(), content );
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addCodeVolume(const QString &content, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph)
{
    QUrl codeOfLawUrl = Nepomuk2::NBIB::Legislation(publication).codeOfLaw();

    if(!codeOfLawUrl.isValid()) {
        // create new code of law, seems none existed up to now
        Nepomuk2::NBIB::CodeOfLaw codeOfLaw;
        codeOfLaw.setTitle( content );

        publication.setProperty(NBIB::codeOfLaw(), codeOfLaw.uri()  );
        codeOfLaw.setVolume( content );

        Q_D( PublicationPipe );
        if(d->projectThing.isValid()) {
            codeOfLaw.addProperty( NAO::isRelated() , d->projectThing.uri() );
        }

        graph << codeOfLaw;
    }
    else {
        // add to existing code
        graph[codeOfLawUrl].setProperty(NBIB::volume(), content );
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addReporter(const QString &content, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph)
{
    Nepomuk2::NBIB::CourtReporter courtReporter;
    courtReporter.setTitle( content );
    publication.setProperty(NBIB::courtReporter(), courtReporter.uri()  );

    Q_D( PublicationPipe );
    if(d->projectThing.isValid()) {
        courtReporter.addProperty( NAO::isRelated() , d->projectThing.uri() );
    }

    graph << courtReporter;
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addReporterVolume(const QString &content, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph)
{
    QUrl courtReporterUrl;
    if(!publication.property(NBIB::courtReporter()).isEmpty()) {
        courtReporterUrl = publication.property(NBIB::courtReporter()).first().toUrl();
    }

    Q_D( PublicationPipe );
    if(!courtReporterUrl.isValid()) {
        // create new ccourt reporter, seems none existed up to now
        Nepomuk2::NBIB::CourtReporter courtReporter;
        publication.setProperty(NBIB::courtReporter(), courtReporter.uri() );
        courtReporter.setVolume( content );

        if(d->projectThing.isValid()) {
            courtReporter.addProperty( NAO::isRelated() , d->projectThing.uri() );
        }

        graph << courtReporter;
    }
    else {
        // add to existing court reporter
        graph[courtReporterUrl].setProperty(NBIB::volume(), content );
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addEvent(const QString &content, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph)
{
    Nepomuk2::NCAL::Event event;

    event.setProperty(NAO::prefLabel(), content);

    event.addProperty( NBIB::eventPublication(), publication.uri());
    publication.addEvent( event.uri() );

    Q_D( PublicationPipe );
    if(d->projectThing.isValid()) {
        event.addProperty( NAO::isRelated() , d->projectThing.uri() );
    }

    graph << event;
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addSeries(const QString &content, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph)
{
    Nepomuk2::NBIB::Series series;

    if( publication.contains(RDF::type(), NBIB::Book()) || publication.contains(RDF::type(), NBIB::Booklet()) ) {
        series.addType( NBIB::BookSeries() );
    }

    series.setTitle( content );

    series.addSeriesOf( publication.uri() );
    publication.setInSeries( series.uri()  );

    Q_D( PublicationPipe );
    if(d->projectThing.isValid()) {
        series.addProperty( NAO::isRelated() , d->projectThing.uri() );
    }

    graph << series;
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addTitle(const QString &content, Nepomuk2::NBIB::Publication &publication, Nepomuk2::NBIB::Reference &reference, Nepomuk2::SimpleResourceGraph &graph, const QString & originalEntryType)
{
    // in the case of @InCollection title means title of the article in the book
    // while booktitle is the actual title of the book
    if(originalEntryType == QLatin1String("incollection") || originalEntryType == QLatin1String("dictionaryentry")) {

        QUrl chapterUrl = reference.referencedPart();

        if(!chapterUrl.isValid()) {
            Nepomuk2::NBIB::Chapter chapter;
            chapter.setTitle( content );
            //BUG: the chapter needs a "unique identifier, otherwise we merge them with other chapters together";
            chapter.addType(NIE::DataObject());
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
            graph[chapterUrl].setProperty( NIE::title(), content);
        }
    }
    else {
        publication.setProperty(NIE::title(), content);
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addWebsite(const QString &content, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph)
{
    //TODO: split webpages if necessary ( if we get them as list )
    KUrl url( content );

    QString protocol = url.scheme();

    if(protocol.isEmpty()) {
        kDebug() << "tried to add invalid website without protocol. Add http:// for " << content;
        url.setScheme(QLatin1String("http"));
    }

    if( url.scheme().startsWith(QLatin1String("http")) && url.isValid()) {
        Nepomuk2::NFO::WebDataObject website ( url );

        publication.addLinks( website.uri() );

        graph << website;

    }
    else if( url.scheme().startsWith(QLatin1String("file")) || url.scheme().startsWith(QLatin1String("/home"))  || url.scheme().startsWith('~')) {
        addFileUrl(content, publication, graph);
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addFileUrl(const QString &content, Nepomuk2::NBIB::Publication &publication, Nepomuk2::SimpleResourceGraph &graph)
{
    KUrl url( content );

    if( url.isLocalFile() ) {
        Nepomuk2::NFO::Document localFile( url );
        localFile.addType(NFO::FileDataObject());

        publication.addIsPublicationOf( localFile.uri() );
        localFile.addPublishedAs( publication.uri() );

        graph << localFile;
    }
    else {
        QString protocol = url.scheme();

        if(protocol.isEmpty()) {
            kDebug() << "tried to add invalid remote ur without protocol. Add http:// for " << content;
            url.setScheme(QLatin1String("http"));
        }

        Nepomuk2::NFO::Document remoteFile( url );
        remoteFile.addType(NFO::FileDataObject());
        remoteFile.addType(NFO::RemoteDataObject());

        publication.addIsPublicationOf( remoteFile.uri() );
        remoteFile.addPublishedAs( publication.uri() );

        graph << remoteFile;
    }
}
void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addPublicationDate(const QString &fullDate, Nepomuk2::NBIB::Publication &publication)
{
    QDateTime publicationDate = createDateTime( fullDate );

    if(publicationDate.isValid()) {
        publication.setPublicationDate( publicationDate );
    }
    else {
        kDebug() << "could not parse publication date " << fullDate;
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addPublicationDate(const QString &year, const QString &month, const QString &day, Nepomuk2::NBIB::Publication &publication)
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
            QString contentMonth = finalMonth.toLower();
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

    // create date in iso formart so it will be saved correctly in nepomuk
    QString parsedDate = finalYear + QLatin1String("-") + finalMonth + QLatin1String("-") + finalDay + QLatin1String("T12:12:12");
    addPublicationDate( parsedDate, publication );
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addTag(const QStringList &content, Nepomuk2::SimpleResource &resource, Nepomuk2::SimpleResourceGraph &graph)
{
    foreach(const QString &tagText, content) {
        Nepomuk2::NAO::Tag tag;

        tag.addPrefLabel( tagText );
        tag.addProperty( NAO::identifier(), KUrl::fromEncoded(tagText.toLatin1()));

        graph << tag;
        resource.addProperty( NAO::hasTag(), tag.uri());
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addTopic(const QStringList &content, Nepomuk2::SimpleResource &resource, Nepomuk2::SimpleResourceGraph &graph)
{
    foreach(const QString &topicText, content) {
        Nepomuk2::PIMO::Topic topic;

        topic.setTagLabel(topicText);
        topic.addProperty( NAO::identifier(), KUrl::fromEncoded(topicText.toLatin1()) );

        graph << topic;
        resource.addProperty( NAO::hasTopic(), topic.uri());
    }
}

void  NepomukMetaDataExtractor::Pipe::PublicationPipe::addContact(const QString &contentValue, Nepomuk2::SimpleResource &resource, Nepomuk2::SimpleResourceGraph &graph, QUrl contactProperty)
{
    QList<Nepomuk2::NCO::Contact> personsList = createPersonContacts( contentValue );

    foreach(const Nepomuk2::NCO::Contact &person, personsList) {
        resource.addProperty(contactProperty, person);
        graph << person;
    }

}
