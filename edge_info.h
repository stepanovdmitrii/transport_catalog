#pragma once

#include <string>

namespace Models {
    struct EdgeInfo {
        std::string bus;
        std::string from;
        size_t span_count;
        double trip_time;
    };
}