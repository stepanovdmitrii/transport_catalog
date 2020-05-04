#pragma once

#include <string>
#include <vector>

namespace Models {
    struct RouteSegmentInfo {
        double wait_time = 0.0;
        std::string stop_from;
        std::vector<std::string_view> stops;
        std::string bus;
        size_t span_count = 0;
        double trip_time = 0.0;
    };
}