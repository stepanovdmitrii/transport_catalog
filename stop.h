#pragma once
#include "point.h"
#include "position.h"

#include <set>
#include <string>
#include <unordered_map>

namespace Models {
    struct Stop {
        std::string name;
        Position position;
        std::set<std::string> buses;
        std::unordered_map<std::string, double> known_distances;
    };
}