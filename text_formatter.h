#pragma once

#include "i_formatter.h"
#include "bus_stat_info.h"
#include "stop_stat_info.h"
#include "route_info.h"

#include <iostream>
#include <exception>

namespace IO {
    class TextFormatter : public IFormatter {
    public:
        TextFormatter(std::ostream& output);

        virtual void Write(const Models::BusStatInfo& info) override;
        virtual void Write(const Models::StopStatInfo& info) override;
        virtual void Write(const Models::RouteInfo& info) override;
        virtual void Write(const Models::DrawMapInfo& info) override;
        virtual void Flush() override;

    private:
        std::ostream& _output;
    };
}
