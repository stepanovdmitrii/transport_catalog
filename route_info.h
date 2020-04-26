#pragma once

#include "route_segment_info.h"

#include <vector>

namespace Models {
    struct RouteInfo {
        size_t request_id = 0;
        bool found = false;
        std::vector<RouteSegmentInfo> segments;
        double total_time = 0.0;
    };
}