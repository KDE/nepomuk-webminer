
#include "metadataextractorkcm.h"

#include "webextractor/extractorfactory.h"

#include <mdesettings.h>
#include "configfetcher.h"
#include "configservice.h"
#include "pluginlist.h"

// KDE includes
#include <KDE/KCModuleLoader>
#include <KDE/KPluginFactory>
#include <KDE/KPluginLoader>
#include <KDE/KAboutData>
#include <KDE/KLocalizedString>
#include <KDE/KPageWidget>
#include <KDE/KIcon>

#include <QtGui/QVBoxLayout>

using namespace NepomukMetaDataExtractor;

K_PLUGIN_FACTORY( MetaDataExtractorKCMFactory, registerPlugin<MetaDataExtractorKCM>(); )
K_EXPORT_PLUGIN( MetaDataExtractorKCMFactory("kcm_metadataextractor") )

MetaDataExtractorKCM::MetaDataExtractorKCM(QWidget *parent, const QVariantList &list)
    : KCModule( MetaDataExtractorKCMFactory::componentData(), parent, list )
{

    KAboutData *about = new KAboutData(
                "kcm_metadataextractor", "kcm_metadataextractor", ki18n("Metadata Extractor Configuration Module"),
                KDE_VERSION_STRING, KLocalizedString(), KAboutData::License_GPL,
                ki18n("Copyright 2012 Jörg Ehrichs"));
    about->addAuthor(ki18n("Jörg Ehrichs"), KLocalizedString(), "Joerg.Ehrichs@gmx.de");
    setAboutData(about);
    setButtons(Help|Apply);

    m_ef = new Extractor::ExtractorFactory;

    KPageWidget* pageWidget = new KPageWidget();

    cfd = new ConfigFetcher();
    cfd->setExtractorFactory(m_ef);
    KPageWidgetItem *fetcherPage =  new KPageWidgetItem(cfd, i18n("Fetcher"));
    fetcherPage->setIcon( KIcon(QLatin1String("download")) );
    pageWidget->addPage( fetcherPage );
    connect(cfd, SIGNAL(configChanged(bool)), this, SLOT(changed()) );

    pl = new PluginList();
    pl->setExtractorFactory(m_ef);
    KPageWidgetItem *pluginPage =  new KPageWidgetItem(pl, i18n("Plugins"));
    pluginPage->setIcon( KIcon(QLatin1String("run-build-configure")) );
    pageWidget->addPage( pluginPage );

    csd = new ConfigService();
    KPageWidgetItem *servicePage =  new KPageWidgetItem(csd, i18n("Service"));
    servicePage->setIcon( KIcon(QLatin1String("services")) );
    pageWidget->addPage( servicePage );
    connect(csd, SIGNAL(configChanged(bool)), this, SLOT(changed()) );

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(pageWidget);
}

void MetaDataExtractorKCM::save()
{
    cfd->saveConfig();
    csd->saveConfig();
}

void MetaDataExtractorKCM::load()
{
    cfd->resetConfig();
    csd->resetConfig();
}