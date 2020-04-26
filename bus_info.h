#pragma once

#include <string>
#include <vector>

namespace Models {

    struct BusInfo
    {
        std::string name;
        std::vector<std::string> stops;
        bool is_roundtrip;
    };
}