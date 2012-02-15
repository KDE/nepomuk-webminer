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

#include "odfextractor.h"

#include "../publicationentry.h"

#include <KDE/KZip>
#include <KDE/KDebug>

OdfExtractor::OdfExtractor(QObject *parent)
    : QObject(parent)
{
}

PublicationEntry * OdfExtractor::parseUrl(const KUrl &fileUrl)
{
    KZip zip( fileUrl.toLocalFile() );
    if ( !zip.open( QIODevice::ReadOnly ) ) {
        qWarning() << "Document is not a valid ZIP archive";
        return 0;
    }

    const KArchiveDirectory *directory = zip.directory();
    if ( !directory ) {
      qWarning() << "Invalid document structure (main directory is missing)";
      return 0;
    }

    const QStringList entries = directory->entries();
    if ( !entries.contains( "meta.xml" ) ) {
        qWarning() << "Invalid document structure (meta.xml is missing)";
        return 0;
    }

    m_publicationEntry = new PublicationEntry();
    m_publicationEntry->fileUrl = fileUrl;

    QDomDocument metaData("metaData");
    const KArchiveFile *file = static_cast<const KArchiveFile*>( directory->entry( "meta.xml" ) );
    metaData.setContent( file->data() );

    zip.close();

    // parse metadata ...
    QDomElement docElem = metaData.documentElement();

     QDomNode n = docElem.firstChild().firstChild(); // <office:document-meta> ... <office:meta> ... content
     while(!n.isNull()) {
         QDomElement e = n.toElement();
         if(!e.isNull()) {

             if( e.tagName() == QLatin1String("dc:description")) {
                 m_publicationEntry->dataMap.insert(QLatin1String("note"), e.text() );
             }
             else if( e.tagName() == QLatin1String("meta:keyword")) {
                 QString keywords = m_publicationEntry->dataMap.value( QLatin1String("keywords"), QString() );
                 if(!keywords.isEmpty()) {
                     keywords.append( QLatin1String("; ") );
                 }
                 keywords.append( e.text() );
             }
             else if( e.tagName() == QLatin1String("dc:subject")) {
                 QString keywords = m_publicationEntry->dataMap.value( QLatin1String("keywords"), QString() );
                 if(!keywords.isEmpty()) {
                     keywords.append( QLatin1String("; ") );
                 }
                 keywords.append( e.text() );
             }
             else if( e.tagName() == QLatin1String("dc:title")) {
                 m_publicationEntry->dataMap.insert(QLatin1String("title"), e.text() );
             }
             else if( e.tagName() == QLatin1String("meta:document-statistic")) {
                 m_publicationEntry->dataMap.insert(QLatin1String("numofpages"), e.attribute("meta:page-count") );

             }
             else if( e.tagName() == QLatin1String("meta:user-defined")) {
                 m_publicationEntry->dataMap.insert(e.attribute("meta:name"), e.text() );
             }
         }
         n = n.nextSibling();
     }

    return m_publicationEntry;
}
