#pragma once

#include <cmath>

namespace Models {

    struct Position {
        double lat;
        double lon;

        double Distance(const Position& other) {
            return acos(sin(lat) * sin(other.lat) +
                cos(lat) * cos(other.lat) *
                cos(std::abs(lon - other.lon))
            ) * 6371000;
        }
    };
}