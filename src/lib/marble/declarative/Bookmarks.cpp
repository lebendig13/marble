//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2012      Dennis Nienhüser <nienhueser@kde.org>
//

#include "Bookmarks.h"

#include "Planet.h"
#include "MarbleQuickItem.h"
#include "MarbleModel.h"
#include "MarbleMath.h"
#include "MarblePlacemarkModel.h"
#include "BookmarkManager.h"
#include "GeoDataDocument.h"
#include "GeoDataPlacemark.h"
#include "GeoDataFolder.h"
#include "GeoDataTypes.h"
#include "GeoDataExtendedData.h"
#include "GeoDataTreeModel.h"
#include "kdescendantsproxymodel.h"

#include <QSortFilterProxyModel>

namespace Marble {

Bookmarks::Bookmarks( QObject* parent ) : QObject( parent ),
    m_marbleQuickItem( 0 ), m_proxyModel( 0 )
{
    // nothing to do
}

MarbleQuickItem *Bookmarks::map()
{
    return m_marbleQuickItem;
}

void Bookmarks::setMap( MarbleQuickItem* item )
{
    m_marbleQuickItem = item;
}

bool Bookmarks::isBookmark( qreal longitude, qreal latitude ) const
{
    if ( !m_marbleQuickItem || !m_marbleQuickItem->model()->bookmarkManager() ) {
        return false;
    }

    Marble::BookmarkManager* manager = m_marbleQuickItem->model()->bookmarkManager();
    Marble::GeoDataDocument *bookmarks = manager->document();
    Marble::GeoDataCoordinates const compareTo( longitude, latitude, 0.0, Marble::GeoDataCoordinates::Degree );

    qreal planetRadius = m_marbleQuickItem->model()->planet()->radius();
    foreach( const Marble::GeoDataFolder* folder, bookmarks->folderList() ) {
        foreach( const Marble::GeoDataPlacemark * const placemark, folder->placemarkList() ) {
            if ( distanceSphere( placemark->coordinate(), compareTo ) * planetRadius < 5 ) {
                return true;
            }
        }
    }

    return false;
}

void Bookmarks::addBookmark( qreal longitude, qreal latitude, const QString &name, const QString &folderName )
{
    if ( !m_marbleQuickItem || !m_marbleQuickItem->model()->bookmarkManager() ) {
        return;
    }

    Marble::BookmarkManager* manager = m_marbleQuickItem->model()->bookmarkManager();
    Marble::GeoDataDocument *bookmarks = manager->document();
    Marble::GeoDataContainer *target = 0;
    foreach( Marble::GeoDataFolder* const folder, bookmarks->folderList() ) {
        if ( folder->name() == folderName ) {
            target = folder;
            break;
        }
    }

    if ( !target ) {
        manager->addNewBookmarkFolder( bookmarks, folderName );

        foreach( Marble::GeoDataFolder* const folder, bookmarks->folderList() ) {
            if ( folder->name() == folderName ) {
                target = folder;
                break;
            }
        }

        Q_ASSERT( target );
    }

    Marble::GeoDataPlacemark placemark;
    Marble::GeoDataCoordinates coordinate( longitude, latitude, 0.0, Marble::GeoDataCoordinates::Degree );
    placemark.setCoordinate( coordinate );
    placemark.setName( name );
    placemark.extendedData().addValue( Marble::GeoDataData( "isBookmark", true ) );

    manager->addBookmark( target, placemark );
}

void Bookmarks::removeBookmark( qreal longitude, qreal latitude )
{
    if ( !m_marbleQuickItem || !m_marbleQuickItem->model()->bookmarkManager() ) {
        return;
    }

    Marble::BookmarkManager* manager = m_marbleQuickItem->model()->bookmarkManager();
    Marble::GeoDataDocument *bookmarks = manager->document();
    Marble::GeoDataCoordinates const compareTo( longitude, latitude, 0.0, Marble::GeoDataCoordinates::Degree );

    qreal planetRadius = m_marbleQuickItem->model()->planet()->radius();
    foreach( const Marble::GeoDataFolder* folder, bookmarks->folderList() ) {
        foreach( Marble::GeoDataPlacemark * placemark, folder->placemarkList() ) {
            if ( distanceSphere( placemark->coordinate(), compareTo ) * planetRadius < 5 ) {
                manager->removeBookmark( placemark );
                return;
            }
        }
    }
}

BookmarksModel *Bookmarks::model()
{
    if ( !m_proxyModel && m_marbleQuickItem && m_marbleQuickItem->model()->bookmarkManager() ) {
        Marble::BookmarkManager* manager = m_marbleQuickItem->model()->bookmarkManager();
        Marble::GeoDataTreeModel* model = new Marble::GeoDataTreeModel( this );
        model->setRootDocument( manager->document() );

        KDescendantsProxyModel* flattener = new KDescendantsProxyModel( this );
        flattener->setSourceModel( model );

        m_proxyModel = new BookmarksModel( this );
        m_proxyModel->setFilterFixedString( Marble::GeoDataTypes::GeoDataPlacemarkType );
        m_proxyModel->setFilterKeyColumn( 1 );
        m_proxyModel->setSourceModel( flattener );
    }

    return m_proxyModel;
}

BookmarksModel::BookmarksModel( QObject *parent ) : QSortFilterProxyModel( parent )
{
    connect( this, SIGNAL(layoutChanged()), this, SIGNAL(countChanged()) );
    connect( this, SIGNAL(modelReset()), this, SIGNAL(countChanged()) );
    connect( this, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SIGNAL(countChanged()) );
    connect( this, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SIGNAL(countChanged()) );
}

int BookmarksModel::count() const
{
    return rowCount();
}

qreal BookmarksModel::longitude( int idx ) const
{
    if ( idx >= 0 && idx < rowCount() ) {
        QVariant const value = data( index( idx, 0 ), Marble::MarblePlacemarkModel::CoordinateRole );
        Marble::GeoDataCoordinates const coordinates = value.value<Marble::GeoDataCoordinates>();
        return coordinates.longitude( Marble::GeoDataCoordinates::Degree );
    }
    return 0.0;
}

qreal BookmarksModel::latitude( int idx ) const
{
    if ( idx >= 0 && idx < rowCount() ) {
        QVariant const value = data( index( idx, 0 ), Marble::MarblePlacemarkModel::CoordinateRole );
        Marble::GeoDataCoordinates const coordinates = value.value<Marble::GeoDataCoordinates>();
        return coordinates.latitude( Marble::GeoDataCoordinates::Degree );
    }
    return 0.0;
}

QString BookmarksModel::name( int idx ) const
{
    if ( idx >= 0 && idx < rowCount() ) {
        return data( index( idx, 0 ) ).toString();
    }
    return QString();
}

}

#include "moc_Bookmarks.cpp"
