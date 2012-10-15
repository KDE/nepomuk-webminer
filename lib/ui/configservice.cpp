#include "configservice.h"
#include "ui_configservice.h"

#include <mdesettings.h>

#include <QtDBus/QDBusServiceWatcher>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusConnectionInterface>
#include <QtDBus/QDBusInterface>
#include <QtCore/QProcess>

ConfigService::ConfigService(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigService)
{
    ui->setupUi(this);

    connect(ui->enableService, SIGNAL(clicked(bool)), this, SLOT(serviceEnabled(bool)) );

    QDBusServiceWatcher * watcher = new QDBusServiceWatcher( this );
    watcher->addWatchedService( QLatin1String("org.kde.nepomuk.services.metadataextractorservice") );
    watcher->setConnection( QDBusConnection::sessionBus() );
    watcher->setWatchMode( QDBusServiceWatcher::WatchForRegistration | QDBusServiceWatcher::WatchForUnregistration );

    connect( watcher, SIGNAL( serviceRegistered(const QString&) ),
             this, SLOT( serviceRegistered() ) );
    connect( watcher, SIGNAL( serviceUnregistered(const QString&) ),
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
    MDESettings::setSimultaneousCalls(ui->kintspinbox->value());
}
