//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2013      Sanjiban Bairagya <sanjiban22393@gmail.com>
//

#ifndef KMLORIENTATIONTAGHANDLER_H
#define KMLORIENTATIONTAGHANDLER_H

#include "GeoTagHandler.h"

namespace Marble
{
namespace kml
{

class KmlOrientationTagHandler : public GeoTagHandler
{
public:
    virtual GeoNode* parse(GeoParser&) const;
};

}
}

#endif // KMLORIENTATIONTAGHANDLER_H
