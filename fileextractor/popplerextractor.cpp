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

#include "popplerextractor.h"

#include "../metadataparameters.h"

#include <poppler/qt4/poppler-qt4.h>

#include <KDE/KDebug>

PopplerExtractor::PopplerExtractor(QObject *parent)
    : QObject(parent)
{
}

void PopplerExtractor::parseUrl(MetaDataParameters *mdp, const KUrl &fileUrl)
{
    m_pdfdoc = Poppler::Document::load( fileUrl.toLocalFile(), 0, 0 );

    if(!m_pdfdoc) {
        kWarning() << "could not load " << fileUrl;
        return;
    }

    m_publicationEntry = mdp;
    m_publicationEntry->resourceUri = fileUrl;
    m_publicationEntry->resourceType = QLatin1String("publication");

    QString numOfPages = QString("%1").arg(m_pdfdoc->numPages());
    m_publicationEntry->metaData.insert(QLatin1String("numpages"), numOfPages);

    // get parsed rdf metadata from poppler
    // strip some unneded info and split the keywords
    QStringList metaData = m_pdfdoc->infoKeys();
    foreach(const QString &key, metaData) {

        if(key == QLatin1String("Title")) {
            // sometimes the doi string was added as title value >_<
            if(m_pdfdoc->info(key).contains(QLatin1String("doi:"))) {
                m_publicationEntry->metaData.insert(QLatin1String("doi"), m_pdfdoc->info(key).remove(QLatin1String("doi:")));
            }
            else {
                m_publicationEntry->metaData.insert(QLatin1String("title"), m_pdfdoc->info(key));
            }
        }
        // igrnore the following attributes, they are not of interrest
        else if(key == QLatin1String("Creator") ||
                key == QLatin1String("Producer") ||
                key == QLatin1String("CreationDate") ||
                key == QLatin1String("ModDate") ||
                key == QLatin1String("Trapped") ||
                key.startsWith(QLatin1String("ptex"), Qt::CaseInsensitive)) {
            continue;
        }
        // combine subject and keywords and treat them all as pimo:Topic later on
        else if(key == QLatin1String("Subject") ||
                key == QLatin1String("Keywords")) {
            QString keywords = m_pdfdoc->info(key);
            keywords.replace(',', QLatin1String(";"));

            QString currentKeywordList = m_publicationEntry->metaData.value(QLatin1String("keywords"), QString()).toString();
            if(!currentKeywordList.isEmpty()) {
                keywords.append(QLatin1String(";"));
            }
            currentKeywordList.append( keywords );
            m_publicationEntry->metaData.insert(QLatin1String("keyword"), currentKeywordList);
        }
        else {
            m_publicationEntry->metaData.insert(key.toLower(), m_pdfdoc->info(key));
        }
    }

    // add the toc if available
    QDomDocument *toc = m_pdfdoc->toc();

    if(toc) {
        QDomNode firstNode = toc->firstChild();
        QDomDocument parsedToc;
        tocCreation( parsedToc , firstNode);
        m_publicationEntry->metaData.insert(QLatin1String("toc"), parsedToc.toString());
    }

    parseFirstpage();
}

void PopplerExtractor::tocCreation(const QDomDocument &toc, QDomNode &node)
{
    while(!node.isNull()) {

        QDomElement e = node.toElement();

        if(!e.isNull()) {
            // ignore toc with externel references, won't work anyway
            if(e.hasAttribute(QLatin1String("ExternalFileName"))) {
                node = node.nextSibling();
                continue;
            }

            QString chapterName = e.tagName();
            QString chapterNumber = e.attribute(QLatin1String("DestinationName"));
            chapterNumber.remove(QRegExp("(sub)*section\\."));
            //TODO create toc ...
//            qDebug() << chapterNumber << ":"  << chapterName;
        }

        if(e.hasChildNodes()) {
            QDomNode nextSubSection = e.firstChild();
            tocCreation(toc, nextSubSection);
        }

        node = node.nextSibling();
    }
}

void PopplerExtractor::parseFirstpage()
{
    qDebug() << "parseFirstpage";
    Poppler::Page *p = m_pdfdoc->page(0);

    QList<Poppler::TextBox*> tbList = p->textList();
    QMap<int, QString> possibleTitleMap;

    int currentLargestChar = 0;
    int skipTextboxes = 0;
    foreach(Poppler::TextBox* tb,tbList) {

        // if we added followup words, skip the textboxes here now
        if(skipTextboxes > 0) {
            skipTextboxes--;
            continue;
        }

        int height = tb->charBoundingBox(0).height();

        // if the following text is smaller than the biggest we found up to now, ignore it
        if(height >= currentLargestChar) {
            QString possibleTitle;
            possibleTitle.append( tb->text() );
            currentLargestChar = height;

            // if the text has follow up words add them to to create the full title
            Poppler::TextBox * next =tb->nextWord();
            while(next) {
                possibleTitle.append(QLatin1String(" "));
                possibleTitle.append( next->text() );
                next = next->nextWord();
                skipTextboxes++;
            }

            // now combine text for each font size together, very likeley it must be connected
            QString existingTitlePart = possibleTitleMap.value( currentLargestChar, QString());
            existingTitlePart.append(QLatin1String(" "));
            existingTitlePart.append( possibleTitle );
            possibleTitleMap.insert( currentLargestChar, existingTitlePart);
        }
    }

    qDeleteAll(tbList);
    delete p;

    QList<int> titleSizes = possibleTitleMap.keys();
    qSort(titleSizes.begin(), titleSizes.end(), qGreater<int>());

    QString newPossibleTitle;

    // find the text with the largest font that is not just 1 character
    foreach(int i, titleSizes) {
        QString title = possibleTitleMap.value(i);

        // sometime the biggest part is a single letter
        // as a starting paragraph letter
        if(title.size() < 5) {
            continue;
        }
        else {
            newPossibleTitle = title.trimmed();
            break;
        }
    }

    newPossibleTitle = newPossibleTitle.toAscii();
    QString currentTitle = m_publicationEntry->metaData.value( QLatin1String("title"), QString() ).toString();

    // if no title from metadata was fetched, the possible Title is the title we want
    if( currentTitle.trimmed().isEmpty()) {
        m_publicationEntry->searchTitle = newPossibleTitle;
        return;
    }
    // otherwise check if the actual metadata title is part of the title we found by fetching
    // if so, we can stop, we didn't get a better result, possible just some wrong words at the  end attached
    else if(newPossibleTitle.contains(currentTitle, Qt::CaseInsensitive)) {
        m_publicationEntry->searchTitle = currentTitle;
        return;
    }

    //if the metadata title has no white space, we assume it was just junk and we take the possible title as real title
    if( !currentTitle.contains(' ') || currentTitle.contains(QLatin1String("Microsoft"))) {
        m_publicationEntry->searchTitle = newPossibleTitle;
        m_publicationEntry->searchAltTitle = currentTitle;
        return;
    }

    // in all other cases the possible title is just and alternative title
    m_publicationEntry->searchTitle = currentTitle;
    m_publicationEntry->searchAltTitle = newPossibleTitle;
}
