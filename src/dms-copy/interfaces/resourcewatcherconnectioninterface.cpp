/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -m -p resourcewatcherconnectioninterface /home/joerg/Development/KDE/nepomuk-core/interfaces/org.kde.nepomuk.ResourceWatcherConnection.xml
 *
 * qdbusxml2cpp is Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#include "resourcewatcherconnectioninterface.h"

/*
 * Implementation of interface class OrgKdeNepomukResourceWatcherConnectionInterface
 */

OrgKdeNepomukResourceWatcherConnectionInterface::OrgKdeNepomukResourceWatcherConnectionInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
}

OrgKdeNepomukResourceWatcherConnectionInterface::~OrgKdeNepomukResourceWatcherConnectionInterface()
{
}


#include "resourcewatcherconnectioninterface.moc"