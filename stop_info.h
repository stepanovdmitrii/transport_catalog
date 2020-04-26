#pragma once

#include "position.h"

#include <string>
#include <unordered_map>

namespace Models {
    struct StopInfo
    {
        std::string name;
        Position position;
        std::unordered_map<std::string, double> known_distance;
    };
}