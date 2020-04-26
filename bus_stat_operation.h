#pragma once

#include "i_read_operation.h"
#include "position.h"
#include "bus_stat_info.h"
#include "bus_schedule.h"
#include "i_formatter.h"

#include <string>
#include <unordered_map>
#include <unordered_set>

namespace Operations {
    class BusStat : public IReadOperation {
    public:
        BusStat(std::string bus, size_t request_id = 0);

        // Унаследовано через IReadOperation
        virtual void Do(Catalog::BusShedule& bus_schedule, IO::IFormatter& formatter) override;

    private:
        std::string _bus;
        size_t _request_id = 0;
        std::unordered_map<std::string, Models::Position> _known_stops;
        std::unordered_map<std::string, std::unordered_map<std::string, double>> _known_distances;

        Models::BusStatInfo Collect(Catalog::BusShedule& bus_schedule);

        Models::Position FindPosition(const std::string& stop, const Catalog::BusShedule& bus_schedule);

        double FindRouteDistance(const std::string& first_stop, const std::string& second_stop, const Catalog::BusShedule& bus_schedule);
    };
}

