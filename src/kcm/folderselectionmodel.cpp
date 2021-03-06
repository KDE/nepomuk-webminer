/* This file is part of the KDE Project
   Copyright (c) 2008 Sebastian Trueg <trueg@kde.org>

   Based on CollectionSetup.cpp from the Amarok project
   (C) 2003 Scott Wheeler <wheeler@kde.org>
   (C) 2004 Max Howell <max.howell@methylblue.com>
   (C) 2004 Mark Kretschmann <markey@web.de>
   (C) 2008 Seb Ruiz <ruiz@kde.org>
   (C) 2008-2009 Sebastian Trueg <trueg@kde.org>

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

#include "folderselectionmodel.h"

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QColor>
#include <QBrush>
#include <QPalette>

#include <KDebug>
#include <KIcon>
#include <KLocale>


FolderSelectionModel::FolderSelectionModel( QObject* parent )
    : QFileSystemModel( parent )
{
    setHiddenFoldersShown( false );
}


FolderSelectionModel::~FolderSelectionModel()
{
}


void FolderSelectionModel::setHiddenFoldersShown( bool shown )
{
    if ( shown )
        setFilter( QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Hidden );
    else
        setFilter( QDir::AllDirs | QDir::NoDotAndDotDot );
}


Qt::ItemFlags FolderSelectionModel::flags( const QModelIndex &index ) const
{
    Qt::ItemFlags flags = QFileSystemModel::flags( index );
    flags |= Qt::ItemIsUserCheckable;
    if( isForbiddenPath( filePath( index ) ) )
        flags ^= Qt::ItemIsEnabled; //disabled!
    return flags;
}


QVariant FolderSelectionModel::data( const QModelIndex& index, int role ) const
{
    if( index.isValid() && index.column() == 0 ) {
        if( role == Qt::CheckStateRole ) {
            const IncludeState state = includeState( index );
            switch( state ) {
            case StateNone:
            case StateExclude:
            case StateExcludeInherited:
                return Qt::Unchecked;
            case StateInclude:
            case StateIncludeInherited:
                return Qt::Checked;
            }
        }
        else if( role == IncludeStateRole ) {
            return includeState( index );
        }
        else if( role == Qt::ForegroundRole ) {
            IncludeState state = includeState( index );
            QBrush brush = QFileSystemModel::data( index, role ).value<QBrush>();
            switch( state ) {
            case StateInclude:
            case StateIncludeInherited:
                brush = QPalette().brush( QPalette::Active, QPalette::Text );
                break;
            case StateNone:
            case StateExclude:
            case StateExcludeInherited:
                brush = QPalette().brush( QPalette::Disabled, QPalette::Text );
                break;
            }
            return QVariant::fromValue( brush );
        }
        else if ( role == Qt::ToolTipRole ) {
            IncludeState state = includeState( index );
            if ( state == StateInclude || state == StateIncludeInherited ) {
                return i18nc( "@info:tooltip %1 is the path of the folder in a listview",
                              "<filename>%1</filename><nl/>(will be indexed for desktop search)", filePath( index ) );
            }
            else {
                return i18nc( "@info:tooltip %1 is the path of the folder in a listview",
                              "<filename>%1</filename><nl/> (will <emphasis>not</emphasis> be indexed for desktop search)", filePath( index ) );
            }
        }
        else if ( role == Qt::DecorationRole ) {
            if ( filePath( index ) == QDir::homePath() ) {
                return KIcon( "user-home" );
            }
        }
    }

    return QFileSystemModel::data( index, role );
}


bool FolderSelectionModel::setData( const QModelIndex& index, const QVariant& value, int role )
{
    if( index.isValid() && index.column() == 0 && role == Qt::CheckStateRole ) {
        const QString path = filePath( index );
        const IncludeState state = includeState( path );

        // here we ignore the check value, we treat it as a toggle
        // This is due to our using the Qt checking system in a virtual way
        if( state == StateInclude ||
            state == StateIncludeInherited ) {
            excludePath( path );
            return true;
        }
        else {
            includePath( path );
            return true;
        }
        return false;
    }

    return QFileSystemModel::setData( index, value, role );
}


namespace {
    void removeSubDirs( const QString& path, QSet<QString>& set ) {
        QSet<QString>::iterator it = set.begin();
        while( it != set.end() ) {
            if( it->startsWith( path ) )
                it = set.erase( it );
            else
                ++it;
        }
    }

    QModelIndex findLastLeaf( const QModelIndex& index, FolderSelectionModel* model ) {
        int rows = model->rowCount( index );
        if( rows > 0 ) {
            return findLastLeaf( model->index( rows-1, 0, index ), model );
        }
        else {
            return index;
        }
    }
}

void FolderSelectionModel::includePath( const QString& path )
{
    if( !m_included.contains( path ) ) {
        // remove all subdirs
        removeSubDirs( path, m_included );
        removeSubDirs( path, m_excluded );
        m_excluded.remove( path );

        // only really include if the parent is not already included
        if( includeState( path ) != StateIncludeInherited ) {
            m_included.insert( path );
        }
        emit dataChanged( index( path ), findLastLeaf( index( path ), this ) );
    }
}


void FolderSelectionModel::excludePath( const QString& path )
{
    if( !m_excluded.contains( path ) ) {
        // remove all subdirs
        removeSubDirs( path, m_included );
        removeSubDirs( path, m_excluded );
        m_included.remove( path );

        // only really exclude the path if a parent is included
        if( includeState( path ) == StateIncludeInherited ) {
            m_excluded.insert( path );
        }
        emit dataChanged( index( path ), findLastLeaf( index( path ), this ) );
    }
}


void FolderSelectionModel::setFolders( const QStringList& includeDirs, const QStringList& excludeDirs )
{
    m_included = QSet<QString>::fromList( includeDirs );
    m_excluded = QSet<QString>::fromList( excludeDirs );
    reset();
}


QStringList FolderSelectionModel::includeFolders() const
{
    return m_included.toList();
}


QStringList FolderSelectionModel::excludeFolders() const
{
    return m_excluded.toList();
}


inline bool FolderSelectionModel::isForbiddenPath( const QString& path ) const
{
    // we need the trailing slash otherwise we could forbid "/dev-music" for example
    QString _path = path.endsWith( '/' ) ? path : path + '/';
    QFileInfo fi( _path );
    return( _path.startsWith( QLatin1String( "/proc/" ) ) ||
            _path.startsWith( QLatin1String( "/dev/" ) ) ||
            _path.startsWith( QLatin1String( "/sys/" ) ) ||
            !fi.isReadable() ||
            !fi.isExecutable() );
}


FolderSelectionModel::IncludeState FolderSelectionModel::includeState( const QModelIndex& index ) const
{
    return includeState( filePath( index ) );
}


FolderSelectionModel::IncludeState FolderSelectionModel::includeState( const QString& path ) const
{
    if( m_included.contains( path ) ) {
        return StateInclude;
    }
    else if( m_excluded.contains( path ) ) {
        return StateExclude;
    }
    else {
        QString parent = path.section( QDir::separator(), 0, -2, QString::SectionSkipEmpty|QString::SectionIncludeLeadingSep );
        if( parent.isEmpty() ) {
            return StateNone;
        }
        else if ( QFileInfo( path ).isHidden() ) {
            // we treat hidden files special - they are disabled by default
            return StateNone;
        }
        else {
            IncludeState state = includeState( parent );
            if( state == StateNone )
                return StateNone;
            else if( state == StateInclude || state == StateIncludeInherited )
                return StateIncludeInherited;
            else
                return StateExcludeInherited;
        }
    }
}

#include "folderselectionmodel.moc"
