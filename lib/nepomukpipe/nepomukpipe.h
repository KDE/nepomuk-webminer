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

#ifndef NEPOMUKPIPE_H
#define NEPOMUKPIPE_H

#include <QtCore/QObject>
#include <QtCore/QVariantMap>
#include <QtCore/QUrl>
#include <QtCore/QDateTime>

#include <KDE/KUrl>


namespace Nepomuk2 {
namespace NCO {
    class PersonContact;
    class OrganizationContact;
}
}

class KJob;

namespace NepomukMetaDataExtractor {
namespace Pipe {

/**
  * @brief This part is used to import a plain QVariantMap into Nepomuk
  *
  * The data received from the internet is nothing else than a list of @c key=value pairs.
  * To save one the trouble of writing the Nepomuk parts that does the semantic transition from such an key=value
  * list to the right ontology parts, this pipe appraoch exist.
  *
  * It is not meant to cover all of Nepomuks ontoloy but rather a small defined subset of items that will be fetched
  * from the internet. It can also be reused by any other application that simply wants to stay away from nepomuks ontology
  * and rather likes to push a simple QVariantMap into teh database
  */
class NepomukPipe : public QObject
{
    Q_OBJECT
public:
    explicit NepomukPipe(QObject *parent = 0);
    virtual ~NepomukPipe();

    /**
      * Must be implemented in subclasses. Does teh actual work
      */
    virtual void pipeImport(const QVariantMap &entry) = 0;

protected:
    /**
      * helper function to download the @c banner or @c poster
      *
      * @p bannerUrl web url where the image can be retrieved from
      * @p additionalLocationInfo additional information where to save the image
      * @return a KUrl with the local file url of the new image
      */
    KUrl downloadBanner(const QUrl &bannerUrl, const QString& additionalLocationInfo ) const;

    /**
      * transforms some common variants of date formats into a QDateTime.
      */
    QDateTime createDateTime(const QString &dateString) const;

    /**
      * splits a list of person names and creates a list of @c nco:PersonContact from it.
      *
      * Takes care of Hans Wurst and Franz Ferdinant or Wurst, Hans; Ferdinant, Franz and so on.
      * Also tries to determine family/given name and suffix values
      */
    QList<Nepomuk2::NCO::PersonContact> createPersonContacts(const QString & listOfPersonNames) const;

    /**
      * Splits a list of organization names and create the @c nco:OrganizationContact from them
      */
    QList<Nepomuk2::NCO::OrganizationContact> createOrganizationContacts(const QString & listOfOrganizations) const;

private slots:
    /**
      * will output the error message of the DMS call if it failed
      */
    void slotSaveToNepomukDone(KJob *job);

private:
    /**
      * Struct to hold parsed name parts
      */
    struct Name {
        QString first;
        QString last;
        QString suffix;
        QString full;
    };

    QList<NepomukMetaDataExtractor::Pipe::NepomukPipe::NepomukPipe::Name> splitPersonList(const QString & persons) const;

    // taken from KBibtex, so
    // @author Thomas Fischer <fischer@unix-ag.uni-kl.de> with some modifications
    NepomukMetaDataExtractor::Pipe::NepomukPipe::NepomukPipe::Name splitPersonString(const QString & persos) const;
};
}
}

#endif // NEPOMUKPIPE_H
