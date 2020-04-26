#pragma once

#include <string>

namespace Models {
    struct BusStatInfo {
        std::string bus;
        bool found = false;
        size_t total_stops = 0;
        size_t unique_stops = 0;
        double length = 0.0;
        double curvature = 0.0;
        size_t request_id;
    };
}
