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

#include <KDE/KDebug>

using namespace NepomukMetaDataExtractor;
using namespace Extractor;

static const KAboutData aboutdata("metadataextractorplugin", 0, ki18n("MetaDataExtractorPlugin Settings") , "0.2" );

K_PLUGIN_FACTORY(MetaDataExtractorPluginFactory, registerPlugin<MetaDataExtractorPlugin>(); )
K_EXPORT_PLUGIN(MetaDataExtractorPluginFactory(aboutdata))

MetaDataExtractorPlugin::MetaDataExtractorPlugin(QObject *parent, const QVariantList &args)
        : KParts::Plugin(parent),
          extractionInProgress(false)
{
Q_UNUSED(args);

	// Initialize plugin
	setComponentData(MetaDataExtractorPluginFactory::componentData());

	m_Part = dynamic_cast<KParts::ReadOnlyPart *>(parent);
	if (!m_Part)
	{
		kDebug() << "Unable to get KHTMLPart" << endl;
		return;
    }
    connect(m_Part, SIGNAL(completed()), this, SLOT(lateInitialization()));

    // initilaze extractor factory that holds all plugins
    m_ef = new ExtractorFactory;

}

MetaDataExtractorPlugin::~MetaDataExtractorPlugin()
{
    kDebug() << "destroy plugin again oO";
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
    WebExtractor *we = m_ef->createExtractor( url );

    if(we) {
        kDebug() << "website" << url << "supported";
        m_icon.setVisible(true);
    }
    else {
        kDebug() << "website" << url << "NOT supported";
        m_icon.setVisible(false);
    }
}

void MetaDataExtractorPlugin::extract()
{
    if(extractionInProgress) {
        kDebug() << "extraction already in progress";
        return;
    }

    kDebug() << "extract item";

    // extract item data
    QUrl url = m_Part->url();

    WebExtractor *we = m_ef->createExtractor( url );
    connect(we, SIGNAL(itemResults(QString,QVariantMap)), this, SLOT(pushDataToNepomuk(QString,QVariantMap)));

    if(we) {
        we->extractItem( url, QVariantMap() );
        extractionInProgress = true;
        m_icon.setEnabled(false);
    }
}

void MetaDataExtractorPlugin::pushDataToNepomuk(const QString &resourceType, const QVariantMap &entry)
{
    kDebug() << "finished Item fetching, push to nepomuk";

    Pipe::NepomukPipe *nepomukPipe = 0;
    if(resourceType == QLatin1String("publication")) {
        nepomukPipe = new Pipe::PublicationPipe;
    }
    else if(resourceType == QLatin1String("tvshow")) {
        nepomukPipe = new Pipe::TvShowPipe;
    }
    else if(resourceType == QLatin1String("movie")) {
        nepomukPipe = new Pipe::MoviePipe;
    }

    if(nepomukPipe) {
        nepomukPipe->pipeImport( entry );
    }
    else {
        kDebug() << "No nepomuk pipe available for the resoure type" << resourceType;
    }
    extractionInProgress = false;
    m_icon.setEnabled(true);

    delete nepomukPipe;
    sender()->deleteLater();
}
