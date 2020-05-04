#pragma once

#include "route_segment_info.h"
#include "document.h"

#include <vector>
#include <memory>

namespace Models {
    struct RouteInfo {
        size_t request_id = 0;
        bool found = false;
        std::vector<RouteSegmentInfo> segments;
        double total_time = 0.0;
        std::shared_ptr<Svg::Document> svg;
    };
}