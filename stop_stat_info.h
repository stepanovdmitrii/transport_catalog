#pragma once

#include <string>
#include <vector>

namespace Models {
    struct StopStatInfo {
        std::string stop;
        bool found = false;
        std::vector<std::string> buses;
        size_t request_id;
    };
}
