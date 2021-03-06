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

#include "nepomukpipe.h"

#include <Nepomuk2/StoreResourcesJob>
#include <KDE/KJob>
#include <KDE/KIO/CopyJob>
#include <KDE/KStandardDirs>

#include <KDE/KDebug>

#include <QtCore/QFile>

#include "ontology/nco/personcontact.h"
#include "ontology/nco/organizationcontact.h"

#include <mdesettings.h>


namespace NepomukWebMiner
{
namespace Pipe
{
class NepomukPipePrivate
{
public:
    QString componentname; /**< Name of the component that will be used for the Nepomuk DMS calls.
                                Useful if all data that 1 component created should be deleted.
                                This should not always be the metadata extarctor itself
                                But might be the application, that reuses this library */
};
}
}


NepomukWebMiner::Pipe::NepomukPipe::NepomukPipe(QObject *parent)
    : QObject(parent)
    , d_ptr(new NepomukWebMiner::Pipe::NepomukPipePrivate)
{
    Q_D(NepomukPipe);
    d->componentname = QLatin1String("nepomuk-webminer");

}

NepomukWebMiner::Pipe::NepomukPipe::~NepomukPipe()
{

}

void NepomukWebMiner::Pipe::NepomukPipe::slotSaveToNepomukDone(KJob *job) const
{
    if (job->error()) {
        kDebug() << "Failed to store information in Nepomuk. " << job->errorString();
    } else {
        kDebug() << "Successfully stored data into Nepomuk";
    }
}

KUrl NepomukWebMiner::Pipe::NepomukPipe::downloadBanner(const QUrl &bannerUrl, const QString &filename, const QString& subFolder, const QString &resourceFolder) const
{
    if (!MDESettings::downloadBanner()) {
        return KUrl();
    }

    KUrl localUrl;
    if (MDESettings::saveBannerInResourceFolder()) {
        localUrl = resourceFolder + QLatin1String("/") + QUrl::toPercentEncoding(filename) + QLatin1String(".") + bannerUrl.toString().split('.').last();
    } else {
        localUrl = KGlobal::dirs()->locateLocal("data", QLatin1String("nepomuk/data/banners/") + subFolder + QLatin1String("/") + QUrl::toPercentEncoding(filename) +  QLatin1String(".") + bannerUrl.toString().split('.').last(), true);
    }

    if (!QFile::exists(localUrl.toLocalFile())) {
        KIO::CopyJob* job = KIO::copy(bannerUrl, localUrl, KIO::HideProgressInfo | KIO::Overwrite);
        if (!job->exec()) {
            return KUrl();
        }
    }
    return localUrl;
}

QDateTime NepomukWebMiner::Pipe::NepomukPipe::createDateTime(const QString &dateString)
{
    QDateTime dateTime;

    if (!dateTime.isValid()) {
        dateTime = QDateTime::fromString(dateString, "yyyy-MM-dd");
        dateTime.setTimeSpec(Qt::UTC);
    }
    if (!dateTime.isValid()) {
        dateTime = QDateTime::fromString(dateString, "dd-MM-yyy");
        dateTime.setTimeSpec(Qt::UTC);
    }
    if (!dateTime.isValid()) {
        dateTime = QDateTime::fromString(dateString, "yyyy-MM");
        dateTime.setTimeSpec(Qt::UTC);
    }
    if (!dateTime.isValid()) {
        dateTime = QDateTime::fromString(dateString, "MM-yyyy");
        dateTime.setTimeSpec(Qt::UTC);
    }
    if (!dateTime.isValid()) {
        dateTime = QDateTime::fromString(dateString, "yyyy.MM.dd");
        dateTime.setTimeSpec(Qt::UTC);
    }
    if (!dateTime.isValid()) {
        dateTime = QDateTime::fromString(dateString, "dd.MM.yyyy");
        dateTime.setTimeSpec(Qt::UTC);
    }
    if (!dateTime.isValid()) {
        dateTime = QDateTime::fromString(dateString, "dd MMMM yyyy");
        dateTime.setTimeSpec(Qt::UTC);
    }
    if (!dateTime.isValid()) {
        dateTime = QDateTime::fromString(dateString, "MM.yyyy");
        dateTime.setTimeSpec(Qt::UTC);
    }
    if (!dateTime.isValid()) {
        dateTime = QDateTime::fromString(dateString, "yyyy.MM");
        dateTime.setTimeSpec(Qt::UTC);
    }
    if (!dateTime.isValid()) {
        dateTime = QDateTime::fromString(dateString, "yyyy");
        dateTime.setTimeSpec(Qt::UTC);
    }
    if (!dateTime.isValid()) {
        dateTime = QDateTime::fromString(dateString, "yy");
        dateTime.setTimeSpec(Qt::UTC);
    }
    if (!dateTime.isValid()) {
        dateTime = QDateTime::fromString(dateString, Qt::ISODate);
    }
    if (!dateTime.isValid()) {
        kWarning() << "Could not determine correct datetime format from:" << dateString;
        return QDateTime();
    }

    return dateTime;
}

void NepomukWebMiner::Pipe::NepomukPipe::overrideComponentName(const QString &name)
{
    Q_D(NepomukPipe);
    d->componentname = name;
}

QString NepomukWebMiner::Pipe::NepomukPipe::componentName() const
{
    //Q_D( NepomukPipe );
    return d_ptr->componentname;
}

QList<Nepomuk2::NCO::Contact> NepomukWebMiner::Pipe::NepomukPipe::createPersonContacts(const QString & listOfPersonNames) const
{
    QList<Nepomuk2::NCO::Contact> resultList;
    QList<Name> personList = splitPersonList(listOfPersonNames);

    foreach (const Name & person, personList) {
        if (person.full.isEmpty()) {
            continue;
        }
        // create new contact resource, duplicates will be merged by the DMS later on
        Nepomuk2::NCO::Contact personResource;
        personResource.setFullname(person.full.trimmed());
        //TODO: reintroduce nco:PersonContact for publication authors? Or change ontology to allow NameGiven/NameFamily etc in the generic nco:Contact
        //personResource.setNameGiven( person.first );
        //personResource.setNameFamily( person.last );
        //QStringList suffixes;
        //suffixes << person.suffix;
        //personResource.setNameHonorificSuffixs( suffixes );

        resultList << personResource;
    }

    return resultList;
}


QList<Nepomuk2::NCO::OrganizationContact> NepomukWebMiner::Pipe::NepomukPipe::createOrganizationContacts(const QString & listOfOrganizations) const
{
    QList<Nepomuk2::NCO::OrganizationContact> resultList;
    QList<Name> personList = splitPersonList(listOfOrganizations);

    foreach (const Name & person, personList) {
        // create new contact resource, duplicates will be merged by the DMS later on
        Nepomuk2::NCO::OrganizationContact organizationResource;
        organizationResource.setFullname(person.full);

        resultList << organizationResource;
    }

    return resultList;
}

QList<NepomukWebMiner::Pipe::NepomukPipe::NepomukPipe::Name> NepomukWebMiner::Pipe::NepomukPipe::splitPersonList(const QString & person)
{
    // split list of names into single Strings
    // first remove any  { } rom the string
    QString cleanedPersonString = person;
    cleanedPersonString = cleanedPersonString.remove('{');
    cleanedPersonString = cleanedPersonString.remove('}');

    QStringList personStringList;

    // first try usual list of people returned from the plugins
    // Hans Wurst; Jochen Junker
    personStringList = cleanedPersonString.split(QLatin1String(";"));

    if (personStringList.size() == 1) {
        // just in case: Hans Wurst & Jochen Junker
        personStringList = cleanedPersonString.split(QLatin1String("&"));
    }

    if (personStringList.size() == 1) {
        // now try bibtex like description: Hans Wurst and Jochen Junker
        personStringList = cleanedPersonString.split(QLatin1String(" and "));
    }

    // ok now we have 1 to several names, they can be specified in different versions
    // Hans Wurst or Wurst, Hans or H. Wurst or Hans W. and so on.
    QList<NepomukPipe::Name> personList;

    foreach (const QString & person, personStringList) {
        personList.append(splitPersonString(person));
    }

    return personList;
}

NepomukWebMiner::Pipe::NepomukPipe::NepomukPipe::Name NepomukWebMiner::Pipe::NepomukPipe::splitPersonString(const QString & person)
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
    foreach (const QString & token, personTokens) {

        // Position where comma was found, or -1 if no comma in token
        int p = -1;
        if (commaCount < 2) {
            // Only check if token contains comma
            // if no comma was found before
            int bracketCounter = 0;
            for (int i = 0; i < token.length(); ++i) {
                // Consider opening curly brackets
                if (token[i] == QChar('{')) ++bracketCounter;
                // Consider closing curly brackets
                else if (token[i] == QChar('}')) --bracketCounter;
                // Only if outside any open curly bracket environments
                // consider comma characters
                else if (bracketCounter == 0 && token[i] == QChar(',')) {
                    // Memorize comma's position and break from loop
                    p = i;
                    break;
                } else if (bracketCounter < 0)
                    // Should never happen: more closing brackets than opening ones
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
        NepomukPipe::Name newName;
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
        // Name was actually given in PubMed format
        NepomukPipe::Name newName;
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
            // handle name suffices like "Jr" or "III."
            partC.prepend(*it);
        else if (partB.isEmpty() || (it->size() > 1 && it->at(0).isLower()))
            partB.prepend(*it);
        else
            partA.prepend(*it);
    }
    if (!partB.isEmpty()) {
        // Name was actually like "Peter Ole van der Tuckwell",
        // split into "Peter Ole" and "van der Tuckwell"
        NepomukPipe::Name newName;
        newName.first = partA.join(QChar(' '));
        newName.last = partB.join(QChar(' '));
        newName.suffix = partC.isEmpty() ? QString::null : partC.join(QChar(' '));
        newName.full = newName.first + ' ' + newName.last + newName.suffix;
        newName.full = newName.full.trimmed();

        return newName;
    }

    kWarning() << "Don't know how to handle name" << personTokens.join(QChar(' '));
    NepomukPipe::Name newName;
    return newName;
}
