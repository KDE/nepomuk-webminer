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

#include "metadataextractorplugin.h"

#include <KDE/KPluginFactory>
#include <KDE/KPluginLoader>
#include <KAboutData>
#include <KDE/KLocalizedString>

#include <KParts/StatusBarExtension>

#include "webextractor/extractorfactory.h"
#include "webextractor/webextractor.h"

#include "nepomukpipe/nepomukpipe.h"
#include "nepomukpipe/moviepipe.h"
#include "nepomukpipe/publicationpipe.h"
#include "nepomukpipe/tvshowpipe.h"
#include "nepomukpipe/musicpipe.h"

#include <KDE/KDebug>

using namespace NepomukMetaDataExtractor;
using namespace Extractor;

static const KAboutData aboutdata("metadataextractorplugin", 0, ki18n("MetaDataExtractorPlugin Settings") , "0.2");

K_PLUGIN_FACTORY(MetaDataExtractorPluginFactory, registerPlugin<MetaDataExtractorPlugin>();)
K_EXPORT_PLUGIN(MetaDataExtractorPluginFactory(aboutdata))

MetaDataExtractorPlugin::MetaDataExtractorPlugin(QObject *parent, const QVariantList &args)
    : KParts::Plugin(parent),
      extractionInProgress(false)
{
    Q_UNUSED(args);

    // Initialize plugin
    setComponentData(MetaDataExtractorPluginFactory::componentData());

    m_Part = dynamic_cast<KParts::ReadOnlyPart *>(parent);
    if (!m_Part) {
        kDebug() << "Unable to get KHTMLPart" << endl;
        return;
    }
    connect(m_Part, SIGNAL(completed()), this, SLOT(lateInitialization()));

    // initilaze extractor factory that holds all plugins
    m_ef = new ExtractorFactory;

}

MetaDataExtractorPlugin::~MetaDataExtractorPlugin()
{
    delete m_ef;
}

void MetaDataExtractorPlugin::lateInitialization()
{
    kDebug() << "late initialization";
    disconnect(m_Part, SIGNAL(completed()), this, SLOT(lateInitialization()));

    // add the icon to the status bar
    // TODO: get it into the url bar and show the icon there
    KIconLoader *loader = KIconLoader::global();
    m_icon.setPixmap(loader->loadIcon("nepomuk", KIconLoader::Small));
    KParts::StatusBarExtension* barExt = KParts::StatusBarExtension::childObject(m_Part);
    if (barExt)
        barExt->addStatusBarItem(&m_icon, 0, false);
    m_icon.setVisible(false);
    m_icon.setToolTip(i18n("Import Item into Nepomuk"));

    connect(m_Part, SIGNAL(started(KIO::Job*)), this, SLOT(urlSwitched()));
    connect(&m_icon, SIGNAL(leftClickedUrl()), this, SLOT(extract()));

    urlSwitched();
}

void MetaDataExtractorPlugin::urlSwitched()
{
    // check if current url is supported by one of the plugins
    QUrl url = m_Part->url();
    WebExtractor *we = m_ef->getExtractor(url);

    if (we) {
        kDebug() << "website" << url << "supported";
        m_icon.setVisible(true);
    } else {
        kDebug() << "website" << url << "NOT supported";
        m_icon.setVisible(false);
    }
}

void MetaDataExtractorPlugin::extract()
{
    if (extractionInProgress) {
        kDebug() << "extraction already in progress";
        return;
    }

    kDebug() << "extract item";

    // extract item data
    QUrl url = m_Part->url();

    WebExtractor *we = m_ef->getExtractor(url);
    connect(we, SIGNAL(itemResults(QString,QVariantMap)), this, SLOT(pushDataToNepomuk(QString,QVariantMap)));

    if (we) {
        we->extractItem(url, QVariantMap());
        extractionInProgress = true;
        m_icon.setEnabled(false);
    }
}

void MetaDataExtractorPlugin::pushDataToNepomuk(const QString &resourceType, const QVariantMap &entry)
{
    kDebug() << "finished Item fetching, push to nepomuk";

    Pipe::NepomukPipe *nepomukPipe = 0;
    if (resourceType == QLatin1String("publication")) {
        nepomukPipe = new Pipe::PublicationPipe;
    } else if (resourceType == QLatin1String("tvshow")) {
        nepomukPipe = new Pipe::TvShowPipe;
    } else if (resourceType == QLatin1String("movie")) {
        nepomukPipe = new Pipe::MoviePipe;
    } else if (resourceType == QLatin1String("music")) {
        nepomukPipe = new Pipe::MusicPipe;
    }

    if (nepomukPipe) {
        nepomukPipe->pipeImport(entry);
    } else {
        kDebug() << "No nepomuk pipe available for the resource type" << resourceType;
    }
    extractionInProgress = false;
    m_icon.setEnabled(true);

    delete nepomukPipe;
    sender()->deleteLater();
}
