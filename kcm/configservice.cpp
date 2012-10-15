#include "configservice.h"
#include "ui_configservice.h"

#include <mdesettings.h>

#include <QtDBus/QDBusServiceWatcher>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusConnectionInterface>
#include <QtDBus/QDBusInterface>
#include <QtCore/QProcess>

using namespace NepomukMetaDataExtractor;

ConfigService::ConfigService(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigService)
{
    ui->setupUi(this);

    connect(ui->enableService, SIGNAL(clicked(bool)), this, SLOT(serviceEnabled(bool)) );

    m_watcher = new QDBusServiceWatcher( this );
    m_watcher->addWatchedService( QLatin1String("org.kde.nepomuk.services.metadataextractorservice") );
    m_watcher->setConnection( QDBusConnection::sessionBus() );
    m_watcher->setWatchMode( QDBusServiceWatcher::WatchForRegistration | QDBusServiceWatcher::WatchForUnregistration );

    connect( m_watcher, SIGNAL( serviceRegistered(const QString&) ),
             this, SLOT( serviceRegistered() ) );
    connect( m_watcher, SIGNAL( serviceUnregistered(const QString&) ),
             this, SLOT( serviceUnregistered() ) );

    if( QDBusConnection::sessionBus().interface()->isServiceRegistered( "org.kde.nepomuk.services.metadataextractorservice" ) ) {
        ui->enableService->setChecked(true);
    }

    ui->kintspinbox->setValue( MDESettings::simultaneousCalls() );
    connect( ui->kintspinbox, SIGNAL(valueChanged(int)), this, SLOT(changeSettings()) );
}

ConfigService::~ConfigService()
{
    delete ui;
    delete m_watcher;
}

void ConfigService::serviceEnabled(bool enabled)
{
    if(enabled) {
        QProcess::startDetached( QLatin1String( "nepomukservicestub metadataextractorservice" ) );
    }
    else {
        QDBusInterface service( "org.kde.nepomuk.services.metadataextractorservice", "/servicecontrol",
                                "org.kde.nepomuk.ServiceControl" );
        service.call( "shutdown" );
    }
}

void ConfigService::serviceRegistered()
{
    ui->enableService->setChecked(true);
}

void ConfigService::serviceUnregistered()
{
    ui->enableService->setChecked(false);
}

void ConfigService::changeSettings()
{
    emit configChanged(true);
}

void ConfigService::saveConfig()
{
    MDESettings::setSimultaneousCalls(ui->kintspinbox->value());

    MDESettings::self()->writeConfig();
}

void ConfigService::resetConfig()
{
    ui->kintspinbox->setValue( MDESettings::simultaneousCalls() );
}
