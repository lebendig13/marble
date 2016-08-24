//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2010 Utku Aydin <utkuaydin34@gmail.com>
//

#include "EarthquakeModel.h"
#include "EarthquakeItem.h"

#include "MarbleGlobal.h"
#include "MarbleModel.h"
#include "GeoDataCoordinates.h"
#include "GeoDataLatLonAltBox.h"
#include "MarbleDebug.h"

#include <QDebug>
#include <QString>
#include <QUrl>
#include <QScriptEngine>
#include <QScriptValue>
#include <QScriptValueIterator>

namespace Marble {

EarthquakeModel::EarthquakeModel( const MarbleModel *marbleModel, QObject *parent )
    : AbstractDataPluginModel( "earthquake", marbleModel, parent ),
      m_minMagnitude( 0.0 ),
      m_startDate( QDateTime::fromString( "2006-02-04", "yyyy-MM-dd" ) ),
      m_endDate( QDateTime::currentDateTime() )
{
    // nothing to do
}

EarthquakeModel::~EarthquakeModel()
{
}

void EarthquakeModel::setMinMagnitude( double minMagnitude )
{
    m_minMagnitude = minMagnitude;
}

void EarthquakeModel::setStartDate( const QDateTime& startDate )
{
    m_startDate = startDate;
}

void EarthquakeModel::setEndDate( const QDateTime& endDate )
{
    m_endDate = endDate;
}

void EarthquakeModel::getAdditionalItems( const GeoDataLatLonAltBox& box, qint32 number )
{
    if (marbleModel()->planetId() != QLatin1String("earth")) {
        return;
    }

    const QString geonamesUrl( QLatin1String("http://ws.geonames.org/earthquakesJSON") +
        QLatin1String("?north=")   + QString::number(box.north() * RAD2DEG) +
        QLatin1String("&south=")   + QString::number(box.south() * RAD2DEG) +
        QLatin1String("&east=")    + QString::number(box.east() * RAD2DEG) +
        QLatin1String("&west=")    + QString::number(box.west() * RAD2DEG) +
        QLatin1String("&date=")    + m_endDate.toString("yyyy-MM-dd") +
        QLatin1String("&maxRows=") + QString::number(number) +
        QLatin1String("&username=marble") +
        QLatin1String("&formatted=true"));
    downloadDescriptionFile( QUrl( geonamesUrl ) );
}

void EarthquakeModel::parseFile( const QByteArray& file )
{
    QScriptValue data;
    QScriptEngine engine;

    // Qt requires parentheses around json code
    data = engine.evaluate( '(' + QString( file ) + ')' );

    // Parse if any result exists
    if ( data.property( "earthquakes" ).isArray() ) {
        QScriptValueIterator iterator( data.property( "earthquakes" ) );
        // Add items to the list
        QList<AbstractDataPluginItem*> items;
        while ( iterator.hasNext() ) {
            iterator.next();
            // Converting earthquake's properties from QScriptValue to appropriate types
            QString eqid = iterator.value().property( "eqid" ).toString(); // Earthquake's ID
            double longitude = iterator.value().property( "lng" ).toNumber();
            double latitude = iterator.value().property( "lat" ).toNumber();
            double magnitude = iterator.value().property( "magnitude" ).toNumber();
            QString data = iterator.value().property( "datetime" ).toString();
            QDateTime date = QDateTime::fromString( data, "yyyy-MM-dd hh:mm:ss" );
            double depth = iterator.value().property( "depth" ).toNumber();

            if( date <= m_endDate && date >= m_startDate && magnitude >= m_minMagnitude ) {
                if( !itemExists( eqid ) ) {
                    // If it does not exists, create it
                    GeoDataCoordinates coordinates( longitude, latitude, 0.0, GeoDataCoordinates::Degree );
                    EarthquakeItem *item = new EarthquakeItem( this );
                    item->setId( eqid );
                    item->setCoordinate( coordinates );
                    item->setMagnitude( magnitude );
                    item->setDateTime( date );
                    item->setDepth( depth );
                    items << item;
                }
            }
        }

        addItemsToList( items );
    }
}


}

#include "moc_EarthquakeModel.cpp"
