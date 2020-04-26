#pragma once

#include "bus_stat_info.h"
#include "stop_stat_info.h"
#include "route_info.h"
#include "draw_map_info.h"

namespace IO {
    class IFormatter {
    public:
        virtual void Write(const Models::BusStatInfo& info) = 0;
        virtual void Write(const Models::StopStatInfo& info) = 0;
        virtual void Write(const Models::RouteInfo& info) = 0;
        virtual void Write(const Models::DrawMapInfo& info) = 0;
        virtual void Flush() = 0;
        virtual ~IFormatter();
    };
}