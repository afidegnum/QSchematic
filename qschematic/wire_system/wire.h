#pragma once

#include <QList>
#include "items/wirepoint.h"

namespace QSchematic {
    class Wire;
}

using namespace QSchematic; // TODO: Needs to be removed

namespace wire_system
{
    class wire
    {

    public:
        int points_count() const;


    protected: // TODO: All these members should be private
        QVector<WirePoint> _points;
        QList<Wire*> _connectedWires; // TODO: Should be QList<wire_system::wire>
    };
}