/* This file is part of the KDE Project
   Copyright (c) 2008 Sebastian Trueg <trueg@kde.org>
   Copyright (c) 2010-12 Vishesh Handa <me@vhanda.in>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "eventmonitor.h"
#include "indexscheduler.h"

#include <KDebug>
#include <KLocale>
#include <KDiskFreeSpaceInfo>
#include <KStandardDirs>
#include <KIcon>
#include <KConfigGroup>
#include <KIdleTime>

#include <Solid/PowerManagement>

#include <QtDBus/QDBusInterface>

// TODO: Make idle timeout configurable?
static int s_idleTimeout = 1000 * 60 * 2; // 2 min
static int s_availSpaceTimeout = 1000 * 30; // 30 seconds

Nepomuk2::EventMonitor::EventMonitor( QObject* parent )
    : QObject( parent )
{
    // monitor the powermanagement to not drain the battery
    connect( Solid::PowerManagement::notifier(), SIGNAL(appShouldConserveResourcesChanged(bool)),
             this, SLOT(slotPowerManagementStatusChanged(bool)) );

    connect( Solid::Networking::notifier(), SIGNAL(statusChanged(Solid::Networking::Status)),
            this, SLOT(slotNetworkgStatusChanged(Solid::Networking::Status)) );

    // setup the avail disk usage monitor
    connect( &m_availSpaceTimer, SIGNAL(timeout()), this, SLOT(slotCheckAvailableSpace()) );

    // setup idle time
    KIdleTime* idleTime = KIdleTime::instance();
    connect( idleTime, SIGNAL(timeoutReached(int)), this, SLOT(slotIdleTimeoutReached()) );
    connect( idleTime, SIGNAL(resumingFromIdle()), this, SLOT(slotResumeFromIdle()) );

    m_isOnBattery = Solid::PowerManagement::appShouldConserveResources();
    m_isIdle = false;
    m_isDiskSpaceLow = false; /* We hope */
    m_enabled = false;
    m_isNetworkDisabled = false;
}

Nepomuk2::EventMonitor::~EventMonitor()
{
}

void Nepomuk2::EventMonitor::slotPowerManagementStatusChanged( bool conserveResources )
{
    m_isOnBattery = conserveResources;
    if( m_enabled ) {
        emit powerManagementStatusChanged( conserveResources );
    }
}

void Nepomuk2::EventMonitor::slotCheckAvailableSpace()
{
    if( !m_enabled ) {
        return;
    }

    // we do not send status information here.
    // this is already done by the nepomuk fileindexer
    // we simply stop the webminer

    QString path = KStandardDirs::locateLocal( "data", "nepomuk/repository/", false );
    KDiskFreeSpaceInfo info = KDiskFreeSpaceInfo::freeSpaceInfo( path );
    if ( info.isValid() ) {

        //Take settings from nepomukfileindexer

        KConfig config( "nepomukstrigirc" );
        KConfigGroup cfg = config.group( "General" );

        //default 200 MB
        if ( info.available() <= cfg.readEntry( "min disk space", KIO::filesize_t( 200*1024*1024 ) ) ) {
            m_isDiskSpaceLow = true;
            emit diskSpaceStatusChanged( true );
        }
        else if( m_isDiskSpaceLow ) {
            // We only emit this signal, if previously emitted with the value true
            m_isDiskSpaceLow = false;
            emit diskSpaceStatusChanged( false );
        }
    }
    else {
        // if it does not work once, it will probably never work
        m_availSpaceTimer.stop();
    }
}

void Nepomuk2::EventMonitor::slotNetworkgStatusChanged(Solid::Networking::Status status)
{
    if(status == Solid::Networking::Connected) {
        m_isNetworkDisabled = false;
    }
    else {
        m_isNetworkDisabled = true;
    }
}

void Nepomuk2::EventMonitor::enable()
{
    /* avoid add multiple idle timeout */
    if (!m_enabled) {
        m_enabled = true;
        KIdleTime::instance()->addIdleTimeout( s_idleTimeout );
    }

    if (!m_availSpaceTimer.isActive()) {
        m_availSpaceTimer.start( s_availSpaceTimeout );
    }
}

void Nepomuk2::EventMonitor::disable()
{
    if (m_enabled) {
        m_enabled = false;
        KIdleTime::instance()->removeAllIdleTimeouts();
    }

    m_availSpaceTimer.stop();
}

void Nepomuk2::EventMonitor::suspendDiskSpaceMonitor()
{
    m_availSpaceTimer.stop();
}

void Nepomuk2::EventMonitor::resumeDiskSpaceMonitor()
{
    if (m_enabled && !m_availSpaceTimer.isActive()) {
        m_availSpaceTimer.start( s_availSpaceTimeout );
    }
}

void Nepomuk2::EventMonitor::slotIdleTimeoutReached()
{
    if( m_enabled ) {
        m_isIdle = true;
        emit idleStatusChanged( true );
    }
    KIdleTime::instance()->catchNextResumeEvent();
}

void Nepomuk2::EventMonitor::slotResumeFromIdle()
{
    m_isIdle = false;
    if( m_enabled ) {
        emit idleStatusChanged( false );
    }
}


#include "eventmonitor.moc"
