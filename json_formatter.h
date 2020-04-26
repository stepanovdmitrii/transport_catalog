#pragma once

#include "i_formatter.h"
#include "bus_stat_info.h"
#include "stop_stat_info.h"
#include "route_info.h"


#include <iostream>
#include <vector>

namespace IO {
    class JsonFormatter : public IFormatter {
    public:
        JsonFormatter(std::ostream& output);

        // Унаследовано через IOutputFormatter
        virtual void Write(const Models::BusStatInfo& info) override;
        virtual void Write(const Models::StopStatInfo& info) override;
        virtual void Write(const Models::RouteInfo& info) override;
        virtual void Write(const Models::DrawMapInfo& info) override;
        virtual void Flush() override;
    private:
        std::ostream& _output;
        std::vector<Models::BusStatInfo> _bus_infos;
        std::vector<Models::StopStatInfo> _stop_infos;
        std::vector<Models::RouteInfo> _route_infos;
        std::vector<Models::DrawMapInfo> _map_infos;

        void WriteSquareBrace(bool open);

        void WriteBrace(bool open);

        void WriteBusElement(const Models::BusStatInfo& bus);

        void WriteStopElement(const Models::StopStatInfo& stop);

        void WriteRouteElement(const Models::RouteInfo& route);

        void WriteMapElement(const Models::DrawMapInfo& map);

        void WriteElements();
    };
}

