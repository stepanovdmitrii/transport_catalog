#pragma once

#include <string>
#include <vector>
#include <string_view>

namespace Models {
    struct EdgeInfo {
        std::string bus;
        std::string from;
        std::vector<std::string_view> stops;
        size_t span_count;
        double trip_time;
    };
}