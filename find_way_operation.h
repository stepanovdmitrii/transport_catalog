#pragma once

#include "i_read_operation.h"
#include "bus_schedule.h"
#include "i_formatter.h"
#include "i_draw_map_operation.h"

#include <string>

namespace Operations {
    class FindWay : public IDrawMapOperation {
    public:
        FindWay(std::string from, std::string to, size_t request_id);

        virtual void DrawMap(Catalog::BusShedule& bus_schedule, Catalog::MapBuilder& map_builder, IO::IFormatter& formatter) override;
    private:
        std::string _from;
        std::string _to;
        size_t _request_id;
    };
}

