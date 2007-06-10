//
// This file is part of the Marble Desktop Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2006-2007 Torsten Rahn <tackat@kde.org>"
// Copyright 2007      Inge Wallin  <ingwa@kde.org>"
//


#ifndef VECTORMAP_H
#define VECTORMAP_H

#include <QtGui/QPen>
#include <QtGui/QBrush>
#include <QtGui/QImage>
#include <QtGui/QPixmap>

#include "Quaternion.h"
#include "GeoPoint.h"
#include "ScreenPolygon.h"


class PntMap;
class ClipPainter;


class VectorMap : public ScreenPolygon::Vector
{
 public:
    VectorMap();
    ~VectorMap();
    void createFromPntMap(const PntMap*, const int&, Quaternion&);

    void paintMap(ClipPainter *, bool );
    void drawMap(QPaintDevice *, bool );

    void resizeMap(const QPaintDevice *);

    void setPen ( const QPen & p )     { m_pen   = p; }
    void setBrush ( const QBrush & b ) { m_brush = b; }
    void setzBoundingBoxLimit ( const float zBoundingBoxLimit ) {
        m_zBoundingBoxLimit = zBoundingBoxLimit; }
    void setzPointLimit ( const float zPointLimit ) {
        m_zPointLimit = zPointLimit; }

    //	void clearNodeCount(){ m_debugNodeCount = 0; }
    //	int nodeCount(){ return m_debugNodeCount; }

 private:
    void createPolyLine( GeoPoint::Vector::ConstIterator, 
                         GeoPoint::Vector::ConstIterator, const int);

    inline void    initCrossHorizon();
    void           manageCrossHorizon();
    const QPointF  horizonPoint();
    void           createArc();

    int            getDetailLevel() const;

 private:
    GeoPoint::Vector  boundary;

    float             m_zlimit;
    float             m_plimit;
    float             m_zBoundingBoxLimit;	
    float             m_zPointLimit;	

    //	Quaternion m_invRotAxis;
    matrix            m_rotMatrix;

    //	int m_debugNodeCount;

    ScreenPolygon     m_polygon;

    int               m_imgrx;
    int               m_imgry;
    int               m_imgradius;
    int               m_imgwidth;
    int               m_imgheight;

    QPointF           m_currentPoint;
    QPointF           m_lastPoint; 

    QPen              m_pen;
    QBrush            m_brush;

    // Dealing with the horizon
    bool              m_firsthorizon;
    bool              m_lastvisible;
    bool              m_currentlyvisible;
    bool              m_horizonpair;
    QPointF           m_firstHorizonPoint;
    QPointF           m_horizona;
    QPointF           m_horizonb;
	
    int               m_radius;
    int               m_rlimit;
};


#endif // VECTORMAP_H
